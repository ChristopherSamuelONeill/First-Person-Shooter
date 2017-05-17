#ifndef PICKABLE_H
#define PICKABLE_H

//#include "stdafx.h"
#include "./glm/glm.hpp"
#include "Robot.h"
#include <math.h>   
#include <iostream>
#include <Model.h>

using namespace std;


class Pickable
{
private:

	int m_iPoints;
	float m_fFadius;
	glm::vec3 m_v3Pos;
	glm::vec3 m_v3Size;
	float m_fMove;
	bool m_bUpwards = true;


public:
	Pickable(); // Default Constructor
	Pickable(int score, glm::vec3 pos,string objectType); // Overloaded Constructor setting the score of the object

	bool m_bOverlapped(Robot* robot);
	void animate();
	Model* m_model;
	bool m_bPicked;

};

#endif