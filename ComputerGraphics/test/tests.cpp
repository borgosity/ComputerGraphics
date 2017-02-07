#include "tests.h"
#include "Loader.h"
#include "Renderer.h"
#include "Shaderer.h"


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
	Shaderer * shader = new Shaderer();
	GLuint shaderProgram = shader->buildShader();


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
	std::cout << "Starting OpenGL Test - 02" << std::endl;

	// initialise display
	DisplayManager * display = new DisplayManager();
	display->createDisplay();

	// initialise loader
	Loader * loader = new Loader();
	Renderer * renderer = new Renderer();

	// Build and compile our shader program
	Shaderer * shader = new Shaderer();
	GLuint shaderProgram = shader->buildShader();

	// initialise vertices for triangle
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // Left  
		0.5f, -0.5f, 0.0f, // Right 
		0.0f,  0.5f, 0.0f  // Top   
	};

	RawModel * model = loader->loadToVAO(vertices, 9);

	// draw loop
	while (!glfwWindowShouldClose(display->window()))
	{
		// check and call events
		glfwPollEvents();

		// game logic
		// render
		renderer->prepare(0.2f, 0.3f, 0.3f);
		//renderer->render(model, shaderProgram);

		// Draw our first triangle
		glUseProgram(shaderProgram);
		std::cout << "VAO ID - " << model->vaoID() << std::endl;
		glBindVertexArray(model->vaoID());
		glDrawArrays(GL_TRIANGLES, 0, model->vertexCount());
		glBindVertexArray(0);

		// swap the buffers
		glfwSwapBuffers(display->window());
		glfwSetKeyCallback(display->window(), key_callback);

	}
	// VAO VBO clean up
	loader->cleanUp();
	// Terminate GLFW
	glfwTerminate();
	std::cout << "Finished!!  - 02 \n\n" << std::endl;
}

/// key press callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		std::cout << "Escape Key Pressed" << std::endl;
	}
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

