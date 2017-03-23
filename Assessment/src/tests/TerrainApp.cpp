#include "TerrainApp.h"



TerrainApp::TerrainApp()
{
}


TerrainApp::~TerrainApp()
{
	// display
	deallocate(m_display);
	deallocate(m_loader);
	deallocate(m_renderer);
	deallocate(m_camera);
	deallocate(m_cameraController);
	// shaders
	deallocate(m_colourSP);
	deallocate(m_cameraSP);
	deallocate(m_phongSP);
	deallocate(m_modelSP);
	deallocate(m_rippleSP);
	// textures
	deallocate(m_soulSpearTexture);
	deallocate(m_soulSpearNormalMap);
	deallocate(m_duckTexture);
	deallocate(m_rocksTexture);
	deallocate(m_rocksNormalMap);
	deallocate(m_waterTexture);

	// models
	deallocate(m_gridModel);
	deallocate(m_cubeModel);
	deallocate(m_soulSpearRM);
	deallocate(m_rocksModel);
	deallocate(m_duckRM);

	// textured models
	deallocate(m_waterTM);
	deallocate(m_duckTM);
	deallocate(m_flagTM);
	deallocate(m_soulSpearTM);
	deallocate(m_rocksTM);
	
	// entities 
	deallocate(m_flag);
	deallocate(m_rubberDuck);
	deallocate(m_rocksE);
	deallocate(m_water);
	deallocate(m_soulSpear);

	// lights
	deallocate(m_duckLight);

}

bool TerrainApp::start()
{
	std::cout << "\n### --> Start TerrainApp" << std::endl;

	// camera
	m_camera = new Camera(glm::vec3(0, 3, 10), CAM_SPEED, FOV, NEAR_PLANE, FAR_PLANE);
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
	m_colourSP = new ShaderProgram(Shader::dualTextureShader);
	m_cameraSP = new ShaderProgram(Shader::cameraShader);
	m_phongSP = new ShaderProgram(Shader::phongShader);
	m_modelSP = new ShaderProgram(Shader::modelShader);
	m_rippleSP = new ShaderProgram(Shader::rippleShader);

	m_staticShader = new StaticShader(Shader::phongShader);

	// load model to VAO
	m_loader = new Loader();
	m_renderer = new Renderer();

	// ============= grid setup =====================
	// create grid model
	m_gridModel = new RawModel();
	DynamicModels::grid(*m_gridModel, 50, 25);
	// water
	m_waterTexture = new Texture("res/textures/water_0.png");
	m_waterTM = new TexturedModel(*m_gridModel, *m_waterTexture, *m_waterTexture, m_rippleSP->ID());
	m_water = new Entity(m_waterTM, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);
	// flag
	m_flagTM = new TexturedModel(*m_gridModel, *m_waterTexture, *m_waterTexture, m_rippleSP->ID());
	m_flag = new Entity(m_waterTM, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);


	//============= obj setup ===========================
	m_soulSpearRM = new RawModel();
	//OBJLoader::loadObjModel("res/models/soulspear/soulspear.obj", *m_soulSpearRM, *m_loader);
	OBJLoader::loadObjModel("res/models/stanford/Buddha.obj", *m_soulSpearRM, *m_loader);
	//m_soulSpearTexture = new Texture("res/models/soulspear/soulspear_diffuse.png");
	//m_soulSpearNormalMap = new Texture("res/models/soulspear/soulspear_specular.png");
	m_soulSpearTexture = new Texture("res/textures/white.png");
	m_soulSpearNormalMap = new Texture("res//textures/white.png");
	m_soulSpearTM = new TexturedModel(*m_soulSpearRM, *m_soulSpearTexture, *m_soulSpearNormalMap, m_phongSP->ID());
	m_soulSpear = new Entity(m_soulSpearTM, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);
	
	// rubber duck
	m_duckRM = new RawModel();
	OBJLoader::loadObjModel("res/models/rubberDuck.obj", *m_duckRM, *m_loader);
	m_duckTexture = new Texture("res/textures/Moon.png");
	m_duckTM = new TexturedModel(*m_duckRM, *m_duckTexture, *m_waterTexture, m_staticShader->ID());
	//m_duckTM = new TexturedModel(*m_duckRM, *m_duckTexture, *m_duckTexture, m_modelSP->ID());

	m_duckTM->shine(10);
	m_duckTM->reflection(1);
	m_rubberDuck = new Entity(m_duckTM, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);

	// duck light
	m_duckLight = new Light(glm::vec3(0,5,-20), glm::vec3(1,1,1));

	//================ normal map test ====================

	m_rocksModel = new RawModel();
	DynamicModels::squareTBN(*m_rocksModel, 1);
	m_rocksTexture = new Texture("res/textures/fourShapes/four_diffuse.png");
	m_rocksNormalMap = new Texture("res/textures/fourShapes/four_normal.png");
	m_rocksTM = new TexturedModel(*m_rocksModel, *m_rocksTexture, *m_rocksNormalMap, m_modelSP->ID());
	m_rocksE = new Entity(m_rocksTM, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f);



	return true;
}

bool TerrainApp::update(GLfloat a_deltaTime)
{

	m_cameraController->update(*m_camera, a_deltaTime);
	
	return true;
}

bool TerrainApp::fixedUpdate(GLfloat a_deltaTime)
{
	return true;
}

bool TerrainApp::draw(GLfloat a_deltaTime)
{
	GLdouble time = glfwGetTime();
	// Render
	// Clear the colorbuffer
	m_renderer->prepare(0.0f, 0.3f, 0.2f);
	//############################### START DRAW CODE ##########################################################

	//==================== ripple grid =============================
	// Activate shader
	m_rippleSP->start();
	// pass uniform data
	m_rippleSP->uniformFloat("time", (GLfloat)time * 1.5f);
	m_rippleSP->uniformFloat("heightScale", 0.5f);
	// pass camera position to shader 
	m_rippleSP->uniformMat4("view", m_camera->viewMatrix());
	// pass camera projection to shader
	m_rippleSP->uniformMat4("projection", m_camera->projection());

	// ================== flag  ================
	// create transform, with rotation changed over time
	m_rippleSP->uniformVec4("uniformColour", glm::vec4(1, 0, 0, 1));
	m_flag->transform(Maths::createTransormationMatrix(glm::vec3(-7.75, 0, -5), glm::vec3(90, 0, 0), 0.4f));
	//m_renderer->renderEntity(m_flag, m_rippleSP);

	// ================== water  ================
	// create transform, with rotation changed over time
	m_rippleSP->uniformVec4("uniformColour", glm::vec4(0, 0, 1, 1));
	m_water->transform(Maths::createTransormationMatrix(glm::vec3(-10, 0, -10), glm::vec3(0, 0, 0), 0.5f));
	m_renderer->renderEntity(m_water, m_rippleSP);

	// De-activate shader
	m_rippleSP->stop();

	// ================================= soul spear model =========================
	// Activate shader
	m_phongSP->start();
	// pass camera position to shader 
	m_phongSP->uniformMat4("view", m_camera->viewMatrix());
	// pass camera projection to shader
	m_phongSP->uniformMat4("projection", m_camera->projection());

	// --- generate light source ------
	glm::vec3 light(sin(time), 1, cos(time));
	m_phongSP->uniformVec3("lightPos", glm::vec3(0.5, 1, 0));
	m_phongSP->uniformVec3("lightColour", glm::vec3(1, 0, 0));
	m_phongSP->uniformFloat("shineDamper", 10);
	m_phongSP->uniformFloat("reflectivity", 1);

	// create transform, with rotation changed over time
	m_soulSpear->transform(Maths::createTransormationMatrix(glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), 1.0f));
	m_renderer->renderEntity(m_soulSpear, m_phongSP);

	m_phongSP->stop();

	// ================================= rubber duck model =========================
	// Activate shader
	//m_modelSP->start();
	//// pass camera position to shader 
	//m_modelSP->uniformMat4("view", m_camera->viewMatrix());
	//// pass camera projection to shader
	//m_modelSP->uniformMat4("projection", m_camera->projection());

	//// --- generate light source ------
	//m_modelSP->uniformVec3("lightDirection", light);

	//// create transform, with rotation changed over time
	//m_rubberDuck->transform(Maths::createTransormationMatrix(glm::vec3(0, (glm::sin(time) * 0.25), 0), glm::vec3(0, 0, 0), 1.0f));
	//m_renderer->renderEntity(m_rubberDuck, m_modelSP);

	//m_modelSP->stop();

	// static shader
	m_staticShader->start();

	m_staticShader->loadCamera(*m_camera);
	m_staticShader->loadLight(*m_duckLight);

	// create transform, with rotation changed over time
	m_rubberDuck->transform(Maths::createTransormationMatrix(glm::vec3(0, (glm::sin(time) * 0.25), 0), glm::vec3(0, 0, 0), 1.0f));
	m_renderer->renderWithShader(m_rubberDuck, m_staticShader);

	m_staticShader->stop();

	//=========== normals test
	// Activate shader
	m_modelSP->start();
	// pass camera position to shader 
	m_modelSP->uniformMat4("view", m_camera->viewMatrix());
	// pass camera projection to shader
	m_modelSP->uniformMat4("projection", m_camera->projection());

	// --- generate light source ------
	m_modelSP->uniformVec3("lightDirection", light);

	m_rocksE->transform(Maths::createTransormationMatrix(glm::vec3(-5, 4.25, -5), glm::vec3(0, 0, 180), 1.0f));
//	m_renderer->renderEntity(m_rocksE, m_modelSP);

	// De-activate shader
	m_modelSP->stop();
	// ================================ end obj model ==========================

	//std::cout << m_camera->position().x << ", " 
	//	<< m_camera->position().y << ", "
	//	<< m_camera->position().x  << "\n lookat - "
	//	<< m_camera->front().x << ", "
	//	<< m_camera->front().y << ", "
	//	<< m_camera->front().z
	//	<< std::endl;

	// ##################### END DRAW STUFF ###########################################################
	// Swap the screen buffers
	glfwSwapBuffers(m_display->window());
	return true;
}

bool TerrainApp::stop()
{
	// Properly de-allocate all resources once they've outlived their purpose

	m_loader->cleanUp();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return true;
}

