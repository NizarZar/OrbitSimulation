#pragma once
#include "Shader.h"
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Vector {
	double x, y, z;

	Vector operator+(const Vector& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}
	Vector operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}
};

class Body {
public:
	Vector position;
	Vector velocity;
	double mass;
	float radius;
	glm::vec3 color;
	int segments;
	unsigned int VBO, VAO;


	Body(Vector pos, Vector vel, double m, float radius, glm::vec3 color, int segments);
	~Body();
	void updatePosition(double dt);
	void updateVelocity(Vector acceleration, double dt);
	void setupCircle();
	void draw();

};
