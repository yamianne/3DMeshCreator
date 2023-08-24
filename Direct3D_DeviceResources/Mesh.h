#pragma once
#include "ReadShaderBytecode.h"
#include "DXHelpers.h"
#include "Edge.h"

class Mesh
{
public:
    Mesh(std::string name) : Name(name) {}
    Mesh(std::string name, D3D_PRIMITIVE_TOPOLOGY topology) : Name(name), m_topology(topology) {}
    Mesh(std::string name, Vector3 position) : Name(name), m_position(position) {}
    Mesh(std::string name, Vector3 position, D3D_PRIMITIVE_TOPOLOGY topology) : Name(name), m_position(position), m_topology(topology) {}

    template<typename VertexType>
    Mesh(ID3D11Device* device, std::string name,
        const std::vector<VertexType>& verts, const std::vector<unsigned short>& idxs, 
        Vector3 position = Vector3::Zero, Vector4 color = Vector4::One, 
        const std::wstring& vsString = L"VertexShader.cso", const std::wstring& psString = L"PixelShader.cso") :
        Name(name), Color(color), m_position(position)
    {
        m_indexCount = idxs.size();
        m_stride = sizeof(VertexType);
        m_offset = 0;
        auto blob = DX::ReadShaderBytecode(vsString.c_str());
        DX::ThrowIfFailed(device->CreateVertexShader(&blob.front(), blob.size(),
            nullptr, m_vertexShader.ReleaseAndGetAddressOf()));

        DX::ThrowIfFailed(CreateInputLayout(device, VertexType::Layout, blob, m_inputLayout.GetAddressOf()));

        blob = DX::ReadShaderBytecode(psString.c_str());
        DX::ThrowIfFailed(device->CreatePixelShader(&blob.front(), blob.size(),
            nullptr, m_pixelShader.ReleaseAndGetAddressOf()));

        DX::ThrowIfFailed(DirectX::CreateStaticBuffer(device, verts, D3D11_BIND_VERTEX_BUFFER, m_shapeVB.GetAddressOf()));
        DX::ThrowIfFailed(DirectX::CreateStaticBuffer(device, idxs, D3D11_BIND_INDEX_BUFFER, m_shapeIB.GetAddressOf()));

        // Create world CB
        m_worldCB.Create(device);
        m_colorCB.Create(device);

        RecalculateWorldMatrix();
    }

    ~Mesh();

    void                                        Init(ID3D11Device* device);
    virtual void                                Render(ID3D11DeviceContext* context, DirectX::XMFLOAT4 selectedColor);
    void                                        RecalculateWorldMatrix();

    Vector3*                                    GetPosition() { return &m_position; };
    Vector3*                                    GetRotation() { return &m_rotation; };
    Vector3*                                    GetScale() { return &m_scale; };
    Matrix                                      GetWorldMatrix() { return m_world; }

    bool					                    IsRenaming = false;
    bool					                    IsHidden = false;
    bool					                    IsSelected = false;
    std::string				                    Name = "SceneObject";
    DirectX::XMFLOAT4				            Color = { 0.88f,  0.8f,  0.8f, 1.00f };

protected:
    // Geometry
    std::vector<Vector3>                        m_vertices;
    std::vector<Edge>		                    m_edges;

    // Geometry orientation resources
    Vector3					                    m_position = Vector3::Zero;
    Vector3					                    m_rotation = Vector3::Zero;
    Vector3					                    m_scale = Vector3::One;
    Matrix                                      m_world = Matrix::Identity;

    DirectX::ConstantBuffer<DirectX::XMMATRIX>  m_worldCB;
    DirectX::ConstantBuffer<DirectX::XMFLOAT4>  m_colorCB;
    inline static int				            m_creationIdx = 0;

    // Drawing resources
    UINT                                        m_indexCount = 0;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>  m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader>m_geometryShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        m_shapeVB;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        m_shapeIB;
    UINT                                        m_stride = 0;
    UINT                                        m_offset = 0;
    D3D_PRIMITIVE_TOPOLOGY                      m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};