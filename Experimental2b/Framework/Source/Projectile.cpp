//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Projectile.h"
#include "Renderer.h"
#include <GL/glew.h>
#include "Collision.h"
#include "World.h"
#include "Collision.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace glm;

Projectile::Projectile(vec3 Parent)
{
	time = 0;
	lifetime =5.0;
	vec3 size(1.0f);
	color = vec3 (1.0f, 0.05f, 0.05f); 
	SphereModel::SphereModel(size);
	aim = World::GetInstance()->mModel[3]->GetPosition() - Parent;
	aim = glm::normalize(aim);//vec3(1.0, 0.0, 0.0); //
}
Projectile::Projectile() 
{
	vec3 size(1.0f);
	color = vec3 (1.0f, 0.05f, 0.05f); 
	SphereModel* bullet = new SphereModel(vec3(1.0f));
	//vec3 aim = World::GetInstance()->mModel[3]->GetPosition() - World::GetInstance()->mModel[1]->GetPosition();
	aim = vec3(1.0, 0.0, 0.0); //glm::normalize(aim);
}
	
	

Projectile::~Projectile()
{
	bullet.~SphereModel();
}
void Projectile::ChangeColor(){
	if(color  == vec3(1.0f, 0.5f, 0.5f))
		color = vec3(0.0f, 1.0f, 1.0f);
	else
		color = vec3(1.0f, 0.5f, 0.5f);

//	for(std::vector<Vertex*>::iterator it = mVertexBuffer.begin(); it != mVertexBuffer.end(),i++){
//	}
}

void Projectile::Update(float dt)
{
	//for (list<Billboard*>::iterator it = mBillboardList.begin(); it != mBillboardList.end(); ++it)
	//vec3 travel = aim * dt;
	//mat4 translate4 = glm::translate(mat4(1.0f), travel);
	//vec4 tempPos = translate4*vec4(GetPosition(),1.0f) ;
	//SetPosition(vec3(tempPos.x,tempPos.y,tempPos.z));
	this->SetPosition(this->GetPosition()+(aim*dt));
	time+=dt;
	if(time >= lifetime){
		//Projectile::~Projectile();
	}
	bullet.Update(dt);
}

void Projectile::Draw()
{
    // Draw the Vertex Buffer
    // Note this draws a unit Cube
    // The Model View Projection transforms are computed in the Vertex Shader
   //bullet.Draw();
   SphereModel::Draw();
}

bool Projectile::ParseLine(const std::vector<ci_string> &token)
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