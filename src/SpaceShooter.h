#pragma once

#include <vector>

#include <SDL.h>


// NOTE: 1080x1920 / 2
const int SCREEN_WIDTH = 540;
const int SCREEN_HEIGHT = 960;


class SpaceShooter
{
public:
	void LoadData(SDL_Renderer* renderer);
	void Quit();  // unload data

	void ProcessInput(const Uint8* key_states);
	void Update(float delta_time);
	void Render(SDL_Renderer* renderer);


	SpaceShooter()
		: background_(nullptr)
		, bg_width_(0)
		, bg_height_(0)
		, stars_(nullptr)
	{}

private:
	SDL_Texture* background_;
	int bg_width_;
	int bg_height_;

	SDL_Texture* stars_;

	// std::vector<SDL_Texture*> background_layers_;
};