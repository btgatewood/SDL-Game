#include <iostream>
#include <string>

#include <SDL_image.h>

#include "SpaceShooter.h"


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


void SpaceShooter::ProcessInput(const Uint8* key_states)
{

}

void SpaceShooter::Update(float delta_time)
{

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
	SDL_QueryTexture(background_, nullptr, nullptr, &bg_width_, &bg_height_);

	// load the stars texture
	stars_ = load_texture(renderer, "data/Space_BG_01/Layers/Stars.png");
	if (!stars_)
	{
		std::cout << "Error: Failed to load background texture!" << std::endl;
	}
}


const int BG_SPRITE_WIDTH = 1080;
const int BG_SPRITE_HEIGHT = 1920;


void SpaceShooter::Render(SDL_Renderer* renderer)
{
	const SDL_Rect dstrect{
		-(bg_width_ / 4), -(bg_height_ / 2), bg_width_, bg_height_
	};

	SDL_RenderCopy(renderer, background_, nullptr, &dstrect);
	SDL_RenderCopy(renderer, stars_, nullptr, &dstrect);
}


void SpaceShooter::Quit()
{
	SDL_DestroyTexture(stars_);
	SDL_DestroyTexture(background_);
}