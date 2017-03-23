#include "tests.h"
// display
#include "Loader.h"
#include "Renderer.h"
#include "Camera.h"
// utility
#include "Maths.h"
// shaders
#include "ShaderProgram.h"
#include "Shaders.h"
// textures
#include "ModelTexture.h"
#include "TexturedModel.h"
#include "Texture.h"
// models
#include "Entity.h"
#include "DynamicModels.h"




bool wireFrame = false;

void helloWorld()
{
	std::cout << "Starting OpenGL Test - 01" << std::endl;

	// create display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	GLfloat green = 0.0f;
	GLfloat red = 0.0f;
	GLfloat blue = 0.0f;
	GLboolean maxColour = false;

	// time
	GLdouble prevTime = glfwGetTime();
	GLdouble currTime = 0;
	float deltaTime = 0;

	// draw loop
	while (currTime = glfwGetTime(), !glfwWindowShouldClose(display->window()))
	{
		// update delta time
		deltaTime = (float)(currTime - prevTime);
		//std::cout << "delta time = " << deltaTime << " modula time = " << (int)currTime % 3 << std::endl;

		// check and call events
		glfwPollEvents();

		// rendering commands
		screenColour(&red, &green, &blue, &maxColour);
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// swap the buffers
		glfwSwapBuffers(display->window());
		glfwSetKeyCallback(display->window(), key_callback);

		// update current time
		prevTime = currTime;
	}
	// Terminate GLFW
	glfwTerminate();
	std::cout << "Finished!!  - 01 \n\n" << std::endl;
}

void helloTriangle()
{
	std::cout << "\n### --> Starting helloTriangle" << std::endl;
	//// Init GLFW
	//glfwInit();
	//// Set all the required options for GLFW
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//// Create a GLFWwindow object that we can use for GLFW's functions
	//GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	//glfwMakeContextCurrent(window);

	//// Set the required callback functions
	//glfwSetKeyCallback(window, key_callback);

	//// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	//glewExperimental = GL_TRUE;
	//// Initialize GLEW to setup the OpenGL Function pointers
	//glewInit();

	//// Define the viewport dimensions
	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);

	// create display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// Build and compile our shader program
	ShaderProgram * shader = new ShaderProgram();
	GLuint shaderProgram = shader->ID();


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // Left  
		0.5f, -0.5f, 0.0f, // Right 
		0.0f,  0.5f, 0.0f  // Top   
	};
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	// Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(display->window());
		// Check for key strokes
		glfwSetKeyCallback(display->window(), key_callback);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void renderEngineTest()
{
	std::cout << "\n### --> Starting renderEngineTest" << std::endl;

	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();
	
	// initialise shader program
	ShaderProgram * staticShader = new ShaderProgram(Shader::staticShader);

	// initialise loader and renderer
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();
	

	// initialise vertices for triangle
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions        
		0.5f, -0.5f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		0.0f,  0.5f, 0.0f    // Top   
	};

	// load model to VAO
	RawModel * model = loader->loadToVAO(vertices, sizeof(vertices));

	// draw loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// check and call events
		glfwPollEvents();

		// game logic

		// render
		renderer->prepare(0.0f, 0.3f, 0.3f);

		// starty using shader
		staticShader->start();
		
		// Draw triangle
		renderer->render(model);

		// stop using shader
		staticShader->stop();

		// update display
		display->updateDisplay();
		
		// check key presses
		glfwSetKeyCallback(display->window(), key_callback);

	}
	//clean up
	staticShader->cleanUp();
	loader->cleanUp();
	// Terminate GLFW
	glfwTerminate();
	std::cout << "Finished!!  - 02 \n\n" << std::endl;
}

void indexBuffer()
{
	std::cout << "\n### --> Starting indexBuffer" << std::endl;
	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// initialise shader program
	ShaderProgram * staticShader = new ShaderProgram(Shader::indexShader);

	// initialise loader and renderer
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();


	// initialise vertices for triangle
	// Set up vertex data (and buffer(s)) and attribute pointers
	//GLfloat vertices[] = {
	//	// First triangle
	//	0.5f,  0.5f, 0.0f,  // Top Right
	//	0.5f, -0.5f, 0.0f,  // Bottom Right
	//	-0.5f,  0.5f, 0.0f,  // Top Left 
	//	// Second triangle
	//	0.5f, -0.5f, 0.0f,  // Bottom Right
	//	-0.5f, -0.5f, 0.0f,  // Bottom Left
	//	-0.5f,  0.5f, 0.0f   // Top Left
	//};
	GLfloat vertices[] = {
		0.5f,  0.5f, 0.0f,  // Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 0.0f   // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};

	// load model to VAO
	RawModel * model = loader->loadToVAO(vertices, sizeof(vertices), indices, sizeof(indices));

	// draw loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// check and call events
		glfwPollEvents();

		// game logic

		// render
		renderer->prepare(0.0f, 0.3f, 0.3f);

		// starty using shader
		staticShader->start();

		// Draw triangle
		renderer->renderInd(model);

		// stop using shader
		staticShader->stop();

		// update display
		display->updateDisplay();

		// check key presses
		glfwSetKeyCallback(display->window(), key_callback);

	}
	//clean up
	staticShader->cleanUp();
	loader->cleanUp();
	// Terminate GLFW
	glfwTerminate();
}

void greenTriangleChange()
{
	std::cout << "\n### --> Starting greenTriangleTute" << std::endl;

	const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 color;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position, 1.0);\n"
		"ourColor = color;\n"
		"}\0";
	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		"color = ourColor;\n"
		"}\n\0";

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, 800, 600);


	// Build and compile our shader program
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions        
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		0.0f,  0.5f, 0.0f   // Top 
	};
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO


						  // Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Be sure to activate the shader
		glUseProgram(shaderProgram);

		// Update the uniform color
		GLfloat timeValue = (GLfloat)glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Draw the triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void gtShaderTest()
{
	std::cout << "\n### --> Starting gtShaderTest" << std::endl;
	// create display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();


	// Build and compile our shader program
	ShaderProgram * shader = new ShaderProgram(Shader::uniformShader);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions        
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		0.0f,  0.5f, 0.0f   // Top 
	};
	// create VAO
	Loader * loader = new Loader();
	// load model to VAO
	RawModel * model = loader->loadToVAO(vertices, sizeof(vertices));
	Renderer * renderer = new Renderer();


						  // Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		renderer->prepare(0.2f, 0.3f, 0.3f);

		// Be sure to activate the shader
		shader->start();
		// Update the uniform color
		GLfloat timeValue = (GLfloat)glfwGetTime();
		GLfloat greenValue = ((GLfloat)sin(timeValue) / 2) + 0.5f;
		shader->uniformVec4("ourColor", glm::vec4(0.0f, greenValue, 0.0f, 1.0f));
		// Draw the triangle
		renderer->render(model);
		// stop using shader
		shader->stop();
		// Swap the screen buffers
		display->updateDisplay();
		// check key presses
		glfwSetKeyCallback(display->window(), key_callback);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void rainbowTri()
{
	std::cout << "\n### --> Starting Rainbow Triangle" << std::endl;

	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();
	// initialise shader program
	ShaderProgram * indexShader = new ShaderProgram(Shader::indexShader);

	// initialise loader and renderer
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();


	// initialise vertices for triangle
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions         // Colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top   
	};

	// load model to VAO
	RawModel * model = loader->loadToVAO(vertices, sizeof(vertices), 2);

	// draw loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// check and call events
		glfwPollEvents();

		// game logic

		// render
		renderer->prepare(0.0f, 0.3f, 0.3f);

		// starty using shader
		indexShader->start();

		// Draw triangle
		renderer->render(model);

		// stop using shader
		indexShader->stop();

		// update display
		display->updateDisplay();

		// check key presses
		glfwSetKeyCallback(display->window(), key_callback);

	}
	//clean up
	indexShader->cleanUp();
	loader->cleanUp();
	// Terminate GLFW
	glfwTerminate();
	std::cout << "Finished!! \n\n" << std::endl;
}

void multipleShaders()
{
	std::cout << "\n### --> Starting multiple shaders test" << std::endl;

	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// initialise loader and renderer
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();

	// initialise shader program
	ShaderProgram * uniformShader = new ShaderProgram(Shader::uniformShader);
	ShaderProgram * indexShader = new ShaderProgram(Shader::indexShader);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat verticesA[] = {
		// Positions        
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		0.0f,  0.5f, 0.0f   // Top 
	};

	// initialise vertices for triangle
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat verticesB[] = {
		// Positions         // Colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top   
	};

	// load model to VAO
	RawModel * modelA = loader->loadToVAO(verticesA, sizeof(verticesA));
	RawModel * modelB = loader->loadToVAO(verticesB, sizeof(verticesB), 2);
	// draw loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// check and call events
		glfwPollEvents();

		// game logic

		// render
		renderer->prepare(0.0f, 0.3f, 0.3f);

		// starty using shader
		uniformShader->start();
		GLfloat timeValue = (GLfloat)glfwGetTime();
		GLfloat greenValue = ((GLfloat)sin(timeValue) / 2) + 0.5f;
		uniformShader->uniformVec4("ourColor", glm::vec4(0.0f, greenValue, 0.0f, 1.0f));
		// Draw triangle
		renderer->render(modelA);
		// stop using shader
		uniformShader->stop();

		// starty using shader
		indexShader->start();
		// Draw triangle
		renderer->render(modelB);
		// stop using shader
		indexShader->stop();

		// update display
		display->updateDisplay();

		// check key presses
		glfwSetKeyCallback(display->window(), key_callback);

	}
	//clean up
	indexShader->cleanUp();
	loader->cleanUp();
	// Terminate GLFW
	glfwTerminate();
	std::cout << "Finished!!  - 02 \n\n" << std::endl;
}

void textures()
{
	std::cout << "\n### --> Starting Textures" << std::endl;
	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// initialise shader program
	ShaderProgram * textureShader = new ShaderProgram(Shader::textureShader);

	// initialise loader and renderer
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();


	// initialise vertices for triangle
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // Top Left  
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	// load model to VAO
	RawModel * model = loader->loadTextureVAO(vertices, sizeof(vertices), indices, sizeof(indices));
	Texture * texture = new Texture("res/textures/container.jpg");
	TexturedModel * texturedModel = new TexturedModel(*model, *texture);


	// draw loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// check and call events
		glfwPollEvents();

		// game logic

		// render
		renderer->prepare(0.0f, 0.3f, 0.3f);

		// starty using shader
		textureShader->start();

/***********************************************************************************

OpenGL 3D Game Tutorial 6: Texturing
7m:20s

************************************************************************************/
		// Draw triangle
		renderer->renderTexturedModel(texturedModel);

		// stop using shader
		textureShader->stop();

		// update display
		display->updateDisplay();

		// check key presses
		glfwSetKeyCallback(display->window(), key_callback);

	}
	//clean up
	textureShader->cleanUp();
	loader->cleanUp();
	// Terminate GLFW
	glfwTerminate();

}

void texturesTute()
{
	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// initialise shader program
	ShaderProgram * textureShader = new ShaderProgram(Shader::textureShader);


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO


	// Load and create a texture 
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("res/textures/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


									 // Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// Activate shader
		textureShader->start();

		// Draw container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		textureShader->stop();

		// Swap the screen buffers
		glfwSwapBuffers(display->window());
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void texturesSplitData()
{
	std::cout << "\n### --> Starting Textures Split Data" << std::endl;
	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// initialise shader program
	ShaderProgram * textureShader = new ShaderProgram(Shader::textureShader);

	// initialise loader and renderer
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();


	// initialise vertices for triangle
	GLfloat vertices[] = {
		// Positions          
		0.5f,  0.5f, 0.0f,      // Top Right
		0.5f, -0.5f, 0.0f,      // Bottom Right
		-0.5f, -0.5f, 0.0f,     // Bottom Left
		-0.5f,  0.5f, 0.0f      // Top Left  
	};
	GLfloat colours[] = {
		// Colors        
		1.0f, 0.0f, 0.0f, // Top Right
		0.0f, 1.0f, 0.0f, // Bottom Right
		0.0f, 0.0f, 1.0f, // Bottom Left
		1.0f, 1.0f, 0.0f  // Top Left  
	};
	GLfloat textureCord[] = {
		// Texture Coords
		2.0f, 2.0f,   // Top Right
		2.0f, 0.0f,   // Bottom Right
		0.0f, 0.0f,   // Bottom Left
		0.0f, 2.0f    // Top Left  
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	// load model to VAO
	RawModel * model = loader->loadToVAO(vertices, sizeof(vertices), colours, sizeof(colours), 
										 textureCord, sizeof(textureCord),indices, sizeof(indices));
	Texture * texture = new Texture("res/textures/container.jpg");
	TexturedModel * texturedModel = new TexturedModel(*model, *texture);


	// draw loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// check and call events
		glfwPollEvents();

		// game logic

		// render
		renderer->prepare(0.0f, 0.3f, 0.3f);

		// starty using shader
		textureShader->start();

		// Draw triangle
		renderer->renderTexturedModel(texturedModel);

		// stop using shader
		textureShader->stop();

		// update display
		display->updateDisplay();

		// check key presses
		glfwSetKeyCallback(display->window(), key_callback);

	}
	//clean up
	textureShader->cleanUp();
	loader->cleanUp();
	// Terminate GLFW
	glfwTerminate();

}

void dualTextures()
{
	std::cout << "\n### --> Starting Textures" << std::endl;
	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// initialise shader program
	ShaderProgram * textureShader = new ShaderProgram(Shader::dualTextureShader);

	// initialise loader and renderer
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();


	// initialise vertices for triangle
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left  
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	// load model to VAO
	RawModel * model = loader->loadTextureVAO(vertices, sizeof(vertices), indices, sizeof(indices));
	Texture * texture1 = new Texture("res/textures/container.jpg");
	Texture * texture2 = new Texture("res/textures/awesomeface.png");
	TexturedModel * texturedModel1 = new TexturedModel(*model, *texture1);
	TexturedModel * texturedModel2 = new TexturedModel(*model, *texture2);


	// draw loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// check and call events
		glfwPollEvents();

		// game logic

		// render
		renderer->prepare(0.0f, 0.3f, 0.3f);

		// starty using shader
		textureShader->start();

		// bind textures
		glActiveTexture(GL_TEXTURE0);
		renderer->bindTexture(texturedModel1, textureShader, "ourTexture1", 0);
		glActiveTexture(GL_TEXTURE1);
		renderer->bindTexture(texturedModel2, textureShader, "ourTexture2", 1);
		// Draw 
		renderer->renderTexturedModel(texturedModel1);

		// stop using shader
		textureShader->stop();

		// update display
		display->updateDisplay();

		// check key presses
		glfwSetKeyCallback(display->window(), key_callback);

	}
	//clean up
	textureShader->cleanUp();
	loader->cleanUp();
	// Terminate GLFW
	glfwTerminate();

}

void dualTextTute()
{
	std::cout << "\n### --> Starting Textures Tute" << std::endl;

	//glm::mat4 matrix = Maths::createTransormationMatrix(glm::vec3(3, 3, 3), glm::vec3(2, 2, 2), 5.0f);

	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();


	// Build and compile our shader program
	// initialise shader program
	ShaderProgram * textureShader = new ShaderProgram(Shader::dualTextureShader);


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords  (Note that we changed them to 'zoom in' on our texture image)
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	// load model to VAO
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();
	RawModel * model = loader->loadTextureVAO(vertices, sizeof(vertices), indices, sizeof(indices));
	


	 // Load and create a texture 
	Texture * textureFile = new Texture("res/textures/container.jpg");
	TexturedModel * texturedModel = new TexturedModel(*model, *textureFile);
	
	GLuint texture1;
	GLuint texture2;
	// ====================
	// Texture 1
	// ====================
	//texture1 = loader->loadTexture("res/textures/container.jpg");
	texture1 = textureFile->ID();

									 
	// ===================
	// Texture 2
	// ===================
	texture2 = loader->loadTexture("res/textures/awesomeface.png");


	// Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		renderer->prepare(0.0f, 0.3f, 0.3f);

		// Activate shader
		textureShader->start();

		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture2"), 1);

		// Draw 
		renderer->renderTexturedModel(texturedModel);

		// stop using shader
		textureShader->stop();

		// update display
		display->updateDisplay();

		// check for key presses
		glfwSetKeyCallback(display->window(), key_callback);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}
void transformsTute()
{
	std::cout << "\n### --> Starting Transforms Tute" << std::endl;


	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();


	// Build and compile our shader program
	// initialise shader program
	ShaderProgram * textureShader = new ShaderProgram(Shader::transformShader);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords  (Note that we changed them to 'zoom in' on our texture image)
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	// load model to VAO
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();
	RawModel * model = loader->loadTextureVAO(vertices, sizeof(vertices), indices, sizeof(indices));

	// Load and create a texture 
	Texture * textureFile1 = new Texture("res/textures/container.jpg");
	Texture * textureFile2 = new Texture("res/textures/awesomeface.png");

	TexturedModel * texturedModel = new TexturedModel(*model, *textureFile1, *textureFile2);

	GLuint texture1 = textureFile1->ID();
	GLuint texture2 = textureFile2->ID();

	glm::mat4 transformMatrix = Maths::createTransormationMatrix(glm::vec3(0, 0, 0), glm::vec3(45.0f, 0, 45.0f), 0.5f);

	// Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		renderer->prepare(0.0f, 0.3f, 0.3f);

		// Activate shader
		textureShader->start();

		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture2"), 1);

		textureShader->uniformMat4("transform", transformMatrix);

		// Draw 
		renderer->renderTexturedModel(texturedModel);

		// stop using shader
		textureShader->stop();

		// update display
		display->updateDisplay();

		// check for key presses
		glfwSetKeyCallback(display->window(), key_callback);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}
void transformsTuteRotation()
{
	std::cout << "\n### --> Starting Transforms Tute" << std::endl;


	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();


	// Build and compile our shader program
	// initialise shader program
	ShaderProgram * textureShader = new ShaderProgram(Shader::transformShader);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords  (Note that we changed them to 'zoom in' on our texture image)
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	// load model to VAO
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();
	RawModel * model = loader->loadTextureVAO(vertices, sizeof(vertices), indices, sizeof(indices));

	// Load and create a texture 
	Texture * textureFile1 = new Texture("res/textures/container.jpg");
	Texture * textureFile2 = new Texture("res/textures/awesomeface.png");

	TexturedModel * texturedModel = new TexturedModel(*model, *textureFile1, *textureFile2);

	GLuint texture1 = textureFile1->ID();
	GLuint texture2 = textureFile2->ID();


	// Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		renderer->prepare(0.0f, 0.3f, 0.3f);


		// Activate shader
		textureShader->start();

		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture2"), 1);

		// ================== first container ================
		// create transform, with rotation changed over time
		glm::mat4 transformMatrix = Maths::createTransormationMatrix(glm::vec3(0.3f, -0.3f, 0), glm::vec3(45.0f, 0, glfwGetTime() * 20.0f), 0.5f);
		// set matrix to trnasformuniform
		textureShader->uniformMat4("transform", transformMatrix);
		// Draw 
		renderer->renderTexturedModel(texturedModel);

		// ================== second container ================
		// reset transform transform
		transformMatrix = glm::mat4();
		// set transform wirh scale changed over time
		transformMatrix = Maths::createTransormationMatrix(glm::vec3(-0.25f, 0.25f, 0), glm::vec3(45.0f, 0, 45.0f), (GLfloat)sin(glfwGetTime()));
		// set matrix to trnasformuniform
		textureShader->uniformMat4("transform", transformMatrix);
		// Draw 
		renderer->renderTexturedModel(texturedModel);
		
		// stop using shader
		textureShader->stop();

		// update display
		display->updateDisplay();

		// check for key presses
		glfwSetKeyCallback(display->window(), key_callback);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}
void transformsRotationPlanets()
{
	std::cout << "\n### --> Starting Rotating Planets" << std::endl;


	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();


	// Build and compile our shader program
	// initialise shader program
	ShaderProgram * textureShader = new ShaderProgram(Shader::transformShader);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords  (Note that we changed them to 'zoom in' on our texture image)
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	// load model to VAO
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();
	RawModel * model = loader->loadTextureVAO(vertices, sizeof(vertices), indices, sizeof(indices));
	//RawModel * rawModel = loader->loadTextureVAO(vertices, sizeof(vertices), indices, sizeof(indices));

	// Load and create a texture 
	Texture * sunTexture = new Texture("res/textures/Sun.png");
	Texture * planetTexture = new Texture("res/textures/M-Class.png");
	Texture * moonTexture = new Texture("res/textures/Moon.png");

	TexturedModel * sunModel = new TexturedModel(*model, *sunTexture, *sunTexture);
	TexturedModel * planetModel = new TexturedModel(*model, *planetTexture);
	TexturedModel * moonModel = new TexturedModel(*model, *moonTexture);


	// Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		renderer->prepare(0.0f, 0.0f, 0.0f);


		// Activate shader
		textureShader->start();

		// Bind Textures using texture units
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		//glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture2"), 1);

		// ================== sun container ================
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sunTexture->ID());
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		// create transform, with rotation changed over time
		glm::mat4 transformMatrix = Maths::createTransormationMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, glfwGetTime() * -10.0f), 0.3f);
			
		// set matrix to trnasformuniform
		textureShader->uniformMat4("transform", transformMatrix);
		// Draw 
		renderer->renderTexturedModel(sunModel);

		// ================== planet container ================
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetTexture->ID());
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		// reset transform transform
		transformMatrix = glm::mat4();
		// set transform with local and world rotation changed over time
		transformMatrix = Maths::createWorldTransformMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, glfwGetTime() * -20.0f), 0) *
			Maths::createTransormationMatrix(glm::vec3(0.45f, -0.45f, 0), glm::vec3(0, 0, glfwGetTime() * -20.0f), 0.2f);
		// set matrix to trnasformuniform
		textureShader->uniformMat4("transform", transformMatrix);
		// Draw 
		renderer->renderTexturedModel(planetModel);

		// ================== moon container ================
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, moonTexture->ID());
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		// multiple planets transformation by world rotation changed over time (scale and translate value need to be increased above the norm to achieve similar sizing and distance)
		transformMatrix = transformMatrix * Maths::createWorldTransformMatrix(glm::vec3(0.8f, -0.8f, 0), glm::vec3(0, 0, glfwGetTime() * -40.0f), 0.5f);
		// set matrix to trnasformuniform
		textureShader->uniformMat4("transform", transformMatrix);
		// Draw 
		renderer->renderTexturedModel(moonModel);

		// stop using shader
		textureShader->stop();

		// update display
		display->updateDisplay();

		// check for key presses
		glfwSetKeyCallback(display->window(), key_callback);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

/******************************************************************************************
*******************************************************************************************/
void quaterionsTute()
{
	std::cout << "\n### --> Starting Quaternion tute" << std::endl;

//==================== Start Up===============================
	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();


	// Build and compile our shader program
	// initialise shader program
	ShaderProgram * textureShader = new ShaderProgram(Shader::transformShader);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords  (Note that we changed them to 'zoom in' on our texture image)
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	// Quaterions
	glm::vec3 positions[2];
	glm::quat rotations[2];

	positions[0] = glm::vec3(0,0,0);
	positions[1] = glm::vec3(0,0,0);
	rotations[0] = glm::quat(glm::vec3(0, -1, 0));
	rotations[1] = glm::quat(glm::vec3(0, 1, 0));


	// load model to VAO
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();
	RawModel * model = loader->loadTextureVAO(vertices, sizeof(vertices), indices, sizeof(indices));

	// Load and create a texture 
	Texture * starTexture = new Texture("res/textures/Stars.png");
	Texture * sunTexture = new Texture("res/textures/Sun.png");
	Texture * planetTexture = new Texture("res/textures/M-Class.png");
	Texture * moonTexture = new Texture("res/textures/Moon.png");

	TexturedModel * starsModel = new TexturedModel(*model, *starTexture);
	TexturedModel * sunModel = new TexturedModel(*model, *sunTexture);
	TexturedModel * planetModel = new TexturedModel(*model, *planetTexture);
	TexturedModel * moonModel = new TexturedModel(*model, *moonTexture);
//=================== End Start Up ========================

//=================== Update & Draw ==============================

	// Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();


		// Render
		// Clear the colorbuffer
		renderer->prepare(0.0f, 0.0f, 0.0f);

		// Activate shader
		textureShader->start();

		// ================== stars ========================
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, starTexture->ID());
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		// create transform, with rotation changed over time
		glm::mat4 transformMatrix = Maths::createTransormationMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 2.0f);

		textureShader->uniformMat4("transform", transformMatrix);
		// Draw 
		renderer->renderTexturedModel(sunModel);

		// ================== sun container ================
		// quaterion values
		// use time to animate a alue between [0, 1]
		float s = glm::cos((float)glfwGetTime() * 0.25f);
		// standard linear interpolation
		glm::vec3 p = (1.0f - s) * positions[0] + s * positions[1];
		// quaternion slerp
		glm::quat r = glm::slerp(rotations[0], rotations[1], s);
		// build a matrix
		glm::mat4 m = glm::translate(p) * glm::toMat4(r);
	
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sunTexture->ID());
		glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		// reset transform transform
		transformMatrix = glm::mat4();
		// create transform, with rotation changed over time
		transformMatrix = Maths::quaternionTransformation(glm::vec3(s, 0, s), r, 0.3f);

		// set matrix to trnasformuniform
		textureShader->uniformMat4("transform", transformMatrix);
		// Draw 
		renderer->renderTexturedModel(sunModel);

		//// ================== planet container ================
		//// Bind Textures using texture units
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, planetTexture->ID());
		//glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		//// reset transform transform
		//transformMatrix = glm::mat4();
		//// set transform with local and world rotation changed over time
		//transformMatrix = Maths::createWorldRotationMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, glfwGetTime() * -20.0f), 0) *
		//	Maths::createTransormationMatrix(glm::vec3(0.45f, -0.45f, 0), glm::vec3(0, 0, glfwGetTime() * -20.0f), 0.2f);
		//// set matrix to trnasformuniform
		//textureShader->uniformMat4("transform", transformMatrix);
		//// Draw 
		//renderer->renderTexturedModel(planetModel);

		//// ================== moon container ================
		//// Bind Textures using texture units
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, moonTexture->ID());
		//glUniform1i(glGetUniformLocation(textureShader->ID(), "ourTexture1"), 0);
		//// multiple planets transformation by world rotation changed over time (scale and translate value need to be increased above the norm to achieve similar sizing and distance)
		//transformMatrix = transformMatrix * Maths::createWorldRotationMatrix(glm::vec3(0.8f, -0.8f, 0), glm::vec3(0, 0, glfwGetTime() * -40.0f), 0.5f);
		//// set matrix to trnasformuniform
		//textureShader->uniformMat4("transform", transformMatrix);
		//// Draw 
		//renderer->renderTexturedModel(moonModel);

		// stop using shader
		textureShader->stop();

		// update display
		display->updateDisplay();

		// check for key presses
		glfwSetKeyCallback(display->window(), key_callback);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	textureShader->cleanUp();
	loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}
/******************************************************************************************
*******************************************************************************************/
void threeDeeObjects()
{
	std::cout << "\n### --> Starting Transforms Tute" << std::endl;


	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	// initialise shader program
	ShaderProgram * shaderProgram = new ShaderProgram(Shader::cameraShader);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// load model to VAO
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();

	RawModel * rawModel = loader->loadToVAO(vertices, sizeof(vertices), 5); 

	// Load and create a texture 
	Texture * texture1 = new Texture("res/textures/container.jpg");
	Texture * texture2 = new Texture("res/textures/awesomeface.png");
	Texture * texture3 = new Texture("res/textures/colourfulWindow.bmp");

	// create a textured model
	TexturedModel * texturedModel = new TexturedModel(*rawModel, *texture3, *texture3, shaderProgram->ID());

	// the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	glm::mat4 cameraProjection;
	// set the cameras - FOV, Screen Ratio, near plane, far plane
	cameraProjection = glm::perspective(55.0f, 800.0f / 600.0f, 0.1f, 100.0f);

	// camera position
	glm::mat4 cameraPosition;
	cameraPosition = glm::translate(cameraPosition, glm::vec3(0.0f, 0.0f, -3.0f));

	// Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		GLdouble time = glfwGetTime();
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		renderer->prepare(0.0f, 0.0f, 0.0f );

		// Activate shader
		shaderProgram->start();

		// create transform, with rotation changed over time
		glm::mat4 modelTransform = Maths::createTransormationMatrix(glm::vec3(0, 0, 0),  // position
																	glm::vec3(time * -20.0f, time * -15.0f, 0.0f), // rotation
																	1.0f); // scale
		// pass updated model to shader
		shaderProgram->uniformMat4("model", modelTransform);
		
		// pass camera position to shader
		shaderProgram->uniformMat4("view", cameraPosition);
		
		// pass camera projection to shader
		shaderProgram->uniformMat4("projection", cameraProjection);

		// Draw container
		renderer->renderTexturedModel(texturedModel);

		// Swap the screen buffers
		glfwSwapBuffers(display->window());

		// check for key presses
		glfwSetKeyCallback(display->window(), key_callback);
	}
	// Properly de-allocate all resources once they've outlived their purpose

	loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void camera()
{
	std::cout << "\n### --> Starting Transforms Tute" << std::endl;


	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	// initialise shader program
	ShaderProgram * shaderProgram = new ShaderProgram(Shader::cameraShader);

	// load model to VAO
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();
	
	// create cube model
	RawModel * rawModel = DynamicModels::cube(); 

	// create textures
	Texture * sunTexture = new Texture("res/textures/Sun.png");
	Texture * planetTexture = new Texture("res/textures/M-Class.png");
	Texture * moonTexture = new Texture("res/textures/Moon.png");

	// add textures to cube models
	TexturedModel * sunModel = new TexturedModel(*rawModel, *sunTexture, *sunTexture, shaderProgram->ID());
	TexturedModel * planetModel = new TexturedModel(*rawModel, *planetTexture, *planetTexture, shaderProgram->ID());
	TexturedModel * moonModel = new TexturedModel(*rawModel, *moonTexture, *moonTexture, shaderProgram->ID());

	// create planetary entities 
	Entity * sun = new Entity(sunModel, glm::vec3(0,0,0), glm::vec3(0, 0, 0), 1.0f);
	Entity * planet = new Entity(planetModel, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);
	Entity * moon = new Entity(moonModel, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);

	// the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	glm::mat4 cameraProjection;
	// set the cameras - FOV, Screen Ratio, near plane, far plane
	cameraProjection = glm::perspective(55.0f, 800.0f / 600.0f, 0.1f, 100.0f);

	// camera position
	glm::mat4 cameraPosition;
	cameraPosition = glm::translate(cameraPosition, glm::vec3(0.0f, 0.5f, -2.0f));

	// Game loop
	while (!glfwWindowShouldClose(display->window()))
	{
		GLdouble time = glfwGetTime();
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		renderer->prepare(0.0f, 0.0f, 0.0f);

		// Activate shader
		shaderProgram->start();

		// pass camera position to shader
		shaderProgram->uniformMat4("view", cameraPosition);

		// pass camera projection to shader
		shaderProgram->uniformMat4("projection", cameraProjection);

		// ================== sun  ================
		// create transform, with rotation changed over time
		sun->transform(Maths::createTransormationMatrix(glm::vec3(0, 0, 0), glm::vec3(0, time * -15.0f, 0), 0.3f));
		renderer->renderEntity(sun, shaderProgram);

		// ================== planet  ================
		// set transform with local and world rotation changed over time
		planet->transform(Maths::createWorldTransformMatrix(glm::vec3(0, 0, 0), glm::vec3(0, time * -25.0f, 0), 0) *
			Maths::createTransormationMatrix(glm::vec3(0.45f, 0, -0.45f), glm::vec3(0, time * -25.0f, 0), 0.2f));
		renderer->renderEntity(planet, shaderProgram);

		// ================== moon  ================
		// multiple planets transformation by world rotation changed over time 
		// - scale and translate value need to be increased above the norm to achieve similar sizing and distance
		moon->transform(planet->transform() * Maths::createWorldTransformMatrix(glm::vec3(0.8f, 0, -0.8f), glm::vec3(0, time * -75.0f, 0), 0.5f));
		renderer->renderEntity(moon, shaderProgram);

		// Swap the screen buffers
		glfwSwapBuffers(display->window());

		// check for key presses
		glfwSetKeyCallback(display->window(), key_callback);
	}
	// Properly de-allocate all resources once they've outlived their purpose

	loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


/// key press callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		// toggle wireFrame bool
		wireFrame = !wireFrame; 
		// toggle wire frame based on bool state
		wireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//if (action == GLFW_PRESS)
	//	keys[key] = true;
	//else if (action == GLFW_RELEASE)
	//	keys[key] = false;

}

/// function that alternates screens background colour
void screenColour(GLfloat *red, GLfloat *green, GLfloat *blue, GLboolean *allColour)
{
	//std::cout << "red = " << *red << " green = " << *green << " blue = " << *blue << std::endl;
	if (*red >= 1.0f && *green >= 1.0f && *blue >= 1.0f)
	{
		*allColour = true;
		//std::cout << " - allColour = true" << std::endl;
	}
	if (*red <= 0 && *green <= 0 && *blue <= 0)
	{
		*allColour = false;
		//std::cout << " - allColour = false" << std::endl;
	}
	// change red
	if (!*allColour)
	{
		if (*red < 1.0f)
		{
			*red += 0.01f;
		}
		else if (*green < 1.0f)
		{
			*green += 0.01f;
		}
		else if (*blue < 1.0f)
		{
			*blue += 0.01f;
		}
	}
	else if (*allColour)
	{
		if (*blue > 0.0f)
		{
			*blue -= 0.01f;
		}
		else if (*green > 0.0f)
		{
			*green -= 0.01f;
		}
		else if (*red > 0.0f)
		{
			*red -= 0.01f;
		}
	}



}

