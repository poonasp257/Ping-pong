#ifndef BOUNCEBALL_H
#define BOUNCEBALL_H

class BounceBall : public Component {
private:
	D3DXVECTOR3 moveDir;
	std::unique_ptr<Sound> sound;

private:
	void move(D3DXVECTOR3 dir);

public:
	BounceBall(GameObject* gameObject, Transform* transform);
	virtual ~BounceBall();

	void start();
	void update();

	void loadSound(HWND hwnd, const char *fileName);
	
	virtual void OnCollisionEnter(Collider *collider);
	virtual void OnCollisionStay(Collider *collider);
	virtual void OnCollisionExit(Collider *collider);
};
#endif