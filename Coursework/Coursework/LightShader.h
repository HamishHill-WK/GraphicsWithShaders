#pragma once

#include "DXF.h"
#include "myLight.h"

using namespace std;
using namespace DirectX;

class LightShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
	};

	struct LightBufferType
	{
		XMFLOAT3 direction;
		int norms;
		XMFLOAT3 position;
		float pad;
		XMFLOAT3 skypos;
		float pad1;
		XMFLOAT3 atten;
		float pad2;
		XMFLOAT4 ambient;
		XMFLOAT4 skyAmbient;
		XMFLOAT4 diffuse;
		XMFLOAT4 skyDiffuse;
	};

	struct spotLightBuffer
	{
		XMFLOAT3 spotDirection;
		float specPower;
		XMFLOAT4 specDiffuse;
		XMFLOAT3 spotPosition;
		float range;
		XMFLOAT4 spotAmbient;
		XMFLOAT4 spotDiffuse;
		XMFLOAT3 spotAtten;
		float cone;
	};


public:
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMap, myLight* light, myLight* skylight, myLight* spotlight, bool normals);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;	
	ID3D11Buffer* spotlightBuffer;
};

