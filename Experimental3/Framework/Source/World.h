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
#include "Billboard.h"
#include <vector>
#include "sceneLoader.h"
#include "Projectile.h"
#include "PlayerProjectile.h"
#include "ShipEnnemyModel.h"

class Camera;
class Model;
class Animation;
class AnimationKey;
class ParticleSystem;


class World
{
public:
	World();
	~World();
	

    static World* GetInstance();

	unsigned int nextProjectile;
	int droidTextureID;// = TextureLoader::LoadTexture("../Assets/Textures/droid.tga");
	int MeteorTextureID;// = TextureLoader::LoadTexture("../Assets/Textures/meteor.jpg");
	int shipTextureID;// = TextureLoader::LoadTexture("../Assets/Textures/ship1.jpg");
	int projTextureID;

	sceneLoader* meteorScene;
	sceneLoader* droidScene;
	sceneLoader* projScene;
	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);
    Animation* FindAnimation(ci_string animName);
	AnimationKey* FindAnimationKey(ci_string keyName);

    const Camera* GetCurrentCamera() const;
    void AddBillboard(Billboard* b);
    void RemoveBillboard(Billboard* b);
    void AddParticleSystem(ParticleSystem* particleSystem);
    void RemoveParticleSystem(ParticleSystem* particleSystem);

	
	glm::vec3 lightColor;// = vec3(1.0f, 1.0f, 1.0f);
	float lightKc;// = 0.0f;
	float lightKl;//= 0.0f;
	float lightKq;//  = 1.0f;
	glm::vec4 lightPosition;// = vec4(2.0f, 2.0f, -2.0f, 1.0f);
	// Material Coefficients
	float ka ;
	float kd ;
	float ks ;
	float n ;
	float spawntime;
	std::vector<Model*> mModel;
	std::vector<Projectile*> mProjectile;
	std::vector<PlayerProjectile*> mPlayerProjectile;
	std::vector<ShipEnnemyModel*> mShipEnnemyModel;
	Animation* GetmAnimation(int i);
	void LoadNextProjectile();
	void ResetSpawnTime();
    
private:
    static World* instance;
    
	
    std::vector<Animation*> mAnimation;
    std::vector<AnimationKey*> mAnimationKey;
	std::vector<Camera*> mCamera;
    std::vector<ParticleSystem*> mParticleSystemList;
	unsigned int mCurrentCamera;

    BillboardList* mpBillboardList;
};
//const glm::vec4 lightPosition(2.0f, 2.0f, -2.0f, 1.0f);
//const glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
