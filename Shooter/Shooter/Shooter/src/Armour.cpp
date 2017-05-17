#include "Armour.h"

Armour::Armour()
{
	m_fProtection = 0;
	m_fHitPoints = 0;
}

Armour::Armour(float hitPoints, float protection)
{
	m_fProtection = protection;
	m_fHitPoints = hitPoints;
}
