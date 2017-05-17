#ifndef __ROBOT_H
#define __ROBOT_H

#include "stdafx.h"
#include "glm\glm.hpp"

// constants for arm and leg movement states
const char BACKWARD_STATE = 0;
const char FORWARD_STATE  = 1;

// index constants for accessing arm and leg array data
const char LEFT  = 0;
const char RIGHT = 1;

class Robot
{
private:
	char legStates[2];	
	char armStates[2];

	float legAngles[2];
	float armAngles[2];


	// draws a unit cube
	void DrawCube(float xPos, float yPos, float zPos);

	// methods to draw the parts of the robot
	void DrawArm(float xPos, float yPos, float zPos);
	void DrawHead(float xPos, float yPos, float zPos);
	void DrawTorso(float xPos, float yPos, float zPos);
	void DrawLeg(float xPos, float yPos, float zPos);
	void DrawFoot(float xPos, float yPos, float zPos);

public:

	float m_fScore;
	float m_xPos;
	float m_yPos;
	float m_zPos;
	float rotationAngle;
	float speed;
	float rotationSpeed;
	float m_fFuel;
	
	float m_fRadius;

	void movefoward(int dir);

	glm::vec3 getPos();

	Robot();
	Robot(glm::vec3 pos, float r, float f);
	virtual ~Robot();

	// draws the entire robot
	void DrawRobot(float xPos, float yPos, float zPos,float rot);

	// updates the robot data
	void Prepare(float dt);
};

#endif