#include "cannon.h"

Cannon::Cannon() { angle = 3.141592f / 2; canshoot = true;}

void Cannon::setCannon(Sphere& currentsphere_, Sphere& nextsphere_) {
	currentsphere = currentsphere_; nextsphere = nextsphere_;
}

float Cannon::getAngle() const { return angle; }
void Cannon::setAngle(float angle_) { angle = angle_; }
bool Cannon::getCanshoot() const { return canshoot; }
void Cannon::setCanshoot(bool canshoot_) { canshoot = canshoot_; }
Sphere Cannon::getCurrentsphere() const { return currentsphere; }
Sphere Cannon::getNextsphere() const { return nextsphere; }
void Cannon::setCurrentsphere(const Sphere& sphere_) { currentsphere = sphere_; }
void Cannon::setNextsphere(const Sphere& sphere_) { nextsphere = sphere_; }

void Cannon::drawline() const {
		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(2.5f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3, 0xAAAA);
		glBegin(GL_LINES);
		glVertex2f(0.0f, -300.0f); 
		glVertex2f(1000.0f*cos(angle), 1000.0f* sin(angle) - 300.0f); 
		glEnd();
}
void Cannon::drawmarbles() {
	currentsphere.draw(); nextsphere.draw();
}
void Cannon::move() {
	currentsphere.setCenter(40 * cos(angle), -300.0f + 40 * sin(angle), 0.0f);
}



