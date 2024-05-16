#include "Shader.h"
#include "Body.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define PI 3.14159265358979323846

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
// constant of gravitation
const double G = 6.67430e-11;
// 1 hour in seconds
double dt = 3600;
// segments
const int segments = 360;
// bodies
std::vector<Body*> bodies;

std::unique_ptr<Body> earth;
std::unique_ptr<Body> moon;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void updateSimulation();


int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Simulation", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	// shader initialization
	Shader shader("shaders/default.vert", "shaders/default.frag");

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 100.0f);

	earth = std::make_unique<Body>(Vector{ 0,0 }, Vector{ 0,0 }, 5.972e24, 0.5f, glm::vec3(0.0f, 0.05f, 1.0f), segments);
	moon = std::make_unique<Body>(Vector{ 384400e3, 0 }, Vector{ 0,1022 }, 7.342e22, 0.1f, glm::vec3(0.8f, 0.8f, 0.8f), segments);
	earth->setupCircle();
	moon->setupCircle();

	bodies.push_back(earth.get());
	bodies.push_back(moon.get());

	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// simulation
		updateSimulation();

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		int viewLocation = glGetUniformLocation(shader.getID(), "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		int projectionLocation = glGetUniformLocation(shader.getID(), "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));


		for (auto& body : bodies) {
			body->draw();
		}

		

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

void updateSimulation() {
	// bodies[0] = earth
	// bodies[1] = moon
	Vector r = { moon->position.x - earth->position.x, moon->position.y - earth->position.y};
	double distance = sqrt(r.x * r.x + r.y * r.y);
	double forceMagnitude = G * earth->mass * earth->mass / (distance * distance);
	Vector force = { forceMagnitude * r.x / distance, forceMagnitude * r.y / distance };

	Vector earth_a = { force.x / earth->mass, force.y / earth->mass};
	Vector moon_a = { -force.x / moon->mass, -force.y / moon->mass};

	earth->updateVelocity(earth_a, dt);
	moon->updateVelocity(moon_a, dt);

	earth->updatePosition(dt);
	moon->updatePosition(dt);
}

