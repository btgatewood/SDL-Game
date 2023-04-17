#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

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

	SpaceShooter space_shooter_;

	TTF_Font* vs2022_font_ = nullptr;
	SDL_Texture* tex_vs2022_font_ = nullptr;
};
