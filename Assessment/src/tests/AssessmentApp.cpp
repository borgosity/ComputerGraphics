#include "AssessmentApp.h"



AssessmentApp::AssessmentApp()
{
}


AssessmentApp::~AssessmentApp()
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
	deallocate(m_mirror);
	deallocate(m_waterCube);

	// particles
	deallocate(m_emitter);

	// lights
	deallocate(m_lamp);
	deallocate(m_light);
	deallocate(m_pointLamp);
	deallocate(m_pointLight);
	deallocate(m_spotLamp);
	deallocate(m_spotLight);
	deallocate(m_softSpotLamp);
	deallocate(m_softSpotLight);

	// collisions
	deallocate(m_boundingSphere);
}

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

	// load textures
	m_whiteTexture = new Texture("res/textures/white.png");
	m_lightTexture = new Texture("res/textures/light.png");
	//-------------------------------- lights ----------------------------------	
	setupLights();

	//----------------------------------- models ----------------------------
	setupModels();

	// ---------------------------- particles ----------------------------
	GLuint  maxParticles = 1000;
	GLuint  emitRate = 200;
	GLfloat lifetimeMin = 0.1f;
	GLfloat lifetimeMax = 1.0f;
	GLfloat velocityMin = 0.01f;
	GLfloat velocityMax = 1.0f;
	GLfloat startSize = 0.15f;
	GLfloat endSize = 0.001f;
	const glm::vec4 startColour(0, 0.6, 0, 1);
	const glm::vec4 endColour(0, 0, 0.6, 1);

	m_emitter = new ParticleEmitter(glm::vec3(3.0f, 0.78f, 0.0f));
	m_emitter->init(maxParticles, emitRate, 
					lifetimeMin, lifetimeMax,
					velocityMin, velocityMax, 
					startSize, endSize,
					startColour, endColour);

	// ------------------------ GUI --------------------------------------
	m_clearColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	setupGUI();
	
	m_particleColour = startColour;
	m_animeSP->lightPosition(m_emitter->emitterPosition());

	// -------------------------- collisions -------------------------
	Maths::frustrumPlanes(m_camera->projection(), m_camera->viewMatrix(), m_vPlanes);
	m_boundingSphere = new BoundingSphere(m_mirror->position(), 0.5f);
	return true;
}

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
	m_emitter->startColour(m_particleColour);
	m_emitter->update((GLfloat)a_deltaTime, m_camera->transform());

	// update planes and call frustrum culling update
	Maths::frustrumPlanes(m_camera->projection(), m_camera->viewMatrix(), m_vPlanes);
	
	return true;
}

bool AssessmentApp::fixedUpdate(GLdouble a_deltaTime)
{
	return true;
}

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


	// ------------------------- post processing ----------------
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

bool AssessmentApp::stop()
{
	// Properly de-allocate all resources once they've outlived their purpose
	m_loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return true;
}

void AssessmentApp::objectDraw(GLdouble a_deltaTime)
{
	GLdouble time = glfwGetTime();
	// hand animation
	m_animeSP->start();
	m_animeSP->specularColour(m_particleColour);
	m_animeSP->lightPosition(m_emitter->emitterPosition());
	m_animeSP->update(*m_camera, *m_softSpotLight);
	m_handAnimated->transform = Maths::createTransormationMatrix(m_handPosition, glm::vec3(0.0f, 0.0f, -90.0f), 2.0f);
	m_handAnimated->animate(*m_animeSP);
	m_animeSP->stop();
	// water cube
	//m_pLightSP->start();
	//m_pLightSP->update(*m_camera, *m_softSpotLight);
	//m_waterCube->transform() = Maths::createTransormationMatrix(m_cubePosition, glm::vec3(45.0f, time * 25.0f, 0.0f), 0.05f);
	//m_waterCube->draw(*m_pLightSP);
	//m_pLightSP->stop();
	// particles
	m_particleSP->start();
	m_particleSP->update(*m_camera, *m_softSpotLight);
	m_emitter->draw(*m_particleSP);
	m_particleSP->stop();
	// plane
	m_plane->draw(*m_camera);
}

void AssessmentApp::setupLights()
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

void AssessmentApp::setupModels()
{
	// load models
	m_handAnimated = new AnimatedModel("res/models/hand/hand_00.obj", "res/models/hand/hand_39.obj");
	m_handPosition = glm::vec3(3.0f, 0.78f, 0.5f);
	m_handAnimated->transform = Maths::createTransormationMatrix(m_handPosition, glm::vec3(0.0f, 00.0f, 0.0f), 2.0f);

	m_dragonModel = new MeshModel("res/models/stanford/Dragon.obj");
	m_dragonModel->transform = Maths::createTransormationMatrix(glm::vec3(4.5f, -1.75f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f);
	
	// fbx models
	m_duckModel = new MeshModel("res/models/fbx/duck/duck_ascii.fbx");
	m_duckModel->transform = Maths::createTransormationMatrix(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), 0.01f);

	m_plane = new Plane(glm::vec3(0.0f, 0.0f, 0.0f), 20);
	m_mirror = new Mirror(glm::vec3(0.0f, 1.0f, 1.0f));

	m_cubePosition = glm::vec3(2.0f, 1.0f, 0.5f);
	m_waterCube = new WaterCube(m_cubePosition);
	m_waterCube->transform() = Maths::createTransormationMatrix(m_cubePosition, glm::vec3(0.0f, 0.0f, 0.0f), 0.05f);
}

void AssessmentApp::setupGUI()
{
}

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

