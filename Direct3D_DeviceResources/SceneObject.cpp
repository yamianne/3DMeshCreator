#include "pch.h"
#include "SceneObject.h"
#include <numeric>

void SceneObject::Init(ID3D11Device* device)
{
    m_stride = sizeof(Vector3);
    m_offset = 0;
    auto blob = DX::ReadShaderBytecode(L"VertexShader.cso");
    DX::ThrowIfFailed(device->CreateVertexShader(&blob.front(), blob.size(),
        nullptr, m_vertexShader.ReleaseAndGetAddressOf()));

    DX::ThrowIfFailed(CreateInputLayout(device, VertexTypes::VertexPosition::Layout, blob, m_inputLayout.GetAddressOf()));

    blob = DX::ReadShaderBytecode(L"PixelShader.cso");
    DX::ThrowIfFailed(device->CreatePixelShader(&blob.front(), blob.size(),
        nullptr, m_pixelShader.ReleaseAndGetAddressOf()));

    //auto idxs = std::accumulate(m_edges.begin(), m_edges.end(), std::vector<unsigned short>(),
    //    [](std::vector<unsigned short> result, Edge& e)
    //    {
    //        result.emplace_back(e.GetStartVertexIndex());
    //        result.emplace_back(e.GetEndVertexIndex());
    //        return result;
    //    }
    //);

    DX::ThrowIfFailed(DirectX::CreateStaticBuffer(device, m_vertices, D3D11_BIND_VERTEX_BUFFER, m_shapeVB.GetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateStaticBuffer(device, m_edges, D3D11_BIND_INDEX_BUFFER, m_shapeIB.GetAddressOf()));
    m_indexCount = m_edges.size();

    // Create CB
    m_worldCB.Create(device);
    m_colorCB.Create(device);

    RecalculateWorldMatrix();
}

SceneObject::~SceneObject()
{
    m_inputLayout.Reset();
    m_pixelShader.Reset();
    m_vertexShader.Reset();
    m_geometryShader.Reset();
    m_shapeVB.Reset();
    m_shapeIB.Reset();
}

void SceneObject::Render(ID3D11DeviceContext* context, DirectX::XMFLOAT4 selectedColor)
{
    if (IsHidden) return;
    m_worldCB.SetData(context, m_world);
    auto buffer = m_worldCB.GetBuffer();
    context->VSSetConstantBuffers(0, 1, &buffer);

    DirectX::XMFLOAT4 color = IsSelected ? selectedColor : Color;
    m_colorCB.SetData(context, color);
    buffer = m_colorCB.GetBuffer();
    context->PSSetConstantBuffers(0, 1, &buffer);

    context->VSSetShader(m_vertexShader.Get(), 0, 0);
    context->PSSetShader(m_pixelShader.Get(), 0, 0);
    if (m_geometryShader)
        context->GSSetShader(m_geometryShader.Get(), 0, 0);
    else context->GSSetShader(nullptr, 0, 0);
    context->IASetInputLayout(m_inputLayout.Get());
    context->IASetPrimitiveTopology(m_topology);
    context->IASetIndexBuffer(m_shapeIB.Get(), DXGI_FORMAT_R16_UINT, 0);
    context->IASetVertexBuffers(0, 1, m_shapeVB.GetAddressOf(), &m_stride, &m_offset);
    context->DrawIndexed(m_indexCount, 0, 0);
}

void SceneObject::RecalculateWorldMatrix()
{
    m_world = Matrix::CreateScale(m_scale) *
        Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(GetRotation()->x, GetRotation()->y, GetRotation()->z)) *
        Matrix::CreateTranslation(*GetPosition());
}
