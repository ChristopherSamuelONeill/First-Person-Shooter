#ifndef LEVEL_H
#define LEVEL_H

#include<stdafx.h>
#include"Camera.h"
#include"Pickable.h"
#include"tinyxml2.h"
#include"Robot.h"

class Level
{
private:
	void loadLevel(const char dir[]);


public:
	Level();
	Level(const char dir[]);
	~Level();

	//to keep track of how many pick ups
	int m_iNumPicks;
	int m_iPlayerPicks=0;

	//game objects
	vector<Camera*> m_LevelCameras;
	vector<Pickable*> m_LevelPickables;
	Robot *theRobot;
	vector<Model*> models;
	glm::vec3 m_lightPos;
};

#endif // !LEVEL_H
