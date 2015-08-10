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
<<<<<<< HEAD
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();


	// The Camera moves based on the User inputs
	// - You can access the mouse motion with EventManager::GetMouseMotionXY()
	// - For mapping A S D W, you can look in World.cpp for an example of accessing key states
	// - Don't forget to use dt to control the speed of the camera motion

	// Mouse motion to get the variation in angle
	mHorizontalAngle -= EventManager::GetMouseMotionX() * mAngularSpeed * dt;
	mVerticalAngle   -= EventManager::GetMouseMotionY() * mAngularSpeed * dt;

	// Clamp vertical angle to [-85, 85] degrees
	mVerticalAngle = std::max(-85.0f, std::min(85.0f, mVerticalAngle));
	if (mHorizontalAngle > 360)
	{
		mHorizontalAngle -= 360;
	}
	else if (mHorizontalAngle < -360)
	{
		mHorizontalAngle += 360;
	}

	float theta = radians(mHorizontalAngle);
	float phi = radians(mVerticalAngle);



	int kFrame = 0; // initialize  key frame index 0
	//while(World::GetInstance()->mAnimation[1]->mKeyTime[kFrame+1] < World::GetInstance()->mAnimation[1]->mCurrentTime){ // gets index of last key frame  that occured by looping through frame time values.
	//	kFrame ++;
	//}
	//int kFrame2 =0;
	//while(World::GetInstance()->mAnimation[0]->mKeyTime[kFrame2+1] < World::GetInstance()->mAnimation[0]->mCurrentTime){ // gets index of last key frame  that occured by looping through frame time values.
	//	kFrame2 ++;
	//}
	//float tNorm =  (World::GetInstance()->mAnimation[1]->mCurrentTime - World::GetInstance()->mAnimation[1]->mKeyTime[kFrame])/(World::GetInstance()->mAnimation[1]->mKeyTime[kFrame+1] - World::GetInstance()->mAnimation[1]->mKeyTime[kFrame]);
	//float tNorm2 =  (World::GetInstance()->mAnimation[0]->mCurrentTime - World::GetInstance()->mAnimation[0]->mKeyTime[kFrame2])/(World::GetInstance()->mAnimation[0]->mKeyTime[kFrame2+1] - World::GetInstance()->mAnimation[0]->mKeyTime[kFrame2]);
	// mPosition = glm::mix(World::GetInstance()->mAnimation[1]->mKey[kFrame].GetPosition(),World::GetInstance()->mAnimation[1]->mKey[kFrame+1].GetPosition(),tNorm)+ vec3(0,10.0f,0);
	////mLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
	//mLookAt =  (glm::mix(World::GetInstance()->mAnimation[0]->mKey[kFrame2].GetPosition(),World::GetInstance()->mAnimation[0]->mKey[kFrame2+1].GetPosition(),tNorm2)- mPosition);
	//while(World::GetInstance()->GetmAnimation(1)->GetmKeyTime(kFrame+1) < World::GetInstance()->GetmAnimation(1)->GetmCurrentTime()){ // gets index of last key frame  that occured by looping through frame time values.
	//	kFrame ++;
	//}
	//int kFrame2 =0;
	//while(World::GetInstance()->GetmAnimation(0)->GetmKeyTime(kFrame2+1) < World::GetInstance()->GetmAnimation(0)->GetmCurrentTime()){ // gets index of last key frame  that occured by looping through frame time values.
	//	kFrame2 ++;
	//}
	//float tNorm =  (World::GetInstance()->GetmAnimation(1)->GetmCurrentTime() - World::GetInstance()->GetmAnimation(1)->GetmKeyTime(kFrame))/(World::GetInstance()->GetmAnimation(1)->GetmKeyTime(kFrame+1) - World::GetInstance()->GetmAnimation(1)->GetmKeyTime(kFrame));
	//float tNorm2 =  (World::GetInstance()->GetmAnimation(0)->GetmCurrentTime() - World::GetInstance()->GetmAnimation(0)->GetmKeyTime(kFrame2))/(World::GetInstance()->GetmAnimation(0)->GetmKeyTime(kFrame2+1) - World::GetInstance()->GetmAnimation(0)->GetmKeyTime(kFrame2));
	mPosition = vec3(0.0,6.0,-6.0);
	mLookAt = World::GetInstance()->mModel[0]->GetPosition() - mPosition + vec3(0,2.5,0);
	normalize(mLookAt);
=======
	
	//mPosition+=(glm::vec3(0,0,0.5)*dt);
	mLookAt = World::GetInstance()->mModel[0]->GetPosition()-  mPosition + vec3(0,0,5.0f);
	glm::normalize(mLookAt);
>>>>>>> 81ada3de0ef471e3dee285a6943c339306d3d31c
	vec3 sideVector = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(sideVector);



	
}



glm::mat4 RailCamera::GetViewMatrix() const
{
	return glm::lookAt(	mPosition, mPosition + mLookAt, vec3(0.0f, 1.0f, 0.0f) );
}
