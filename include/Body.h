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
	int segments;
	unsigned int VBO{}, VAO{};


	Body(glm::vec3 pos, glm::vec3 vel, double mass, float radius, glm::vec3 color, int segments);
	~Body();
	void update(double dt, std::vector<Body*>& bodies);
	void draw(Shader& shader);
private:
    void setupCircle();
};
