#include "Shader.h"
#include <cmath>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Body.h"

#define PI 3.14159265358979323846

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
// constant of gravitation
const double G = 6.67430e-11;
// 1 hour in seconds
double dt = 3600;
// bodies
Body earth({ 0,0 }, { 0,0 }, 5.972e24);
Body moon({ 384400e3, 0 }, { 0,1022 }, 7.342e22);

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

	const int numSegments = 360;
	float vertices[(numSegments+2) * 6];

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 100.0f);


	float radius = 0.5f;
	
	// center vertex
	vertices[0] = 0.0f; // x, center of circle
	vertices[1] = 0.0f; // y, center of circle
	vertices[2] = 0.0f; // z, center of circle
	vertices[3] = 1.0f; // red
	vertices[4] = 1.0f; // green
	vertices[5] = 1.0f; // blue

	for (int i = 0; i < numSegments+2; i++) {
		float angle = 2.0f * PI *float(i) / float(numSegments);
		int index = 6 + i * 6;
		vertices[index + 0] = cosf(angle) * radius; // x
		vertices[index + 1] = sinf(angle) * radius; // y
		vertices[index + 2] = 0.0f; // z
		vertices[index + 3] = 0.0f; // red
		vertices[index + 4] = 0.5f; // green
		vertices[index + 5] = 1.0f; // blue

	}
	

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	

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
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLocation = glGetUniformLocation(shader.getID(), "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments+2);

		

		// swap buffers and pull IO events (callbacks)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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
	Vector r = { moon.position.x - earth.position.x, moon.position.y - earth.position.y };
	double distance = sqrt(r.x * r.x + r.y * r.y);
	double forceMagnitude = G * earth.mass * moon.mass / (distance * distance);
	Vector force = { forceMagnitude * r.x / distance, forceMagnitude * r.y / distance };

	Vector earth_a = { force.x / earth.mass, force.y / earth.mass };
	Vector moon_a = { -force.x / moon.mass, -force.y / moon.mass };

	earth.updateVelocity(earth_a, dt);
	moon.updateVelocity(moon_a, dt);

	earth.updatePosition(dt);
	moon.updatePosition(dt);	
}

