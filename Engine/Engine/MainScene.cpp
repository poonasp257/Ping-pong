#include "stdafx.h"
#include "MainScene.h"


MainScene::MainScene() : Scene() {
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;
}


MainScene::MainScene(const MainScene& other)
{
}


MainScene::~MainScene()
{
}


bool MainScene::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Initialize the Direct3D object.
	result = direct3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->setPosition(0.0f, 0.0f, -40.0f);

	const char* fileNames[] = {
		"../Engine/data/doll.obj",
		"../Engine/data/sword.obj",
		"../Engine/data/M1911.obj"
	};

	const WCHAR* textures[] = {
		L"../Engine/data/t_doll.dds",
		L"../Engine/data/t_sword.dds",
		L"../Engine/data/t_M1911.dds"
	};

	// Create the model object.
	for (int i = 0; i < 5; ++i)
	{
		ModelClass* newModel = new ModelClass;
		result = newModel->Initialize(direct3D->getDevice(), L"../Engine/data/seafloor.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_Models.push_back(newModel);
	}

	// Create the model object.
	for (int i = 0; i < 3; ++i)
	{
		ModelClass* newModel = new ModelClass;
		result = newModel->Initialize(direct3D->getDevice(), fileNames[i], textures[i]);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_Models.push_back(newModel);
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(direct3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_LightShader->Initialize(direct3D->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	D3DXMATRIX baseViewMatrix;

	m_Camera->render();
	m_Camera->getViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	// Initialize the text object.
	result = m_Text->Initialize(direct3D->getDevice(), direct3D->getDeviceContext(), hwnd, screenWidth,
		screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	result = m_Text->SetNumOfObjects(m_Models.size(), direct3D->getDeviceContext());
	if (!result)
	{
		return false;
	}

	int numOfPolygons = 0;
	for (auto model : m_Models) numOfPolygons += model->GetPolygonCount();

	result = m_Text->SetNumOfPolygons(numOfPolygons, direct3D->getDeviceContext());
	if (!result)
	{
		return false;
	}

	m_skyBox = new SkyBox;
	result = m_skyBox->initialize(direct3D->getDevice(), hwnd, L"../Engine/data/skymap.dds");
	if (!result)
	{
		return false;
	}

	return true;
}


bool MainScene::frame(int fps, float frameTime, int cpu) {
	bool result;

	if (Input.GetKeyDown(DIK_1)) {
		m_LightShader->SetFilter(direct3D->getDevice(), D3D11_FILTER_MIN_MAG_MIP_POINT);
	}
	else if (Input.GetKeyDown(DIK_2)) {
		m_LightShader->SetFilter(direct3D->getDevice(), D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT);
	}
	else if (Input.GetKeyDown(DIK_3)) {
		m_LightShader->SetFilter(direct3D->getDevice(), D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	}
	else if (Input.GetKeyDown(DIK_4)) {
		m_LightShader->SetFilter(direct3D->getDevice(), D3D11_FILTER_ANISOTROPIC);
	}

	m_Camera->frame(frameTime);

	if (Input.GetKeyDown(DIK_F1)) {
		m_Text->TurnOnOffRenderInfo();
	}

	result = m_Text->SetCameraPosition(m_Camera->getPosition(), direct3D->getDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the frames per second.
	result = m_Text->SetFPS(fps, direct3D->getDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCPU(cpu, direct3D->getDeviceContext());
	if (!result)
	{
		return false;
	}



	// Render the graphics scene.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}


bool MainScene::render()
{
	bool result;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	D3DXMATRIX newProjectionMatrix;

	// Clear the buffers to begin the scene.
	direct3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->getViewMatrix(viewMatrix);
	m_Camera->getProjectionMatrix(newProjectionMatrix);
			  
	direct3D->getWorldMatrix(worldMatrix);
	direct3D->getProjectionMatrix(projectionMatrix);
	direct3D->getOrthoMatrix(orthoMatrix);

	D3DXMATRIX objMatrix[8], matRot;
	for (int i = 0; i < 8; ++i) {
		objMatrix[i] = worldMatrix;
	}

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixTranslation(&objMatrix[1], 0.0f, -10.0f, 10.0f);
	D3DXMatrixRotationX(&matRot, D3DXToRadian(90.0f));
	D3DXMatrixMultiply(&objMatrix[1], &objMatrix[1], &matRot);

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixTranslation(&objMatrix[2], 0.0f, 10.0f, 10.0f);
	D3DXMatrixRotationX(&matRot, D3DXToRadian(-90.0f));
	D3DXMatrixMultiply(&objMatrix[2], &objMatrix[2], &matRot);

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixTranslation(&objMatrix[3], 10.0f, 0.0f, 10.0f);
	D3DXMatrixRotationY(&matRot, D3DXToRadian(90.0f));
	D3DXMatrixMultiply(&objMatrix[3], &objMatrix[3], &matRot);

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixTranslation(&objMatrix[4], -10.0f, 0.0f, 10.0f);
	D3DXMatrixRotationY(&matRot, D3DXToRadian(-90.0f));
	D3DXMatrixMultiply(&objMatrix[4], &objMatrix[4], &matRot);

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixTranslation(&objMatrix[5], -5.0f, -9.0f, -5.0f);

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixTranslation(&objMatrix[6], 0.0f, -9.0f, -5.0f);

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixTranslation(&objMatrix[7], 5.0f, -9.0f, -5.0f);

	for (int i = 0; i < m_Models.size(); ++i) {
		m_Models[i]->Render(direct3D->getDeviceContext());
		if (i < 5) {
			result = m_TextureShader->Render(direct3D->getDeviceContext(), m_Models[i]->GetIndexCount(), objMatrix[i], viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture());
			if (!result) {
				return false;
			}

			continue;
		}

		// Render the model using the texture shader.
		result = m_LightShader->Render(direct3D->getDeviceContext(), m_Models[i]->GetIndexCount(), objMatrix[i], viewMatrix, projectionMatrix,
			m_Models[i]->GetTexture(), m_Light->GetDirection(),
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->getPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if (!result) {
			return false;
		}
	}

	//Reset sphereWorld
	D3DXMATRIX sphereWorld, sphereScale, sphereTranslation;
	D3DXVECTOR3 cameraPos = m_Camera->getPosition();
	D3DXMatrixIdentity(&sphereWorld);

	//Define sphereWorld's world space matrix
	D3DXMatrixScaling(&sphereScale, 5.0f, 5.0f, 5.0f);
	//Make sure the sphere is always centered around camera
	D3DXMatrixTranslation(&sphereTranslation, cameraPos.x, cameraPos.y, cameraPos.z);

	//Set sphereWorld's world space using the transformations
	sphereWorld = sphereScale * sphereTranslation;

	m_skyBox->render(direct3D->getDeviceContext(), sphereWorld, viewMatrix, newProjectionMatrix);


	// Turn off the Z buffer to begin all 2D rendering.
	direct3D->turnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	direct3D->turnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(direct3D->getDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	direct3D->turnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	direct3D->turnZBufferOn();

	// Present the rendered scene to the screen.
	direct3D->endScene();

	return true;
}