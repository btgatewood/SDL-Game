#pragma once

#include <SDL.h>


const int BG_SPRITE_WIDTH = 1080;
const int BG_SPRITE_HEIGHT = 1920;

// NOTE: 1080x1920 / 2
const int SCREEN_WIDTH = 540;
const int SCREEN_HEIGHT = 960;


class SpaceShooter
{
public:
	SpaceShooter() 
		: background_(nullptr)
	{}

	void LoadData(SDL_Renderer* renderer);
	void Quit();  // unload data

	void ProcessInput(const Uint8* key_states);
	void Update(float delta_time);
	void Render(SDL_Renderer* renderer);

private:
	SDL_Texture* background_;
};