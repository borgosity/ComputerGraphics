#include "AnimationApp.h"



AnimationApp::AnimationApp()
{
}


AnimationApp::~AnimationApp()
{
	// display
	deallocate(m_display);
	deallocate(m_loader);
	deallocate(m_renderer);
	deallocate(m_camera);
	deallocate(m_cameraController);
	deallocate(m_uiController);


	// shaders
	deallocate(m_lightSP);
	deallocate(m_pLightSP);
	deallocate(m_sLightSP);
	deallocate(m_ssLightSP);
	deallocate(m_planeSP);
	deallocate(m_animeSP);

	// textures
	deallocate(m_whiteTexture);
	deallocate(m_lightTexture);
	// models
	deallocate(m_dragonModel);
	deallocate(m_handModel);
	deallocate(m_handAnimated);

	// fbx
	deallocate(m_duckModel);
	// raw models

	// textured models

	// entities 
	deallocate(m_square);
	deallocate(m_plane);

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

bool AnimationApp::start()
{
	std::cout << "\n### --> Start AnimationApp" << std::endl;

	// camera
	m_camera = new Camera(glm::vec3(0, 3, 10), CAM_SPEED, FOV, NEAR_PLANE, FAR_PLANE);
	
	// controllers
	m_cameraController = new CameraController();
	m_uiController = new UIController();

	// initialise display
	m_display = new DisplayManager("Model Example App", SCREEN_W, SCREEN_H);
	m_display->createDisplay();

	// Set the required callback functions for user input
	glfwSetKeyCallback(m_display->window(), Controller::keyCallback);
	glfwSetCursorPosCallback(m_display->window(), Controller::mouseCallback);
	glfwSetScrollCallback(m_display->window(), Controller::scrollCallback);

	// Build and compile our shader program
	// initialise shader program
	m_lightSP = new LightShader(Shader::lightShader);
	m_pLightSP = new PointLightShader(Shader::pointLight);
	m_sLightSP = new SpotLightShader(Shader::spotLight);
	m_ssLightSP = new SoftSpotShader(Shader::spotLight_soft);

	m_planeSP = new ShaderProgram(Shader::planeShader);
	m_animeSP = new AnimeShader(Shader::animeShader);

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

	m_plane = new Plane(glm::vec3(-32.0f,0.0f, -32.0f), 64);
	
	return true;
}

bool AnimationApp::update(GLfloat a_deltaTime)
{
	// updat eui controller
	m_uiController->update(a_deltaTime);

	// update camera controller
	m_cameraController->update(*m_camera, a_deltaTime);
	
	return true;
}

bool AnimationApp::fixedUpdate(GLfloat a_deltaTime)
{
	return true;
}

bool AnimationApp::draw(GLfloat a_deltaTime)
{
	GLdouble time = glfwGetTime();
	// Render
	// Clear the colorbuffer
	m_renderer->prepare(0.0f, 0.0f, 0.0f);
	// ###############################> START DRAW CODE <#########################################################

	m_animeSP->start();
	m_animeSP->update(*m_camera, *m_softSpotLight);
	// draw stuff here
	m_handAnimated->transform = Maths::createTransormationMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, time * 25.0f, 0.0f), 1.0f);
	m_handAnimated->animate(*m_animeSP);
	//m_handModel->draw(*m_animeSP);
	m_animeSP->stop();

	//---------------- Directional Light -----------------
	m_lightSP->start();
	
	// update shader with light
	m_lightSP->update(*m_camera, *m_light);
	
	// draw model

	//m_dragonModel->transform = Maths::createTransormationMatrix(glm::vec3(4.0f, -1.75f, 0.0f), glm::vec3(0.0f, time * 25.0f, 0.0f), 0.2f);
	//m_dragonModel->draw(*m_lightSP);
	// stop shader
	m_lightSP->stop();

	// ------------ PointLight --------------------
	m_pLightSP->start();
	m_pLightSP->update(*m_camera, *m_pointLight);

	//m_duckModel->transform = Maths::createTransormationMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, time * 25.0f, 0.0f), 0.01f);
	//m_duckModel->draw(*m_pLightSP);
	
	m_pLightSP->stop();

	// ------------ SpotLight --------------------
	m_sLightSP->start();
	m_sLightSP->update(*m_camera, *m_spotLight);

	m_sLightSP->stop();

	// ------------ SoftSpotLight --------------------
	m_ssLightSP->start();

	m_ssLightSP->update(*m_camera, *m_softSpotLight);

	//m_handModel->transform = Maths::createTransormationMatrix(glm::vec3(-1.0f, 0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	//m_handModel->draw(*m_ssLightSP);

	m_ssLightSP->stop();

	
	//m_plane->draw(*m_camera);

	// draw light source
	//m_lamp->draw(*m_camera);
	//m_pointLamp->draw(*m_camera);
	//m_spotLamp->draw(*m_camera);
	m_softSpotLamp->draw(*m_camera);


	// ##############################> END DRAW STUFF <###########################################################
	// Swap the screen buffers
	m_display->updateDisplay();
	return true;
}

bool AnimationApp::stop()
{
	// Properly de-allocate all resources once they've outlived their purpose

	m_loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return true;
}

void AnimationApp::setupLights()
{
	m_lamp = new Lamp(glm::vec3(0.0f, 3.0f, 2.0f));
	m_pointLamp = new Lamp(glm::vec3(0.0f, 5.0f, 0.0f));
	m_spotLamp = new Lamp(glm::vec3(0.0f, 3.0f, 0.0f));
	m_softSpotLamp = new Lamp(glm::vec3(0.0f, 5.0f, 1.0f));
	
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

void AnimationApp::setupModels()
{
	m_square = new Square(glm::vec3(0.0f, 0.0f, 0.0f));

	// load models
	m_handModel = new MeshModel("res/models/hand/hand_00.obj");
	m_handModel->transform = Maths::createTransormationMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2.0f);
	m_handAnimated = new AnimatedModel("res/models/hand/hand_00.obj", "res/models/hand/hand_37.obj");
	m_handAnimated->transform = Maths::createTransormationMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2.0f);

	m_dragonModel = new MeshModel("res/models/stanford/Dragon.obj");
	m_dragonModel->transform = Maths::createTransormationMatrix(glm::vec3(4.5f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	// fbx models
	m_duckModel = new MeshModel("res/models/fbx/duck/duck_ascii.fbx");
	m_duckModel->transform = Maths::createTransormationMatrix(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), 0.01f);

}

