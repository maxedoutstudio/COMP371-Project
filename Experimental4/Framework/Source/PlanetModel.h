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
#include "mesh.h"
#include "sceneLoader.h"
class PlanetModel : public Model
{
public:
	//PlanetModel(int texture, bool rotSelf = false, bool rotOrbit = false, vec4 surfaceProperties = vec4(0.2f, 0.8f, 0.2f, 90.0f));
	PlanetModel(int texture, bool rotSelf = false, bool rotOrbit = false, glm::vec4 surfaceProperties = glm::vec4(0.2f, 0.8f, 0.2f, 90.0f), glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~PlanetModel();

	virtual void Update(float dt);
	virtual void Draw();
	int mTextureID;
	sceneLoader* planetScene;

	bool selfRotation;
	bool orbitRotation;

	float ka;
	float kd;
	float ks;
	float n;

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
