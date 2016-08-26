#include "Application.h"
#include "Controls.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneSummoner.h"
#include "SceneGolem.h"
#include "SceneSnakeBoss.h"
#include "SceneDetlaff.h"
#include "MainMenu.h"

#include "MeshManager.h"

GLFWwindow* m_window;
int Application::m_window_width = 800;
int Application::m_window_height = 600;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0,
Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	Application::GetInstance().SetWindowWidth(w);
	Application::GetInstance().SetWindowHeight(h);
}

void mouseWheel_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Controls::GetInstance().mouse_ScrollX = xoffset;
	Controls::GetInstance().mouse_ScrollY = yoffset;
}

bool IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}

void Application::GetMouseUpdate()
{
	//Handles mouse movements
	{
		glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

		// Calculate the difference in positions
		mouse_diff_x = mouse_current_x - mouse_last_x;
		mouse_diff_y = mouse_current_y - mouse_last_y;

		//Calculate the yaw and pitch
		Controls::GetInstance().mouse_xOffset = Math::DegreeToRadian((float)mouse_diff_x);
		Controls::GetInstance().mouse_yOffset = Math::DegreeToRadian((float)mouse_diff_y);

		// Do a wraparound if the mouse cursor has gone out of the deadzone
		//if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width-m_window_deadzone))
		{
			mouse_current_x = m_window_width >> 1;
			glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
		}
		//if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height-m_window_deadzone))
	{
		mouse_current_y = m_window_height >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;
	}
}

void Application::ShowCursor()
{
	glfwSetInputMode(m_window,
		GLFW_CURSOR,
		GLFW_CURSOR_NORMAL
		);
}
void Application::HideCursor()
{
	glfwSetInputMode(m_window,
		GLFW_CURSOR,
		GLFW_CURSOR_HIDDEN
		);
}
void Application::SetCursorPos(double xPos, double yPos)
{
	glfwSetCursorPos(m_window, xPos, yPos);
}
void Application::GetCursorPos(double &xpos, double &ypos)
{
	glfwGetCursorPos(m_window, &xpos, &ypos);
}
void Application::SetWindowWidth(int w)
{
	m_window_width = w;
}
void Application::SetWindowHeight(int h)
{
	m_window_height = h;
}
int Application::GetWindowWidth()
{
	return m_window_width;
}
int Application::GetWindowHeight()
{
	return m_window_height;
}

Application::Application() :
theSoundEngine(NULL),
bgm_menu(NULL),
menu_hover(NULL),
menu_click(NULL),
sound_machinegun(NULL)
{
}

Application::~Application()
{
	if (theSoundEngine != NULL)
		theSoundEngine->drop();
}

void Application::LoadMedia()
{
	Application::GetInstance().bgm_menu = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//bgm_menu.mp3");
	//Application::GetInstance().bgm_menu->setDefaultVolume(0.5);

    Application::GetInstance().menu_hover = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//sound_onhover.wav");
    Application::GetInstance().menu_click = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//sound_onclick.wav");
    Application::GetInstance().sound_shotgun = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//sound_shotgun.wav");
    Application::GetInstance().sound_shotgun->setDefaultVolume(0.5);
    Application::GetInstance().sound_machinegun = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//sound_machinegun.wav");
    Application::GetInstance().sound_machinegun->setDefaultVolume(0.7);
    Application::GetInstance().sound_splitgun = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//sound_splitgun.wav");

	//Application::GetInstance().sound_Footstep = Application::GetInstance().theSoundEngine->play2D("media//footsteps.wav", true, true, true);
	//Application::GetInstance().sound_jumpLanding = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//jump_Landing.flac");
	//Application::GetInstance().sound_gunshot = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//gunshot.wav");
	//Application::GetInstance().sound_hookshot = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//hookshot.wav");
	//Application::GetInstance().sound_rocketshot = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//rocketshot.wav");

	//Application::GetInstance().sound_bulletImpact = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//bulletImpact.wav");
	//Application::GetInstance().sound_bulletImpact->setDefaultMinDistance(30.0f);
	//Application::GetInstance().sound_bulletImpact->setDefaultMaxDistance(300.0f);

	//Application::GetInstance().sound_rocketImpact = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//rocketImpact.mp3");
	//Application::GetInstance().sound_rocketImpact->setDefaultMinDistance(150.0f);
	//Application::GetInstance().sound_rocketImpact->setDefaultMaxDistance(800.0f);

	//Application::GetInstance().sound_ballBounce = Application::GetInstance().theSoundEngine->addSoundSourceFromFile("media//bounce.wav");
	//Application::GetInstance().sound_ballBounce->setDefaultVolume(1.0f);
	//Application::GetInstance().sound_ballBounce->setDefaultMinDistance(100.0f);
	//Application::GetInstance().sound_ballBounce->setDefaultMaxDistance(1300.0f);
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "Game Development Module", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetScrollCallback(m_window, mouseWheel_callback);

	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_ThreadOne = 0.f;
	m_dAccumulatedTime_ThreadTwo = 0.f;

	//scene = new SceneText();

	// start the sound engine with default parameters
	theSoundEngine = createIrrKlangDevice();
	if (!theSoundEngine)
		return; // error starting up the sound engine

	//Load media
	LoadMedia();
	
}

void Application::Run()
{
	Controls &control = Controls::GetInstance();
	MeshManager &meshManager = MeshManager::GetInstance();
	SceneManager &sm = SceneManager::GetInstance();

	meshManager.Init();

	//sm.SetScene(new SceneSnakeBoss());
	//sm.SetScene(new SceneSnakeBoss());
	//sm.SetScene(new SceneSummoner());
	//sm.SetScene(new SceneDetlaff());
	//sm.SetScene(new SceneGolem());
    sm.SetScene(new MainMenu());

	//Main Loop

	sm.GetScene()->Init();

	//scene->Init();
    HideCursor();

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !Controls::GetInstance().OnPress(Controls::KEY_ESC))
	{
		//Get elapsed time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_ThreadOne += m_dElapsedTime;
		m_dAccumulatedTime_ThreadTwo += m_dElapsedTime;
		if (m_dAccumulatedTime_ThreadOne > 0.01)
		{
			//GetMouseUpdate();
			control.UpdateInputs();
			//scene->Update(m_dElapsedTime);
			sm.GetScene()->Update(m_dElapsedTime);
			m_dAccumulatedTime_ThreadOne = 0.0;
		}
		if (m_dAccumulatedTime_ThreadTwo > 1.0)
		{
			//UpdateAI();
			m_dAccumulatedTime_ThreadTwo = 0.0;
		}
		//scene->Render();
		sm.GetScene()->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	//scene->Exit();
	//delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}