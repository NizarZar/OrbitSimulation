#pragma once
#include "Shader.h"
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vector {
    double x, y, z;

    // Addition
    Vector operator+(const Vector& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    // Subtraction
    Vector operator-(const Vector& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    // Scalar multiplication
    Vector operator*(double scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    // Scalar division
    Vector operator/(double scalar) const {
        return { x / scalar, y / scalar, z / scalar }; // Make sure scalar is not zero when using this operator
    }
    // Vector negation
    Vector operator-() const {
        return { -x, -y, -z };
    }

    // Compute magnitude of the vector
    double magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    // Normalize the vector
    Vector normalize() const {
        double mag = magnitude();
        if (mag == 0) return { 0, 0, 0 };  // Prevent division by zero
        return { x / mag, y / mag, z / mag };
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
	void draw(Shader& shader);

};
