////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"

CameraClass::CameraClass() :
	m_sensitivity(0.005f), m_moveSpeed(0.3f),
	m_yaw(0.0f), m_pitch(0.0f), m_roll(0.0f),
	m_position(0.0f, 0.0f, 0.0f),
	m_rotation(0.0f, 0.0f, 0.0f),
	m_velocity(0.0f, 0.0f, 0.0f) {

}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

void CameraClass::Move(float x, float y, float z) {
	m_velocity.x += x;
	m_velocity.y += y;
	m_velocity.z += z;
}

void CameraClass::Rotate(float x, float y, float z)
{
	m_rotation.x += x;
	m_rotation.y += y;
	m_rotation.z += z;
}

D3DXVECTOR3 CameraClass::GetPosition() {
	return m_position;
}


D3DXVECTOR3 CameraClass::GetRotation() {
	return m_rotation;
}

void CameraClass::Render() {
	D3DXVECTOR3 defaultForward = { 0.0f, 0.0f, 1.0f };
	D3DXVECTOR3 defaultRight = { 1.0f, 0.0f, 0.0f };
	D3DXMATRIX	rotationMatrix;
	D3DXVECTOR3 up, lookAt;
	
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_yaw, m_pitch, 0);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	
	D3DXMATRIX rotateYTempMatrix;
	D3DXMatrixRotationY(&rotateYTempMatrix, m_yaw);
	
	D3DXVec3TransformCoord(&m_forward, &defaultForward, &rotateYTempMatrix);
	D3DXVec3TransformCoord(&m_right, &defaultRight, &rotateYTempMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotateYTempMatrix);
	
	m_position += m_velocity.x * m_right;
	m_position += m_velocity.z * m_forward;

	m_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	lookAt = m_position + lookAt;

	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &lookAt, &up);

	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, 0.4f*3.14f, 800 / 600, 1.0f, 1000.0f);
}

void CameraClass::Frame(float frameTime) {
	float units = m_moveSpeed * frameTime;

	if (Input.GetKey(DIK_A)) {
		Move(-units, 0.0f, 0.0f);
	}
	if (Input.GetKey(DIK_D)) {
		Move(units, 0.0f, 0.0f);
	}
	if (Input.GetKey(DIK_W)) {
		Move(0.0f, 0.0f, units);
	}
	if (Input.GetKey(DIK_S))  {
		Move(0.0f, 0.0f, -units);
	}

	LONG lx = Input.GetAxisX();
	LONG ly = Input.GetAxisY();

	m_yaw += lx * m_sensitivity;
	m_pitch += ly * m_sensitivity;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix) {
	viewMatrix = m_viewMatrix;
}

void CameraClass::GetProjectionMatrix(D3DXMATRIX& viewMatrix) {
	viewMatrix = m_projectionMatrix;
}