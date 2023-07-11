// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"

#include "verManipShader.h"
#include "texShader.h"
#include "LightShader.h"
#include "DepthShader.h"
#include "verEdgeShader.h"
#include "verManipDepthShader.h"
#include "myLight.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

	//object position variables
	XMFLOAT3 treePos1 = XMFLOAT3(0, 0, 0);
	XMFLOAT3 treePos2 = XMFLOAT3(0, 0, 0);
	XMFLOAT3 treePos3 = XMFLOAT3(0, 0, 0);
	XMFLOAT3 treePos4 = XMFLOAT3(0, 0, 0);
	XMFLOAT3 treePos5 = XMFLOAT3(0, 0, 0);
	XMFLOAT3 groundPos = XMFLOAT3(0, 0, 0);

	//modifiable lighting values for imgui
	//point light variables
	XMFLOAT3 pos = XMFLOAT3(0, 0, 0);
	XMFLOAT4 pointdiffuse = XMFLOAT4(0, 0, 0, 0);
	XMFLOAT3 attenu = XMFLOAT3(0, 0, 0);
	XMFLOAT4 ambi = XMFLOAT4(0, 0, 0, 0);

	//skylight variables
	XMFLOAT3 skyPos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 direction = XMFLOAT3(0, 0, 0);
	XMFLOAT4 skydiffuse = XMFLOAT4(0, 0, 0, 0);
	XMFLOAT4 skyAmbi = XMFLOAT4(0, 0, 0, 0);

	//spotlight variables
	XMFLOAT3 spotDirection = XMFLOAT3(0, 0, 0);
	XMFLOAT4 spotDiffuse = XMFLOAT4(0, 0, 0, 0);
	XMFLOAT3 spotPos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 spotAttenu = XMFLOAT3(0, 0, 0);
	XMFLOAT4 spotAmbi = XMFLOAT4(0, 0, 0, 0);
	XMFLOAT4 specularColour = XMFLOAT4(0, 0, 0, 0);
	float specularPower = 0;
	float spotRange = 0;
	float spotCone = 0;

	bool renderSphere = 0;	//variable to control rendering sphere objects at the position of light sources
	bool edgeEnabled = 0;	//variable to toggle post processing effect
	bool showNorms = 0;		//variable to toggle displaying vertex normals

	//default values for modifiable variables
	//default object position variables
	XMFLOAT3 treePos1Def = XMFLOAT3(-27, -11, 30);
	XMFLOAT3 treePos2Def = XMFLOAT3(-44, -11, 38);
	XMFLOAT3 treePos3Def = XMFLOAT3(-50, -11, 24);
	XMFLOAT3 treePos4Def = XMFLOAT3(-76, -11, 14);
	XMFLOAT3 treePos5Def = XMFLOAT3(-31, -11, 40);
	XMFLOAT3 groundPosDef = XMFLOAT3(-92, -20, -16);
	
	//Default point light
	XMFLOAT3 posDef = XMFLOAT3(-40.0f, -5.0f, 24.0f);
	XMFLOAT4 pointdiffuseDef = XMFLOAT4(1.0f, 0.4f, 0.4f, 1.0f);
	XMFLOAT4 ambiDef = XMFLOAT4(0.07f, 0.07f, 0.0f, 0.1f);
	XMFLOAT3 attenuDef = XMFLOAT3(0.0f, 0.022f, 0.005f);

	//Default sky light
	XMFLOAT4 skydiffuseDef = XMFLOAT4(0.45f, 0.6f, 0.75f, 1.0f);
	XMFLOAT4 skyAmbiDef = XMFLOAT4(0.25f, 0.25f, 0.6f, 1.0f);
	XMFLOAT3 skyPosDef = XMFLOAT3(-10.0f, 15.0f, 30.0f);
	XMFLOAT3 directionDef = XMFLOAT3(-0.85f, -0.5f, 0.0f);

	//Default spot light
	XMFLOAT4 spotDiffuseDef = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT3 spotAttenuDef = XMFLOAT3(0.06f, 0.016f, 0.0f);
	XMFLOAT3 spotPosDef = XMFLOAT3(-76.0f, 0.0f, 35.0f);
	XMFLOAT3 spotDirectionDef = XMFLOAT3(0.0f, 0.5f, 0.9f);
	XMFLOAT4 spotAmbiDef = XMFLOAT4(0.05f, 0.05f, 0.0f, 0.1f);
	XMFLOAT4 specularColourDef = XMFLOAT4(0.2f, 0.2f, 0.0f, 1.0f);
	float specularPowerDef = 1.0f;
	float spotRangeDef = 30.0f;
	float spotConeDef = 1.2f;

	//render functions
	void wireFrameRender();
	void depthRender();
	void firstRender();
	void verticalEdge();
	void finalPass();

private:
	verManipShader* groundShader = 0;
	texShader* textureShader = 0;
	LightShader* lightShader = 0;
	DepthShader* depthShader = 0;
	verEdgeShader* verEdgeShader1 =0;
	verManipDepthShader* verManipDepthShader1 = 0;

	PlaneMesh* ground = 0;
	AModel* model[5] = { 0, 0, 0, 0, 0};

	SphereMesh* pointLightSphere = 0;
	SphereMesh* spotLightSphere = 0;	
	SphereMesh* skyLightSphere = 0;

	myLight* skylight = 0;
	myLight* pointlight = 0;
	myLight* spotlight = 0;

	OrthoMesh* orthomesh = 0;
	RenderTexture* renderTexture = 0;
	RenderTexture* vertEdgeTexture = 0;

	ShadowMap* shadowMap = 0;
};

#endif