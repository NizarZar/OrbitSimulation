#include "Simulation.h"

const double G = 6.67430e-11f;

void Simulation::addBody(std::unique_ptr<Body> body) {
    bodies.push_back(std::move(body));
}


glm::vec3 Simulation::calculateGravitationalForce(Body &body1, Body &body2) {
    glm::vec3 direction = body2.position - body1.position;
    float distance = glm::length(direction);
    direction = glm::normalize(direction);
    float forceMagnitude = (G * body1.mass * body2.mass) / (distance * distance);
    return direction * forceMagnitude;

}

void Simulation::update(float deltaTime) {
    std::vector<glm::vec3> forces(bodies.size(), glm::vec3(0.0f));

    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = 0; j < bodies.size(); ++j) {
            if (i != j) {
                forces[i] += calculateGravitationalForce(*bodies[i], *bodies[j]);
            }
        }
    }

    for (size_t i = 0; i < bodies.size(); ++i) {
        glm::vec3 acceleration = forces[i].operator/=(bodies[i]->mass);
        bodies[i]->velocity += acceleration * deltaTime;
        bodies[i]->position += bodies[i]->velocity * deltaTime;
    }
}

void Simulation::render(Shader& shader){
    for(auto& body : bodies){
        body->draw(shader);
    }

}
