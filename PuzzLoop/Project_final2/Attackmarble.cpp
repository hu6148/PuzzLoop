#include "Attackmarble.h"

Attackmarble::Attackmarble() { canmove = true; }

void Attackmarble::move() {
	attackmarble.setCenter(attackmarble.getCenter()[0] + 8 * cos(shooting_angle),
		attackmarble.getCenter()[1] + 8 * sin(shooting_angle), 0.0f);
}

void Attackmarble::draw() {
	attackmarble.draw();
}

void Attackmarble::reset() {
	enable = false;
	canmove = true;
	attackmarble = Sphere();
	shooting_angle=0.0f;
}

bool Attackmarble::getEnable() const { return enable; }
bool Attackmarble::getCanmove() const { return canmove; }

Sphere& Attackmarble::getAttackmarble() { return attackmarble; }
void Attackmarble::setAttackmarble(const Sphere& sphere_) { attackmarble = sphere_; }

void Attackmarble::setEnable(bool enable_) { enable = enable_; }
void Attackmarble::setCanmove(bool canmove_) { canmove = canmove_; }

void Attackmarble::setShooting_angle(float shooting_angle_) { shooting_angle = shooting_angle_; }