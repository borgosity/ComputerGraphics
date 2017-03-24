#include "AssessmentApp.h"



AssessmentApp::AssessmentApp()
{
}


AssessmentApp::~AssessmentApp()
{

}
/// ***************************************************************************************
///	Start application by initialising things
/// ***************************************************************************************
bool AssessmentApp::start()
{
	std::cout << "\n### --> Start AssessmentApp" << std::endl;

	// camera
	m_camera = new Camera(glm::vec3(0, 1, 5), CAM_SPEED, FOV, NEAR_PLANE, FAR_PLANE);
	
	// controllers
	m_cameraController = new CameraController();
	m_uiController = new UIController();

	// initialise display
	m_display = new DisplayManager("Assessment App", SCREEN_W, SCREEN_H);
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
	m_particleSP = new ParticleShader(Shader::particleShader);

	// load model to VAO
	m_loader = new Loader();
	m_renderer = new Renderer();

	// models
	setupModels();

	// lights	
	setupLights();

	// particles
	GLuint  maxParticles = 1000;
	GLuint  emitRate = 200;
	GLfloat lifetimeMin = 0.1f;
	GLfloat lifetimeMax = 1.0f;
	GLfloat velocityMin = 0.01f;
	GLfloat velocityMax = 1.0f;
	GLfloat startSize = 0.15f;
	GLfloat endSize = 0.001f;
	const glm::vec4 startColour(0, 0.7, 0, 1);
	const glm::vec4 endColour(0, 0.2, 0, 1);
	// emitter
	m_emitter = new ParticleEmitter(glm::vec3(6.0f, 0.78f, 0.5f));
	m_emitter->init(maxParticles, emitRate, 
					lifetimeMin, lifetimeMax,
					velocityMin, velocityMax, 
					startSize, endSize,
					startColour, endColour);
	
	m_particleColour = startColour;
	m_animeSP->lightPosition(m_emitter->emitterPosition());

	// GUI
	m_clearColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// collisions/culling
	Maths::frustrumPlanes(m_camera->projection(), m_camera->viewMatrix(), m_vPlanes);
	m_boundingSphere = new BoundingSphere(m_mirror->position(), 0.5f);
	return true;
}
/// ***************************************************************************************
///	Application updates
/// ***************************************************************************************
bool AssessmentApp::update(GLdouble a_deltaTime)
{
	// update ui controller
	m_uiController->update(a_deltaTime);
	if (m_uiController->toggleCursor()) {
		glfwSetInputMode(m_display->window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(m_display->window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// update camera controller
		m_cameraController->update(*m_camera, a_deltaTime);
	}

	// update particles
	m_emitter->startColour(m_particleColour); // update required for gui update
	m_emitter->update((GLfloat)a_deltaTime, m_camera->transform());
	m_emitterPosition = m_emitter->emitterPosition();
	// update planes and call frustrum culling update
	Maths::frustrumPlanes(m_camera->projection(), m_camera->viewMatrix(), m_vPlanes);
	
	return true;
}
/// ***************************************************************************************
///	Application fixed updates
/// ***************************************************************************************
bool AssessmentApp::fixedUpdate(GLdouble a_deltaTime)
{
	return true;
}
/// ***************************************************************************************
///	Draw Application
/// ***************************************************************************************
bool AssessmentApp::draw(GLdouble a_deltaTime)
{
	GLdouble time = glfwGetTime();
	// bind frame buffer start for post processing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	m_mirror->bindFBO();

	// GUI start
	ImGui_ImplGlfwGL3_NewFrame();
	debugGUI();

	// Render
	// Clear the colorbuffer
	m_renderer->prepare(m_clearColour);

	// =================================> START DRAW CODE <=================================
	// ++++ Passs One +++++++++
	// draw objects
	objectDraw(a_deltaTime);

	// ++++ Passs Two +++++++++
	m_mirror->unbindFBO();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//draw objects
	objectDraw(a_deltaTime);

	// post processing
	// don't render cube if not in view
	if (AABBculling(*m_mirror->aabb())) {
		m_mirror->draw(*m_camera);
	}

	// draw GUI 
	ImGui::Render();
	
	// =================================>  END DRAW STUFF <=================================
	// Swap the screen buffers
	m_display->updateDisplay();
	return true;
}
/// ***************************************************************************************
/// Stop Application
/// ***************************************************************************************
bool AssessmentApp::stop()
{
	// Properly de-allocate all resources once they've outlived their purpose
	m_loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return true;
}
/// ***************************************************************************************
///	Function for drawing objects so we can repeat draws
/// ***************************************************************************************
void AssessmentApp::objectDraw(GLdouble a_deltaTime)
{
	GLdouble time = glfwGetTime();
	// draw light sources
	m_dragonlamp->draw(*m_camera);
	m_lucyLamp->draw(*m_camera);
	m_duckLamp->draw(*m_camera);
	m_planeLamp->draw(*m_camera);
	// ------------ PointLight --------------------
	m_pLightSP->start();
	m_pLightSP->update(*m_camera, *m_pointLight);
	m_lucyModel->transform = Maths::createTransormationMatrix(m_lucyPosition, glm::vec3(0.0f, time * 25.0f, 0.0f), 0.2f);
	m_lucyModel->draw(*m_pLightSP);
	m_pLightSP->stop();
	// ------------ SpotLight --------------------
	m_sLightSP->start();
	m_sLightSP->update(*m_camera, *m_spotLight);
	m_dragonModel->transform = Maths::createTransormationMatrix(m_dragonPosition, glm::vec3(0.0f, time * 25.0f, 0.0f), 0.2f);
	m_dragonModel->draw(*m_sLightSP);
	m_sLightSP->stop();
	// ------------ SoftSpotLight --------------------
	m_ssLightSP->start();
	m_ssLightSP->update(*m_camera, *m_softSpotLight);
	m_duckModel->transform = Maths::createTransormationMatrix(m_duckPosition, glm::vec3(0.0f, time * 25.0f, 0.0f), 0.01f);
	m_duckModel->draw(*m_ssLightSP);
	m_ssLightSP->stop();
	// particles
	m_particleSP->start();
	m_particleSP->update(*m_camera, *m_softSpotLight);
	//m_emitter->emitterPosition(glm::vec3(cosf((float)glfwGetTime() * 3.0f) - m_emitterPosition.x, m_emitterPosition.y, m_emitterPosition.z));
	m_emitter->draw(*m_particleSP);
	m_particleSP->stop();
	// hand animation
	m_animeSP->start();
	m_animeSP->specularColour(m_particleColour);
	m_animeSP->lightPosition(m_emitter->emitterPosition() - m_handPosition);
	m_animeSP->update(*m_camera, *m_softSpotLight);
	m_handAnimated->transform = Maths::createTransormationMatrix(m_handPosition, glm::vec3(0.0f, 0.0f, -90.0f), 2.0f);
	m_handAnimated->animate(*m_animeSP);
	m_animeSP->stop();
	// plane
	m_plane->draw(*m_camera);
}
/// ***************************************************************************************
///	Setup lights for use in application
/// ***************************************************************************************
void AssessmentApp::setupLights()
{
	// setup lamps
	m_dragonlamp = new Lamp(m_dragonPosition + glm::vec3(-6.0f, 1.5f, 0.0f));
	m_lucyLamp = new Lamp(m_lucyPosition + glm::vec3(0.0f, 5.0f, 1.0f));
	m_duckLamp = new Lamp(m_duckPosition + glm::vec3(0.0f, 1.5f, 6.0f));
	m_planeLamp = new Lamp(m_plane->lightPos());
	
	// light shader attributes
	glm::vec3 lightPosition = m_lucyLamp->position();
	glm::vec3 lightColour = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 lightDirection = m_lucyLamp->position();
	glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	// point light
	glm::vec3 pLightDirection = -m_lucyLamp->position();
	glm::vec3 pLightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 pLightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 pLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	GLfloat	  pLightConstant = 1.0f;
	GLfloat	  pLightLinear = 0.22f;
	GLfloat	  pLightQuadratic = 0.20f;
	// spot light
	glm::vec3 sLightPosition = m_dragonlamp->position();
	glm::vec3 sLightDirection = -m_dragonlamp->position();
	glm::vec3 sLightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 sLightDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 sLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	GLfloat	  sLightConstant = 1.0f;
	GLfloat	  sLightLinear = 0.09f;
	GLfloat	  sLightQuadratic = 0.032f;
	GLfloat   sLightCutOff = glm::cos(glm::radians(12.5f));
	// soft spot light
	glm::vec3 ssLightPosition = m_duckLamp->position();
	glm::vec3 ssLightDirection = -m_duckLamp->position();
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
/// ***************************************************************************************
///	Setup models for use in application
/// ***************************************************************************************
void AssessmentApp::setupModels()
{
	// load obj models
	// hand
	m_handAnimated = new AnimatedModel("res/models/hand/hand_00.obj", "res/models/hand/hand_39.obj");
	m_handPosition = glm::vec3(6.0f, 0.78f, 1.0f);
	m_handAnimated->transform = Maths::createTransormationMatrix(m_handPosition, glm::vec3(0.0f, 00.0f, 0.0f), 2.0f);
	// dragon
	m_dragonModel = new MeshModel("res/models/stanford/Dragon.obj");
	m_dragonPosition = glm::vec3(-6.0f, 0.0f, 0.0f);
	m_dragonModel->transform = Maths::createTransormationMatrix(m_dragonPosition, glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	// lucy
	m_lucyPosition = glm::vec3(0.0f, 0.0f, -6.0f);
	m_lucyModel = new MeshModel("res/models/stanford/Lucy.obj");
	m_lucyModel->transform = Maths::createTransormationMatrix(m_lucyPosition, glm::vec3(0.0f, 0.0f, 0.0f), 0.05f);

	// fbx models
	m_duckPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	m_duckModel = new MeshModel("res/models/fbx/duck/duck_ascii.fbx");
	m_duckModel->transform = Maths::createTransormationMatrix(m_duckPosition, glm::vec3(0.0f, 180.0f, 0.0f), 0.01f);
	
	// dynamic models
	m_plane = new Plane(glm::vec3(0.0f, 0.0f, 0.0f), 20);
	m_mirror = new Mirror(glm::vec3(0.0f, 1.0f, 0.0f));

}
/// ***************************************************************************************
///	GUI for adjusting in application elements
/// ***************************************************************************************
void AssessmentApp::debugGUI()
{
	static float f = 0.0f;
	static bool check = true;
	ImGui::Begin("Adjuster");
		ImGui::ColorEdit3("BG Colour", (float*)&m_clearColour);
		ImGui::ColorEdit3("Particle Colour", (float*)&m_particleColour);
		ImGui::Checkbox("Default", &m_mirror->m_imageStyle.default); ImGui::SameLine();
		ImGui::Checkbox("Blur", &m_mirror->m_imageStyle.blur);
		ImGui::Checkbox("Invert", &m_mirror->m_imageStyle.invert); ImGui::SameLine();
		ImGui::Checkbox("Sharp", &m_mirror->m_imageStyle.sharpen);
		ImGui::Checkbox("Grey", &m_mirror->m_imageStyle.greyScale); ImGui::SameLine();
		ImGui::Checkbox("Edge", &m_mirror->m_imageStyle.edgeDetect);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
/// ***************************************************************************************
///	Culling function for bounding spheres
/// ***************************************************************************************
bool AssessmentApp::culling()
{
	bool result = true;
	// iterate through planes
	for (int i = 0; i < 6; i++)
	{
		float d = glm::dot(glm::vec3(m_vPlanes[i]), m_boundingSphere->centre()) + m_vPlanes[i].w;
		// if outside of plane return false
		if (d < -m_boundingSphere->radius()) {
			result = false;
			break;
		}
	}
	return result;
}
/// ***************************************************************************************
///	Culling functions for AABBs
/// ***************************************************************************************
bool AssessmentApp::AABBculling(AABB & a_box)
{
	bool result = true;
	// Indexed for the 'index trick' later
	glm::vec3 box[] = { a_box.min(), a_box.max() };

	// We only need to do 6 point-plane tests
	for (int i = 0; i < 6; ++i)
	{
		// This is the current plane
		glm::vec4 p = m_vPlanes[i];

		// p-vertex selection (with the index trick)
		// According to the plane normal we can know the
		// indices of the positive vertex
		const int px = static_cast<int>(p.x > 0.0f);
		const int py = static_cast<int>(p.y > 0.0f);
		const int pz = static_cast<int>(p.z > 0.0f);

		// How far is p-vertex from the origin
		const float dp = (p.x*box[px].x) +
						 (p.y*box[py].y) +
						 (p.z*box[pz].z);

		// Doesn't intersect if it is behind the plane
		if (dp < -p.w) {
			result = false;
			break;
		}
	}
	return result;
}
/// ***************************************************************************************
///	Destroy all objects function
/// ***************************************************************************************
void AssessmentApp::destroy()
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
	deallocate(m_particleSP);

	// models
	deallocate(m_lucyModel);
	deallocate(m_dragonModel);
	deallocate(m_handModel);
	deallocate(m_handAnimated);

	// fbx
	deallocate(m_duckModel);

	// entities 
	deallocate(m_plane);
	deallocate(m_mirror);

	// particles
	deallocate(m_emitter);

	// lights
	deallocate(m_dragonlamp);
	deallocate(m_light);
	deallocate(m_lucyLamp);
	deallocate(m_pointLight);
	deallocate(m_duckLamp);
	deallocate(m_spotLight);
	deallocate(m_planeLamp);
	deallocate(m_softSpotLight);

	// collisions
	deallocate(m_boundingSphere);
}