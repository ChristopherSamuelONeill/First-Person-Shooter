#include <stdafx.h>
#include "Pickable.h"

Pickable::Pickable()
{
	m_iPoints = 0;
	m_fMove = 0;
	m_bPicked = false;
}

Pickable::Pickable(int score, glm::vec3 pos, string objectType)
{
	m_v3Pos = pos;
	m_iPoints = score;
	m_fMove = 0;
	m_bPicked = false;
	if (objectType == "battery")
	{
		m_model = new Model("Assets/models/Battery.obj", "Assets/images/Battery.bmp");
	}
	m_model->tanslate(pos, true);
}



bool Pickable::m_bOverlapped(Robot* robot)
{
	glm::vec3 dist = robot->getPos() - m_v3Pos;
	float mag = (dist.x * dist.x) + (dist.y * dist.y) + (dist.z * dist.z);
	mag = std::sqrt(mag);

	if ( mag <  robot->m_fRadius && m_bPicked == false)
	{
		m_bPicked = true;
		robot->m_fScore += m_iPoints;
		robot->m_fFuel += m_iPoints;
		return true;
	}
	else
	{
		return false;
	}
	
}

void Pickable::animate()
{
	
	if (m_fMove >= 0.25) m_bUpwards = false;
	if (m_fMove <= -0.25) m_bUpwards = true;

	if (m_bUpwards == true)
	{
		m_fMove += 0.01f;
	}
	else if (m_bUpwards == false)
	{
		m_fMove -= 0.01f;
	}
	

	m_model->rotate(glm::vec3(0,1,0), false);
	m_model->tanslate(glm::vec3(0, m_fMove,0),false);
}
