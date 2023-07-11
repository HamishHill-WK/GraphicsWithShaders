#include "App1.h"

App1::App1()
{
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Initalise scene variables.
	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	textureMgr->loadTexture(L"myHeightMap", L"res/myHeightMap.png");	
	textureMgr->loadTexture(L"snowTexture", L"res/snowTexture.png");
	textureMgr->loadTexture(L"treeTex2", L"res/treeTex2.png");

	for(int i = 0; i < 5; i++)
		model[i] = new AModel(renderer->getDevice(), "res/por_tree.obj");

	ground = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());	
	pointLightSphere = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	spotLightSphere = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());	
	skyLightSphere = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	groundShader = new verManipShader(renderer->getDevice(), hwnd);
	textureShader = new texShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	verEdgeShader1 = new verEdgeShader(renderer->getDevice(), hwnd);

	orthomesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth , screenHeight);
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	vertEdgeTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	verManipDepthShader1 = new verManipDepthShader(renderer->getDevice(), hwnd);

	// Variables for defining shadow map
	int shadowmapWidth = screenWidth;
	int shadowmapHeight = screenHeight;
	int sceneWidth = 100;
	int sceneHeight = 100;

	// This is your shadow map
	shadowMap = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);

	renderSphere = false;
	edgeEnabled = true;
	showNorms = false;

	skylight = new myLight();
	skylight->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	//pointlight variables
	pos = posDef;
	ambi = ambiDef;
	attenu = attenuDef;
	pointdiffuse = pointdiffuseDef;

	//skylight variables 
	skydiffuse = skydiffuseDef;
	skyAmbi = skyAmbiDef;
	skyPos = skyPosDef;
	direction = directionDef;

	//spotlight variables
	spotAmbi = spotAmbiDef;
	spotDiffuse = spotDiffuseDef;
	spotDirection = spotDirectionDef;
	spotPos = spotPosDef;
	specularColour = specularColourDef;
	specularPower = specularPowerDef;
	spotAttenu = spotAttenuDef;
	spotCone = spotConeDef;
	spotRange = spotRangeDef;

	//object positions
	treePos1 = treePos1Def;
	treePos2 = treePos2Def;
	treePos3 = treePos3Def;
	treePos4 = treePos4Def;
	treePos5 = treePos5Def;
	groundPos = groundPosDef;

	skylight->setDiffuseColour(skydiffuse.x, skydiffuse.y, skydiffuse.z, skydiffuse.w);
	skylight->setAmbientColour(skyAmbi.x, skyAmbi.y, skyAmbi.z, skyAmbi.w);
	skylight->setDirection(direction.x, direction.y, directionDef.z);
	skylight->setPosition(skyPos.x, skyPos.y, skyPos.z);

	pointlight = new myLight();
	pointlight->setAmbientColour(ambi.w, ambi.x, ambi.y, ambi.z);
	pointlight->setDiffuseColour(pointdiffuse.x, pointdiffuse.y, pointdiffuse.z, pointdiffuse.w);
	pointlight->setPosition(pos.x, pos.y, pos.z);
	pointlight->setAtten(attenu.x, attenu.y, attenu.z);

	spotlight = new myLight();
	spotlight->setAmbientColour(spotAmbi.w, spotAmbi.x, spotAmbi.y, spotAmbi.z);
	spotlight->setDiffuseColour(spotDiffuse.w, spotDiffuse.x, spotDiffuse.y, spotDiffuse.z);
	spotlight->setPosition(spotPos.x, spotPos.y, spotPos.z);
	spotlight->setAtten(spotAttenu.x, spotAttenu.y, spotAttenu.z);
	spotlight->setDirection(spotDirection.x, spotDirection.y, spotDirection.z);
	spotlight->setSpecularColour(specularColour.w, specularColour.x, specularColour.y, specularColour.z);
	spotlight->setSpecularPower(specularPower);
	spotlight->setCone(spotCone);
	spotlight->setRange(spotRange);
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	if (ground)
	{
		delete ground;
		ground = 0;
	}

	if (pointLightSphere)
	{
		delete pointLightSphere;
		pointLightSphere = 0;
	}	
	
	if (spotLightSphere)
	{
		delete spotLightSphere;
		spotLightSphere = 0;
	}
		
	if (skyLightSphere)
	{
		delete skyLightSphere;
		skyLightSphere = 0;
	}

	if (renderTexture)
	{
		delete renderTexture;
		renderTexture = 0;
	}

	if (vertEdgeTexture)
	{
		delete vertEdgeTexture;
		vertEdgeTexture = 0;
	}

	if (orthomesh)
	{
		delete orthomesh;
		orthomesh = 0;
	}

	if (skylight)
	{
		delete skylight;
		skylight = 0;
	}

	if (pointlight)
	{
		delete  pointlight;
		pointlight = 0;
	}	
	
	if (spotlight)
	{
		delete  spotlight;
		spotlight = 0;
	}

	for (int i = 0; i < 5; i++)
	{
		if (model[i])
		{
			delete model[i];
			model[i] = 0;
		}
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
	}

	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}

	if (groundShader)
	{
		delete groundShader;
		groundShader = 0;
	}

	if (verEdgeShader1)
	{
		delete verEdgeShader1;
		verEdgeShader1 = 0;
	}

	if (depthShader)
	{
		delete depthShader;
		depthShader = 0;
	}

	if (verManipDepthShader1)
	{
		delete verManipDepthShader1;
		verManipDepthShader1 = 0;
	}	
	
	if (verEdgeShader1)
	{
		delete verEdgeShader1;
		verEdgeShader1 = 0;
	}

	if (shadowMap)
	{
		delete shadowMap;
		shadowMap = 0;
	}
}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	//set lighting variables to modifiable variables
	pointlight->setPosition(pos.x, pos.y, pos.z);
	pointlight->setDiffuseColour(pointdiffuse.x, pointdiffuse.y, pointdiffuse.z, pointdiffuse.w);
	pointlight->setAmbientColour(ambi.x, ambi.y, ambi.z, ambi.w);
	pointlight->setAtten(attenu.x, attenu.y, attenu.z);

	skylight->setDirection(direction.x, direction.y, direction.z);
	skylight->setDiffuseColour(skydiffuse.x, skydiffuse.y, skydiffuse.z, skydiffuse.w);
	skylight->setAmbientColour(skyAmbi.x, skyAmbi.y, skyAmbi.z, skyAmbi.w);
	skylight->setPosition(skyPos.x, skyPos.y, skyPos.z);	
	
	spotlight->setDirection(spotDirection.x, spotDirection.y, spotDirection.z);
	spotlight->setDiffuseColour(spotDiffuse.x, spotDiffuse.y, spotDiffuse.z, spotDiffuse.w);
	spotlight->setAmbientColour(spotAmbi.x, spotAmbi.y, spotAmbi.z, spotAmbi.w);
	spotlight->setPosition(spotPos.x, spotPos.y, spotPos.z);
	spotlight->setSpecularColour(specularColour.x, specularColour.y, specularColour.z, specularColour.w);
	spotlight->setSpecularPower(specularPower);
	spotlight->setAtten(spotAttenu.x, spotAttenu.y, spotAttenu.z);
	spotlight->setRange(spotRange);
	spotlight->setCone(spotCone);

	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	camera->update();

	renderer->setBackBufferRenderTarget();

	if (wireframeToggle)
		wireFrameRender();

	if (!wireframeToggle) {
		depthRender();
		firstRender();

		if (edgeEnabled)
			verticalEdge();

		finalPass();
	}
	gui();

	renderer->endScene();

	return true;
}

//render models in scene for wireframe conversion
//this is done in a seperate render function as normal scene render goes to a render texture. 
//wireframe mode does not show full scene when used on a rendertexture.
void App1::wireFrameRender()
{
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	worldMatrix = XMMatrixTranslation(groundPos.x, groundPos.y, groundPos.z);
	ground->sendData(renderer->getDeviceContext());
	groundShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"myHeightMap"), textureMgr->getTexture(L"snowTexture"), NULL, skylight, pointlight, spotlight, showNorms);
	groundShader->render(renderer->getDeviceContext(), ground->getIndexCount());
	worldMatrix = XMMatrixTranslation(-groundPos.x, -groundPos.y, -groundPos.z);

	worldMatrix = XMMatrixTranslation(treePos1.x, treePos1.y, treePos1.z);
	model[0]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), NULL, pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[0]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos1.x, -treePos1.y, -treePos1.z);

	worldMatrix = XMMatrixTranslation(treePos2.x, treePos2.y, treePos2.z);
	model[1]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), NULL, pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[1]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos2.x, -treePos2.y, -treePos2.z);

	worldMatrix = XMMatrixTranslation(treePos3.x, treePos3.y, treePos3.z);
	model[2]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), NULL, pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[2]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos3.x, -treePos3.y, -treePos3.z);

	worldMatrix = XMMatrixTranslation(treePos4.x, treePos4.y, treePos4.z);
	model[3]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[3]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos4.x, -treePos4.y, -treePos4.z);

	worldMatrix = XMMatrixTranslation(treePos5.x, treePos5.y, treePos5.z);
	model[4]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[4]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos5.x, -treePos5.y, -treePos5.z);

	if (renderSphere)
	{
		worldMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);
		pointLightSphere->sendData(renderer->getDeviceContext());
		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"snowTexture"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
		lightShader->render(renderer->getDeviceContext(), pointLightSphere->getIndexCount());		worldMatrix = XMMatrixTranslation(pointlight->getPosition().x, pointlight->getPosition().y, pointlight->getPosition().z);
		worldMatrix = XMMatrixTranslation(-pos.x, -pos.y, -pos.z);

		worldMatrix = XMMatrixTranslation(spotPos.x, spotPos.y, spotPos.z);
		spotLightSphere->sendData(renderer->getDeviceContext());
		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"snowTexture"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
		lightShader->render(renderer->getDeviceContext(), spotLightSphere->getIndexCount());
		worldMatrix = XMMatrixTranslation(-spotPos.x, -spotPos.y, -spotPos.z);

		worldMatrix = XMMatrixTranslation(skyPos.x, skyPos.y, skyPos.z);
		skyLightSphere->sendData(renderer->getDeviceContext());
		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"snowTexture"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
		lightShader->render(renderer->getDeviceContext(), skyLightSphere->getIndexCount());
		worldMatrix = XMMatrixTranslation(-skyPos.x, -skyPos.y, -skyPos.z);
	}
}

//objects are first passed through depth shader to generate shadow map
void App1::depthRender()
{
	shadowMap->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	// get the world, view, and projection matrices from the camera and d3d objects.
	skylight->generateViewMatrix();
	XMMATRIX lightViewMatrix = skylight->getViewMatrix();
	XMMATRIX lightProjectionMatrix = skylight->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//vertex manipulated geometry requires its own depth shader which passes in the height map which will be applied to it.
	//this is done so that the ground object is not rendered as a flat plane on the shadow map.
	worldMatrix = XMMatrixTranslation(groundPos.x, groundPos.y, groundPos.z);
	ground->sendData(renderer->getDeviceContext());
	verManipDepthShader1->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, textureMgr->getTexture(L"myHeightMap"));
	verManipDepthShader1->render(renderer->getDeviceContext(), ground->getIndexCount());
	worldMatrix = XMMatrixTranslation(-groundPos.x, -groundPos.y, -groundPos.z);
	
	worldMatrix = XMMatrixTranslation(treePos1.x, treePos1.y, treePos1.z);
	model[0]->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model[0]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos1.x, -treePos1.y, -treePos1.z);

	worldMatrix = XMMatrixTranslation(treePos2.x, treePos2.y, treePos2.z);
	model[1]->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model[1]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos2.x, -treePos2.y, -treePos2.z);

	worldMatrix = XMMatrixTranslation(treePos3.x, treePos3.y, treePos3.z);
	model[2]->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model[2]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos3.x, -treePos3.y, -treePos3.z);

	worldMatrix = XMMatrixTranslation(treePos4.x, treePos4.y, treePos4.z);
	model[3]->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model[3]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos4.x, -treePos4.y, -treePos4.z);

	worldMatrix = XMMatrixTranslation(treePos5.x, treePos5.y, treePos5.z);
	model[4]->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model[4]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos5.x, -treePos5.y, -treePos5.z);

	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::firstRender()
{
	renderTexture->setRenderTarget(renderer->getDeviceContext());
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.5f, 1.0f);

	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	//ground shader applies vertex manipulation, texturing, lighting and shadows to object.
	worldMatrix = XMMatrixTranslation(groundPos.x, groundPos.y, groundPos.z);
	ground->sendData(renderer->getDeviceContext());
	groundShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"myHeightMap"), textureMgr->getTexture(L"snowTexture"), shadowMap->getDepthMapSRV(), skylight, pointlight, spotlight ,showNorms);
	groundShader->render(renderer->getDeviceContext(), ground->getIndexCount());
	worldMatrix = XMMatrixTranslation(-groundPos.x, -groundPos.y, -groundPos.z);

	//light shader applies lighting, texturing and shadows to objects.
	worldMatrix = XMMatrixTranslation(treePos1.x, treePos1.y, treePos1.z);
	model[0]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[0]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos1.x, -treePos1.y, -treePos1.z);

	worldMatrix = XMMatrixTranslation(treePos2.x, treePos2.y, treePos2.z);
	model[1]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[1]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos2.x, -treePos2.y, -treePos2.z);

	worldMatrix = XMMatrixTranslation(treePos3.x, treePos3.y, treePos3.z);
	model[2]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[2]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos3.x, -treePos3.y, -treePos3.z);

	worldMatrix = XMMatrixTranslation(treePos4.x, treePos4.y, treePos4.z);
	model[3]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[3]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos4.x, -treePos4.y, -treePos4.z);

	worldMatrix = XMMatrixTranslation(treePos5.x, treePos5.y, treePos5.z);
	model[4]->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"treeTex2"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
	lightShader->render(renderer->getDeviceContext(), model[4]->getIndexCount());
	worldMatrix = XMMatrixTranslation(-treePos5.x, -treePos5.y, -treePos5.z);

	if (renderSphere)	//if the render spehere option is turned on in the imGui() then sphere objects will be rendered at the location of light sources in the scene.
	{
		worldMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);
		pointLightSphere->sendData(renderer->getDeviceContext());
		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"snowTexture"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
		lightShader->render(renderer->getDeviceContext(), pointLightSphere->getIndexCount());		worldMatrix = XMMatrixTranslation(pointlight->getPosition().x, pointlight->getPosition().y, pointlight->getPosition().z);
		worldMatrix = XMMatrixTranslation(-pos.x, -pos.y, -pos.z);

		worldMatrix = XMMatrixTranslation(spotPos.x, spotPos.y, spotPos.z);
		spotLightSphere->sendData(renderer->getDeviceContext());
		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"snowTexture"), shadowMap->getDepthMapSRV(), pointlight, skylight,spotlight, showNorms);
		lightShader->render(renderer->getDeviceContext(), spotLightSphere->getIndexCount());
		worldMatrix = XMMatrixTranslation(-spotPos.x, -spotPos.y, -spotPos.z);
	
		worldMatrix = XMMatrixTranslation(skyPos.x, skyPos.y, skyPos.z);
		skyLightSphere->sendData(renderer->getDeviceContext());
		lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"snowTexture"), shadowMap->getDepthMapSRV(), pointlight, skylight, spotlight, showNorms);
		lightShader->render(renderer->getDeviceContext(), skyLightSphere->getIndexCount());
		worldMatrix = XMMatrixTranslation(-skyPos.x, -skyPos.y, -skyPos.z);
	}

	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();

	orthomesh->sendData(renderer->getDeviceContext());
	
	//the scene is then added to this orthomesh for post processing. 
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap->getDepthMapSRV());
	textureShader->render(renderer->getDeviceContext(), orthomesh->getIndexCount());

	renderer->setZBuffer(true);

	renderer->setBackBufferRenderTarget();
}

void App1::verticalEdge()
{
	//this pass applies edge detection to the full scene. 
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeY = (float)vertEdgeTexture->getTextureHeight();
	float screenSizeX = (float)vertEdgeTexture->getTextureWidth();
	vertEdgeTexture->setRenderTarget(renderer->getDeviceContext());
	vertEdgeTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 1.0f, 1.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = vertEdgeTexture->getOrthoMatrix();

	// Render for Vertical Blur
	renderer->setZBuffer(false);
	orthomesh->sendData(renderer->getDeviceContext());
	verEdgeShader1->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, renderTexture->getShaderResourceView(), screenSizeY, screenSizeX);
	verEdgeShader1->render(renderer->getDeviceContext(), orthomesh->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::finalPass()
{
	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	if (edgeEnabled)	//if edge detection is enabled, render the scene with edge detection applied to it.
	{
		orthomesh->sendData(renderer->getDeviceContext());
		textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, vertEdgeTexture->getShaderResourceView());
		textureShader->render(renderer->getDeviceContext(), orthomesh->getIndexCount());
		renderer->setZBuffer(true);
	}

	if (!edgeEnabled)	//if edge detection is not enabled, render the scene before edge detection was applied.
	{
		orthomesh->sendData(renderer->getDeviceContext());
		textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture->getShaderResourceView());
		textureShader->render(renderer->getDeviceContext(), orthomesh->getIndexCount());
		renderer->setZBuffer(true);
	}
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);	
	ImGui::Checkbox("Edge Detection ", &edgeEnabled);	
	ImGui::Checkbox("Show displacement map normals ", &showNorms);
	ImGui::Checkbox("Show Light Sources", &renderSphere);

	if (ImGui::Button("Reset to default values"))	//resets all variables to their default setting. Default settings are decalred in "App1.h"
	{
		//point light variables
		pos = posDef;
		ambi = ambiDef;
		attenu = attenuDef;
		pointdiffuse = pointdiffuseDef;

		//skylight variables 
		skydiffuse = skydiffuseDef;
		skyAmbi = skyAmbiDef;
		skyPos = skyPosDef;
		direction = directionDef;

		//spotlight variables
		spotAmbi = spotAmbiDef;
		spotDiffuse = spotDiffuseDef;
		spotDirection = spotDirectionDef;
		spotPos = spotPosDef;
		specularColour = specularColourDef;
		specularPower = specularPowerDef;
		spotAttenu = spotAttenuDef;
		spotCone = spotConeDef;
		spotRange = spotRangeDef;

		//object positions
		treePos1 = treePos1Def;
		treePos2 = treePos2Def;
		treePos3 = treePos3Def;
		treePos4 = treePos4Def;
		treePos5 = treePos5Def;
		groundPos = groundPosDef;

		renderSphere = false;
		edgeEnabled = true;
		showNorms = false;
	}

	if (ImGui::CollapsingHeader("Hide Lights"))	//buttons to hide indiviual lights by setting diffuse + ambient values to (0, 0, 0, 0).
	{
		ImGui::Indent(10);
		if (ImGui::Button("Hide point light"))
		{
			ambi = XMFLOAT4(0, 0, 0, 0);
			pointdiffuse = XMFLOAT4(0, 0, 0, 0);
		}

		if (ImGui::Button("Hide sky light"))
		{
			skydiffuse = XMFLOAT4(0, 0, 0, 0);
			skyAmbi = XMFLOAT4(0, 0, 0, 0);
		}

		if (ImGui::Button("Hide spot light"))
		{
			spotAmbi = XMFLOAT4(0, 0, 0, 0);
			spotDiffuse = XMFLOAT4(0, 0, 0, 0);
			specularColour = XMFLOAT4(0, 0, 0, 0);
		}	

		if (ImGui::Button("Hide All lights"))
		{
			ambi = XMFLOAT4(0, 0, 0, 0);
			pointdiffuse = XMFLOAT4(0, 0, 0, 0);

			skydiffuse = XMFLOAT4(0, 0, 0, 0);
			skyAmbi = XMFLOAT4(0, 0, 0, 0);

			spotAmbi = XMFLOAT4(0, 0, 0, 0);
			spotDiffuse = XMFLOAT4(0, 0, 0, 0);
			specularColour = XMFLOAT4(0, 0, 0, 0);
		}

		if (ImGui::Button("Hide Ambient light"))
		{
			ambi = XMFLOAT4(0, 0, 0, 0);
			skyAmbi = XMFLOAT4(0, 0, 0, 0);
			spotAmbi = XMFLOAT4(0, 0, 0, 0);
		}
		ImGui::Indent(-10);
	}

	if (ImGui::CollapsingHeader("Spot light controls"))
	{
		ImGui::Indent(10);
		if (ImGui::CollapsingHeader("Spot Position_"))
		{
			ImGui::SliderFloat("Spot x pos", &spotPos.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Spot y pos", &spotPos.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Spot z pos", &spotPos.z, -100.0f, 100.0f);
		}

		if (ImGui::CollapsingHeader("Spot Direction_"))
		{
			ImGui::SliderFloat("Spot x direction", &spotDirection.x, -1.0f, 1.0f);
			ImGui::SliderFloat("Spot y direction", &spotDirection.y, -1.0f, 1.0f);
			ImGui::SliderFloat("Spot z direction", &spotDirection.z, -1.0f, 1.0f);
			ImGui::SliderFloat("Spot Range", &spotRange, 0.0f, 100.0f);
			ImGui::SliderFloat("Spot Cone", &spotCone, 0.0f, 10.0f);
		}

		if (ImGui::CollapsingHeader("Spot Attenuation_"))
		{
			ImGui::SliderFloat("Spot Attenuation x", &spotAttenu.x, 0.0f, 1.0f);
			ImGui::SliderFloat("Spot Attenuation y", &spotAttenu.y, 0.0f, 1.0f);
			ImGui::SliderFloat("Spot Attenuation z", &spotAttenu.z, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("SpotDiffuse Colour_"))
		{

			ImGui::SliderFloat("Spot Red ", &spotDiffuse.x, 0.0f, 1.0f);
			ImGui::SliderFloat("Spot Green ", &spotDiffuse.y, 0.0f, 1.0f);
			ImGui::SliderFloat("Spot Blue ", &spotDiffuse.z, 0.0f, 1.0f);
			ImGui::SliderFloat("Spot Alpha ", &spotDiffuse.w, 0.0f, 1.0f);
		}		
		
		if (ImGui::CollapsingHeader("Specular Diffuse Colour_"))
		{

			ImGui::SliderFloat("spec Red_", &specularColour.x, 0.0f, 1.0f);
			ImGui::SliderFloat("spec Green", &specularColour.y, 0.0f, 1.0f);
			ImGui::SliderFloat("spec Blue", &specularColour.z, 0.0f, 1.0f);
			ImGui::SliderFloat("spec Alpha_", &specularColour.w, 0.0f, 1.0f);
			ImGui::SliderFloat("Specular Power", &specularPower, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Spot Ambient Colour_"))
		{
			ImGui::SliderFloat("Spot Ambient Red_", &spotAmbi.x, 0.0f, 1.0f);
			ImGui::SliderFloat("Spot Ambient Green_", &spotAmbi.y, 0.0f, 1.0f);
			ImGui::SliderFloat("Spot Ambient Blue_", &spotAmbi.z, 0.0f, 1.0f);
			ImGui::SliderFloat("Spot Ambient Alpha_", &spotAmbi.w, 0.0f, 1.0f);
		}

		ImGui::Indent(-10);
	}

	if (ImGui::CollapsingHeader("Point light controls"))
	{
		ImGui::Indent(10);
		if (ImGui::CollapsingHeader("Point Position"))
		{
			ImGui::SliderFloat("Point x pos", &pos.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Point y pos", &pos.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Point z pos", &pos.z, -100.0f, 100.0f);
		}

		if (ImGui::CollapsingHeader("Point Attenuation"))
		{
			ImGui::SliderFloat("Point Attenuation x", &attenu.x, 0.0f, 1.0f);
			ImGui::SliderFloat("Point Attenuation y", &attenu.y, 0.0f, 1.0f);
			ImGui::SliderFloat("Point Attenuation z", &attenu.z, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Point Diffuse Colour"))
		{
			
			ImGui::SliderFloat("Point Red", &pointdiffuse.x, 0.0f, 1.0f);
			ImGui::SliderFloat("Point Green", &pointdiffuse.y, 0.0f, 1.0f);
			ImGui::SliderFloat("Point Blue", &pointdiffuse.z, 0.0f, 1.0f);
			ImGui::SliderFloat("Point Alpha", &pointdiffuse.w, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Point Ambient Colour"))
		{
			ImGui::SliderFloat("Point Ambient Red", &ambi.x, 0.0f, 1.0f);
			ImGui::SliderFloat("Point Ambient Green", &ambi.y, 0.0f, 1.0f);
			ImGui::SliderFloat("Point Ambient Blue", &ambi.z, 0.0f, 1.0f);
			ImGui::SliderFloat("Point Ambient Alpha", &ambi.w, 0.0f, 1.0f);
		}
	
		ImGui::Indent(-10);
	}
	
	if (ImGui::CollapsingHeader("Sky light controls"))
	{
		ImGui::Indent(10);
		if (ImGui::CollapsingHeader("Sky Position"))
		{
			ImGui::SliderFloat("Sky x pos  ", &skyPos.x, -10.0f, 30.0f);
			ImGui::SliderFloat("Sky y pos  ", &skyPos.y, -10.0f, 30.0f);
			ImGui::SliderFloat("Sky z pos  ", &skyPos.z, -10.0f, 30.0f);
		}

		if (ImGui::CollapsingHeader("Sky Direction"))
		{
			ImGui::SliderFloat("Sky Direction x", &direction.x, -1.0f, 1.0f);
			ImGui::SliderFloat("Sky Direction y", &direction.y, -1.0f, -0.5f);
			ImGui::SliderFloat("Sky Direction z", &direction.z, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Sky Diffuse Colour"))
		{
			ImGui::SliderFloat("Sky Red ", &skydiffuse.x, 0.0f, 1.0f);
			ImGui::SliderFloat("Sky Green ", &skydiffuse.y, 0.0f, 1.0f);
			ImGui::SliderFloat("Sky Blue ", &skydiffuse.z, 0.0f, 1.0f);
			ImGui::SliderFloat("Sky Alpha ", &skydiffuse.w, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Sky Ambient Colour"))
		{
			ImGui::SliderFloat("Sky Ambient Red ", &skyAmbi.x, 0.0f, 1.0f);
			ImGui::SliderFloat("Sky Ambient Green ", &skyAmbi.y, 0.0f, 1.0f);
			ImGui::SliderFloat("Sky Ambient Blue ", &skyAmbi.z, 0.0f, 1.0f);
			ImGui::SliderFloat("Sky Ambient Alpha ", &skyAmbi.w, 0.0f, 1.0f);
		}
		ImGui::Indent(-10);
	}

	if (ImGui::CollapsingHeader("Object positions"))
	{
		ImGui::Indent(10);
		if (ImGui::CollapsingHeader("Tree 1 Position"))
		{
			ImGui::SliderFloat("Tree1 x pos", &treePos1.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree1 y pos", &treePos1.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree1 z pos", &treePos1.z, -100.0f, 100.0f);
		}		
		
		if (ImGui::CollapsingHeader("Tree 2 Position"))
		{
			ImGui::SliderFloat("Tree2 x pos", &treePos2.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree2 y pos", &treePos2.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree2 z pos", &treePos2.z, -100.0f, 100.0f);
		}		
		
		if (ImGui::CollapsingHeader("Tree 3 Position"))
		{
			ImGui::SliderFloat("Tree3 x pos", &treePos3.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree3 y pos", &treePos3.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree3 z pos", &treePos3.z, -100.0f, 100.0f);
		}			
		
		if (ImGui::CollapsingHeader("Tree 4 Position"))
		{
			ImGui::SliderFloat("Tree4 x pos", &treePos4.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree4 y pos", &treePos4.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree4 z pos", &treePos4.z, -100.0f, 100.0f);
		}			
		
		if (ImGui::CollapsingHeader("Tree 5 Position"))
		{
			ImGui::SliderFloat("Tree5 x pos", &treePos5.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree5 y pos", &treePos5.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Tree5 z pos", &treePos5.z, -100.0f, 100.0f);
		}		
		
		if (ImGui::CollapsingHeader("Ground Position"))
		{
			ImGui::SliderFloat("Ground x pos", &groundPos.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Ground y pos", &groundPos.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Ground z pos", &groundPos.z, -100.0f, 100.0f);
		}

		ImGui::Indent(-10);
	}

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

