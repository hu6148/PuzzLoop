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
	float angle; //�����ڸ� �̿��� 90���� ���� �ʱ�ȭ
	bool canshoot; // �߻��� �� �ִ� �������� ����, �����ڸ� �̿��� true�� �ʱ�ȭ
	Sphere currentsphere; //spacebar�� ������ �߻�� ��
	Sphere nextsphere; //spacebar�� ������ currentsphere�� �� ��
};