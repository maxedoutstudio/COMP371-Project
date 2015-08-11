//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "Model.h"
//#include "Projectile.h"
#include "mesh.h"
#include "sceneLoader.h"
class HeartModel : public Model
{
public:
	HeartModel(int texture,glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~HeartModel();

//	Projectile bullet;
	virtual void Update(float dt);
	virtual void Draw();

	virtual void Spawn();
	virtual void Reset();
	int mTextureID;
	sceneLoader* heartScene;

	virtual void Collision();


protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int uvbuffer;
	unsigned int normalbuffer;
	unsigned int TextureID;
	unsigned int Texture;
};
