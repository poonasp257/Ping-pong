#ifndef WALLCONTROLLER_H
#define WALLCONTROLLER_H

class WallController : public Component {
private:
	int hitCount;

public:
	WallController(GameObject *gameObject, Transform *transform);
	virtual ~WallController();

	void start();
	void update();

	int getHitCount() const { return hitCount; }

	void increaseHitCount() { ++hitCount; }
	void decreaseHitCount() { --hitCount; }
};
#endif