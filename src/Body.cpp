#include "Body.h"


Body::Body(Vector pos, Vector vel, double m) {
	this->position = pos;
	this->velocity = vel;
	this->mass = m;

}

void Body::updatePosition(double dt) {
	position = position + (velocity * dt);
}

void Body::updateVelocity(Vector acceleration, double dt) {
	velocity = velocity + (acceleration * dt);
}





