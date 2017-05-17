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
				
							m_Models.push_back(new Model(modelPath, texturePath));

						}
						if (strcmp(Value, "ground") == 0)
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

							m_Ground.push_back(new Model(modelPath, texturePath));

						}
						if (strcmp(Value, "skybox") == 0)
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

							m_skyBox = new Model(modelPath, texturePath);

						}
					}
				}
				if (strcmp(Value, "Player") == 0)
				{
					glm::vec3 Position;
					glm::vec3 Rotation;
					string model;
				
					for (tinyxml2::XMLNode* child4 = child2->FirstChild(); child4 != NULL; child4 = child4->NextSiblingElement())
					{

						Value = child4->Value();
						if (strcmp(Value, "Position") == 0)
						{
							float x;
							float y;
							float z;

							for (tinyxml2::XMLNode* pos = child4->FirstChild(); pos != NULL; pos = pos->NextSiblingElement())
							{
								Value = pos->Value();
								if (strcmp(Value, "x") == 0)
								{
									string sy = pos->ToElement()->GetText();
									y = std::stof(sy);
								}
								if (strcmp(Value, "y") == 0)
								{
									string sy = pos->ToElement()->GetText();
									y = std::stof(sy);
								}
								if (strcmp(Value, "z") == 0)
								{
									string sz = pos->ToElement()->GetText();
									z = std::stof(sz);
								}

							}

							Position = glm::vec3(x, y, z);


						}
						if (strcmp(Value, "Rotation") == 0)
						{
							float x;
							float y;
							float z;

							for (tinyxml2::XMLNode* rot = child4->FirstChild(); rot != NULL; rot = rot->NextSiblingElement())
							{
								Value = rot->Value();
								if (strcmp(Value, "x") == 0)
								{
									string sy = rot->ToElement()->GetText();
									y = std::stof(sy);
								}
								if (strcmp(Value, "y") == 0)
								{
									string sy = rot->ToElement()->GetText();
									y = std::stof(sy);
								}
								if (strcmp(Value, "z") == 0)
								{
									string sz = rot->ToElement()->GetText();
									z = std::stof(sz);
								}

							}

							Rotation = glm::vec3(x, y, z);


						}
						if (strcmp(Value, "Model") == 0)
						{

							model = child4->ToElement()->GetText();
						}
						
					}
			
					m_Player = Player(model, Position, Rotation);
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
	
	loadLevel(dir);
}

Level::~Level()
{
	for (int i = 0; i < m_Models.size(); i++)
	{
		m_Models[i]->~Model();
		delete(m_Models[i]);
		m_Models[i] = NULL;
	}
	for (int i = 0; i < m_Ground.size(); i++)
	{
		m_Ground[i]->~Model();
		delete(m_Ground[i]);
		m_Ground[i] = NULL;
	}
	m_skyBox->~Model();
	delete(m_skyBox);
	m_skyBox = NULL;
	


}

void Level::update(float dt)
{
	m_Player.update(dt);
}

void Level::draw()
{
	//m_Player.draw();
	
	for (int i = 0 ;i < m_Ground.size(); i++)
	{
		glPushMatrix();
		glLoadIdentity();
			m_Ground[i]->DrawUsingVBO(1, 1);
		glPopMatrix();
	}
	glPushMatrix();
	glLoadIdentity();
		m_skyBox->DrawUsingVBO(1, 1);
	glPopMatrix();
}

