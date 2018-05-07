#include "stdafx.h"
#include "FrustumG.h"
#include "Camera.h"
#include "Shader.h"


FrustumG::FrustumG()
{
}


FrustumG::~FrustumG()
{
	glDeleteBuffers(NUM_BUFFERS, _vertexArrayBuffers);
	glDeleteVertexArrays(1, &_vertexArrayObject);
}

void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD)
{
	// store the information
	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	// compute width and height of the near and far plane sections
	tang = (float)tan(angle * 0.5f);
	nh = nearD * tang;
	nw = nh * ratio;
	fh = farD * tang;
	fw = fh * ratio;
}

void FrustumG::setCamDef(glm::vec3 & p, glm::vec3 & l, glm::vec3 & u)
{
	glm::vec3 dir, nc, fc, X, Y, Z;

	// compute the Z axis of camera
	// this axis points in the opposite direction from
	// the looking direction
	Z = glm::normalize(p - l);

	// X axis of camera with given "up" vector and Z axis
	X = glm::normalize(glm::cross(u, Z));

	// the real "up" vector is the cross product of Z and X
	Y = glm::cross(Z, X);

	// compute the centers of the near and far planes
	nc = p - Z * nearD;
	fc = p - Z * farD;

	// compute the 4 corners of the frustum on the near plane
	ntl = nc + (Y * nh) - (X * nw);
	ntr = nc + (Y * nh) + (X * nw);
	nbl = nc - (Y * nh) - (X * nw);
	nbr = nc - (Y * nh) + (X * nw);

	// compute the 4 corners of the frustum on the far plane
	ftl = fc + (Y * fh) - (X * fw);
	ftr = fc + (Y * fh) + (X * fw);
	fbl = fc - (Y * fh) - (X * fw);
	fbr = fc - (Y * fh) + (X * fw);

	// compute the six planes
	// the function set3Points assumes that the points
	// are given in counter clockwise order
	pl[TOP].set3Points(ntr, ntl, ftl);
	pl[BOTTOM].set3Points(nbl, nbr, fbr);
	pl[LEFT].set3Points(ntl, nbl, fbl);
	pl[RIGHT].set3Points(nbr, ntr, fbr);
	pl[NEARP].set3Points(ntl, ntr, nbr);
	pl[FARP].set3Points(ftr, ftl, fbl);
}

int FrustumG::pointInFrustum(glm::vec3 & p)
{
	int result = INSIDE;
	for (int i = 0; i < 6; i++) {

		if (pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return(result);
}

int FrustumG::boxInFrustum(const BoundingBox & b, Camera* c)
{
	unsigned int Out, In;
	int res = INSIDE;
	// for each plane do ...
	for (size_t i = 0; i < 6; i++)
	{
		// reset counters for corners in and out
		Out = In = 0;
		// for each corner of the box do ...
		// get out of the cycle as soon as a box as corners
		// both inside and out of the frustum
		for (size_t k = 0; k < 8 && (In == 0 || Out == 0); k++)
		{
			// is the corner outside or inside
			glm::vec3 vert = b.vertices[k];
			if (pl[i].distance(vert) < 0)
				Out++;
			else
				In++;
		}
		//if all corners are out
		if (In == 0)
			return OUTSIDE;
		// if some corners are out and others are in
		else if (Out > 0)
			res = INTERSECT;
	}
	return(res);
}

void FrustumG::drawPoints(Shader* shader, Camera* camera){
	const glm::vec3 verts[8] = {
		ntl,
		ntr,
		nbl,
		nbr,
		ftl,
		ftr,
		fbl,
		fbr,
	};
	const std::vector<unsigned int> indices = {
		0, 1, 2, 3,
		4, 5, 6, 7,
	};

	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, _vertexArrayBuffers);

	//positions buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//indexes buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	shader->use();

	shader->setMat4("model", glm::mat4(1));
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectionMatrix());

	glBindVertexArray(_vertexArrayObject);

	glPointSize(50);
	glDrawElements(GL_POINTS, 8, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glDeleteBuffers(NUM_BUFFERS, _vertexArrayBuffers);
	glDeleteVertexArrays(1, &_vertexArrayObject);
}

void FrustumG::drawLines(Shader* shader, Camera* camera)
{
	const glm::vec3 verts[8] = {
		ntl,
		ntr,
		nbl,
		nbr,
		ftl,
		ftr,
		fbl,
		fbr
	};
	const std::vector<unsigned int> indices = {
		//near
		0, 1, 3, 2,
		//far
		5, 4, 6, 7,
		//bottom
		2, 3, 7, 6,
		//top
		1, 0, 4, 5,
		//right
		3, 1, 5, 7,
		//left
		0, 2, 6, 4
	};
	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, _vertexArrayBuffers);

	//positions buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//indexes buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	shader->use();

	shader->setMat4("model", glm::mat4(1));
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectionMatrix());

	glBindVertexArray(_vertexArrayObject);

	glLineWidth(1);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(12 * sizeof(GLuint)));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(16 * sizeof(GLuint)));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(20 * sizeof(GLuint)));

	glBindVertexArray(0);

	glDeleteBuffers(NUM_BUFFERS, _vertexArrayBuffers);
	glDeleteVertexArrays(1, &_vertexArrayObject);
}

void FrustumG::drawPlanes()
{
}

void FrustumG::drawNormals(Shader* shader, Camera* camera)
{
	glm::vec3 a, b, c, d, e, f, g, h, i, j, k, l;
	// near
	a = (ntr + ntl + nbr + nbl) * 0.25f;
	b = a + pl[NEARP].normal;

	// far
	c = (ftr + ftl + fbr + fbl) * 0.25f;
	d = c + pl[FARP].normal;

	// left
	e = (ftl + fbl + nbl + ntl) * 0.25f;
	f = e + pl[LEFT].normal;

	// right
	g = (ftr + nbr + fbr + ntr) * 0.25f;
	h = g + pl[RIGHT].normal;

	// top
	i = (ftr + ftl + ntr + ntl) * 0.25f;
	j = i + pl[TOP].normal;

	// bottom
	k = (fbr + fbl + nbr + nbl) * 0.25f;
	l = k + pl[BOTTOM].normal;

	const glm::vec3 verts[12] = {
		a,b, c,d, e,f, g,h, i,j, k,l
	};

	const std::vector<unsigned int> indices = {
		0, 1, 
		2, 3,
		4, 5, 
		6, 7,
		8, 9,
		10, 11
	};;

	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, _vertexArrayBuffers);

	//positions buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//indexes buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	shader->use();

	shader->setMat4("model", glm::mat4(1));
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectionMatrix());

	glBindVertexArray(_vertexArrayObject);

	glLineWidth(1);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (GLvoid*)(2 * sizeof(GLuint)));
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (GLvoid*)(6 * sizeof(GLuint)));
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (GLvoid*)(10 * sizeof(GLuint)));

	glBindVertexArray(0);

	glDeleteBuffers(NUM_BUFFERS, _vertexArrayBuffers);
	glDeleteVertexArrays(1, &_vertexArrayObject);
}
