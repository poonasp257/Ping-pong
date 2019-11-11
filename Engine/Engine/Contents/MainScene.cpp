#include "stdafx.h"
#include "MainScene.h"
#include "BounceBall.h"
#include "PlayerController.h"
#include "EnemyController.h"
#include "WallController.h"

MainScene::MainScene() : Scene(), shaderManager(new ShaderManager),
	mainCamera(new Camera), skyBox(new SkyBox),
	directionalLight(new Light), pointLight(new Light),
	text(nullptr), bgm(new Sound) {
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

	bgm->Initialize(hwnd, "../Engine/data/BGM.wav");
	bgm->PlayWaveFile(-500, true);

	mainCamera->setPosition(0.0f, 60.0f, -60.0f);
	
	ground = std::make_shared<GameObject>(device, deviceContext, "Ground", "Props");
	ground->loadModel(L"../Engine/data/grass.dds");
	ground->getTransform()->setPosition({ 0.0f, 0.0f, 40.0f });
	ground->getTransform()->setRotation({ -90.0f, 0.0f, 0.0f });
	ground->getTransform()->setScale({ 1.0f, 8.0f, 8.0f });

	std::shared_ptr<GameObject> panel1 = std::make_shared<GameObject>(device, deviceContext, "Panel1", "Props");
	panel1->loadModel(L"../Engine/data/wall.dds");
	panel1->getTransform()->setPosition({ 0.0f, 0.0f, 40.0f });
	panel1->getTransform()->setScale({ 1.0f, 1.5f, 1.0f });
	panel1->getTransform()->setRotation({ 0.0f, -180.0f, 0.0f });
	panel1->getTransform()->setForward(D3DXVECTOR3(0, 0, -1));
	panel1->AddComponent<BoxCollider>();
	enemySide = panel1->AddComponent<WallController>();
	
	std::shared_ptr<GameObject> panel2 = std::make_shared<GameObject>(device, deviceContext, "Panel2", "Props");
	panel2->loadModel(L"../Engine/data/wall.dds");
	panel2->getTransform()->setPosition({ 40.0f, 0.0f, 0.0f });
	panel2->getTransform()->setScale({ 1.0f, 1.5f, 1.0f });
	panel2->getTransform()->setRotation({ 0.0f, -90.0f, 0.0f });
	panel2->getTransform()->setForward(D3DXVECTOR3(1, 0, 0));
	panel2->AddComponent<BoxCollider>();

	std::shared_ptr<GameObject> panel3 = std::make_shared<GameObject>(device, deviceContext, "Panel3", "Props");
	panel3->loadModel(L"../Engine/data/wall.dds");
	panel3->getTransform()->setPosition({ -40.0f, 0.0f, 0.0f });
	panel3->getTransform()->setScale({ 1.0f, 1.5f, 1.0f });
	panel3->getTransform()->setRotation({ 0.0f, 90.0f, 0.0f });
	panel3->getTransform()->setForward(D3DXVECTOR3(-1, 0, 0));
	panel3->AddComponent<BoxCollider>();
	
	std::shared_ptr<GameObject> panel4 = std::make_shared<GameObject>(device, deviceContext, "Panel4", "Props");
	panel4->loadModel(L"../Engine/data/wall.dds");
	panel4->getTransform()->setPosition({ 0.0f, 0.0f, -40.0f });
	panel4->getTransform()->setScale({ 1.0f, 1.5f, 1.0f });
	panel4->getTransform()->setForward(D3DXVECTOR3(0, 0, 1));
	panel4->AddComponent<BoxCollider>();
	playerSide = panel4->AddComponent<WallController>();

	gameObjects.push_back(panel1);
	gameObjects.push_back(panel2);
	gameObjects.push_back(panel3);
	gameObjects.push_back(panel4);

	std::shared_ptr<GameObject> ball = std::make_shared<GameObject>(device, deviceContext, "Ball", "Ball");
	ball->loadModel(L"../Engine/data/soccer ball.obj", L"../Engine/data/soccer ball.dds");
	ball->getTransform()->setPosition({ 0.0f, 5.0f, -5.0f });
	ball->getTransform()->setScale({ 0.03f, 0.03f, 0.03f });
	ball->AddComponent<BoxCollider>();
	auto bounceBall = ball->AddComponent<BounceBall>();
	bounceBall->loadSound(hwnd, "../Engine/data/tennis_hit.wav");

	std::shared_ptr<GameObject> player = std::make_shared<GameObject>(device, deviceContext, "Player", "Character");
	player->loadModel(L"../Engine/data/cat.obj", L"../Engine/data/cat.dds");
	player->getTransform()->setPosition({ 0.0f, 5.0f, -35.0f });
	player->getTransform()->setScale({ 0.1f, 0.1f, 0.1f });

	player->AddComponent<PlayerController>();
	player->AddComponent<BoxCollider>();

	std::shared_ptr<GameObject> enemy = std::make_shared<GameObject>(device, deviceContext, "Enemy", "Character");
	enemy->loadModel(L"../Engine/data/PenguinBaseMesh.obj", L"../Engine/data/penguin.dds");
	enemy->getTransform()->setPosition({ 0.0f, 0.0f, 30.0f });
	enemy->getTransform()->setScale({ 5.0f, 5.0f, 5.0f });

	enemy->AddComponent<EnemyController>();
	enemy->AddComponent<BoxCollider>();

	gameObjects.push_back(ball);
	gameObjects.push_back(player);
	gameObjects.push_back(enemy);

	shaderManager->initialize(device, hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	directionalLight->setAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	directionalLight->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight->setDirection(10.0f, 10.0f, -3.0f);
	directionalLight->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight->setSpecularPower(32.0f);

	pointLight->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	pointLight->setPosition(0.0f, 1.0f, 60.0f);

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
		shaderManager->directionalLightShader->setFilter(device, D3D11_FILTER_MIN_MAG_MIP_POINT);
	}
	else if (Input.GetKeyDown(DIK_2)) {
		shaderManager->directionalLightShader->setFilter(device, D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT);
	}
	else if (Input.GetKeyDown(DIK_3)) {
		shaderManager->directionalLightShader->setFilter(device, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	}
	else if (Input.GetKeyDown(DIK_4)) {
		shaderManager->directionalLightShader->setFilter(device, D3D11_FILTER_ANISOTROPIC);
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

	result = text->setScore(enemySide->getHitCount(), playerSide->getHitCount(), deviceContext);

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

	ground->update();

	Transform *transform = ground->getTransform();
	Model *model = ground->getModel();

	result = shaderManager->pointLightShader->render(deviceContext, model->getIndexCount(),
		transform->getWorldMatrix(), viewMatrix, projectionMatrix, model->getTexture(), pointLight.get());
	if (!result) return false;

	/*for (auto& gameObject : gameObjects) {
		gameObject->update();
		if (!gameObject->activeSelf()) continue;

		Transform *transform = gameObject->getTransform();
		Model *model = gameObject->getModel();
		
		result = shaderManager->directionalLightShader->render(deviceContext,
			model->getIndexCount(), transform->getWorldMatrix(), viewMatrix, projectionMatrix,
			model->getTexture(), directionalLight.get(), mainCamera->getPosition());
		if (!result) return false;
	}*/

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