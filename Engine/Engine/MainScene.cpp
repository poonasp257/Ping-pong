#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene() : Scene(), shaderManager(new ShaderManager),
	mainCamera(new Camera), skyBox(new SkyBox),
	light(new Light), text(nullptr) {
	text = std::make_unique<Text>(shaderManager.get());
}

MainScene::~MainScene() {

}

bool MainScene::initialize(bool isFullScreen, int screenWidth, int screenHeight, HWND hwnd) {
	bool result;

	result = direct3D->initialize(screenWidth, screenHeight, true, hwnd, isFullScreen, 1000.0f, 0.1f);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	mainCamera->setPosition(0.0f, 0.0f, -40.0f);

	const char* fileNames[] = {
		"../Engine/data/doll.obj",
		"../Engine/data/sword.obj"
	};

	const WCHAR* textures[] = {
		L"../Engine/data/t_doll.dds",
		L"../Engine/data/t_sword.dds"
	};

	for (int i = 0; i < 5; ++i) {
		Model* newModel = new Model;
		result = newModel->initialize(direct3D->getDevice(), L"../Engine/data/seafloor.dds");
		if (!result) {
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_Models.push_back(newModel);
	}

	for (int i = 0; i < 2; ++i) {
		Model* newModel = new Model;
		result = newModel->initialize(direct3D->getDevice(), fileNames[i], textures[i]);
		if (!result) {
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_Models.push_back(newModel);
	}

	shaderManager->initialize(direct3D->getDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	light->setAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.0f, 0.0f, 1.0f);
	light->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->setSpecularPower(32.0f);

	D3DXMATRIX baseViewMatrix;

	mainCamera->render();
	mainCamera->getViewMatrix(baseViewMatrix);
	
	result = text->initialize(direct3D->getDevice(), direct3D->getDeviceContext(), hwnd, screenWidth,
		screenHeight, baseViewMatrix);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	result = text->setNumOfObjects(m_Models.size(), direct3D->getDeviceContext());
	if (!result) return false;

	int numOfPolygons = 0;
	for (auto model : m_Models) numOfPolygons += model->getPolygonCount();

	result = text->setNumOfPolygons(numOfPolygons, direct3D->getDeviceContext());
	if (!result) return false;

	result = skyBox->initialize(direct3D->getDevice(), hwnd, L"../Engine/data/skymap.dds");
	if (!result) return false;

	return true;
}

bool MainScene::frame(int fps, float frameTime, int cpu) {
	bool result;

	/*if (Input.GetKeyDown(DIK_1)) {
		lightShader->setFilter(direct3D->getDevice(), D3D11_FILTER_MIN_MAG_MIP_POINT);
	}
	else if (Input.GetKeyDown(DIK_2)) {
		lightShader->setFilter(direct3D->getDevice(), D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT);
	}
	else if (Input.GetKeyDown(DIK_3)) {
		lightShader->setFilter(direct3D->getDevice(), D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	}
	else if (Input.GetKeyDown(DIK_4)) {
		lightShader->setFilter(direct3D->getDevice(), D3D11_FILTER_ANISOTROPIC);
	}*/

	mainCamera->frame(frameTime);

	if (Input.GetKeyDown(DIK_F1)) {
		text->turnOnOffRenderInfo();
	}

	result = text->setCameraPosition(mainCamera->getPosition(), direct3D->getDeviceContext());
	if (!result) return false;

	result = text->setFPS(fps, direct3D->getDeviceContext());
	if (!result) return false;

	result = text->setCPU(cpu, direct3D->getDeviceContext());
	if (!result) return false;

	result = render();
	if (!result) return false;

	return true;
}

bool MainScene::render() {
	bool result;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX newProjectionMatrix;

	direct3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	mainCamera->render();

	mainCamera->getViewMatrix(viewMatrix);
	mainCamera->getProjectionMatrix(newProjectionMatrix);
			  
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
 
	for (int i = 0; i < 5; ++i) {
		m_Models[i]->render(direct3D->getDeviceContext());
		if (i < 5) {
			result = shaderManager->getTextureShader()->render(
				direct3D->getDeviceContext(), m_Models[i]->getIndexCount(), objMatrix[i], viewMatrix, projectionMatrix,
				m_Models[i]->getTexture());
			if (!result) return false;

			continue;
		}

		result = shaderManager->getLightShader()->render(direct3D->getDeviceContext(), 
			m_Models[i]->getIndexCount(), objMatrix[i], viewMatrix, projectionMatrix,
			m_Models[i]->getTexture(), light.get(), mainCamera->getPosition());
		if (!result) return false;
	}
	
	D3DXMATRIX sphereWorld, sphereScale, sphereTranslation;
	D3DXVECTOR3 cameraPos = mainCamera->getPosition();
	D3DXMatrixIdentity(&sphereWorld);

	D3DXMatrixScaling(&sphereScale, 5.0f, 5.0f, 5.0f);
	D3DXMatrixTranslation(&sphereTranslation, cameraPos.x, cameraPos.y, cameraPos.z);
	sphereWorld = sphereScale * sphereTranslation;

	skyBox->render(direct3D->getDeviceContext(), sphereWorld, viewMatrix, newProjectionMatrix);

	direct3D->turnZBufferOff();
	direct3D->turnOnAlphaBlending();

	text->render(direct3D->getDeviceContext(), worldMatrix, orthoMatrix);
	
	direct3D->turnOffAlphaBlending();
	direct3D->turnZBufferOn();

	direct3D->endScene();

	return true;
}