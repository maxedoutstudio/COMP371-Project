//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once


#include <glm/glm.hpp>
#include <vector>
#include <list>
#include "mesh.h"
#include "sceneLoader.h"
#include "Model.h"

// We should render billboards in the fewest amount of render calls possible
// Backgrounds are semi-transparent, so they need to be sorted and rendered from back to front
class Background : public Model
{
public:
	Background(int textureID, glm::vec3 mPosition = glm::vec3(0, 0, 0), glm::vec3 size = glm::vec3(1, 1, 1));
    virtual ~Background();

   
    
    virtual void Update(float dt);
    virtual void Draw();


	glm::vec3 mPosition;
	glm::vec3 mSize;

	int mTextureID;

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);
    
private:
    // Each vertex on a billboard
    struct BackgroundVertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    
	std::vector<BackgroundVertex> mVertexBuffer;
    


    unsigned int mVertexArrayID;
    unsigned int mVertexBufferID;
};