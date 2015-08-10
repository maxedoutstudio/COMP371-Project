//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "FirstPersonCamera.h"

#include "CubeModel.h"
#include "SphereModel.h"
#include "Animation.h"
#include "Billboard.h"
#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "TextureLoader.h"

#include "RailCamera.h"
#include "ChairModel.h"
#include "ShipModel.h"
#include "ShipEnnemyModel.h"//TINO
#include "MeteorModel.h"
#include "Projectile.h"
#include "EarthModel.h"
#include "MoonModel.h"
#include "BackgroundSphere.h"
#include "SunModel.h"
#include "MarsModel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>



#include "ParticleDescriptor.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"


using namespace std;
using namespace glm;

World* World::instance;


World::World(int level)
{

    instance = this;

	if (level == 0){

		rotationAxis = vec3(0, 1, 0);
	}
	else if (level == 1){

		rotationAxis = vec3(-1, 0, 0);
	}


	orbitRotationConstantNumerator = 1;
	orbitRotationConstantDenominator = 18;

	stretchConstant = 1;

	selfRotationConstant = 10;

	// Setup Camera
	mCamera.push_back(new FirstPersonCamera(vec3(3.0f, 1.0f, 5.0f)));
	mCamera.push_back(new StaticCamera(vec3(3.0f, 30.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
//	mCamera.push_back(new StaticCamera(vec3(0.5f,  0.5f, 5.0f), vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
	mCamera.push_back(new RailCamera(vec3(3.0f,  3.0f, 3.0f)));

	mCurrentCamera = 2;
	// Light Coefficients
	
	lightColor =  glm::vec3(1.0f, 1.0f, 1.0f);
	lightPosition = glm::vec4(2.0f, 0.0f, -2.0f, 0.0f);
	lightKc = 0.0f;
	lightKl = 0.0f;
	lightKq = 1.0f;
	// Material Coefficients


    
    // TODO: You can play with different textures by changing the billboardTest.bmp to another texture
#if defined(PLATFORM_OSX)
//    int billboardTextureID = TextureLoader::LoadTexture("Textures/BillboardTest.bmp");
    int billboardTextureID = TextureLoader::LoadTexture("Textures/Particle.png");
#else
//    int billboardTextureID = TextureLoader::LoadTexture("../Assets/Textures/BillboardTest.bmp");
	int billboardTextureID = TextureLoader::LoadTexture("../Assets/Textures/Particle.png");
	
#endif
    assert(billboardTextureID != 0);

    mpBillboardList = new BillboardList(2048, billboardTextureID);

	nextProjectile = 0;
	spawntime = 0.0f;

	//Loads the textures
	shipTextureID = TextureLoader::LoadTexture("../Assets/Textures/ship1.jpg");
	droidTextureID = TextureLoader::LoadTexture("../Assets/Textures/droid.tga");
	MeteorTextureID = TextureLoader::LoadTexture("../Assets/Textures/meteor.jpg");
	projTextureID = TextureLoader::LoadTexture("../Assets/Textures/projectile.jpg");
	moonTextureID = TextureLoader::LoadTexture("../Assets/Textures/moonHD.jpg");
	earthTextureID = TextureLoader::LoadTexture("../Assets/Textures/earthHD.jpg");
	spaceTextureID = TextureLoader::LoadTexture("../Assets/Textures/spaceNASA2.jpg");
	sunTextureID = TextureLoader::LoadTexture("../Assets/Textures/sun.jpg");
	marsTextureID = TextureLoader::LoadTexture("../Assets/Textures/mars.jpg");


	 
	droidScene = new sceneLoader("../Assets/Models/droid.obj");
	meteorScene = new sceneLoader("../Assets/Models/meteor.obj");
	projScene = new sceneLoader("../Assets/Models/projectile.obj");
	earthScene = new sceneLoader("../Assets/Models/earthHD.obj");

}

World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();


	for (vector<Projectile*>::iterator it = mProjectile.begin(); it < mProjectile.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

	for (vector<ShipEnnemyModel*>::iterator it = mShipEnnemyModel.begin(); it < mShipEnnemyModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		delete *it;
	}

	mAnimation.clear();

	for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
	{
		delete *it;
	}

	mAnimationKey.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
	{
		delete *it;
	}
	mCamera.clear();

	delete mpBillboardList;
}

World* World::GetInstance()
{
    return instance;
}

void World::Update(float dt)
{

	//Dynamic light test

	quat rotQuatA = glm::angleAxis(0.0f, rotationAxis);
	quat rotQuatB = angleAxis(180.0f, rotationAxis);
	quat slerpedRotation = slerp(rotQuatA, rotQuatB, orbitRotationConstantNumerator * - dt / orbitRotationConstantDenominator);
	mat4 rotation = mat4_cast(slerpedRotation);
	lightPosition = rotation*lightPosition;

	// User Inputs
	spawntime+=dt;

	//Warp speed Activation

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS && selfRotationConstant <= 50)
	{
		orbitRotationConstantNumerator += 0.1;
		selfRotationConstant += 1;
		stretchConstant += 0.1;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS && selfRotationConstant > 10)
	{
		orbitRotationConstantNumerator -= 0.1;
		selfRotationConstant -= 1;
		stretchConstant -= 0.1;
	}
	else {
		//stretchConstant = 0;
	}

	// 0 1 2 to change the Camera
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS)
	{
		mCurrentCamera = 0;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 1)
		{
			mCurrentCamera = 1;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 2)
		{
			mCurrentCamera = 2;
		}
	}



	// Spacebar to change the shader
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0 ) == GLFW_PRESS)
	{
		Renderer::SetShader(SHADER_SOLID_COLOR);
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9 ) == GLFW_PRESS)
	{
		Renderer::SetShader(SHADER_BLUE);
	}

    // Update animation and keys
    for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        (*it)->Update(dt);
    }
    
    for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        (*it)->Update(dt);
    }


	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Update(dt);
	}
    for (vector<Projectile*>::iterator it = mProjectile.begin(); it < mProjectile.end(); ++it)
	{
		(*it)->Update(dt);
	}

	  for (vector<PlayerProjectile*>::iterator it = mPlayerProjectile.begin(); it < mPlayerProjectile.end(); ++it)
	{
		(*it)->Update(dt);
	}

	 for (vector<ShipEnnemyModel*>::iterator it = mShipEnnemyModel.begin(); it < mShipEnnemyModel.end(); ++it)
	{
		(*it)->Update(dt);
	}
    // Update billboards
    
    for (vector<ParticleSystem*>::iterator it = mParticleSystemList.begin(); it != mParticleSystemList.end(); ++it)
    {
        (*it)->Update(dt);
    }
    
    mpBillboardList->Update(dt);

}

void World::Draw()
{
	Renderer::BeginFrame();
	
	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Draw models

	//glFrustum(1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f);
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Draw();
	}
	for (vector<ShipEnnemyModel*>::iterator it = mShipEnnemyModel.begin(); it < mShipEnnemyModel.end(); ++it)
	{
		(*it)->Draw();
	}
	for (vector<Projectile*>::iterator it = mProjectile.begin(); it < mProjectile.end(); ++it)
	{
		(*it)->Draw();
	}

	for (vector<PlayerProjectile*>::iterator it = mPlayerProjectile.begin(); it < mPlayerProjectile.end(); ++it)
	{
		(*it)->Draw();
	}
	// Draw Path Lines
	
	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_PATH_LINES);
	glUseProgram(Renderer::GetShaderProgramID());

	// Send the view projection constants to the shader
	VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
		glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

		(*it)->Draw();
	}

	for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
	{
		mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
		glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

		(*it)->Draw();
	}

    Renderer::CheckForErrors();
    
    // Draw Billboards
    mpBillboardList->Draw();


	// Restore previous shader
	Renderer::SetShader((ShaderType) prevShader);

	Renderer::EndFrame();
}

void World::LoadScene(const char * scene_path)
{
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if(input.fail() )
	{	 
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while( std::getline( input, item, '[' ) )   
	{
        ci_istringstream iss( item );

		ci_string result;
		if( std::getline( iss, result, ']') )
		{
			if( result == "cube" )
			{
				// Box attributes
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				mModel.push_back(cube);
			}
            else if( result == "sphere" )
            {
                SphereModel* sphere = new SphereModel();
                sphere->Load(iss);
                mModel.push_back(sphere);
            }
			else if ( result == "animationkey" )
			{
				AnimationKey* key = new AnimationKey();
				key->Load(iss);
				mAnimationKey.push_back(key);
			}
			else if (result == "animation")
			{
				Animation* anim = new Animation();
				anim->Load(iss);
				mAnimation.push_back(anim);
			}
			else if ( result.empty() == false && result[0] == '#')
			{
				// this is a comment line
			}else if(result == "cModel")
			{
				
				ShipModel* ship = new ShipModel(shipTextureID);
				ship->Load(iss);
				mModel.push_back(ship);
			}else if(result == "eModel")
			{
				

				//TINO set X borders for ennemies
				float lBound = -10.0f;
				float uBound = 10.0f;

				//TINO position
				//positionX will be generated randomly between -5 and 5
				float ennemyPositionX = lBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(uBound-lBound)));
				
				//TINO 10 ennemies
				ShipEnnemyModel* ennemy;
				ennemy = new ShipEnnemyModel(droidTextureID);
				ennemy->Load(iss);
				//float ennemyPositionZ = 10.0f;
				vec3 EnnemyPosition = vec3(ennemyPositionX, 0.0f, ennemy->GetPosition().z);//just modify x and z
				ennemy->SetPosition(EnnemyPosition);
				mShipEnnemyModel.push_back(ennemy);


				////TINO set up the ennemis
				//for(int i=1; i<10;i++){
				//	ennemy[i] = new ShipEnnemyModel(droidTextureID);
				//	ennemy[i]->Load(iss);
					
				//	//vec3 EnnemySize = vec3(0.1f, 0.1f, 0.1f);
				//	ennemy[i]->SetPosition(EnnemyPosition);
				//	ennemy[i]->SetScaling(EnnemySize);
				//	mModel.push_back(ennemy[i]);

				//	ennemyPositionZ += 5.0f;//next ennemy will be 10 step further in Z

				//	//update X random position for next ennemy
				//	ennemyPositionX = lBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(uBound-lBound)));
				//}


				//TINO
				/*ShipEnnemyModel* chair2 = new ShipEnnemyModel(shipTextureID);
				chair2->Load(iss);
				vec3 EnnemyPosition = vec3(0.0f, 0.0f, 10.0f);//just modify x and z
				vec3 EnnemySize = vec3(0.1f, 0.1f, 0.1f);
				chair2->SetPosition(EnnemyPosition);
				chair2->SetScaling(EnnemySize);
				mModel.push_back(chair2);*/

			}else if(result == "Meteor")
			{
				

				//TINO set X borders for ennemies
				float lBound = -10.0f;
				float uBound = 10.0f;

				//TINO position
				//positionX will be generated randomly between -5 and 5
				float MeteorPositionX = lBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(uBound-lBound)));
				float MeteorPositionZ = 10.0f;

				//TINO 10 ennemies
				MeteorModel* Meteor[3];

				//TINO set up the ennemis

				Meteor[0] = new MeteorModel(MeteorTextureID);
				Meteor[0]->Load(iss);
				vec3 MeteorSize = Meteor[0]->GetScaling();
				for(int i=1; i<3;i++){
					Meteor[i] = new MeteorModel(MeteorTextureID);
					Meteor[i]->Load(iss);
					vec3 MeteorPosition = vec3(MeteorPositionX, 0.0f, MeteorPositionZ);//just modify x and z
					//vec3 EnnemySize = vec3(0.1f, 0.1f, 0.1f);
					Meteor[i]->SetPosition(MeteorPosition);
					Meteor[i]->SetScaling(MeteorSize);
					mModel.push_back(Meteor[i]);

					MeteorPositionZ += 5.0f;//next ennemy will be 10 step further in Z

					//update X random position for next ennemy
					MeteorPositionX = lBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(uBound-lBound)));
				}
			}else if(result == "pModel")
			{	
				Projectile* proj = new Projectile(projTextureID);
				proj->Load(iss);
				mProjectile.push_back(proj);
			}

			else if(result == "mpModel")
			{	
				PlayerProjectile* proj = new PlayerProjectile(projTextureID);
				proj->Load(iss);
				mPlayerProjectile.push_back(proj);
			}else if(result == "Earth"){
				EarthModel* earth = new EarthModel(earthTextureID);
				earth->Load(iss);
				mModel.push_back(earth);


			}
			else if(result == "Moon"){
				MoonModel* moon = new MoonModel(moonTextureID);
				moon->Load(iss);
				mModel.push_back(moon);


			}
			else if (result == "BackgroundSphere"){
				BackgroundSphere* background = new BackgroundSphere(spaceTextureID);
				background->Load(iss);
				mModel.push_back(background);


			}
			else if (result == "Sun"){
				//Loading the sun should set the light at the same position as the sun
				SunModel* sun = new SunModel(sunTextureID);
				sun->Load(iss);
				lightPosition = vec4(sun->GetPosition(),0);
				mModel.push_back(sun);
				//

			}
			else if (result == "Mars"){
				MarsModel* mars = new MarsModel(marsTextureID);
				mars->Load(iss);
				mModel.push_back(mars);
			}

			else
			{
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
	    }
	}
	input.close();

	// Set Animation vertex buffers
	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		// Draw model
		(*it)->CreateVertexBuffer();
	}
}


Animation* World::FindAnimation(ci_string animName)
{
    for(std::vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        if((*it)->GetName() == animName)
        {
            return *it;
        }
    }
    return nullptr;
}

AnimationKey* World::FindAnimationKey(ci_string keyName)
{
    for(std::vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        if((*it)->GetName() == keyName)
        {
            return *it;
        }
    }
    return nullptr;
}

const Camera* World::GetCurrentCamera() const
{
     return mCamera[mCurrentCamera];
}

void World::AddBillboard(Billboard* b)
{
    mpBillboardList->AddBillboard(b);
}

void World::RemoveBillboard(Billboard* b)
{
    mpBillboardList->RemoveBillboard(b);
}

void World::AddParticleSystem(ParticleSystem* particleSystem)
{
    mParticleSystemList.push_back(particleSystem);
}

void World::RemoveParticleSystem(ParticleSystem* particleSystem)
{
    vector<ParticleSystem*>::iterator it = std::find(mParticleSystemList.begin(), mParticleSystemList.end(), particleSystem);
    mParticleSystemList.erase(it);
}
Animation* World::GetmAnimation(int i)
{
	return World::mAnimation[i];
}
void World::LoadNextProjectile(){
nextProjectile++;
	if (nextProjectile >= 15){
		nextProjectile-= 15;
	}
}

void World::ResetSpawnTime(){
	spawntime = 0.0f;
}
