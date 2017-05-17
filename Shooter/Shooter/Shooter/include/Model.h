#ifndef MODEL_H
#define MODEL_H


#include "stdafx.h"
#include "ModelReader.h"
#include "TextureLoader.h"
#include <iostream>
#include "glm\glm.hpp"

class Model
{
public:

	Model();
	~Model();

	Model(string objectPath, string texturePath);

	void DrawUsingVBO(bool drawWithNormals, bool drawWithTexture);
	GLuint m_textureID;

	void tanslate(glm::vec3 translation, bool set);
	void rotate(float amount, bool set);
	

private:

	ModelReader* m_mr;
	TextureLoader* m_tl;

	glm::vec3 m_position;
	float m_Rotation;
	float spin;

};
#endif
