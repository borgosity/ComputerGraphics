#include "LightsApp.h"



LightsApp::LightsApp()
{
}


LightsApp::~LightsApp()
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
	deallocate(m_pLightSP);
	deallocate(m_sLightSP);
	deallocate(m_ssLightSP);

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

	// textured models

	// entities 
	deallocate(m_square);
	// lights
	deallocate(m_lamp);
	deallocate(m_light);
	deallocate(m_pointLamp);
	deallocate(m_pointLight);
	deallocate(m_spotLamp);
	deallocate(m_spotLight);
	deallocate(m_softSpotLamp);
	deallocate(m_softSpotLight);
}

bool LightsApp::start()
{
	std::cout << "\n### --> Start LightsApp" << std::endl;

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
	m_pLightSP = new PointLightShader(Shader::pointLight);
	m_sLightSP = new SpotLightShader(Shader::spotLight);
	m_ssLightSP = new SoftSpotShader(Shader::spotLight_soft);

	// load model to VAO
	m_loader = new Loader();
	m_renderer = new Renderer();

	// load textures
	m_whiteTexture = new Texture("res/textures/white.png");
	m_lightTexture = new Texture("res/textures/light.png");
	//-------------------------------- lights ----------------------------------	
	setupLights();

	//----------------------------------- models ----------------------------
	setupModels();


	return true;
}

bool LightsApp::update(GLfloat a_deltaTime)
{

	m_cameraController->update(*m_camera, a_deltaTime);
	
	return true;
}

bool LightsApp::fixedUpdate(GLfloat a_deltaTime)
{
	return true;
}

bool LightsApp::draw(GLfloat a_deltaTime)
{
	GLdouble time = glfwGetTime();
	// Render
	// Clear the colorbuffer
	m_renderer->prepare(0.0f, 0.2f, 0.2f);
	// ###############################> START DRAW CODE <#########################################################

	//---------------- Directional Light -----------------
	m_lightSP->start();
	
	// update shader with light
	m_lightSP->update(*m_camera, *m_light);
	
	// draw model
	m_buddhaModel->draw(*m_lightSP);
	m_crisisModel->draw(*m_lightSP);
	m_soulSpearModel->draw(*m_lightSP);
	// fbx
	m_jeepModel->draw(*m_lightSP);

	// stop shader
	m_lightSP->stop();

	// ------------ PointLight --------------------
	m_pLightSP->start();

	m_pLightSP->update(*m_camera, *m_pointLight);

	m_bunnyModel->transform = Maths::createTransormationMatrix(glm::vec3(-4.0f, -1.75f, 0.0f), glm::vec3(0.0f, time * 25.0f, 0.0f), 0.2f);
	m_bunnyModel->draw(*m_pLightSP);

	//m_square->draw(*m_pLightSP);
	m_lucyModel->transform = Maths::createTransormationMatrix(glm::vec3(-2.0f, -1.75f, 0.0f), glm::vec3(0.0f, time * 25.0f, 0.0f), 0.2f);
	m_lucyModel->draw(*m_pLightSP);

	m_pLightSP->stop();

	// ------------ SpotLight --------------------
	m_sLightSP->start();

	m_sLightSP->update(*m_camera, *m_spotLight);

	m_dragonModel->transform = Maths::createTransormationMatrix(glm::vec3(4.0f, -1.75f, 0.0f), glm::vec3(0.0f, time * 25.0f, 0.0f), 0.2f);
	m_dragonModel->draw(*m_sLightSP);

	m_sLightSP->stop();

	// ------------ SoftSpotLight --------------------
	m_ssLightSP->start();

	m_ssLightSP->update(*m_camera, *m_softSpotLight);

	m_duckModel->transform = Maths::createTransormationMatrix(glm::vec3(6.0f, -1.75f, 0.0f), glm::vec3(0.0f, time * 25.0f, 0.0f), 0.01f);
	m_duckModel->draw(*m_ssLightSP);


	m_ssLightSP->stop();

	

	// draw light source
	m_lamp->draw(*m_camera);
	m_pointLamp->draw(*m_camera);
	m_spotLamp->draw(*m_camera);
	m_softSpotLamp->draw(*m_camera);


	// ##############################> END DRAW STUFF <###########################################################
	// Swap the screen buffers
	m_display->updateDisplay();
	return true;
}

bool LightsApp::stop()
{
	// Properly de-allocate all resources once they've outlived their purpose

	m_loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return true;
}

void LightsApp::setupLights()
{
	m_lamp = new Lamp(glm::vec3(0.0f, 3.0f, 2.0f));
	m_pointLamp = new Lamp(glm::vec3(-3.0f, 2.0f, 0.0f));
	m_spotLamp = new Lamp(glm::vec3(4.0f, 1.0f, 0.0f));
	m_softSpotLamp = new Lamp(glm::vec3(6.0f, 1.0f, 0.0f));
	
	// light shader attributes
	glm::vec3 lightPosition = m_lamp->position();
	glm::vec3 lightColour = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 lightDirection = -m_lamp->position();
	glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	// point light
	glm::vec3 pLightDirection = -m_pointLamp->position();
	glm::vec3 pLightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 pLightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 pLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	GLfloat	  pLightConstant = 1.0f;
	GLfloat	  pLightLinear = 0.22f;
	GLfloat	  pLightQuadratic = 0.20f;
	// spot light
	glm::vec3 sLightPosition = m_spotLamp->position();
	glm::vec3 sLightDirection = -m_spotLamp->position();
	glm::vec3 sLightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 sLightDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 sLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	GLfloat	  sLightConstant = 1.0f;
	GLfloat	  sLightLinear = 0.09f;
	GLfloat	  sLightQuadratic = 0.032f;
	GLfloat   sLightCutOff = glm::cos(glm::radians(12.5f));
	// soft spot light
	glm::vec3 ssLightPosition = m_softSpotLamp->position();
	glm::vec3 ssLightDirection = -m_softSpotLamp->position();
	glm::vec3 ssLightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 ssLightDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 ssLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	GLfloat	  ssLightConstant = 1.0f;
	GLfloat	  ssLightLinear = 0.09f;
	GLfloat	  ssLightQuadratic = 0.032f;
	GLfloat   ssLightCutOff = glm::cos(glm::radians(12.5f));
	GLfloat   ssLightOuterCutOff = glm::cos(glm::radians(17.5f));

	m_light =         new Light(lightPosition, lightColour, lightDirection, lightAmbient, lightDiffuse, lightSpecular);
	m_pointLight =    new Light(pLightDirection, pLightAmbient, pLightDiffuse, pLightSpecular, pLightConstant, pLightLinear, pLightQuadratic);
	m_spotLight =	  new Light(sLightPosition, sLightDirection, sLightAmbient, sLightDiffuse, sLightSpecular, sLightConstant, sLightLinear, sLightQuadratic, sLightCutOff);
	m_softSpotLight = new Light(ssLightPosition, ssLightDirection, ssLightAmbient, ssLightDiffuse, ssLightSpecular, ssLightConstant, ssLightLinear, ssLightQuadratic, ssLightCutOff, ssLightOuterCutOff);

}

void LightsApp::setupModels()
{
	m_square = new Square(glm::vec3(0.0f, 0.0f, 0.0f));

	// load models
	m_crisisModel = new MeshModel("res/models/nanosuit/nanosuit.obj");
	m_crisisModel->transform = Maths::createTransormationMatrix(glm::vec3(0.0f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	m_bunnyModel = new MeshModel("res/models/stanford/Bunny.obj");
	m_bunnyModel->transform = Maths::createTransormationMatrix(glm::vec3(-4.0f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	m_lucyModel = new MeshModel("res/models/stanford/Lucy.obj");
	m_lucyModel->transform = Maths::createTransormationMatrix(glm::vec3(-2.0f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	m_soulSpearModel = new MeshModel("res/models/soulspear/soulspear.obj");
	m_soulSpearModel->transform = Maths::createTransormationMatrix(glm::vec3(1.0f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.3f);
	m_buddhaModel = new MeshModel("res/models/stanford/Buddha.obj");
	m_buddhaModel->transform = Maths::createTransormationMatrix(glm::vec3(2.0f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	m_dragonModel = new MeshModel("res/models/stanford/Dragon.obj");
	m_dragonModel->transform = Maths::createTransormationMatrix(glm::vec3(4.5f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	// fbx models
	m_duckModel = new MeshModel("res/models/fbx/duck/duck_ascii.fbx");
	m_duckModel->transform = Maths::createTransormationMatrix(glm::vec3(6.0f, -1.75f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), 0.01f);
	m_jeepModel = new MeshModel("res/models/fbx/jeep/jeep1.fbx");
	m_jeepModel->transform = Maths::createTransormationMatrix(glm::vec3(-6.0f, -1.75f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f), 0.05f);

}

