#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Plane.h"
#include "BoundingBox.h"

class ENGINE_API FrustumG
{
private:

	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};

	static const unsigned int NUM_BUFFERS = 2;
	GLuint _vertexArrayObject;
	GLuint _vertexArrayBuffers[NUM_BUFFERS];

public:

	static enum { INSIDE, INTERSECT, OUTSIDE };
	Plane pl[6];
	glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle, tang;
	float nw, nh, fw, fh;

	FrustumG::FrustumG();
	FrustumG::~FrustumG();

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u);
	int pointInFrustum(glm::vec3 &p);
	//int sphereInFrustum(glm::vec3 &p, float raio);
	int boxInFrustum(const BoundingBox &b, Camera* c);

	void FrustumG::drawPoints(Shader* shader, Camera* camera);
	void FrustumG::drawLines(Shader* shader, Camera* camera);
	void FrustumG::drawPlanes();
	void FrustumG::drawNormals(Shader* shader, Camera* camera);
};

