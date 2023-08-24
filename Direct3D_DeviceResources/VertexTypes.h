#pragma once
namespace VertexTypes
{
	struct VertexPosition2D
	{
		DirectX::SimpleMath::Vector2 position;

		static const D3D11_INPUT_ELEMENT_DESC Layout[1];
	};

	struct VertexPosition2DColor
	{
		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT3 color;

		static const D3D11_INPUT_ELEMENT_DESC Layout[2];
	};

	struct VertexPosition
	{
		DirectX::SimpleMath::Vector3 position;

		static const D3D11_INPUT_ELEMENT_DESC Layout[1];
	};

	struct VertexPositionColor
	{
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector3 color;

		static const D3D11_INPUT_ELEMENT_DESC Layout[2];
	};
}