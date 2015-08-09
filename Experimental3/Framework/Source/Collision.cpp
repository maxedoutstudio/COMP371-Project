#include "Collision.h"
#include <glm/common.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;
bool sphereToSphere(glm::vec3 s1_Point, float s1_Radius, glm::vec3 s2_Point, float s2_Radius ){
	float diffrence = distance(s1_Point, s2_Point);
	float radiusSum  = s1_Radius + s2_Radius;
	return (diffrence < radiusSum);
};
bool rsphereToSphere(glm::vec3 s1_Point, float s1_Radius, glm::vec3 s2_Point, float s2_Radius, float theta, glm::vec3 normal){
	bool flag = sphereToSphere(s1_Point,  s1_Radius, s2_Point, s2_Radius );
	vec3 nDistance = normalize(s2_Point - s1_Point);
	return (dot(nDistance, normal) < theta);
};