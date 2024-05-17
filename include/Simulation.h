#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Body.h"

class Simulation {
public:
    std::vector<Body> bodies;

    void addBody(Body& body);
    void update(float deltaTime);
    void render(Shader& shader);
    glm::vec3 calculateGravitationalForce(Body& body1, Body& body2);

};

