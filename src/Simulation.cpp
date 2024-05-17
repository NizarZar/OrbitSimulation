#include "Simulation.h"


const double G = 6.67430e-11f;

void Simulation::addBody(Body &body) {
    bodies.push_back(body);
}


glm::vec3 Simulation::calculateGravitationalForce(Body &body1, Body &body2) {
    glm::vec3 direction = body2.position - body1.position;
    float distance = glm::length(direction);
    direction = glm::normalize(direction);
    float forceMagnitude = (G * body1.mass * body2.mass) / (distance * distance);
    return direction * forceMagnitude;

}

void Simulation::update(float deltaTime) {
    for (size_t i = 0; i < bodies.size(); ++i) {
        glm::vec3 totalForce(0.0f);
        for (size_t j = 0; j < bodies.size(); ++j) {
            if (i != j) {
                totalForce += calculateGravitationalForce(bodies[i], bodies[j]);
            }
        }
        glm::vec3 acceleration = totalForce.operator/=(bodies[i].mass);
        bodies[i].velocity += acceleration * deltaTime;
    }

    for (auto& body : bodies) {
        body.position += body.velocity * deltaTime;
    }
}

void Simulation::render(Shader& shader){
    for(auto& body : bodies){
        body.draw(shader);
    }

}
