#include "Player.h"

Player::Player()
{
	m_fHealth = 100;
	m_fBlood = 10000;
	m_iNoWounds = 0;
	
}

Player::Player(string model, glm::vec3 pos, glm::vec3 rot)
{

	if (model == "player")
	{
		m_playerModel = new Model("Assets/models/Battery.obj", "Assets/images/Battery.bmp");
	}
	m_fHealth = 100;
	m_fBlood = 10000;
	m_iNoWounds = 0;
}

void Player::update(float dt)
{
	processVitals(dt);
}

void Player::draw()
{
	m_playerModel->DrawUsingVBO(1, 1);
}



void Player::handleCamera(Window & window)
{
	if (m_bFirstPerson)
	{
		m_FirstPersonCamera.handleInput(window);
		m_FirstPersonCamera.point();
	}
}

void Player::move(glm::vec3 dir)
{


	m_playerModel->tanslate(m_vPosition, true);
}

void Player::takeBulletDamageToArmour(float amount, Armour& armour)
{
	
	if (armour.m_fHitPoints > 0) // armour is there to be used
	{
		//deplete the armours hit points
		armour.m_fHitPoints -= amount;
		if (armour.m_fHitPoints <= 0)
		{
			//the armour was wasted , do less dmage
			m_fHealth -= armour.m_fHitPoints;
			m_iNoWounds++;
		}
		else
		{
			if (amount - armour.m_fProtection <= 0) m_fHealth -= 1;
			else m_fHealth -= amount - armour.m_fProtection;
		}
		

	}
	else // take full damage
	{
		m_fHealth -= amount;
		m_iNoWounds++;
	}
	

}

void Player::takeMeleeDamageToArmour(float amount, Armour & armour)
{
	if (armour.m_fHitPoints > 0) // armour is there to be used
	{
		//deplete the armours hit points
		armour.m_fHitPoints -= amount;
		if (armour.m_fHitPoints <= 0)
		{
			//the armour was wasted , do less dmage
			m_fHealth -= armour.m_fHitPoints;
		}
		else
		{
			if (amount - armour.m_fProtection <= 0) m_fHealth -= 1;
			else m_fHealth -= amount - armour.m_fProtection;
		}


	}
	else // take full damage
	{
		m_fHealth -= amount;
	}

}

void Player::takeBulletDamage(float amount)
{
	m_iNoWounds++;
	m_fBlood -= amount;
	m_fHealth -= amount;
}

void Player::takeMeleeDamage(float amount)
{
	m_fHealth -= amount;
}

void Player::processVitals(float dt)
{
	

	//check if the replenish
	if (m_iNoWounds == 0)
	{
		m_fBlood += 0.1 * dt;
	}
	else if (m_iNoWounds > 0)
	{
		//loose blood
		m_fBlood -= m_iNoWounds * dt;
	}

	// check blood levels for health regen or loss

	//quick heal
	if (m_iNoWounds == 0 && m_fBlood > 7500)m_fHealth += 0.1 * dt;
	//slow heal
	else if (m_iNoWounds == 0 && m_fBlood > 5000)m_fHealth += 0.01 * dt;
	//no change
	else if (m_iNoWounds == 0 && m_fBlood <= 5000)m_fHealth = m_fHealth;
	//slow die
	else if (m_fBlood <= 10000 && m_fBlood > 5000 && m_iNoWounds > 0) m_fHealth -= 0.05 * dt;
	//moderate die
	else if (m_fBlood <= 5000 && m_fBlood > 2500 && m_iNoWounds > 0) m_fHealth -= 0.01 * dt;
	//fast die
	else if (m_fBlood <= 2500 && m_fBlood > 0 && m_iNoWounds > 0) m_fHealth -= 0.1 * dt;
	//bleed out
	else if (m_fBlood <= 0) m_fHealth--;

	//ensure limits
	if (m_fBlood >= 10000)m_fBlood = 10000;
	if (m_fHealth >= 100)m_fHealth = 100;
	if (m_iNoWounds <= 0)m_iNoWounds = 0;

	if (m_fBlood < 0 )m_fBlood = 0;
	if (m_fHealth < 0)m_fHealth = 0;
	if (m_iNoWounds <= 0)m_iNoWounds = 0;

	//check if dead
	if (m_fHealth <= 0); // do death stuff


}
