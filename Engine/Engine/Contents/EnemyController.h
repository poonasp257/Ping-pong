#ifndef ENEMYCONTROLLER_H
#define ENEMYCONTROLLER_H

class EnemyController : public Component {
private:
	GameObject *ball;

public:
	EnemyController(GameObject *gameObject, Transform *transform);
	virtual ~EnemyController();

public:
	void start();
	void update();
};
#endif