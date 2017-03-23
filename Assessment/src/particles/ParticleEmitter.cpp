#include "ParticleEmitter.h"



ParticleEmitter::ParticleEmitter()
{
	m_uiFirstDead = 0;
	m_uiMaxParticles = 0;
	m_vPosition = glm::vec3(0.0f, 1.0f, 0.0f);
	VAO = 0;
	VBO = 0;
	EBO = 0;
}

ParticleEmitter::ParticleEmitter(glm::vec3 a_position)
{
	m_uiFirstDead = 0;
	m_uiMaxParticles = 0;
	m_vPosition = a_position;
	VAO = 0;
	VBO = 0;
	EBO = 0;
}


ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}
///
/// Update Particles
///
void ParticleEmitter::update(GLfloat a_deltaTime, const glm::mat4 & a_cameraTransform)
{
	
	// spwan particles
	m_fEmitTimer += a_deltaTime;
	while (m_fEmitTimer > m_fEmitRate)
	{
		emit();
		m_fEmitTimer -= m_fEmitRate;
	}
	
	GLuint quad = 0;
	// debug
	m_vertCount = 0;
	// update particles and turn live particle into billboard quads
	for (GLuint i = 0; i < m_uiFirstDead; i++)
	{
		Particle * particle = &m_particles[i];
		// update particles remaining life
		particle->lifeTime += a_deltaTime;
		// check if particle is dead if not then update
		if (particle->lifeTime >= particle->lifeSpan) {
			// swap last alive and current particle
			*particle = m_particles[m_uiFirstDead - 1];
			m_uiFirstDead--;
		}
		else {
			// move particle
			particle->position += particle->velocity * a_deltaTime;
			particle->position.y += 5.0f * a_deltaTime;
			// size particle
			particle->size = glm::mix(m_fStartSize, m_fEndSize, particle->lifeTime / particle->lifeSpan);
			// colour particle
			particle->colour = glm::mix(m_vStartColour, m_vEndColour, particle->lifeTime / particle->lifeSpan);
			// set quad size and colour
			float halfSize = particle->size * 0.5f;
			m_vertexData[quad * 4 + 0].position = glm::vec4(halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 0].colour = particle->colour;
			m_vertexData[quad * 4 + 1].position = glm::vec4(-halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 1].colour = particle->colour;
			m_vertexData[quad * 4 + 2].position = glm::vec4(-halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 2].colour = particle->colour;
			m_vertexData[quad * 4 + 3].position = glm::vec4(halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 3].colour = particle->colour;
			// billboard transform
			glm::vec3 zAxis = glm::normalize(glm::vec3(a_cameraTransform[3]) - particle->position);
			glm::vec3 xAxis = glm::cross(glm::vec3(a_cameraTransform[1]), zAxis);
			glm::vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(glm::vec4(xAxis, 0),
								glm::vec4(yAxis, 0),
								glm::vec4(zAxis, 0),
								glm::vec4(0,0,0,1));
			m_vertexData[quad * 4 + 0].position = billboard * m_vertexData[quad * 4 + 0].position + glm::vec4(particle->position,0);
			m_vertexData[quad * 4 + 1].position = billboard * m_vertexData[quad * 4 + 1].position + glm::vec4(particle->position,0);
			m_vertexData[quad * 4 + 2].position = billboard * m_vertexData[quad * 4 + 2].position + glm::vec4(particle->position,0);
			m_vertexData[quad * 4 + 3].position = billboard * m_vertexData[quad * 4 + 3].position + glm::vec4(particle->position,0);
			// increment quad
			++quad;
		}
	}


}
///
/// Draw Particles
///
void ParticleEmitter::draw(ShaderProgram & a_shaderProgram)
{

	// bind VAO
	glBindVertexArray(VAO);
	// sync particle vertex buffer based on number of living particles
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_uiFirstDead * 4 * sizeof(ParticleVertex), m_vertexData);

	// draw particles
	glDrawElements(GL_TRIANGLES, m_uiFirstDead * 6, GL_UNSIGNED_INT, 0);
}
///
/// Inititalise Particle Emitter
///
void ParticleEmitter::init(GLuint a_maxParticles, GLuint a_emitRate, GLfloat a_lifetimeMin, GLfloat a_lifetimeMax, 
						   GLfloat a_velocityMin, GLfloat a_velocityMax, GLfloat a_startSize, GLfloat a_endSize, 
						   const glm::vec4 & a_startColour, const glm::vec4 & a_endColour)
{
	// emit timers
	m_fEmitTimer = 0.0f;
	m_fEmitRate = 1.0f / a_emitRate;

	m_uiMaxParticles = a_maxParticles;
	m_fLifespanMin	= a_lifetimeMin;
	m_fLifespanMax	= a_lifetimeMax;
	m_fVelocityMin	= a_velocityMin;
	m_fVelocityMax	= a_velocityMax;
	m_fStartSize	= a_startSize;
	m_fEndSize		= a_endSize;
	m_vStartColour	= a_startColour;
	m_vEndColour	= a_endColour;

	// particle array 
	m_particles = new Particle[m_uiMaxParticles];
	m_uiFirstDead = 0;
	// vertex data
	m_vertexData = new ParticleVertex[m_uiMaxParticles * 4];
	// indicies
	GLuint * indexData = new GLuint[m_uiMaxParticles * 6];
	
	// generate indices
	for (GLuint i = 0; i < m_uiMaxParticles; i++) {
		// first triangle
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;
		// second triangle
		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	// buffers
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// vertex and indices buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_uiMaxParticles * 4 * sizeof(ParticleVertex), m_vertexData, GL_DYNAMIC_DRAW);
	// bind and set indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiMaxParticles * 6, indexData, GL_STATIC_DRAW);
	// Set the vertex attribute pointers
	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(3); // colour
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (GLvoid*)0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (GLvoid*)offsetof(ParticleVertex, colour));
	// clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
}
///
/// Emit Particles
///
void ParticleEmitter::emit()
{
	// emit only if there is a dead particle to use
	if (m_uiFirstDead >= m_uiMaxParticles)
		return;

	// bring back the dad
	Particle & particle = m_particles[m_uiFirstDead++];
	// set position
	particle.position = m_vPosition;
	// randomise lifespan
	particle.lifeTime = 0;
	particle.lifeSpan = (rand() / (float)RAND_MAX) * (m_fLifespanMax - m_fLifespanMin) + m_fLifespanMin;
	// set start size and colour
	particle.size = m_fStartSize;
	particle.colour = m_vStartColour;
	// randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) * (m_fVelocityMax - m_fVelocityMin) + m_fVelocityMin;
	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;

}
