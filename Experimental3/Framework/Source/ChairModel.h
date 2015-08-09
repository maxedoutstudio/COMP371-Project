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
#include "Projectile.h"

class ChairModel : public Model
{
public:
	ChairModel(int texture);
	virtual ~ChairModel();
	Projectile* bullet;
	virtual void Update(float dt);
	virtual void Draw();
	int mTextureID;
	

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
