#include "Body.h"

#define PI 3.14159265358979323846

Body::Body(Vector pos, Vector vel, double m, float radius, glm::vec3 color, int segments) {
	this->position = pos;
	this->velocity = vel;
	this->mass = m;
	this->radius = radius;
	this->color = color;
	this->segments = segments;
	setupCircle();

}
Body::~Body() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Body::updatePosition(double dt) {
	position = position + (velocity * dt);
}

void Body::updateVelocity(Vector acceleration, double dt) {
	velocity = velocity + (acceleration * dt);
}

void Body::setupCircle() {
	std::vector<float> vertices;

	// center vertex
	vertices.push_back(position.x);
	vertices.push_back(position.y);
	vertices.push_back(position.z);
	vertices.push_back(color.r);
	vertices.push_back(color.g);
	vertices.push_back(color.b);

	for (int i = 0; i <= segments; i++) {
		float angle = 2.0f * PI * i / segments;
		vertices.push_back(position.x + cos(angle) * radius);
		vertices.push_back(position.y + sin(angle) * radius);
		vertices.push_back(position.z);
		vertices.push_back(color.r);
		vertices.push_back(color.g);
		vertices.push_back(color.b);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Body::draw(Shader& shader) {

	shader.use();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, segments+2);
	glBindVertexArray(0);
}



