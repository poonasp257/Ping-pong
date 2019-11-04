#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
	Camera();
	~Camera();

	void setPosition(float, float, float);
	D3DXVECTOR3 getPosition() const;

	void setRotation(float, float, float);
	D3DXVECTOR3 getRotation() const;

	void move(float, float, float);
	void rotate(float, float, float);

	void render();
	void frame(float frameTime);

	void getViewMatrix(D3DXMATRIX&) const ;
	void getProjectionMatrix(D3DXMATRIX&) const;

private:
	float sensitivity;
	float moveSpeed;
	float yaw;
	float pitch;
	float roll;
	D3DXVECTOR3 forward;
	D3DXVECTOR3	right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 velocity;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
};
#endif