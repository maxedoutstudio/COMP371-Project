//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//         with help from Jordan Rutty
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "ParticleEmitter.h"
#include "Model.h"

using namespace glm;

ParticleEmitter::ParticleEmitter(glm::vec3 position, const Model* parent)
: mpParent(parent), mPosition(position)
{
}


// This is a point emitter, nothing is random
// As a small extra task, you could create derived classes
// for more interesting emitters such as line emitters or circular emitters
// In these cases, you would sample a random point on these shapes
glm::vec3 ParticleEmitter::GetRandomPosition()
{
     //@TODO 7 - Position from Parented Emitter
    
     //Return the position where the particle is emitted.
     //If the emitter is parented, the position is relative to its parent
	if(mpParent){
		mat4 parentWorldMatrix = mpParent->GetWorldMatrix();
		glm::vec3 position = vec3(parentWorldMatrix[3][0],parentWorldMatrix[3][1],parentWorldMatrix[3][2]);
		//glm::vec3 position = mPosition;
		return position;
	}else{
    glm::vec3 position = mPosition;
	
    
    return position;
	}
}




