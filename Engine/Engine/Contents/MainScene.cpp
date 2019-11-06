#include "stdafx.h"
#include "MainScene.h"
#include "Ball.h"
#include "PlayerController.h"
#include "EnemyController.h"

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

	device = direct3D->getDevice();
	deviceContext = direct3D->getDeviceContext();

	mainCamera->setPosition(0.0f, 0.0f, -40.0f);

	std::shared_ptr<GameObject> player = std::make_shared<Ball>(device, deviceContext, "Player");
	std::shared_ptr<GameObject> enemy = std::make_shared<Ball>(device, deviceContext, "Enemy");
	
	player->AddComponent<PlayerController>();
	enemy->AddComponent<EnemyController>();
	
	gameObjects.push_back(player);
	gameObjects.push_back(enemy);

	shaderManager->initialize(device, hwnd);
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
	
	result = text->initialize(hwnd, device, deviceContext, screenWidth,
		screenHeight, baseViewMatrix);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	result = text->setNumOfObjects(m_Models.size(), deviceContext);
	if (!result) return false;

	int numOfPolygons = 0;
	for (auto model : m_Models) numOfPolygons += model->getPolygonCount();

	result = text->setNumOfPolygons(numOfPolygons, deviceContext);
	if (!result) return false;

	result = skyBox->initialize(device, hwnd, L"../Engine/data/skymap.dds");
	if (!result) return false;

	return true;
}

bool MainScene::frame(int fps, float frameTime, int cpu) {
	bool result;

	if (Input.GetKeyDown(DIK_1)) {
		shaderManager->getLightShader()->setFilter(device, D3D11_FILTER_MIN_MAG_MIP_POINT);
	}
	else if (Input.GetKeyDown(DIK_2)) {
		shaderManager->getLightShader()->setFilter(device, D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT);
	}
	else if (Input.GetKeyDown(DIK_3)) {
		shaderManager->getLightShader()->setFilter(device, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	}
	else if (Input.GetKeyDown(DIK_4)) {
		shaderManager->getLightShader()->setFilter(device, D3D11_FILTER_ANISOTROPIC);
	}

	mainCamera->frame(frameTime);

	if (Input.GetKeyDown(DIK_F1)) {
		text->turnOnOffRenderInfo();
	}

	result = text->setCameraPosition(mainCamera->getPosition(), deviceContext);
	if (!result) return false;

	result = text->setFPS(fps, deviceContext);
	if (!result) return false;

	result = text->setCPU(cpu, deviceContext);
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

	//for (auto& gameObject : gameObjects) {
	//	gameObject->Update();

	//	/*auto lightShader = shaderManager->getLightShader();
	//	result = lightShader->render(deviceContext,
	//		m_Models[i]->getIndexCount(), objMatrix[i], viewMatrix, projectionMatrix,
	//		m_Models[i]->getTexture(), light.get(), mainCamera->getPosition());
	//	if (!result) return false;*/
	//}


	D3DXMATRIX sphereWorld, sphereScale, sphereTranslation;
	D3DXVECTOR3 cameraPos = mainCamera->getPosition();
	D3DXMatrixIdentity(&sphereWorld);

	D3DXMatrixScaling(&sphereScale, 5.0f, 5.0f, 5.0f);
	D3DXMatrixTranslation(&sphereTranslation, cameraPos.x, cameraPos.y, cameraPos.z);
	sphereWorld = sphereScale * sphereTranslation;

	skyBox->render(deviceContext, sphereWorld, viewMatrix, newProjectionMatrix);

	direct3D->turnZBufferOff();
	direct3D->turnOnAlphaBlending();

	text->render(deviceContext, worldMatrix, orthoMatrix);
	
	direct3D->turnOffAlphaBlending();
	direct3D->turnZBufferOn();

	direct3D->endScene();

	return true;
}