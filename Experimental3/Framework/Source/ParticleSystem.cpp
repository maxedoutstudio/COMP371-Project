//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//         with help from Jordan Rutty
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "ParticleSystem.h"
#include "ParticleDescriptor.h"
#include "ParticleEmitter.h"
#include "EventManager.h"
#include "World.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace glm;


ParticleSystem::ParticleSystem(ParticleEmitter* emitter, ParticleDescriptor* descriptor)
: mpDescriptor(descriptor), mpEmitter(emitter)
{
    assert(mpDescriptor != nullptr);
    assert(mpEmitter != nullptr);
    
    // Pre-allocate the maximum number of particles at a give time, according to
    // lifetime and emission rate
    int maxParticles = static_cast<int>(mpDescriptor->emissionRate * (mpDescriptor->totalLifetime + mpDescriptor->totalLifetimeDelta)) + 1;
    
    mInactiveParticles.resize(maxParticles);
    for (std::list<Particle*>::iterator it = mInactiveParticles.begin(); it != mInactiveParticles.end(); ++it)
    {
        *it = new Particle();
    }
}

ParticleSystem::~ParticleSystem()
{
	for (std::list<Particle*>::iterator it = mInactiveParticles.begin(); it != mInactiveParticles.end(); ++it)
	{
		delete *it;
	}


	for (std::list<Particle*>::iterator it = mParticleList.begin(); it != mParticleList.end(); ++it)
	{
		World::GetInstance()->RemoveBillboard(&(*it)->billboard);
		delete *it;
	}

	mInactiveParticles.resize(0);
	mParticleList.resize(0);
}

void ParticleSystem::Update(float dt)
{
    // Emit particle according to the emission rate
    float averageTimeBetweenEmission = 1.0f / mpDescriptor->emissionRate;
    float randomValue = EventManager::GetRandomFloat(0.0f, 1.0f) * averageTimeBetweenEmission;
    while (mInactiveParticles.size() > 0 && randomValue < dt)
    {
        randomValue += averageTimeBetweenEmission;
        
        // emit particle
        // transfer a particle from the inactive pool to the active pool
        Particle* newParticle = mInactiveParticles.back();
        mParticleList.push_back(newParticle);
        mInactiveParticles.pop_back();
        
        // Set particle initial parameters
        newParticle->billboard.position = mpEmitter->GetRandomPosition();
        newParticle->billboard.size = mpDescriptor->initialSize + EventManager::GetRandomFloat(-1.0f, 1.0f) * mpDescriptor->initialSizeDelta;
        newParticle->billboard.color = mpDescriptor->initialColor;
        newParticle->currentTime = 0.0f;
        newParticle->lifeTime = mpDescriptor->totalLifetime + mpDescriptor->totalLifetimeDelta * EventManager::GetRandomFloat(-1.0f, 1.0f);
        newParticle->velocity = mpDescriptor->velocity;
        
        // @TODO 6 - Initial Random Particle Velocity vector
        //
        // Adjust the random velocity according to a random vertical angle variation on a cone
        //
        // Step 1 : You can rotate the velocity vector by a random number between 0 and
        //          mpDescriptor->velocityDeltaAngle.
        // Step 2 : You can rotate the result in step 1 by an random angle from 0 to
        //          360 degrees about the original velocity vector
		
		vec3 orthogonalVecToVelo;
		if(cross(newParticle->velocity, vec3(0,1,0)) == vec3(0.0f)){
			 orthogonalVecToVelo = cross(newParticle->velocity, vec3(1,0,0));
		}else{
			orthogonalVecToVelo = cross(newParticle->velocity, vec3(0,1,0));
		}
		orthogonalVecToVelo = normalize(orthogonalVecToVelo);

		mat4 rotate4 = glm::rotate(mat4(1.0f), EventManager::GetRandomFloat(0.0f, mpDescriptor->velocityDeltaAngle), orthogonalVecToVelo);
		mat4 rotate4b = glm::rotate(mat4(1.0f), EventManager::GetRandomFloat(0.0f, 360.0f), newParticle->velocity);
		vec4 rotateVec = rotate4b* rotate4 *vec4(newParticle->velocity,0);
		newParticle->velocity = vec3( rotateVec.x, rotateVec.y, rotateVec.z);
       
        World::GetInstance()->AddBillboard(&newParticle->billboard);
    }
    
    
    for (std::list<Particle*>::iterator it = mParticleList.begin(); it != mParticleList.end(); )
    {
        Particle* p = *it;
		p->currentTime += dt;

        // @TODO 5 - Update each particle's parameter
        //
        // Update the velocity and position of the particle from the acceleration of the descriptor
        // Update the size of the particle according to its growth
        // Update The color is also updated in 3 phases
        //
        //
        // Phase 1 - Initial: from t = [0, fadeInTime] - Linearly interpolate between initial color and mid color
        // Phase 2 - Mid:     from t = [fadeInTime, lifeTime - fadeOutTime] - color is mid color
        // Phase 3 - End:     from t = [lifeTime - fadeOutTime, lifeTime]
                
        
        // ...
		p->velocity += mpDescriptor->acceleration * dt;
        
		p->billboard.position += p->velocity * dt;
		
		p->billboard.color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		
		if(p->currentTime < mpDescriptor->fadeInTime){
		float tNorm =  (p->currentTime)/( mpDescriptor->fadeInTime);
		p->billboard.color = glm::mix(mpDescriptor->initialColor,mpDescriptor->midColor,tNorm);
		}else if(p->currentTime < p->lifeTime - mpDescriptor->fadeOutTime){
			float tNorm =  (p->currentTime - mpDescriptor->fadeInTime)/(p->lifeTime - mpDescriptor->fadeOutTime - mpDescriptor->fadeInTime);
			p->billboard.color = mpDescriptor->midColor;
		}else{
				float tNorm =  (p->currentTime- (p->lifeTime - mpDescriptor->fadeOutTime - mpDescriptor->fadeInTime))/(mpDescriptor->fadeOutTime );
				p->billboard.color = glm::mix(mpDescriptor->midColor,mpDescriptor->endColor,tNorm);
		}
		
		p->billboard.size += 0.01f;
		p->billboard.size += mpDescriptor->sizeGrowthVelocity * dt ;
        // ...
        
        
        
        // Particles are destroyed if expired
        // Remove from the send particle to inactive list
        // Remove the billboard from the world
        if (p->currentTime > p->lifeTime)
        {
            mInactiveParticles.push_back(*it);
            
            World::GetInstance()->RemoveBillboard(&(*it)->billboard);
            it = mParticleList.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    

}
