#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "Body.h"

class Simulation {
public:

    void addBody(std::unique_ptr<Body> body);
    void update(float deltaTime);
    void render(Shader& shader);
    glm::vec3 calculateGravitationalForce(Body& body1, Body& body2);
private:
    std::vector<std::unique_ptr<Body>> bodies;
};

