#include "Shader.h"
#include "Body.h"
#include "Simulation.h"
#include <cmath>
#include <memory>

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


// bodies
std::vector<Body*> bodies;
std::unique_ptr<Body> earth;
std::unique_ptr<Body> moon;
std::unique_ptr<Body> venus;

//
const double EARTH_MASS = 1000000000.0f;
const double MOON_MASS = 1000.f;
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

    Body earth(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 100.f, 0.5f, glm::vec3(0.0f, 0.5f, 1.0f));
    Body moon(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 10.f, 0.27f, glm::vec3(0.8f, 0.8f, 0.8f));

    simulation.addBody(earth);
    simulation.addBody(moon);
	//glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 2.0f ), // Move the camera back enough to see both objects
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

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
        //simulation.update(deltaTime);
        // rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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




