#pragma once

template<unsigned int N>
HRESULT CreateInputLayout(ID3D11Device* device, 
	const D3D11_INPUT_ELEMENT_DESC(&elements)[N], 
	const std::vector<uint8_t>& vsCode, 
	ID3D11InputLayout** ppInputLayout)
{
	auto hr = device->CreateInputLayout(elements, N, reinterpret_cast<const void*>(vsCode.data()), vsCode.size(), ppInputLayout);
	return hr;
}

