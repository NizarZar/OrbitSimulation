#include "Body.h"

// constants
const double G = 6.67430e-11;
const unsigned int MAX_TRAIL_SIZE = 100;
// Constants for simulation;

Body::Body(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec3 color) :
        position(position), velocity(velocity), mass(mass), radius(radius), color(color)
    {
	setupSphere();
    setupTrail();

}
Body::~Body() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &trailVAO);
    glDeleteBuffers(1, &trailVBO);
}

// setting up the sphere (visual)
void Body::setupSphere() {
    // Generate vertices and indices for a UV sphere
    int stacks = 20;
    int slices = 20;
    std::vector<float> vertices;
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * M_PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        for (int j = 0; j <= slices; ++j) {
            float sliceAngle = j * 2 * M_PI / slices;
            float x = xy * cosf(sliceAngle);
            float y = xy * sinf(sliceAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);
        }
    }


    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (slices + 1);
        int k2 = k1 + slices + 1;
        for (int j = 0; j < slices; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Body::setupTrail() {
    glGenVertexArrays(1, &trailVAO);
    glGenBuffers(1, &trailVBO);

    glBindVertexArray(trailVAO);
    glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_TRAIL_SIZE * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Body::updateTrail() {
    trail.push_back(position);
    if(trail.size() > MAX_TRAIL_SIZE){
        trail.erase(trail.begin());
    }

    glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, trail.size() * sizeof(glm::vec3), trail.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Body::draw(Shader& shader) {

	shader.use();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), this->position);
    //model = glm::scale(model, glm::vec3(this->radius));
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

    // trail
    glUniform4f(glGetUniformLocation(shader.getID(), "trailColor"), color.r, color.g, color.b, 1.0f);

    glBindVertexArray(trailVAO);
    glDrawArrays(GL_LINE_STRIP, 0, trail.size());
    glBindVertexArray(0);

}



