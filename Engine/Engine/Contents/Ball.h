#ifndef BALL_H
#define BALL_H

class Ball : public GameObject {
private:
	ID3D11Device			*device;
	ID3D11DeviceContext	*deviceContext;

private:
	void update();

public:
	Ball(ID3D11Device *device, ID3D11DeviceContext *deviceContext, const std::string& tag);
	virtual ~Ball();
};
#endif