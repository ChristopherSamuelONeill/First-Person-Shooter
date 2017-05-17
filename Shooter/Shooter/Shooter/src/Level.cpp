#include <stdafx.h>
#include "Level.h"



void Level::loadLevel(const char dir[])
{
	
	tinyxml2::XMLDocument levelFile;
	levelFile.LoadFile(dir);
	
	for (tinyxml2::XMLNode* child = levelFile.FirstChild(); child != NULL; child = child->NextSibling())
	{
		const char* Value = child->Value();

		if (strcmp(Value, "Level" )== 0)
		{
			for (tinyxml2::XMLNode* child2 = child->FirstChild(); child2 != NULL; child2 = child2->NextSibling())
			{
				Value = child2->Value();
				
				if (strcmp(Value, "Scene") == 0)
				{
					for (tinyxml2::XMLNode* child3 = child2->FirstChild(); child3 != NULL; child3 = child3->NextSibling())
					{
						Value = child3->Value();
						if (strcmp(Value, "object") == 0)
						{
							string modelPath;
							string texturePath;
							for (tinyxml2::XMLNode* child4 = child3->FirstChild(); child4 != NULL; child4 = child4->NextSiblingElement())
							{
								Value = child4->Value();
								if (strcmp(Value, "modelPath") == 0)
								{
									modelPath = child4->ToElement()->GetText();
								}
								if (strcmp(Value, "texturePath") == 0)
								{
									texturePath = child4->ToElement()->GetText();
								}
								
							}
				
							models.push_back(new Model(modelPath, texturePath));

						}
					}
				}
				if (strcmp(Value, "Pickups") == 0)
				{
					for (tinyxml2::XMLNode* child3 = child2->FirstChild(); child3 != NULL; child3 = child3->NextSibling())
					{
						Value = child3->Value();
						if (strcmp(Value, "numb") == 0)
						{
							string num = child3->ToElement()->GetText();
							m_iNumPicks = std::stof(num);
						}
						if (strcmp(Value, "object") == 0)
						{
							string type;
							string score;
							float x;
							float y;
							float z;

							for (tinyxml2::XMLNode* child4 = child3->FirstChild(); child4 != NULL; child4 = child4->NextSiblingElement())
							{
								
								Value = child4->Value();
								if (strcmp(Value, "type") == 0)
								{
									type = child4->ToElement()->GetText();
								}
								if (strcmp(Value, "score") == 0)
								{
									score = child4->ToElement()->GetText();
								}
								if (strcmp(Value, "x") == 0)
								{
									
									string sx = child4->ToElement()->GetText();
									x = std::stof(sx);
								}
								if (strcmp(Value, "y") == 0)
								{
									string sy= child4->ToElement()->GetText();
									y = std::stof(sy);
								}
								if (strcmp(Value, "z") == 0)
								{
									string sz = child4->ToElement()->GetText();
									z = std::stof(sz);
								}

							}
							float s = std::stof(score);
							glm::vec3 pos(x, y, z);
							m_LevelPickables.push_back(new Pickable(s, pos, type));
							

						}
					}
				}
				if (strcmp(Value, "Robot") == 0)
				{
			
					float f;
					float r;
					float x;
					float y;
					float z;

					for (tinyxml2::XMLNode* child4 = child2->FirstChild(); child4 != NULL; child4 = child4->NextSiblingElement())
					{

						Value = child4->Value();
						if (strcmp(Value, "x") == 0)
						{
							string sx = child4->ToElement()->GetText();
							x = std::stof(sx);
						}
						if (strcmp(Value, "y") == 0)
						{
							string sy = child4->ToElement()->GetText();
							y = std::stof(sy);
						}
						if (strcmp(Value, "z") == 0)
						{
							string sz = child4->ToElement()->GetText();
							z = std::stof(sz);
						}
						if (strcmp(Value, "r") == 0)
						{
							string sr = child4->ToElement()->GetText();
							r = std::stof(sr);
						}
						if (strcmp(Value, "f") == 0)
						{
							string sf = child4->ToElement()->GetText();
							f = std::stof(sf);
						}

					}
						
					glm::vec3 pos(x, y, z);
					theRobot = new Robot(pos,r,f);
						
				}
				
				if (strcmp(Value, "Camera") == 0)
				{
					for (tinyxml2::XMLNode* child3 = child2->FirstChild(); child3 != NULL; child3 = child3->NextSibling())
					{
						Value = child3->Value();
						if (strcmp(Value, "cam") == 0)
						{
						
							float x;
							float y;
							float z;

							for (tinyxml2::XMLNode* child4 = child3->FirstChild(); child4 != NULL; child4 = child4->NextSiblingElement())
							{

								Value = child4->Value();
								if (strcmp(Value, "x") == 0)
								{
									string sx = child4->ToElement()->GetText();
									x = std::stof(sx);
								}
								if (strcmp(Value, "y") == 0)
								{
									string sy = child4->ToElement()->GetText();
									y = std::stof(sy);
								}
								if (strcmp(Value, "z") == 0)
								{
									string sz = child4->ToElement()->GetText();
									z = std::stof(sz);
								}
								

							}

							glm::vec3 pos(x, y, z);
							m_LevelCameras.push_back(new Camera(pos));
						}
					}
				}
				if (strcmp(Value, "Light") == 0)
				{
					float lx;
					float ly;
					float lz;

					for (tinyxml2::XMLNode* child3 = child2->FirstChild(); child3 != NULL; child3 = child3->NextSiblingElement())
					{

						Value = child3->Value();
						if (strcmp(Value, "x") == 0)
						{
							string sx = child3->ToElement()->GetText();
							lx = std::stof(sx);
						}
						if (strcmp(Value, "y") == 0)
						{
							string sy = child3->ToElement()->GetText();
							ly = std::stof(sy);
						}
						if (strcmp(Value, "z") == 0)
						{
							string sz = child3->ToElement()->GetText();
							lz = std::stof(sz);
						}
					}
					glm::vec3 pos(lx, ly, lz);
					m_lightPos = pos;
				}

			}
			
			
		}



	}


}

Level::Level()
{
}

Level::Level(const char dir[])
{
	m_iPlayerPicks = 0;
	theRobot = new Robot();
	loadLevel(dir);
}

Level::~Level()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->~Model();
		delete(models[i]);
		models[i] = NULL;
	}
	for (int i = 0; i < m_LevelCameras.size(); i++)
	{
		delete(m_LevelCameras[i]);
		m_LevelCameras[i] = NULL;
	}
	delete(theRobot);
	theRobot = NULL;

	for (int i = 0; i < m_LevelPickables.size(); i++)
	{
		delete(m_LevelPickables[i]);
		m_LevelPickables[i] = NULL;
	}


}

