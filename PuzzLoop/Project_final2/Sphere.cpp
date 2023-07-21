#include "Sphere.h"


Sphere::Sphere() {};
Sphere::Sphere(float r, int sl, int st, int ID) :radius(r), slice(sl), stack(st), ID(ID) {}
void Sphere::setID(int ID_) { ID = ID_; }
int Sphere::getID() const { return ID; }

float Sphere::getRadius() const { return radius; }
void Sphere::setCenter(float x, float y, float z) { 
	center[0] = x; center[1] = y; center[2] = z; 
}
float* Sphere::getCenter() { return center; }

void Sphere::setVelocity(float x, float y, float z) { 
	velocity[0] = x; velocity[1] = y; velocity[2] = z;
}

float* Sphere::getVelocity() { return velocity; }

void Sphere::setMTL(const Material& m) { mtl = m; }

void Sphere::move(){
	center[0] += velocity[0];
}

void Sphere::initialmove() {

	center[0] += 8 * velocity[0];

} // 2022-11-24 ¼öÁ¤ pre-progress

void Sphere::draw() const{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());
	
	
	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(radius, slice, stack);
	glPopMatrix();
}