#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"
#include "glm\glm.hpp"

using namespace sf;

class Camera
{
private:
	
	int m_screenWidth; 
	int m_screenHeight;

public:
	Camera();
	Camera(glm::vec3 pos);
	glm::vec3 m_Position;
	glm::vec3 m_View;
	glm::vec3 m_Up;

	glm::vec3 m_offset;

	void point();


};

#endif // !CAMERA_H
