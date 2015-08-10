//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Background.h"
#include "Renderer.h"
#include "World.h"
#include "Camera.h"
#include <GL/glew.h>

#include "objloader.hpp"
#include "Renderer.h"
//#include "texture.hpp"
//#include "SOIL.h"
#include"sceneLoader.h"
#include "EventManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


using namespace std;
using namespace glm;


Background::Background(int textureID, glm::vec3 position, glm::vec3 size)
: Model() //mTextureID(textureID), mPosition(position), size(backgroundSize)
{
	mPosition = position;
	this->mSize = size;
    // Pre-allocate Vertex Buffer - 6 vertices by billboard (2 triangles)
    mVertexBuffer.resize(6);
    
    // Initialize vertices to white, with UVs taking all the texture
    for (int i=0; i<6; ++i)
    {
        
        // Normal on Z axis
        mVertexBuffer[i].normal = vec3(0.0f, 0.0f, 1.0f);
        
        // Texture coordinates
        switch(i)
        {
            case 0: mVertexBuffer[i].uv = vec2(0.0f, 1.0f); break;
            case 1: mVertexBuffer[i].uv = vec2(0.0f, 0.0f); break;
            case 2: mVertexBuffer[i].uv = vec2(1.0f, 1.0f); break;
            case 3: mVertexBuffer[i].uv = vec2(1.0f, 1.0f); break;
            case 4: mVertexBuffer[i].uv = vec2(0.0f, 0.0f); break;
            case 5: mVertexBuffer[i].uv = vec2(1.0f, 0.0f); break;
        }
    }
    
    // Create a vertex array
    glGenVertexArrays(1, &mVertexArrayID);
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
    // Note the vertex buffer will change over time, we use GL_DYNAMIC_DRAW
    glGenBuffers(1, &mVertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, mVertexBuffer.size() * sizeof(BackgroundVertex), &mVertexBuffer[0], GL_DYNAMIC_DRAW);
    Renderer::CheckForErrors();

}

Background::~Background()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);

	mVertexBuffer.resize(0);
}


void Background::Update(float dt)
{
    // Sort billboards according to their depth, the functor CompareBackgroundAlongZ does the magic!
    

    // Maybe the view matrix will be useful to align the billboards
    const Camera* cam = World::GetInstance()->GetCurrentCamera();
    mat4 viewMatrix = cam->GetViewMatrix();
	
   
	vec3 CameraRight_worldspace = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	vec3 CameraUp_worldspace = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	vec3 CameraZ_worldspace = vec3(viewMatrix[0][2],viewMatrix[1][2], viewMatrix[2][2]) ;
  

    // ... The code below needs to be modified ...
        
    // First triangle
    // Top left
		
	vec3 vertexPosition_worldspace = mPosition 
		- CameraRight_worldspace * (0.5f*mSize.x) 
		+ CameraUp_worldspace * (0.5f*mSize.y) ;
    mVertexBuffer[0].position.x = vertexPosition_worldspace.x;
    mVertexBuffer[0].position.y = vertexPosition_worldspace.y;
	mVertexBuffer[0].position.z = vertexPosition_worldspace.z;
	mVertexBuffer[0].normal = CameraZ_worldspace;

        
    // Bottom Left
	vertexPosition_worldspace = mPosition 
		- CameraRight_worldspace * (0.5f*mSize.x) 
		- CameraUp_worldspace * (0.5f*mSize.y) ;
    mVertexBuffer[+1].position.x = vertexPosition_worldspace.x;
    mVertexBuffer[+1].position.y = vertexPosition_worldspace.y;
    mVertexBuffer[+1].position.z = vertexPosition_worldspace.z;
	mVertexBuffer[+1].normal =  CameraZ_worldspace;


        
    // Top Right
	vertexPosition_worldspace = mPosition 
		+ CameraRight_worldspace * (0.5f*mSize.x) 
		+ CameraUp_worldspace * (0.5f*mSize.y) ;
    mVertexBuffer[+2].position.x = vertexPosition_worldspace.x;
    mVertexBuffer[+2].position.y = vertexPosition_worldspace.y;
    mVertexBuffer[+2].position.z = vertexPosition_worldspace.z;
	mVertexBuffer[+2].normal =  CameraZ_worldspace;


    // Second Triangle
    // Top Right
	//vertexPosition_worldspace = mPosition 
	//	+ CameraRight_worldspace * (0.5f*mSize.x) 
	//	+ CameraUp_worldspace * (0.5f*mSize.y);
    mVertexBuffer[+3].position.x = vertexPosition_worldspace.x;
    mVertexBuffer[+3].position.y = vertexPosition_worldspace.y;
    mVertexBuffer[+3].position.z = vertexPosition_worldspace.z;
	mVertexBuffer[+3].normal = CameraZ_worldspace;


        
    // Bottom Left
	vertexPosition_worldspace = mPosition 
		- CameraRight_worldspace * (0.5f*mSize.x) 
		- CameraUp_worldspace * (0.5f*mSize.y) ;
    mVertexBuffer[+4].position.x = vertexPosition_worldspace.x;
    mVertexBuffer[+4].position.y = vertexPosition_worldspace.y;
    mVertexBuffer[+4].position.z = vertexPosition_worldspace.z;
	mVertexBuffer[+4].normal =  CameraZ_worldspace;


        
    // Bottom Right
		
	vertexPosition_worldspace = mPosition 
		+ CameraRight_worldspace * (0.5f*mSize.x) 
		- CameraUp_worldspace * (0.5f*mSize.y) ;
    mVertexBuffer[+5].position.x = vertexPosition_worldspace.x;
    mVertexBuffer[+5].position.y = vertexPosition_worldspace.y;
    mVertexBuffer[+5].position.z = vertexPosition_worldspace.z;
	mVertexBuffer[+5].normal =  CameraZ_worldspace;


    
    Renderer::CheckForErrors();
    
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6*sizeof(BackgroundVertex), (void*)&mVertexBuffer[0]);
}

void Background::Draw()
{
    Renderer::CheckForErrors();

    
    // Set current shader to be the Textured Shader
    ShaderType oldShader = (ShaderType)Renderer::GetCurrentShader();
    
    Renderer::SetShader(TEX_LIGHT2);
    glUseProgram(Renderer::GetShaderProgramID());

    Renderer::CheckForErrors();

    
    GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "mySamplerTexture");
    glActiveTexture(GL_TEXTURE0);
	unsigned int programID = Renderer::GetShaderProgramID();

    Renderer::CheckForErrors();

    
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glUniform1i(textureLocation, 0);				// Set our Texture sampler to user Texture Unit 0

    
    Renderer::CheckForErrors();




    //// This looks for the MVP Uniform variable in the Vertex Program
    //GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
    //
    //// Send the view projection constants to the shader
    //const Camera* currentCamera = World::GetInstance()->GetCurrentCamera();
    //mat4 VP = currentCamera->GetViewProjectionMatrix();
    //glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
	GLuint VMatrixLocation = glGetUniformLocation(programID, "ViewTransform");
    // Send the view projection constants to the shader
    const Camera* currentCamera = World::GetInstance()->GetCurrentCamera();
	mat4 V = currentCamera->GetViewMatrix();
    glUniformMatrix4fv(VMatrixLocation, 1, GL_FALSE, &V[0][0]);

	GLuint PMatrixLocation = glGetUniformLocation(programID, "ProjectionTransform");
	mat4 P = currentCamera->GetProjectionMatrix();
	glUniformMatrix4fv(PMatrixLocation, 1, GL_FALSE, &P[0][0]);
    // Draw the Vertex Buffer
    // Note this draws a unit Cube
    // The Model View Projection transforms are computed in the Vertex Shader
    glBindVertexArray(mVertexArrayID);
    
    GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");

    // Background position are all relative to the origin
    mat4 worldMatrix(1.0f);
    glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);




		// Get a handle for Light Attributes uniform
	
	GLuint LightPositionID = glGetUniformLocation(programID, "WorldLightPosition");
	GLuint LightColorID = glGetUniformLocation(programID, "lightColor");
	GLuint LightAttenuationID = glGetUniformLocation(programID, "lightAttenuation");

	// Get a handle for Material Attributes uniform
	GLuint MaterialAmbientID = glGetUniformLocation(programID, "materialAmbient");
    GLuint MaterialDiffuseID = glGetUniformLocation(programID, "materialDiffuse");
    GLuint MaterialSpecularID = glGetUniformLocation(programID, "materialSpecular");
    GLuint MaterialExponentID = glGetUniformLocation(programID, "materialExponent");
	
	// Set shader constants
	const float ka = 0.2f;
	const float kd = 0.8f;
	const float ks = 0.2f;
	const float n = 90.0f;
        glUniform1f(MaterialAmbientID, ka);
        glUniform1f(MaterialDiffuseID, kd);
        glUniform1f(MaterialSpecularID, ks);
        glUniform1f(MaterialExponentID, n);
		//vec4 lightPosition = World::GetInstance()->lightPosition;
		//vec3 lightColor = World::GetInstance()->lightColor;
	//const vec3 lightColor(1.0f, 1.0f, 1.0f);
	//const float lightKc = 0.0f;
	//const float lightKl = 0.0f;
	//const float lightKq = 1.0f;
	//const vec4 lightPosition(5.0f, 5.0f, -5.0f, 1.0f);
	vec3 lightColor = World::GetInstance()->lightColor;
	float lightKc = World::GetInstance()->lightKc;
	float lightKl = World::GetInstance()->lightKl;
	float lightKq = World::GetInstance()->lightKq;
	vec4 lightPosition = World::GetInstance()->lightPosition;
		glUniform4f(LightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
		glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);

    
    // 1st attribute buffer : vertex Positions
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(0,				// attribute. No particular reason for 0, but must match the layout in the shader.
                          3,				// size
                          GL_FLOAT,		// type
                          GL_FALSE,		// normalized?
                          sizeof(BackgroundVertex), // stride
                          (void*)0        // array buffer offset
                          );
    
    // 2nd attribute buffer : vertex normal
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(BackgroundVertex),
                          (void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
                          );
    
  
    
    // 3rd attribute buffer : texture coordinates
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(BackgroundVertex),
                          (void*) (2* sizeof(vec3) + sizeof(vec4)) // texture coordinates are Offseted by 2 vec3 (see class Vertex) and a vec4
                          );
    
    
    // Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer.size()); // 6 vertices by billboard
    
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    Renderer::CheckForErrors();
    
    Renderer::SetShader(oldShader);
}


bool Background::ParseLine(const std::vector<ci_string> &token)
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