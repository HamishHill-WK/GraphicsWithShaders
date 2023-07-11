#pragma once

#include "DXF.h"

	using namespace std;
using namespace DirectX;


class verManipDepthShader : public BaseShader
{

public:

	verManipDepthShader(ID3D11Device* device, HWND hwnd);
	~verManipDepthShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* heightTex);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;

};
