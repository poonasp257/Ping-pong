#ifndef ENEMYCONTROLLER_H
#define ENEMYCONTROLLER_H

class EnemyController : public Component {
public:
	EnemyController(GameObject *gameObject, Transform *transform);
	virtual ~EnemyController();

public:
	void start();
	void update();
};
#endif