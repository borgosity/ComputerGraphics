#include "ModelApp.h"



ModelApp::ModelApp()
{
}


ModelApp::~ModelApp()
{
	// display
	deallocate(m_display);
	deallocate(m_loader);
	deallocate(m_renderer);
	deallocate(m_camera);
	deallocate(m_cameraController);
	// shaders
	deallocate(m_meshSP);
	deallocate(m_lightSP);
	// textures
	deallocate(m_whiteTexture);
	deallocate(m_lightTexture);
	// models
	deallocate(m_crisisModel);
	deallocate(m_dragonModel);
	deallocate(m_bunnyModel);
	deallocate(m_buddhaModel);
	deallocate(m_lucyModel);
	deallocate(m_soulSpearModel);
	// fbx
	deallocate(m_duckModel);
	deallocate(m_jeepModel);
	// raw models
	deallocate(m_cubeRm);

	// textured models
	deallocate(m_lightingTM);
	

	// entities 

	// lights
	deallocate(m_lamp);
	deallocate(m_light);

}

bool ModelApp::start()
{
	std::cout << "\n### --> Start ModelApp" << std::endl;

	// camera
	m_camera = new Camera(glm::vec3(0, 3, 10), CAM_SPEED, FOV, NEAR_PLANE, FAR_PLANE);
	m_cameraController = new CameraController();

	// initialise display
	m_display = new DisplayManager("Model Example App", SCREEN_W, SCREEN_H);
	m_display->createDisplay();

	// Set the required callback functions for user input
	glfwSetKeyCallback(m_display->window(), Controller::keyCallback);
	glfwSetCursorPosCallback(m_display->window(), Controller::mouseCallback);
	glfwSetScrollCallback(m_display->window(), Controller::scrollCallback);

	// Build and compile our shader program
	// initialise shader program

	m_staticShader = new StaticShader(Shader::phongShader);
	m_lightSP = new LightShader(Shader::lightShader);
	m_meshSP = new ShaderProgram(Shader::meshShader);

	// load model to VAO
	m_loader = new Loader();
	m_renderer = new Renderer();

	// load textures
	m_whiteTexture = new Texture("res/textures/white.png");
	m_lightTexture = new Texture("res/textures/light.png");
	
	// create lights
	m_cubeRm = DynamicModels::cube();
	m_lightingTM = new TexturedModel(*m_cubeRm, *m_lightTexture, *m_lightTexture, m_lightSP->ID());

	m_lamp = new Lamp(glm::vec3(0.0f, 3.0f, 2.0f));
	// light shader attributes
	glm::vec3 lightPosition = m_lamp->position();
	glm::vec3 lightColour = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 lightDirection = -m_lamp->position();
	glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

	m_light = new Light(lightPosition, lightColour, lightDirection, lightAmbient, lightDiffuse, lightSpecular);

	// load models
	m_crisisModel = new MeshModel("res/models/nanosuit/nanosuit.obj");
	m_crisisModel->transform = Maths::createTransormationMatrix(glm::vec3(0.0f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	m_dragonModel = new MeshModel("res/models/stanford/Dragon.obj");
	m_dragonModel->transform = Maths::createTransormationMatrix(glm::vec3(2.5f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	m_bunnyModel = new MeshModel("res/models/stanford/Bunny.obj");
	m_bunnyModel->transform = Maths::createTransormationMatrix(glm::vec3(-4.0f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	m_lucyModel = new MeshModel("res/models/stanford/Lucy.obj");
	m_lucyModel->transform = Maths::createTransormationMatrix(glm::vec3(-2.0f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	m_soulSpearModel = new MeshModel("res/models/soulspear/soulspear.obj");
	m_soulSpearModel->transform = Maths::createTransormationMatrix(glm::vec3(-1.42f, 3.0f, 0.52f), glm::vec3(180.0f, 0.0f, 0.0f), 0.3f);
	m_buddhaModel = new MeshModel("res/models/stanford/Buddha.obj");
	m_buddhaModel->transform = Maths::createTransormationMatrix(glm::vec3(4.5f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	// fbx models
	m_duckModel = new MeshModel("res/models/fbx/duck/duck_ascii.fbx");
	m_duckModel->transform = Maths::createTransormationMatrix(glm::vec3(6.0f, -1.75f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), 0.01f);
	m_jeepModel = new MeshModel("res/models/fbx/jeep/jeep1.fbx");
	m_jeepModel->transform = Maths::createTransormationMatrix(glm::vec3(-6.0f, -1.75f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f), 0.05f);

	return true;
}

bool ModelApp::update(GLfloat a_deltaTime)
{

	m_cameraController->update(*m_camera, a_deltaTime);
	
	return true;
}

bool ModelApp::fixedUpdate(GLfloat a_deltaTime)
{
	return true;
}

bool ModelApp::draw(GLfloat a_deltaTime)
{
	GLdouble time = glfwGetTime();
	// Render
	// Clear the colorbuffer
	m_renderer->prepare(0.0f, 0.2f, 0.2f);
	// ###############################> START DRAW CODE <#########################################################
	// Activate shader
	m_lightSP->start();
	
	// update shader with light
	m_lightSP->update(*m_camera, *m_light);
	
	// draw model
	m_crisisModel->draw(*m_lightSP);
	//m_dragonModel->draw(*m_lightSP);
	//m_bunnyModel->draw(*m_lightSP);
	//m_buddhaModel->draw(*m_lightSP);
	//m_lucyModel->transform = Maths::createTransormationMatrix(glm::vec3(-2.0f, -1.75f, 0.0f), glm::vec3(0.0f, time * 25.0f, 0.0f), 0.2f);
	m_lucyModel->draw(*m_lightSP);	
	m_soulSpearModel->draw(*m_lightSP);
	// fbx
	m_duckModel->draw(*m_lightSP);
	//m_jeepModel->draw(*m_lightSP);

	// stop shader
	m_lightSP->stop();

	// draw light source
	m_lamp->draw(*m_camera);

	// ##############################> END DRAW STUFF <###########################################################
	// Swap the screen buffers
	m_display->updateDisplay();
	return true;
}

bool ModelApp::stop()
{
	// Properly de-allocate all resources once they've outlived their purpose

	m_loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return true;
}

