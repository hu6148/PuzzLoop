#pragma once

#include <cmath>
#include <iostream>
#include "Sphere.h"
#include <GL/glut.h>

class Cannon
{
public:
	Cannon();
	void setCannon(Sphere& currentsphere_, Sphere& nextsphere_);
	float getAngle() const;
	void setAngle(float angle_);
	bool getCanshoot() const;
	void setCanshoot(bool canshoot_);
	Sphere getCurrentsphere() const;
	Sphere getNextsphere() const;
	void setCurrentsphere(const Sphere& sphere_);
	void setNextsphere(const Sphere& sphere_);

	void drawline() const;
	void drawmarbles();
	void move();
	
private:
	float angle; //생성자를 이용해 90도로 각도 초기화
	bool canshoot; // 발사할 수 있는 상태인지 여부, 생성자를 이용해 true로 초기화
	Sphere currentsphere; //spacebar를 누르면 발사될 구
	Sphere nextsphere; //spacebar를 누르면 currentsphere가 될 구
};