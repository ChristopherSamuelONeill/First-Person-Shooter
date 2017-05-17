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
void mainMenu();  // contains the main menu 
void gameOver(bool success); // game over screen , parameter is for if the player failed or won (displays differnt screen)
void Game();	// contains the main game loop
void drawHud(RenderWindow& window); // draws any hud emelements and laoding screen 

//loading screen elements
bool bLoading = false;  // true when the game is loading
Text loadingText;
Font g_sfHuntFont;  // font for the Hud


//levels
Level *g_level; // pointer to object LEVEL

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
	if (g_bDEBUGMODE)
	{
		Game();
	}
	else
	{
		//load the hud font
		if (!g_sfHuntFont.loadFromFile("Assets/fonts/ariblk.ttf"))
		{
			cout << "Error: Font ariblk.ttf was unable to load.";
		};
		//launch the main menu
		mainMenu();
		
	}

	if (g_level != NULL)g_level ->~Level();
	return 0;
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
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "First Person Shooter", 7U, context);
	window.setMouseCursorVisible(false);
	// check we got the settings we need
	
	
	// Create a clock for measuring time elapsed
	Clock clock;
	Time time;

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


	if (g_bDEBUGMODE)
	{
		if (g_level != NULL)
		{
			g_level ->~Level();
			cout << "DECONSTRCTED" << endl;
			delete(g_level);
			g_level = NULL;

		}
		g_level = new Level("./Assets/XML/ShooterTest.xml");
	}
	else
	{
		//decide level
	}
	
	bLoading = false;
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
			{
				g_level ->~Level();
				cout << "DECONSTRCTED" << endl;
				delete(g_level);
				g_level = NULL;
				window.close();
				mainMenu();
			}
				

			float fMoveSpeed = 10;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
			{
				fMoveSpeed *= 3;
			}
			//key presses
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			{
				
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				
			}

			// Resize event : adjust viewport
			if (Event.type == Event::Resized)
				glViewport(0, 0, Event.size.width, Event.size.height);

		}

		//forces gameplay at the desired frame rate
		if (clock.getElapsedTime().asSeconds() >= REFRESH_RATE) // only draw and update objects on correct frequency
		{
			
			// clear screen and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
			glLoadIdentity();

			// setup cameras
			gluPerspective(52.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 5000000.0f);
			g_level->m_Player.handleCamera(window);
		

			// set up lighting based on the position taken from the level
			ConfigureLightSources(g_level->m_lightPos.x, g_level->m_lightPos.y, g_level->m_lightPos.z);

			// Apply transformations
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			//update stuff -----------------------------------------------
			g_level->update(clock.getElapsedTime().asSeconds());


			//draw stuff---------------------------------------------------------------------------
		
			SetMaterialNormal();
			g_level->draw();

			
			
			
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
	
}
