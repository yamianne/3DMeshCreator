#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(std::string name, GeometryType meshType) : SceneObject(name), MeshType(meshType)
{
	if (meshType == GeometryType::Quad)
		m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	else if (meshType == GeometryType::Triangle)
		m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void Mesh::Init(ID3D11Device* device)
{
	if (MeshType == GeometryType::Quad)
	{
		m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		m_stride = sizeof(Vector3);
		m_offset = 0;
		auto blob = DX::ReadShaderBytecode(L"VertexShader.cso");
		DX::ThrowIfFailed(device->CreateVertexShader(&blob.front(), blob.size(),
			nullptr, m_vertexShader.ReleaseAndGetAddressOf()));

		DX::ThrowIfFailed(CreateInputLayout(device, VertexTypes::VertexPosition::Layout, blob, m_inputLayout.GetAddressOf()));

		blob = DX::ReadShaderBytecode(L"PixelShader.cso");
		DX::ThrowIfFailed(device->CreatePixelShader(&blob.front(), blob.size(),
			nullptr, m_pixelShader.ReleaseAndGetAddressOf()));

		DX::ThrowIfFailed(DirectX::CreateStaticBuffer(device, m_vertices, D3D11_BIND_VERTEX_BUFFER, m_shapeVB.GetAddressOf()));
		DX::ThrowIfFailed(DirectX::CreateStaticBuffer(device, m_edges, D3D11_BIND_INDEX_BUFFER, m_shapeIB.GetAddressOf()));
		m_indexCount = m_edges.size();
	}
	else
	{
		m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_stride = sizeof(VertexTypes::VertexPositionNormal);
		m_offset = 0;
		auto blob = DX::ReadShaderBytecode(L"VertexShader_Phong.cso");
		DX::ThrowIfFailed(device->CreateVertexShader(&blob.front(), blob.size(),
			nullptr, m_vertexShader.ReleaseAndGetAddressOf()));

		DX::ThrowIfFailed(CreateInputLayout(device, VertexTypes::VertexPositionNormal::Layout, blob, m_inputLayout.GetAddressOf()));

		blob = DX::ReadShaderBytecode(L"PixelShader_Phong.cso");
		DX::ThrowIfFailed(device->CreatePixelShader(&blob.front(), blob.size(),
			nullptr, m_pixelShader.ReleaseAndGetAddressOf()));

		std::vector<VertexTypes::VertexPositionNormal> vertNorm(m_vertices.size());
		for (size_t i = 0; i < m_vertices.size(); i++)
		{
			vertNorm[i] = { m_vertices[i], m_normals[i] };
		}

		DX::ThrowIfFailed(DirectX::CreateStaticBuffer(device, vertNorm, D3D11_BIND_VERTEX_BUFFER, m_shapeVB.GetAddressOf()));
		DX::ThrowIfFailed(DirectX::CreateStaticBuffer(device, m_edges, D3D11_BIND_INDEX_BUFFER, m_shapeIB.GetAddressOf()));
		m_indexCount = m_edges.size();
	}

	// Create CB
	m_worldCB.Create(device);
	m_colorCB.Create(device);

	RecalculateWorldMatrix();
}

void Mesh::RecalculateGeometry(ID3D11Device* device)
{
	m_vertices.clear();
	m_edges.clear();
	m_normals.clear();
	if (MeshType == GeometryType::Quad)
		RecalculateQuadMesh(device);
	else if (MeshType == GeometryType::Triangle)
		RecalculateTriMesh(device);
	Init(device);
}
