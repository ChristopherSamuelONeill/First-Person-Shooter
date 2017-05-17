#include <stdafx.h>
#include "Camera.h"

Camera::Camera()
{
	m_Position = glm::vec3(0, 0, 0);
	m_View =	 glm::vec3(0, 0, 0);
	m_Up =		 glm::vec3(0, 1, 0);
	m_screenWidth = 1280;
	m_screenHeight = 1024;


}
Camera::Camera(glm::vec3 pos)
{
	m_Position = pos;
	m_View = glm::vec3(0, 0, 0);
	m_Up = glm::vec3(0, 1, 0);
	m_screenWidth = 1280;
	m_screenHeight = 1024;
}
void Camera::point()
{

	gluLookAt(m_Position.x, m_Position.y, m_Position.z,
		m_View.x, m_View.y, m_View.z,
		m_Up.x, m_Up.y, m_Up.z);
}

