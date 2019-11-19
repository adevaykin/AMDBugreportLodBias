///=========================================================================================
/// Press 1 to switch to the textured view
/// Press 2 to switch to the mip level visualization
/// Press SPACE to save screenshot of current viewport
///=========================================================================================

#include <glad/glad.h>

#define GLFW_DLL 1
#include <GLFW/glfw3.h>

#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <learnopengl/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <array>
#include <vector>
#include <chrono>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 1024;
const unsigned int LOD_BIAS = 1;

bool showTexture = true;

void saveScreenshot(GLFWwindow* window)
{
	glm::ivec2 viewportSize;
	glfwGetWindowSize(window, &(viewportSize.x), &(viewportSize.y));
	const auto CHANNELS_NUM = 3u;
	std::vector<unsigned char> readData(viewportSize.x * viewportSize.y * CHANNELS_NUM);
	glReadBuffer(GLenum(GL_COLOR_ATTACHMENT0));
	glReadPixels(0, 0, viewportSize.x, viewportSize.y, GL_RGB, GL_UNSIGNED_BYTE, readData.data());
	auto timestamp = std::chrono::seconds(std::time(NULL));
	stbi_write_bmp(("screenshot" + std::to_string(timestamp.count()) + ".bmp").c_str(), viewportSize.x, viewportSize.y, 3, readData.data());
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Camera
	auto cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	auto cameraDirection = glm::normalize(cameraPosition - cameraTarget);
	auto up = glm::vec3(0.0f, 1.0f, 0.0f);
	auto cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	auto cameraUp = glm::cross(cameraDirection, cameraRight);
	auto view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
	auto projection = glm::perspective(glm::radians(60.f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

	// build and compile our shader program
	// ------------------------------------
	Shader texShader("shaders/texture.vs", "shaders/texture.fs");
	Shader mipShader("shaders/texture.vs", "shaders/miplevel.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	auto vertices1 = std::array<float, 8 * 4>{
		// positions          // colors           // texture coords
		 0.5f,  0.5f,  2.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // bottom right
		 0.5f, -0.5f, -10.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // top right
		-0.5f, -0.5f, -10.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // top left
		-0.5f,  0.5f,  2.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // bottom left 
	};
	auto vertices2 = std::array<float, 8 * 4>{
		// positions          // colors           // texture coords
		 0.7f,  0.3f,  2.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // bottom right
		 0.7f, -0.3f,  2.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // top right
		 0.4f, -0.3f,  2.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // top left
		 0.4f,  0.3f,  2.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // bottom left 
	};
	auto vertices3 = std::array<float, 8 * 4>{
		// positions          // colors           // texture coords
		 -11.f,  3.3f, -15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
		 -11.f, -3.3f, -13.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top right
		  -5.f, -3.3f, -13.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top left
		  -5.f,  3.3f, -15.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // bottom left 
	};
	auto vertices4 = std::array<float, 8 * 4>{
		// positions         // colors           // texture coords
		 0.5f, -0.8f, -15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
		 0.5f, -1.0f,   1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top right
		-0.5f, -1.0f,   1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top left
		-0.5f, -0.8f, -15.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // bottom left 
	};

	const auto allQuads = { vertices1, vertices2, vertices3, vertices4 };
	std::vector<std::tuple<GLuint, GLuint, GLuint>> quadBuffers;

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	for (const auto& quad : allQuads)
	{
		GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 8 * 4, quad.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		quadBuffers.emplace_back(std::tuple<GLuint, GLuint, GLuint>{ VAO, VBO, EBO });
	}

	// load and create a texture 
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, LOD_BIAS);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 8.f);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("textures/squares.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		if (showTexture)
		{
			texShader.use();
			texShader.setMat4("View", view);
			texShader.setMat4("Projection", projection);
		}
		else {
			mipShader.use();
			mipShader.setMat4("View", view);
			mipShader.setMat4("Projection", projection);
		}

		for (const auto [VAO, VBO, EBO] : quadBuffers)
		{
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	for (const auto [VAO, VBO, EBO] : quadBuffers)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		showTexture = true;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		showTexture = false;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		saveScreenshot(window);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
