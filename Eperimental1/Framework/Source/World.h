//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "ParsingHelper.h"
#include <vector>
#include <GLM/glm.hpp>

class Camera;
class Model;
class Animation;
class AnimationKey;

class World
{
public:
	World();
	~World();
	
    static World* GetInstance();

	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);
    Animation* FindAnimation(ci_string animName);
	AnimationKey* FindAnimationKey(ci_string keyName);
	std::vector<Model*> mModel;
	Animation* GetmAnimation(int i);
	static glm::vec3 gspos();

protected:
    static World* instance;
	std::vector<Animation*> mAnimation;
	
  
    std::vector<AnimationKey*> mAnimationKey;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;
};
