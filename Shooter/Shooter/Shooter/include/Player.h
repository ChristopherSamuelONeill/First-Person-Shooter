#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "Camera.h"
#include "Armour.h"

#include <Model.h>

class Player
{
public:
	Player(); // <! Default Constructor
	Player(string model, glm::vec3 pos, glm::vec3 rot);

	//Camera's
	Camera m_FirstPersonCamera;
	Camera m_ThirdPersonCamera;
	bool m_bFirstPerson = true;

	//Weapons / Items


	//Armour
	Armour m_Helmet;


	//Vitality Variables
	float m_fHealth;
	float m_fBlood;
	int m_iNoWounds;

	//functions
	void update(float dt);
	void draw();

	//Positional Data
	glm::vec3 m_vPosition;
	glm::vec3 m_vRotation;

	void handleCamera(Window& window);
	void move(glm::vec3 dir);


private:

	void takeBulletDamageToArmour(float amount, Armour& armour);
	void takeMeleeDamageToArmour(float amount, Armour& armour);

	void takeBulletDamage(float amount);
	void takeMeleeDamage(float amount);

	void processVitals(float dt);
	
	
	Model* m_playerModel;


};

#endif