#pragma once
#include <vector>
#include "Sphere.h"

class Attackmarble
{
public:
	Attackmarble();

	void move();
	void draw();
	void reset();
	bool getEnable() const;
	bool getCanmove() const;
	Sphere& getAttackmarble();
	void setAttackmarble(const Sphere& sphere_);

	void setEnable(bool enable_);
	void setCanmove(bool canmove_);

	void setShooting_angle(float shooting_angle_);

private:
	bool enable; //attack marble�� �����ϴ� �������� ����
	bool canmove; // attack marble�� �����̰� �ִ��� ����, �����ڸ� �̿��� true�� �ʱ�ȭ��
	Sphere attackmarble;
	float shooting_angle;
};


