#include "pch.h"
#include "VertexTypes.h"

const D3D11_INPUT_ELEMENT_DESC VertexTypes::VertexPosition2D::Layout[1] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, offsetof(VertexTypes::VertexPosition2D, position), 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexTypes::VertexPosition2DColor::Layout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, offsetof(VertexTypes::VertexPosition2DColor, position), 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexTypes::VertexPosition2DColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexTypes::VertexPosition::Layout[1] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, offsetof(VertexTypes::VertexPosition, position), 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexTypes::VertexPositionColor::Layout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, offsetof(VertexTypes::VertexPositionColor, position), 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexTypes::VertexPositionColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};