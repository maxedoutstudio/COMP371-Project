//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Animation.h"
#include "Renderer.h"
#include "World.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;
using namespace std;

AnimationKey::AnimationKey() : Model()
{
}

AnimationKey::~AnimationKey()
{
}

void AnimationKey::Update(float dt)
{
	Model::Update(dt);
}

void AnimationKey::Draw()
{
	Model::Draw();
}

bool AnimationKey::ParseLine(const std::vector<ci_string> &token)
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

Animation::Animation() 
	: mName(""), mCurrentTime(0.0f), mDuration(0.0f), mVertexBufferID(0), mVertexArrayID(0)
{
}

Animation::~Animation()
{
}

void Animation::CreateVertexBuffer()
{
    // This is just to display lines between the animation keys
    for (int i=0; i<mKey.size(); ++i)
    {
        Vertex v;
        v.position = mKey[i].mPosition;
        mVertexBuffer.push_back(v);
    }
    
	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*mVertexBuffer.size(), &(mVertexBuffer[0]), GL_STATIC_DRAW);
}

void Animation::Update(float dt)
{
	mCurrentTime += dt;

    while(mCurrentTime > mDuration)
    {
        mCurrentTime -= mDuration;
    }
}

void Animation::Draw()
{
	// Draw lines between waypoints
	//
	// Steps:
	// - Draw the Vertex Buffer
	// - The Shader used to draw the Animation is AnimationLines, only requires to send the positions of the waypoints
	// - You need to bind the vertex buffer created in CreateVertexBuffer() as the input of the vertex shader
	// - Also set the WorldTransform uniform variable in the vertex shader
	// - You Should use GL_LINE_LOOP as a primitive to draw lines (not triangles)
	// - You can look at CubeModel::Draw() for a good example!

	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
    glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &mat4(1.0f)[0][0]);

	// 1st attribute buffer : vertex Positions
    glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(0,
		3,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		sizeof(Vertex), // stride
		(void*)0        // array buffer offset
		);
    
	// Draw the triangles !
	glDrawArrays(GL_LINE_LOOP, 0, mVertexBuffer.size());

	glDisableVertexAttribArray(0);
}


void Animation::Load(ci_istringstream& iss)
{
	ci_string line;

	// Parse model line by line
	while(std::getline(iss, line))
	{
		// Splitting line into tokens
		ci_istringstream strstr(line);
		istream_iterator<ci_string, char, ci_char_traits> it(strstr);
		istream_iterator<ci_string, char, ci_char_traits> end;
		vector<ci_string> token(it, end);

		if (ParseLine(token) == false)
		{
			fprintf(stderr, "Error loading scene file... token:  %s!", token[0].c_str());
			getchar();
			exit(-1);
		}
	}
}


bool Animation::ParseLine(const std::vector<ci_string> &token)
{
    if (token.empty())
    {
        return true;
    }
	else if (token[0] == "name")
	{
		assert(token.size() > 2);
		assert(token[1] == "=");

		mName = token[2];
		return true;
	}
	else if (token[0] == "key")
	{
		assert(token.size() > 5);
        assert(token[1] == "=");
        assert(token[3] == "time");
        assert(token[4] == "=");

		ci_string name = token[2];
		AnimationKey *key = World::GetInstance()->FindAnimationKey(name);
        
		assert(key != nullptr);
		AddKey(key, atof(token[5].c_str()));
		return true;
	}
	return false;
}

void Animation::AddKey(AnimationKey* key, float time)
{
    assert(time >= mDuration);
	mKey.push_back(*key);
    mKeyTime.push_back(time);
    mDuration = time;
}

ci_string Animation::GetName() const
{
	return mName;
}

glm::mat4 Animation::GetAnimationWorldMatrix() const
{
    // @TODO 3 - Find the 2 keys to interpolate the transformation
    //           Interpolate the position, scaling and rotation separately
    //           Finally concatenate the interpolated transforms into a single
    //           world transform and return it.
    
    mat4 worldMatrix(1.0f); // identiy matrix
	int kFrame = 0; // initialize  key frame index 0
	while(mKeyTime[kFrame+1] < mCurrentTime){ // gets index of last key frame  that occured by looping through frame time values.
		kFrame ++;
	}
	float tNorm =  (mCurrentTime - mKeyTime[kFrame])/(mKeyTime[kFrame+1] - mKeyTime[kFrame]); // normalize time
	mat4 translate4b = glm::translate(worldMatrix,mKey[kFrame].GetPosition()); // get translation matrix for base keyframe,the last keyframe that occured.
	mat4 translate4n = glm::translate(worldMatrix,mKey[kFrame+1].GetPosition()); // get  translation matrix for the next keyframe.
	mat4 lerpTranslate = glm::mix(translate4b,translate4n,tNorm); // interperlate translation between base and next keyframe  by normalized time.
	mat4 scale4b = glm::scale(worldMatrix,mKey[kFrame].GetScaling()); // get scale matrix for base keyframe,the last keyframe that occured.
	mat4 scale4n = glm::scale(worldMatrix,mKey[kFrame+1].GetScaling()); // get scale matrix for the next keyframe.
	mat4 lerpScale = glm::mix(scale4b,scale4n,tNorm); // interperlate translation between base and next keyframe.
	
	//roation matrix
	quat rotationQuatb = glm::angleAxis(mKey[kFrame].GetRotationAngle(),mKey[kFrame].GetRotationAxis()); // get rotation quaternion for base keyframe,the last keyframe that occured.
	quat rotationQuatn = glm::angleAxis(mKey[kFrame+1].GetRotationAngle(),mKey[kFrame+1].GetRotationAxis()); // get rotation quaternion for next keyframe.
	quat slerpRotateq = glm::slerp(rotationQuatb,rotationQuatn,tNorm); // spherical interperlation of rotation between base and next keyframe by normalized time.
	mat4 slerpRotate4 = mat4_cast(slerpRotateq); // convert slerp quat to matrix.

	return lerpTranslate*slerpRotate4*lerpScale*worldMatrix; // matrix multiplication chain.
}

AnimationKey Animation::GetmKey(int i)
{
	
	return Animation::mKey[i];
	
}

float Animation::GetmCurrentTime()
{
	return Animation::mCurrentTime;
}
float Animation::GetmKeyTime(int i)
{
	return Animation::mKeyTime[i];
}
