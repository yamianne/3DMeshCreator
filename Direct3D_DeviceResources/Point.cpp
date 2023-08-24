#include "pch.h"
#include "Point.h"

Point::Point(ID3D11Device* device, std::string name, Vector3 position) : 
    Mesh(device, name, std::vector<VertexTypes::VertexPosition>{{{0, 0, 0}}}, { 0 }, position, Vector4::One, L"VertexShader_Point.cso", L"PixelShader_Point.cso")
{
    auto blob = DX::ReadShaderBytecode(L"GeometryShader_Point.cso");
    DX::ThrowIfFailed(device->CreateGeometryShader(&blob.front(), blob.size(),
        nullptr, m_geometryShader.ReleaseAndGetAddressOf()));
    m_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
}

Point::Point(ID3D11Device* device, std::string name, Vector3 position, Vector4 color) :
    Mesh(device, name, std::vector<VertexTypes::VertexPosition>{{{0,0,0}}}, { 0 }, position, color, L"VertexShader_Point.cso", L"PixelShader_Point.cso")
{
    auto blob = DX::ReadShaderBytecode(L"GeometryShader_Point.cso");
    DX::ThrowIfFailed(device->CreateGeometryShader(&blob.front(), blob.size(),
        nullptr, m_geometryShader.ReleaseAndGetAddressOf()));
    m_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
}

