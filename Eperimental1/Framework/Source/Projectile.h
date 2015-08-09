#pragma once

#include "SphereModel.h"
#include <list>

class Projectile : public Model
{
public:
	Projectile(Model* Parent);
    virtual ~Projectile(void);
	SphereModel bullet;
	glm::vec3 aim;
	glm::vec3 color;
	Model* mParent;
	void Projectile::ChangeColor();
    virtual void Update(float dt);
    virtual void Draw();
    
protected:
    virtual bool ParseLine(const std::vector<ci_string> &token);

private:
    // The vertex format could be different for different types of models
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
    };
	std::vector<Vertex> mVertexBuffer;

    unsigned int mVertexArrayID;
    unsigned int mVertexBufferID;
    unsigned int numOfVertices;
};
