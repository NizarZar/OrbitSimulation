#pragma once
#include <vector>

struct Vector {
	double x, y;

	Vector operator+(const Vector& v) const {
		return { x + v.x, y + v.y };
	}

	Vector operator*(double scalar) const {
		return {x * scalar, y * scalar};

	}
};

class Body {
public:
	Vector position;
	Vector velocity;
	double mass;

	Body(Vector pos, Vector vel, double m);
	void updatePosition(double dt);
	void updateVelocity(Vector acceleration, double dt);


};
