#include "Body.h"

// constants
const double PI = 3.14159265358979323846;
const double G = 6.67430e-11;
// Constants for simulation

Body::Body(glm::vec3 pos, glm::vec3 vel, double mass, float radius, glm::vec3 color, int segments) :
        position(pos), velocity(vel), mass(mass), radius(radius), color(color), segments(segments)
    {
	setupCircle();

}
Body::~Body() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Body::update(double dt, std::vector<Body*> &bodies) {
    glm::vec3 totalForce(1.0f);
    for(const Body* other : bodies){
        if(this != other){
            glm::vec3 r = other->position - this->position;
            double distance = glm::length(r);
            if(distance == 0) continue; // avoid zero division
            double forceMagnitude = G * this->mass * other->mass / (distance * distance);
            glm::vec3 forceDirection = glm::normalize(r);
            totalForce += forceDirection * static_cast<float>(forceMagnitude);
        }
    }

    glm::vec3 acceleration = totalForce / static_cast<float>(this->mass);
    velocity += acceleration.operator*=(dt);
    position += velocity.operator*=(dt);
}

void Body::setupCircle() {

	// center vertex
    std::vector<float> vertices;
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
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
	glm::mat4 model = glm::translate(glm::mat4(1.0f), this->position);
    model = glm::scale(model, glm::vec3(this->radius));
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, segments+2);
	glBindVertexArray(0);
}



