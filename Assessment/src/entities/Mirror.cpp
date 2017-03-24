#include "Mirror.h"

Mirror::Mirror()
{
}

Mirror::Mirror(glm::vec3 a_position)
{
	m_vPosition = a_position;
	m_shaderProgram = new ShaderProgram(Shader::staticShader);
	m_vColour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_m4Transform = Maths::createTransormationMatrix(a_position, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	m_mirrorModel = DynamicModels::cube();

	m_testTexture = new Texture("res/textures/dirt.png");

	m_fShininess = 32.0f;
	m_imageStyle = {false, false, false, true, false, false};

	setupFBO();
}


Mirror::~Mirror()
{
	delete m_mirrorModel;
	m_mirrorModel = nullptr;
}
/// ***************************************************************************************
/// Bind FBO
/// ***************************************************************************************
void Mirror::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}
/// ***************************************************************************************
/// Unbind FBO
/// ***************************************************************************************
void Mirror::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
/// ***************************************************************************************
/// Draw
/// ***************************************************************************************
void Mirror::draw(Camera & a_camera)
{
	float time = (float)glfwGetTime();

	// Clear all relevant buffers
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST); 	

	// start shader
	m_shaderProgram->start();
	m_shaderProgram->uniformMat4("view", a_camera.viewMatrix());
	// pass camera projection to shader
	m_shaderProgram->uniformMat4("projection", a_camera.projection());
	m_shaderProgram->uniformMat4("model", m_m4Transform);

	// adjustment styles
	m_shaderProgram->uniformBool("standard", m_imageStyle.default);
	m_shaderProgram->uniformBool("invert", m_imageStyle.invert);
	m_shaderProgram->uniformBool("greyScale", m_imageStyle.greyScale);
	m_shaderProgram->uniformBool("sharpen", m_imageStyle.sharpen);
	m_shaderProgram->uniformBool("blur", m_imageStyle.blur);
	m_shaderProgram->uniformBool("edgeDetection", m_imageStyle.edgeDetect);

	// Draw Screen
	glBindTexture(GL_TEXTURE_2D, m_TBO);	
	glUniform1i(glGetUniformLocation(m_shaderProgram->ID(), "screenTexture"), 0);
	// bind VAO
	glBindVertexArray(m_mirrorModel->vaoID());
	// draw arrays
	glDrawArrays(GL_TRIANGLES, 0, m_mirrorModel->vertexCount());
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	// stop shader
	m_shaderProgram->stop();
}
/// ***************************************************************************************
/// Create buffer objects
/// ***************************************************************************************
void Mirror::setupFBO()
{
	// Framebuffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	// Texturebuffer
	m_TBO = generateTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TBO, 0);
	// Renderbuffer
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	// store depth and stencil in the same buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_W, SCREEN_H);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// attach buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
	// check if frame buffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR --> MIRROR: Framebuffer is not complete!" << std::endl;
	}
	// clear buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
/// ***************************************************************************************
/// Generate Attachment Texture: 
/// - standard colour texture	=  !a_depth && !stencil
/// - depth texture				=  a_depth && !stencil
/// - stencil texture			=  !a_depth && stencil
/// ***************************************************************************************
GLuint Mirror::generateTexture(GLboolean a_depth, GLboolean a_stencil)
{
	// What enum to use?
	GLenum attType;
	if (!a_depth && !a_stencil) {
		attType = GL_RGB;
	}
	else if (a_depth && !a_stencil) {
		attType = GL_DEPTH_COMPONENT;
	}
	else if (!a_depth && a_stencil) {
		attType = GL_STENCIL_INDEX;
	}

	// Generate texture ID 
	GLuint textureID;
	glGenTextures(1, &textureID);
	// bind data
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!a_depth && !a_stencil) {
		glTexImage2D(GL_TEXTURE_2D, 0, attType, SCREEN_W, SCREEN_H, 0, attType, GL_UNSIGNED_BYTE, NULL);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCREEN_W, SCREEN_H, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}
