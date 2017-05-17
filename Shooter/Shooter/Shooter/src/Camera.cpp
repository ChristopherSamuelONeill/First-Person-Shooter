#include <stdafx.h>
#include "Camera.h"

Camera::Camera()
{
	m_Position = glm::vec3(150, 3, 1);
	m_View = glm::vec3(150, 3, 0);
	m_Up = glm::vec3(0, 1, 0);
	m_screenWidth = 1280;
	m_screenHeight = 720;


}

void Camera::handleInput(Window& window)
{
	float fSpeed = 1.0f / 1000.0f;
	float fMoveSpeed = 0;
	Vector2i vMousePos = Mouse::getPosition();
	float xViewAngle = 0.0f;
	float yViewAngle = 0.0f;
	Vector3f movement(0.0f, 0.0f, 0.0f);
	
	Vector2i vWindowCentre(m_screenWidth / 2, m_screenHeight / 2);
	vWindowCentre += window.getPosition();

	xViewAngle = (vWindowCentre - vMousePos).x * fSpeed; //calculate difference in mouse's position from the middle of the screen
	yViewAngle = (vWindowCentre - vMousePos).y * fSpeed;

	glm::vec3 viewVec = m_View - m_Position; //vector of view

											//rotate the camera in terms of mouse displacement
	m_View.x = m_Position.x + cosf(-xViewAngle) * viewVec.x - sinf(-xViewAngle) * viewVec.z;
	m_View.y += yViewAngle * 2.0f;
	m_View.z = m_Position.z + sinf(-xViewAngle) * viewVec.x + cosf(-xViewAngle) * viewVec.z;

	Mouse::setPosition(vWindowCentre);


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

