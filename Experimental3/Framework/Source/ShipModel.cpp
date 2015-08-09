//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "ShipModel.h"
#include "objloader.hpp"
#include "Renderer.h"
//#include "texture.hpp"
//#include "SOIL.h"
#include "Projectile.h"
#include "Camera.h"
#include "World.h"
#include "Mesh.h"
#include "Model.h"
#include"sceneLoader.h"
#include "EventManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>


// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>
using namespace glm;
sceneLoader* scene;
ShipModel::ShipModel(int shipTextureID,glm::vec3 size) : Model()
{
	// Create Vertex Buffer for all the verices of the Cube
	//Vertex vertexBuffer[] = loadOBJ("../Assets/Model/Chair.obj" );
	// Create a vertex array
	mTextureID = shipTextureID;
	scene = new sceneLoader("../Assets/Models/ship1.obj");
	mRotationAxis = vec3(0,0,1);
	float firetime = 0;
	float fireRate = 2;
	NextProjectle = 0;
	
}

ShipModel::~ShipModel()
{
	// Free the GPU from the Vertex Buffer
	scene->~sceneLoader();
}

void ShipModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second
	//this->SetPosition(this->GetPosition()+(glm::vec3(0,0,0.5)*dt));
	
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS && mPosition.x < 6)
	{
		//bankLeftTime += dt;
		mPosition+=(glm::vec3(10.0f,0,0)*dt);
		if(mRotationAngleInDegrees > -45){
			mRotationAngleInDegrees -= 60 * dt;
		}

	} else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS && mPosition.x > -6)
	{
		//bankRightTime += dt;
		mPosition+=(glm::vec3(-10.0f,0,0)*dt);
		if(mRotationAngleInDegrees < 45){
			mRotationAngleInDegrees += 60 * dt;
		}
	} else {

		
		if(mRotationAngleInDegrees > 0.001){
			mRotationAngleInDegrees -= 23 * dt;
		} else if (mRotationAngleInDegrees < 0.001){
			mRotationAngleInDegrees += 23 * dt;
		}
		if(mRotationAngleInDegrees > 0){
			mRotationAngleInDegrees -= 23 * dt;
		} else if (mRotationAngleInDegrees < 0){
			mRotationAngleInDegrees += 23 * dt;
		}
	}
	//	bullet.Update(dt);
	Model::Update(dt);

	if(NextProjectle >= 15){
	NextProjectle-=15;
	}
	if(firetime > firetime && glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS){
		firetime = 0;
		World::GetInstance()->mPlayerProjectile[NextProjectle]->Fire(this->GetPosition());
		NextProjectle++;
	}
}

void ShipModel::Draw()
{
	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	ShaderType oldShader = (ShaderType)Renderer::GetCurrentShader();
	Renderer::SetShader(TEX_LIGHT2);
	Renderer::CheckForErrors();
    glUseProgram(Renderer::GetShaderProgramID());

    Renderer::CheckForErrors();
	//bullet.Draw();

	GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "mySamplerTexture");
	Renderer::CheckForErrors();
    glActiveTexture(GL_TEXTURE0);
	Renderer::CheckForErrors();
	unsigned int programID = Renderer::GetShaderProgramID();
	Renderer::CheckForErrors();
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	Renderer::CheckForErrors();
	glUniform1i(textureLocation, 0);	
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	Renderer::CheckForErrors();
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
	Renderer::CheckForErrors();
	GLuint VMatrixLocation = glGetUniformLocation(programID, "ViewTransform");
	Renderer::CheckForErrors();
	 const Camera* currentCamera = World::GetInstance()->GetCurrentCamera();
	Renderer::CheckForErrors();
	mat4 V = currentCamera->GetViewMatrix();
	Renderer::CheckForErrors();
    glUniformMatrix4fv(VMatrixLocation, 1, GL_FALSE, &V[0][0]);
	Renderer::CheckForErrors();
	GLuint PMatrixLocation = glGetUniformLocation(programID, "ProjectionTransform");
	Renderer::CheckForErrors();
	mat4 P = currentCamera->GetProjectionMatrix();
	Renderer::CheckForErrors();
	glUniformMatrix4fv(PMatrixLocation, 1, GL_FALSE, &P[0][0]);
	Renderer::CheckForErrors();
	

	
    // Send the view projection constants to the shader
   



		// Get a handle for Light Attributes uniform
	
	GLuint LightPositionID = glGetUniformLocation(programID, "WorldLightPosition");
	Renderer::CheckForErrors();
	GLuint LightColorID = glGetUniformLocation(programID, "lightColor");
	Renderer::CheckForErrors();
	GLuint LightAttenuationID = glGetUniformLocation(programID, "lightAttenuation");
	Renderer::CheckForErrors();

	// Get a handle for Material Attributes uniform
	GLuint MaterialAmbientID = glGetUniformLocation(programID, "materialAmbient");
	Renderer::CheckForErrors();
    GLuint MaterialDiffuseID = glGetUniformLocation(programID, "materialDiffuse");
	Renderer::CheckForErrors();
    GLuint MaterialSpecularID = glGetUniformLocation(programID, "materialSpecular");
	Renderer::CheckForErrors();
    GLuint MaterialExponentID = glGetUniformLocation(programID, "materialExponent");
	Renderer::CheckForErrors();

		// Set shader constants
	const float ka = 0.2f;
	Renderer::CheckForErrors();
	const float kd = 0.8f;
	Renderer::CheckForErrors();
	const float ks = 0.2f;
	Renderer::CheckForErrors();
	const float n = 90.0f;
	Renderer::CheckForErrors();
        glUniform1f(MaterialAmbientID, ka);
		Renderer::CheckForErrors();
        glUniform1f(MaterialDiffuseID, kd);
		Renderer::CheckForErrors();
        glUniform1f(MaterialSpecularID, ks);
		Renderer::CheckForErrors();
        glUniform1f(MaterialExponentID, n);
		Renderer::CheckForErrors();

		vec3 lightColor = World::GetInstance()->lightColor;
		Renderer::CheckForErrors();
	float lightKc = World::GetInstance()->lightKc;
	Renderer::CheckForErrors();
	float lightKl = World::GetInstance()->lightKl;
	Renderer::CheckForErrors();
	float lightKq = World::GetInstance()->lightKq;
	Renderer::CheckForErrors();
	vec4 lightPosition = World::GetInstance()->lightPosition;
	Renderer::CheckForErrors();
		glUniform4f(LightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
		Renderer::CheckForErrors();
		glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
		Renderer::CheckForErrors();
		glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);
		Renderer::CheckForErrors();

   scene->draw(programID);
	
	

	// 3rd attribute buffer : vertex color
	//glEnableVertexAttribArray(2);
	//glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	//glVertexAttribPointer(	2,
	//						2,
	//						GL_FLOAT,
	//						GL_FALSE,
	//						sizeof(Vertex),
	//						(void*) (2* sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
	//					);


	
	Renderer::CheckForErrors();
    
    Renderer::SetShader(oldShader);
}

bool ShipModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		return Model::ParseLine(token);
	}
}
