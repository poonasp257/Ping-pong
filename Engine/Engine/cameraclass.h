////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <d3dx10math.h>
#include "inputclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Move(float, float, float);
	void Rotate(float, float, float);

	void Render();
	void Frame(float frameTime);

	void GetViewMatrix(D3DXMATRIX&);
	void GetProjectionMatrix(D3DXMATRIX&);

private:
	float m_sensitivity;
	float m_moveSpeed;
	float m_yaw;
	float m_pitch;
	float m_roll;
	D3DXVECTOR3 m_forward;
	D3DXVECTOR3	m_right;
	D3DXVECTOR3 m_up;
	D3DXVECTOR3 m_lookAt;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_velocity;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;
};

#endif