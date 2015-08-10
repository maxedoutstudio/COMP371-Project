//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#include "RailCamera.h"
#include "EventManager.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "World.h"
#include "Animation.h"

#include <GLFW/glfw3.h>
#include <algorithm>



using namespace glm;


RailCamera::RailCamera(glm::vec3 position) :  Camera(), mPosition(position), mLookAt(0.0f, 0.0f, -1.0f), mHorizontalAngle(90.0f), mVerticalAngle(0.0f), mSpeed(5.0f), mAngularSpeed(2.5f)
{
	mPosition =  vec3(0,6.0f,-6.0f);
	mLookAt = vec3(0,0,5.0f);
	glm::normalize(mLookAt);
	vec3 sideVector = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(sideVector);

}

RailCamera::~RailCamera()
{
}
void RailCamera::Update(float dt)
{
	
	//mPosition+=(glm::vec3(0,0,0.5)*dt);
	mLookAt = World::GetInstance()->mModel[0]->GetPosition()-  mPosition + vec3(0,0,5.0f);
	glm::normalize(mLookAt);
	vec3 sideVector = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(sideVector);



	
}



glm::mat4 RailCamera::GetViewMatrix() const
{
	return glm::lookAt(	mPosition, mPosition + mLookAt, vec3(0.0f, 1.0f, 0.0f) );
}