#ifndef LEVEL_H
#define LEVEL_H

#include <stdafx.h>
#include "Camera.h"
#include "Pickable.h"
#include "tinyxml2.h"
#include "Player.h"


class Level
{
private:
	void loadLevel(const char dir[]);


public:
	Level();
	Level(const char dir[]);
	~Level();

	void update(float dt);
	void draw();

	//game objects

	Player m_Player;

	Model* m_skyBox;
	vector<Model*> m_Ground;

	vector<Model*> m_Models;

	glm::vec3 m_lightPos;

};

#endif // !LEVEL_H
