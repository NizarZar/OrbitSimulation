#include "Shader.h"
#include "Body.h"
#include "Simulation.h"
#include <cmath>
#include <memory>

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


// bodies
std::unique_ptr<Body> earth;
std::unique_ptr<Body> moon;
std::unique_ptr<Body> venus;
std::unique_ptr<Body> sun;

double G = 6.67430e-11;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Simulation", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	// shader initialization
	Shader shader("shaders/default.vert", "shaders/default.frag");

    // initialize simulator
    Simulation simulation;

    glm::vec3 sunPosition(0.0f, 0.0f, 0.0f);
    glm::vec3 sunVelocity(0.0f, 0.0f, 0.0f);
    float sunMass = 10000.f;
    float sunRadius = 1.f;

    glm::vec3 earthPosition(10.f, 0.0f, 0.0f);
    float distanceE = glm::length(earthPosition - sunPosition);
    float earthVelocityY = sqrt(G * sunMass / distanceE);
    glm::vec3 earthVelocity(0.0f, earthVelocityY, 0.0f);
    float earthMass = 100.f;
    float earthRadius = 0.3f;

    glm::vec3 moonPosition(10.4f, 0.0f, 0.0f);
    float distanceM = glm::length(moonPosition - earthPosition);
    float moonVelocityY = sqrt(G * earthMass / distanceM);
    glm::vec3 moonVelocity(0.0f, moonVelocityY, 0.0f);
    float moonMass = 5.f;
    float moonRadius = 0.08f;

    glm::vec3 venusPosition(-5.5f, 0.0f, 0.0f);
    float distanceV = glm::length(venusPosition - sunPosition);
    float venusVelocityY = sqrt(G * sunMass / distanceV);
    glm::vec3 venusVelocity(0.0f, venusVelocityY, 0.0f);
    float venusMass = 50.f;
    float venusRadius = 0.11f;


    earth = std::make_unique<Body>(earthPosition, earthVelocity, earthMass, earthRadius, glm::vec3(0.0f, 0.5f, 1.0f));
    moon = std::make_unique<Body>(moonPosition, moonVelocity, moonMass, moonRadius, glm::vec3(0.8f, 0.8f, 0.8f));
    venus = std::make_unique<Body>(venusPosition, venusVelocity, venusMass, venusRadius, glm::vec3(0.4f, 0.5f, 0.3f));
    sun = std::make_unique<Body>(sunPosition, sunVelocity, sunMass, sunRadius, glm::vec3(1.f, 0.5f, 0.3f));
    simulation.addBody(std::move(moon));
    simulation.addBody(std::move(earth));
    simulation.addBody(std::move(venus));
    simulation.addBody(std::move(sun));
	//glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 8.0f ), // Move the camera back enough to see both objects
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 100.0f);


    float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window)) {
        // dt
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		// input
		processInput(window);

        // update simulation;
        simulation.update(75);
        // rendering
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		int viewLocation = glGetUniformLocation(shader.getID(), "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		int projectionLocation = glGetUniformLocation(shader.getID(), "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        simulation.render(shader);

		// swap buffers and pull IO events (callbacks)
		glfwSwapBuffers(window);
		glfwPollEvents();



	}
	shader.remove();
	glfwTerminate();

	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}




