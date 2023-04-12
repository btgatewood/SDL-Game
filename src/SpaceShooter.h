#pragma once

#include <SDL.h>


// 16:9 * 64 == 1024x576
// 4:3 * 256 == 1024x768

// 16:9 * 120 == 1920x1080
// 16:9 * 90  == 1440x810
// 16:9 * 75  == 1200x675
// 16:9 * 60  == 960x540

// 1080x1920 / 2 == 540x960
// 1080x1920 / 3 == 360x640

// NOTE: 1920x1080 / 2 == 16:9 * 60 == 960x540
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;
// TODO: Set resolution based on user's hardware.


class SpaceShooter
{
public:
	void LoadData(SDL_Renderer* renderer);
	void Quit();

	void ProcessInput(const Uint8* key_states);
	void Update(float delta_time);
	void Render(SDL_Renderer* renderer);


	SpaceShooter()
		: background_(nullptr)
		, stars_(nullptr)
		, planets_(nullptr)
		, meteors_(nullptr)
		, dstrect_()
	{}

private:
	// background layer textures
	SDL_Texture* background_;
	SDL_Texture* stars_;
	SDL_Texture* planets_;
	SDL_Texture* meteors_;

	SDL_Rect dstrect_;
};