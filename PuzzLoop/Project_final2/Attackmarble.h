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
	bool enable; //attack marble이 존재하는 상태인지 여부
	bool canmove; // attack marble이 움직이고 있는지 여부, 생성자를 이용해 true로 초기화함
	Sphere attackmarble;
	float shooting_angle;
};


