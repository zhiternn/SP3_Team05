#ifndef APPLICATION_H
#define APPLICATION_H

#include <irrKlang.h>
using namespace irrklang;

#include "Vector3.h"
#include "timer.h"
#include "SceneManager.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();

	static void ShowCursor();
	static void HideCursor();
	static void SetCursorPos(double xPos, double yPos);
	static void GetCursorPos(double &xpos, double &ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();
	static void SetWindowWidth(int w);
	static void SetWindowHeight(int h);

	void GetMouseUpdate();
	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;

	// The sound engine
	ISoundEngine* theSoundEngine;
    ISoundSource* bgm_menu;
    ISoundSource* menu_hover;
    ISoundSource* menu_click;

	//ISoundSource* bgm_lullaby;
	//ISound* sound_Footstep;
	//ISoundSource* sound_jumpLanding;
	//ISoundSource* sound_gunshot;
	//ISoundSource* sound_hookshot;
	//ISoundSource* sound_rocketshot;
	//ISoundSource* sound_bulletImpact;
	//ISoundSource* sound_rocketImpact;
	//ISoundSource* sound_ballBounce;
	ISoundSource* sound_shotgun;
	ISoundSource* sound_machinegun;
	ISoundSource* sound_splitgun;

	void LoadMedia();
	void StopSound();
	void PlaySoundIn3D(Vector3 playerPos, Vector3 sourcePos);

private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	static int m_window_width;
	static int m_window_height;
	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;
	SceneManager sm;
};

#endif