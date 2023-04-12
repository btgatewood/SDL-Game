#include <iostream>
#include <string>

#include <SDL_image.h>

#include "SpaceShooter.h"


const int BG_WIDTH = 1080;
const int BG_HEIGHT = 1920;

const int BG_SPEED = 60;  // pixels per second


//  TODO:  Implement resource management system.
SDL_Texture* load_texture(SDL_Renderer* renderer, std::string file)
{
	SDL_Surface* surface = IMG_Load(file.c_str());
	if (!surface)
	{
		SDL_Log("IMG_Load() Error: %s", IMG_GetError());
		return nullptr;
	}
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
	{
		SDL_Log("SDL_CreateTextureFromSurface(%s) Error: %s", file.c_str(), SDL_GetError());
		return nullptr;
	}

	SDL_FreeSurface(surface);

	return texture;
}


void SpaceShooter::LoadData(SDL_Renderer* renderer)
{
	// load the background texture
	background_ = load_texture(renderer, "data/Space_BG_01/Layers/BG.png");
	if (!background_)
	{
		// TODO: Better error handling...
		std::cout << "Error: Failed to load background texture!" << std::endl;
	}

	// get background width and height
	// SDL_QueryTexture(background_, nullptr, nullptr, &width_, &height_);

	// load the background layer textures
	stars_ = load_texture(renderer, "data/Space_BG_01/Layers/Stars.png");
	planets_ = load_texture(renderer, "data/Space_BG_01/Layers/Planets.png");
	meteors_ = load_texture(renderer, "data/Space_BG_01/Layers/Meteors.png");
	if (!stars_ || !planets_ || !meteors_)
	{
		std::cout << "Error: Failed to load background layers!" << std::endl;
	}

	// set initial background position
	int x = -(BG_WIDTH - SCREEN_WIDTH) / 2;  // -(1080 - 960) / 2 = -60
	int y = -(BG_HEIGHT - SCREEN_HEIGHT);    // -(1920 - 540) = -1380
	dstrect_ = SDL_Rect{ x, y, BG_WIDTH, BG_HEIGHT };
}


void SpaceShooter::ProcessInput(const Uint8* key_states)
{

}

void SpaceShooter::Update(float delta_time)
{
	dstrect_.y += BG_SPEED * delta_time;
}


void SpaceShooter::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, background_, nullptr, &dstrect_);
	SDL_RenderCopy(renderer, stars_, nullptr, &dstrect_);
	SDL_RenderCopy(renderer, planets_, nullptr, &dstrect_);
	SDL_RenderCopy(renderer, meteors_, nullptr, &dstrect_);
}


void SpaceShooter::Quit()
{
	SDL_DestroyTexture(meteors_);
	SDL_DestroyTexture(planets_);
	SDL_DestroyTexture(stars_);
	SDL_DestroyTexture(background_);
}