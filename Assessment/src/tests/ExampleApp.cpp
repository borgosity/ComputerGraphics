#include "ExampleApp.h"



ExampleApp::ExampleApp()
{
}


ExampleApp::~ExampleApp()
{
}

bool ExampleApp::start()
{
	std::cout << "\n### --> Start ExampleApp" << std::endl;

	// camera
	m_camera = new Camera(glm::vec3(0, -0.5f, 1.5f), CAM_SPEED, FOV, NEAR_PLANE, FAR_PLANE);
	m_cameraController = new CameraController();

	// initialise display
	m_display = new DisplayManager("Example App", SCREEN_W, SCREEN_H);
	m_display->createDisplay();

	// Set the required callback functions for user input
	glfwSetKeyCallback(m_display->window(), Controller::keyCallback);
	glfwSetCursorPosCallback(m_display->window(), Controller::mouseCallback);
	glfwSetScrollCallback(m_display->window(), Controller::scrollCallback);

	// Build and compile our shader program
	// initialise shader program
	m_shaderProgram = new ShaderProgram(Shader::cameraShader);

	// load model to VAO
	m_loader = new Loader();
	m_renderer = new Renderer();

	// create cube model
	m_cubeModel = DynamicModels::cube();
	m_squareModel = DynamicModels::square(2.0f);

	// create textures
	m_sunTexture = new Texture("res/textures/Sun.png");
	m_planetTexture = new Texture("res/textures/M-Class.png");
	m_moonTexture = new Texture("res/textures/Moon.png");

	// add textures to cube models
	m_sunModel = new TexturedModel(*m_cubeModel, *m_sunTexture, *m_sunTexture, m_shaderProgram->ID());
	m_planetModel = new TexturedModel(*m_cubeModel, *m_planetTexture, *m_planetTexture, m_shaderProgram->ID());
	m_moonModel = new TexturedModel(*m_cubeModel, *m_moonTexture, *m_moonTexture, m_shaderProgram->ID());

	// create planetary entities 
	m_sun = new Entity(m_sunModel, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);
	m_planet = new Entity(m_planetModel, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);
	m_moon = new Entity(m_moonModel, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);

	return true;
}

bool ExampleApp::update(GLfloat a_deltaTime)
{

	m_cameraController->update(*m_camera, a_deltaTime);
	
	return true;
}

bool ExampleApp::fixedUpdate(GLfloat a_deltaTime)
{
	return true;
}

bool ExampleApp::draw(GLfloat a_deltaTime)
{
	GLdouble time = glfwGetTime();
	// Render
	// Clear the colorbuffer
	m_renderer->prepare(0.1f, 0.1f, 0.1f);

	// Activate shader
	m_shaderProgram->start();

	// pass camera position to shader 
	m_shaderProgram->uniformMat4("view", m_camera->viewMatrix());
	// pass camera projection to shader
	m_shaderProgram->uniformMat4("projection", m_camera->projection());

	// ================== sun  ================
	// create transform, with rotation changed over time
	m_sun->transform(Maths::createTransormationMatrix(glm::vec3(0, 0, 0), glm::vec3(0, time * -15.0f, 0), 0.4f));
	m_renderer->renderEntity(m_sun, m_shaderProgram);

	// ================== planet  ================
	// create an elliptical type path for the planet to follow
	glm::vec3 elipse = glm::vec3(glm::cos(time) * 0.75f, 0, glm::sin(time) * 1.5f);
	// set transform with local and world rotation changed over time
	m_planet->transform(Maths::createWorldTransformMatrix(glm::vec3(0, 0, 0), glm::vec3(0, time * -25.0f, 0), 0) *	// world transform around sun
						Maths::createTransormationMatrix(elipse, glm::vec3(0, time * -80.0f, 0), 0.2f));			// local rotation
	m_renderer->renderEntity(m_planet, m_shaderProgram);

	// ================== moon  ================
	// multiple planets transformation by world rotation changed over time 
	// - scale and translate value need to be increased above the norm to achieve similar sizing and distance
	m_moon->transform(m_planet->transform() *	// planets transform 
					  Maths::createWorldTransformMatrix(glm::vec3(0.8f, 0, -0.8f), glm::vec3(0, time * -75.0f, 0), 0.5f)); // world rotation
	m_renderer->renderEntity(m_moon, m_shaderProgram);

	// De-activate shader
	m_shaderProgram->stop();

	// Swap the screen buffers
	glfwSwapBuffers(m_display->window());

	return true;
}

bool ExampleApp::stop()
{
	// Properly de-allocate all resources once they've outlived their purpose

	m_loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return true;
}

