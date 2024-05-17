#pragma once

#include "Shader.h"
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Body {
public:
    glm::vec3 position{};
	glm::vec3 velocity{};
	double mass;
	float radius;
	glm::vec3 color{};
	unsigned int VBO{}, VAO{}, EBO{};


	Body(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec3 color);
	~Body();
	void draw(Shader& shader);
private:
    void setupSphere();
};
