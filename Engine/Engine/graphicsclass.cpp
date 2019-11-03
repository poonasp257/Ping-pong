////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -40.0f);
	
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
		result = newModel->Initialize(m_D3D->GetDevice(), L"../Engine/data/seafloor.dds");
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
		result = newModel->Initialize(m_D3D->GetDevice(), fileNames[i], textures[i]);
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
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
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

	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth,
		screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	result = m_Text->SetNumOfObjects(m_Models.size(), m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	int numOfPolygons = 0;
	for (auto model : m_Models) numOfPolygons += model->GetPolygonCount();

	result = m_Text->SetNumOfPolygons(numOfPolygons, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	m_skyBox = new SkyBoxClass;
	result = m_skyBox->Initialize(m_D3D->GetDevice(), hwnd, L"../Engine/data/skymap.dds");
	if (!result)
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_skyBox)
	{
		m_skyBox->Shutdown();
		delete m_skyBox;
		m_skyBox = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}


	if (m_TextureShader)
	{
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	for (auto it = m_Models.begin(); it != m_Models.end(); ) {
		if (*it) {
			(*it)->Shutdown();
			delete *it;
			it = m_Models.erase(it);
		}
		else ++it;
	}


	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(int fps, float frameTime, int cpu) {
	bool result;
	
	if (Input.GetKeyDown(DIK_1)) {
		m_LightShader->SetFilter(m_D3D->GetDevice(), D3D11_FILTER_MIN_MAG_MIP_POINT);
	}
	else if(Input.GetKeyDown(DIK_2)) {
		m_LightShader->SetFilter(m_D3D->GetDevice(), D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT);
	}
	else if (Input.GetKeyDown(DIK_3)) {
		m_LightShader->SetFilter(m_D3D->GetDevice(), D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	}
	else if (Input.GetKeyDown(DIK_4)) {
		m_LightShader->SetFilter(m_D3D->GetDevice(), D3D11_FILTER_ANISOTROPIC);
	}
	
	m_Camera->Frame(frameTime);
	
	if (Input.GetKeyDown(DIK_F1)) {
		m_Text->TurnOnOffRenderInfo();
	} 
	
	result = m_Text->SetCameraPosition(m_Camera->GetPosition(), m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the frames per second.
	result = m_Text->SetFPS(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCPU(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}



	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	bool result;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	D3DXMATRIX newProjectionMatrix;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Camera->GetProjectionMatrix(newProjectionMatrix);

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	   	   
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
		m_Models[i]->Render(m_D3D->GetDeviceContext());
		if (i < 5) {
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), objMatrix[i], viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture());
			if (!result) {
				return false;
			}

			continue;
		}

		// Render the model using the texture shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), objMatrix[i], viewMatrix, projectionMatrix,
			m_Models[i]->GetTexture(), m_Light->GetDirection(),
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if (!result) {
			return false;
		}
	}

	//Reset sphereWorld
	D3DXMATRIX sphereWorld, sphereScale, sphereTranslation;
	D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
	D3DXMatrixIdentity(&sphereWorld);

	//Define sphereWorld's world space matrix
	D3DXMatrixScaling(&sphereScale, 5.0f, 5.0f, 5.0f);
	//Make sure the sphere is always centered around camera
	D3DXMatrixTranslation(&sphereTranslation, cameraPos.x, cameraPos.y, cameraPos.z);

	//Set sphereWorld's world space using the transformations
	sphereWorld = sphereScale * sphereTranslation;

	m_skyBox->Render(m_D3D->GetDeviceContext(), sphereWorld, viewMatrix, newProjectionMatrix);


	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();
	
	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}