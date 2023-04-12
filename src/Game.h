#pragma once

#include <SDL.h>

// #include "Pong.h"
#include "SpaceShooter.h"


class Game
{
public:
	bool Init();
	void Run();
	void Quit();

private:
	void ProcessEvents();
	void Update(float delta_time);
	void Render();

	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
	bool quit_ = false;

	// Pong pong_;
	SpaceShooter space_shooter_;
};
