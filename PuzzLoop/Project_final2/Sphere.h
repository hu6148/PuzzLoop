#pragma once
#include "Material.h"
#include <GL/glut.h>

class Sphere
{
public:
	Sphere();
	Sphere(float r, int sl, int st, int ID);
	void setID(int ID_);
	int getID() const;
	float getRadius() const;
	void setCenter(float x, float y, float z);
	float* getCenter();
	void setVelocity(float x, float y, float z);
	float* getVelocity();
	void setMTL(const Material& m);
	void move();
	void draw() const;
	void initialmove(); // 2022-11-24 ¼öÁ¤ pre-progress
private:
	int ID;
	float radius;
	int slice;
	int stack;
	float center[3];
	float velocity[3];
	Material mtl;

};

