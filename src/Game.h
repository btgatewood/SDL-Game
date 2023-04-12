#pragma once

#include <SDL.h>

// #include "Pong.h"
#include "SpaceShooter.h"


class Game
{
public:
	Game();
	bool Init();
	void Run();
	void Quit();

private:
	void ProcessEvents();
	void Update(float delta_time);
	void Render();

	SDL_Window* window_;
	SDL_Renderer* renderer_;
	bool quit_;

	// Pong pong_;
	SpaceShooter space_shooter_;
};
