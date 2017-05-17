#ifndef ARMOUR_H
#define ARMOUR_H

#include "stdafx.h"


class Armour
{
public:
	Armour(); // <! Default Constructor
	Armour(float hitPoints, float protection);

	float m_fProtection; // <! Damge the protection absorbs
	float m_fHitPoints; // <! How much damage the armour can take before breaking

	//Collision Stuff



};

#endif