#include "ModelReader.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "Robot.h"
#include "Button.h"
#include "Pickable.h"
#include "Level.h"
#include <tinyxml2.h>

using namespace std;
using namespace sf;

//window specific data
const int SCREEN_WIDTH = 1280;		//width of window in pixels 
const int SCREEN_HEIGHT = 720;		//height of window in pexils
const float REFRESH_RATE = 0.016f;  //60 fps

//global functions
void drawRobot(); // contains opengl functions to render robot object
void loadLevel(); // uses calls level object to load xml files
void mainMenu();  // contains the main menu 
void gameOver(bool success); // game over screen , parameter is for if the player failed or won (displays differnt screen)
void Game();	// contains the main game loop
void drawHud(RenderWindow& windo); // draws any hud emelements and laoding screen 

//robot elements
bool g_bWalking = false; // true = animate , false = dont .used for controlling robot animation 
int g_iDirection; // direction the robot should be walking (translating user input)   
float g_fDelta;  // change in time between frames  used for animation

//Hud Elements
Text g_sfScoreText; // the players score text
int g_iScore ;      // value of players score
Text g_sfFuelText;  // the players fuel
int g_iFuel = 100;  // value of players fuel
Text g_sfLevelText; // the players current level
int g_iLevel = 1;   //  value of players level
Font g_sfHuntFont;  // font for the Hud

//loading screen elements
bool bLoading = false;  // true when the game is loading
Text loadingText;

//levels
Level *g_level; // pointer to object LEVEL
int g_iNumberOfLevels; // how many levels are avaible for game (uses XML loader)

bool g_bDEBUGMODE = true;


GLfloat g_lightPosition[] = {0.0f, +1000.0f, 0.0f, 1.0f }; // position of light in level


void SetMaterialRobot() // a bright and shiny material for the robot
{
	GLfloat materialWhiteAmbient[] = { 1.0f,1.0f,0.0f,1.0f};
	GLfloat materialWhiteDiffuse[] = { 1.0f,1.0f,0.0f,1.0f};
	GLfloat materialWhiteSpecular[] = {1.0f,1.0f,0.0f,1.0f };		// so keeps light colour
	GLfloat materialRedShininess = 50.0f;
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialRedShininess);
}

void SetMaterialNormal() // a nomal bland material for all level objects
{
	GLfloat materialWhiteAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat materialWhiteDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat materialWhiteSpecular[] = { 0.1f,0.1f, 0.1f, 1.0f };		// so keeps light colour
	GLfloat materialWhiteShininess = 0.0f;
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}

void SetMaterialSkyBox() // ambient light only 
{
	GLfloat materialRedAmbient[] = { 1, 1.0f, 1.0f, 1.0f };
	GLfloat materialRedDiffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat materialRedSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat materialRedShininess = 0.0f;						// try 500 for a sharper specular highlight
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialRedAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialRedDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialRedSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialRedShininess);
}


void SetLightPosition(float x, float y, float z) // reposition the light 
{
	g_lightPosition[0] = x;
	g_lightPosition[1] = y;
	g_lightPosition[2] = z;
}

void ConfigureLightSources(float x , float y , float z) // set up the lighting
{
	//set up colour of the light source
	GLfloat lightColour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightModelAmbient[] = { 0.7f, 0.7f, 0.7f, 1.0 };

	//love light
	SetLightPosition(x, y, z);

	//set up open gl lgithing
	glLightfv(GL_LIGHT0, GL_POSITION, g_lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColour);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light);			

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);	// use global ambient instead

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	SetMaterialNormal();
}

int main()
{
	//load the hud font
	if (!g_sfHuntFont.loadFromFile("Assets/fonts/ariblk.ttf"))
	{
		cout << "Error: Font ariblk.ttf was unable to load.";
	};
	//launch the main menu
	mainMenu();
	if (g_level != NULL)g_level ->~Level();
	return 0;
}

void drawRobot()
{
	float rot = 0.1; // angle to rotate by

	//disable culling on the robot as the normals are not set, also its not textured so render using the gl color
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	if (!g_bWalking) // if not walking
	{
		glPushMatrix();						
		glLoadIdentity();					
		g_level->theRobot->DrawRobot(0.0f, 0.0f, 0.0f, 0.0f);		
		glPopMatrix();
	}
	if (g_bWalking) // if walking
	{
		
		glPushMatrix();						
		glLoadIdentity();					
		switch (g_iDirection)
		{
			case 1: // turn right
				g_level->theRobot->DrawRobot(0.0f, 0.0f, 0.0f, rot * -1);	
				break;

			case -1: // turn left
				g_level->theRobot->DrawRobot(0.0f, 0.0f, 0.0f, rot * 1);	
				break;

			case 0: // go backwards
				g_level->theRobot->movefoward(-1);
				g_level->theRobot->DrawRobot(0.0f, 0.0f, 0.0f, 0.0f);		
				break;
			case 2: // go forwards
				g_level->theRobot->movefoward(1);
				g_level->theRobot->DrawRobot(0.0f, 0.0f, 0.0f, 0.0f);		
				break;
			default:
				g_level->theRobot->DrawRobot(0.0f, 0.0f, 0.0f, 0.0f);		
				break;


		}
		
		glPopMatrix();
		g_level->theRobot->Prepare(g_fDelta); // animate

	}
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
}

void loadLevel()
{
	if (g_bDEBUGMODE)
	{
		
		if (g_level != NULL)
		{
			g_level ->~Level();
			cout << "DECONSTRCTED" << endl;
			delete(g_level);
			g_level = NULL;

		}
		
		g_level = new Level("Assets/XML/ShooterTest.xml");
		g_level->theRobot->m_fFuel = 1000;

		
	}
	else
	{
		//find how many levels thereare
		tinyxml2::XMLDocument levelFile;
		levelFile.LoadFile("Assets/xml/LevelList.xml");
		for (tinyxml2::XMLNode* child = levelFile.FirstChild(); child != NULL; child = child->NextSibling())
		{
			const char* Value = child->Value();
			if (strcmp(Value, "Number") == 0)
			{
				string number = child->ToElement()->GetText();
				g_iNumberOfLevels = std::stof(number);
			}
		}



		switch (g_iLevel)
		{
			//create scene objects based on level

		case 1:
			if (g_level != NULL)
			{
				g_level ->~Level();
				cout << "DECONSTRCTED" << endl;
				delete(g_level);
				g_level = NULL;

			}
			g_iScore = 0;
			g_level = new Level("Assets/XML/Level_01.xml");
			g_level->theRobot->m_fScore = g_iScore;

			break;

		case 2:
			g_iScore = g_level->theRobot->m_fScore;
			if (g_level != NULL)
			{
				g_level ->~Level();
				cout << "DECONSTRCTED" << endl;
				delete(g_level);
				g_level = NULL;

			}
			g_level = new Level("Assets/XML/Level_02.xml");
			g_level->theRobot->m_fScore = g_iScore;

			break;

		case 3:
			g_iScore = g_level->theRobot->m_fScore;
			if (g_level != NULL)
			{
				g_level ->~Level();
				cout << "DECONSTRCTED" << endl;
				delete(g_level);
				g_level = NULL;

			}
			g_level = new Level("Assets/XML/Level_03.xml");
			g_level->theRobot->m_fScore = g_iScore;

			break;

		}
	}

	
	bLoading = false;
}

void mainMenu()
{
	sf::Texture sfPlashTexture;
	if (!sfPlashTexture.loadFromFile("Assets/textures/splashScreen.png"))
	{
		cout << "Error: splashScreen.png was unable to load.";
	};

	//main menu background-------------------------
	RectangleShape mainMenuBackgroundRect;
	Sprite mainMenuBackgroundSprite;

	mainMenuBackgroundRect.setPosition(0, 0);
	mainMenuBackgroundRect.setSize(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

	mainMenuBackgroundSprite.setTexture(sfPlashTexture);
	mainMenuBackgroundSprite.setScale(Vector2f(1, 1));
	mainMenuBackgroundSprite.setPosition(mainMenuBackgroundRect.getPosition());

	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Main Menu");
	
	Vector2f buttonSize(250 , 100 );
	Vector2f middleOfScreen((SCREEN_WIDTH / 2) - (buttonSize.x / 2), (SCREEN_HEIGHT / 2) - (buttonSize.y / 2));

	Button play("Play", Vector2f(middleOfScreen.x, 250 ), buttonSize, "Button_Green");
	Button quit("Quit", Vector2f(middleOfScreen.x, 550 ), buttonSize, "Button_Green");

	window.setFramerateLimit(60);


	while (window.isOpen())
	{
		Event event;
		Vector2f sfMousePos;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close(); // Allows window to close when 'X' is pressed
				return;
			}
			sfMousePos = window.mapPixelToCoords(Mouse::getPosition(window));
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					
					if (play.m_bClicked(sfMousePos))
					{
						bLoading = true;
						drawHud(window);
						window.display();
						loadLevel();
						window.close();
						Game();

					}
					if (quit.m_bClicked(sfMousePos))
					{
						window.close(); // Allows window to close when 'X' is pressed
						return;
					}
				}
			}
		}
		window.clear(Color::Green);
		window.draw(mainMenuBackgroundSprite);
		window.draw(play);
		window.draw(quit);
		
		window.display();
		
	}

}

void gameOver(bool success)
{
	sf::Texture sfPlashTexture;

	if (success)
	{
		if (!sfPlashTexture.loadFromFile("Assets/textures/gameOverWin.png"))
		{
			cout << "Error: gameOverWin.png was unable to load.";
		};
	}
	else
	{
		if (!sfPlashTexture.loadFromFile("Assets/textures/gameOverDead.png"))
		{
			cout << "Error: gameOverDead.png was unable to load.";
		};
	}

	//main menu background-------------------------
	RectangleShape mainMenuBackgroundRect;
	Sprite mainMenuBackgroundSprite;

	mainMenuBackgroundRect.setPosition(0, 0);
	mainMenuBackgroundRect.setSize(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

	mainMenuBackgroundSprite.setTexture(sfPlashTexture);
	mainMenuBackgroundSprite.setScale(Vector2f(1, 1));
	mainMenuBackgroundSprite.setPosition(mainMenuBackgroundRect.getPosition());

	//create the wondow
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Game Over");

	//create buttons
	Vector2f buttonSize(250, 100);
	Vector2f middleOfScreen((SCREEN_WIDTH / 2) - (buttonSize.x / 2), (SCREEN_HEIGHT / 2) - (buttonSize.y / 2));
	
	Button quit("Quit", Vector2f(SCREEN_WIDTH / 2 - 125, middleOfScreen.y), buttonSize, "Button_Green");

	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		Event event;
		Vector2f sfMousePos;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close(); // Allows window to close when 'X' is pressed
				return;
			}
			sfMousePos = window.mapPixelToCoords(Mouse::getPosition(window)); // get mouse pos

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					
					if (quit.m_bClicked(sfMousePos))// check if the quit button has been clicked
					{
						window.close(); 
						return;
					}
				}
			}
		}
		window.clear(Color::Green);
		window.draw(mainMenuBackgroundSprite);
		window.draw(quit);
		drawHud(window);
		window.display();

	}
}

void Game()
{

	// SFML-2.3.2 depth buffering fails unless we create a more specific window - as below
	int depthBits = 24;
	int stencilBits = 8;
	int antiAliasingLevel = 16;
	int majorVersion = 4;
	int minorVersion = 5;


	// Create the main window

	// Create the main window
	ContextSettings context(depthBits, stencilBits, antiAliasingLevel, majorVersion, minorVersion);
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "Robot Recharge", 7U, context);
	window.setMouseCursorVisible(false);
	// check we got the settings we need
	
	
	// Create a clock for measuring time elapsed
	Clock clock;
	int iCurrentCamera = 0;

	// always setup OpenGL After the window is configured
	// Set color and depth clear value
	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.f, 0.f);

	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// draw single sided faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.f, 5000000.f);

	g_iLevel = 1;

	//find difference in camera andplayer pos
	g_level->m_LevelCameras[iCurrentCamera]->m_offset = g_level->m_LevelCameras[iCurrentCamera]->m_Position - g_level->theRobot->getPos();


	// Start game loop
	while (window.isOpen())
	{
	
		
		// Process events
		Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit
			if (Event.type == Event::Closed)
				window.close();

			// Escape key : exit
			if ((Event.type == Event::KeyPressed) && (Event.key.code == Keyboard::Escape))
				window.close();

			// arrow keys for user input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			{
				g_bWalking = true;
				g_iDirection = 0;
				
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			{
				g_bWalking = true;
				g_iDirection = 2;
				
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			{
				g_bWalking = true;
				g_iDirection = -1;
			
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			{
				g_bWalking = true;
				g_iDirection = 1;
			
			}
			else // if no button pressed dont animate
			{
				g_bWalking = false;
			
			}

			//mouse buttons cycle cameras
			if (Event.type == sf::Event::MouseButtonPressed)
			{
				if (Event.mouseButton.button == sf::Mouse::Left)
				{
					
					iCurrentCamera++;
					if (iCurrentCamera > g_level->m_LevelCameras.size() - 1)
					{
						iCurrentCamera = 0;
					}

				}
				if (Event.mouseButton.button == sf::Mouse::Right)
				{
					iCurrentCamera--;
					if (iCurrentCamera < 0)
					{
						iCurrentCamera = g_level->m_LevelCameras.size() - 1;
					}
				}
			}



			// Resize event : adjust viewport
			if (Event.type == Event::Resized)
				glViewport(0, 0, Event.size.width, Event.size.height);

		}

		//centre the mouse after the user moved it so it doesnt apear
		Vector2i vWindowCentre(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		Mouse::setPosition(window.getPosition() + vWindowCentre);

		//get time elapsed
		g_fDelta = clock.getElapsedTime().asSeconds();
	
		if (clock.getElapsedTime().asSeconds()>REFRESH_RATE) // only draw and update objects on correct frequency
		{
			
			// clear screen and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
			glLoadIdentity();

			// setup cameras
			gluPerspective(52.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 1.0f, 5000000.0f);
			//aim camera at the player
			g_level->m_LevelCameras[iCurrentCamera]->m_View = g_level->theRobot->getPos();
			//move by offset
			g_level->m_LevelCameras[iCurrentCamera]->m_Position = g_level->theRobot->getPos() + g_level->m_LevelCameras[iCurrentCamera]->m_offset;
			g_level->m_LevelCameras[iCurrentCamera]->point();
			
			// set up lighting based on the position taken from the level
			ConfigureLightSources(g_level->m_lightPos.x, g_level->m_lightPos.y, g_level->m_lightPos.z);


			glm::vec3 v;
			glm::vec3 pos = g_level->m_LevelCameras[iCurrentCamera]->m_Position;
			glm::vec3 veiw = g_level->m_LevelCameras[iCurrentCamera]->m_View;


			// Apply some transformations
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		
			if (g_level->m_iPlayerPicks >= g_level->m_iNumPicks && !g_bDEBUGMODE)
			{
				//level completed

				//display loading screen
				bLoading = true;
				window.pushGLStates();
				drawHud(window);
				window.popGLStates();
				window.display();

				//load next level
				g_iLevel++;
				g_level->m_iPlayerPicks = 0;
				g_level->theRobot->m_fScore += g_level->theRobot->m_fFuel;
				loadLevel();
				iCurrentCamera = 0;
			}

			//draw stuff---------------------------------------------------------------------------
			//Scene
			for (int i = 0; i < g_level->models.size(); i++) // loop scene models and render
			{
				glPushMatrix();

				glLoadIdentity();
				if (i == 0) // first model in scene is skybox
				{
					SetMaterialSkyBox();
				}
				else
				{
					SetMaterialNormal();
				}
		
				g_level->models[i]->DrawUsingVBO(1, 1);
				glPopMatrix();
			}
			
			
			if (g_level->theRobot->m_fFuel <= 0 && !g_bDEBUGMODE) // if the player runs out of fuel
			{
				
				gameOver(false);
				window.close();
			}
			if (g_iLevel > g_iNumberOfLevels && !g_bDEBUGMODE) // if the player has completed all the levels
			{
				
				
				g_iLevel = 3;
				gameOver(true);
				window.close();
				
			}
			
			//draw robot
			drawRobot();
			
			//draw picks
			for (int i = 0; i < g_level->m_LevelPickables.size(); i++)
			{
				
				glPushMatrix();
				glLoadIdentity();
				SetMaterialNormal();
				if (g_level->m_LevelPickables[i]->m_bOverlapped(g_level->theRobot))
				{
					g_level->m_iPlayerPicks++;
				}

				if (g_level->m_LevelPickables[i]->m_bPicked == false)
				{
					g_level->m_LevelPickables[i]->animate();
					g_level->m_LevelPickables[i]->m_model->DrawUsingVBO(1, 1);
				}
				glPopMatrix();
			}
		
			
			window.pushGLStates(); // enable SFML rendering
			drawHud(window);		// draw hud
			window.resetGLStates();
			window.popGLStates(); // return to OPEN GL

			clock.restart();
			window.display();
		}


	}
	
}

void drawHud(RenderWindow& window)
{


	if (bLoading) // loading screen
	{

		loadingText.setFont(g_sfHuntFont);
		loadingText.setString("LOADING....");
		loadingText.setCharacterSize(60);
		loadingText.setFillColor(Color(255, 255, 1));
		loadingText.setPosition(475, 360);

		window.draw(loadingText);
	}
	else // draw hud 
	{
		g_iFuel = g_level->theRobot->m_fFuel;

		g_iFuel = g_level->theRobot->m_fFuel;
		g_sfFuelText.setFont(g_sfHuntFont);
		g_sfFuelText.setString("Fuel : " + to_string(g_iFuel));
		g_sfFuelText.setCharacterSize(36);
		g_sfFuelText.setFillColor(Color(255, 255, 1));
		g_sfFuelText.setPosition(0, 0);


		g_iScore = g_level->theRobot->m_fScore;
		g_sfScoreText.setFont(g_sfHuntFont);
		g_sfScoreText.setString("Score : " + to_string(g_iScore));
		g_sfScoreText.setCharacterSize(36);
		g_sfScoreText.setFillColor(Color(255, 255, 1));
		g_sfScoreText.setPosition(525, 0);



		g_sfLevelText.setFont(g_sfHuntFont);
		g_sfLevelText.setString("Level : " + to_string(g_iLevel));
		g_sfLevelText.setCharacterSize(36);
		g_sfLevelText.setFillColor(Color(255, 255, 1));
		g_sfLevelText.setPosition(1100, 0);


		window.draw(g_sfScoreText);
		window.draw(g_sfLevelText);
		window.draw(g_sfFuelText);
	}

}
