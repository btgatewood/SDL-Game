#include <cmath>
#include <iostream>
#include <string>
#include <vector>

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


const int BG_WIDTH = 1080;
const int BG_HEIGHT = 1920;

const int BG_SPEED = 240;  // pixels per second


void SpaceShooter::LoadData(SDL_Renderer* renderer)
{
	// set initial background position
	int x = -(BG_WIDTH - SCREEN_WIDTH) / 2;  // -(1080 - 960) / 2 = -60
	int y = -(BG_HEIGHT - SCREEN_HEIGHT);    // -(1920 - 540) = -1380
	dstrect_ = SDL_Rect{ x, y, BG_WIDTH, BG_HEIGHT };
	y_pos_ = y;

	// load the background layer textures
	background_ = load_texture(renderer, "data/Space_BG_01/Layers/BG.png");
	stars_ = load_texture(renderer, "data/Space_BG_01/Layers/Stars.png");
	planets_ = load_texture(renderer, "data/Space_BG_01/Layers/Planets.png");
	meteors_ = load_texture(renderer, "data/Space_BG_01/Layers/Meteors.png");
	if (!background_ || !stars_ || !planets_ || !meteors_)
	{
		// TODO: Better error handling...
		std::cout << "Error: Failed to load background layers!" << std::endl;
	}

	// get background width and height
	// SDL_QueryTexture(background_, nullptr, nullptr, &width_, &height_);

	// TODO:  Create BackgroundSprite instances!!!
	// std::vector...
}


void SpaceShooter::ProcessInput(const Uint8* key_states)
{

}

void SpaceShooter::Update(float delta_time)
{
	// scroll the background
	y_pos_ += BG_SPEED * delta_time;
	dstrect_.y = std::round(y_pos_);  // avoid loss of fractional data

	// reset position if the background moves off screen
	if (y_pos_ > SCREEN_HEIGHT)
	{
		y_pos_ = -(BG_HEIGHT - SCREEN_HEIGHT);
		dstrect_.y = std::round(y_pos_);
	}
}


void SpaceShooter::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, background_, nullptr, &dstrect_);
	SDL_RenderCopy(renderer, stars_, nullptr, &dstrect_);
	SDL_RenderCopy(renderer, planets_, nullptr, &dstrect_);
	SDL_RenderCopy(renderer, meteors_, nullptr, &dstrect_);

	// tile background by drawing offset textures
	SDL_Rect offset = dstrect_;
	offset.y -= BG_HEIGHT;

	SDL_RenderCopy(renderer, background_, nullptr, &offset);
	SDL_RenderCopy(renderer, stars_, nullptr, &offset);
	SDL_RenderCopy(renderer, planets_, nullptr, &offset);
	SDL_RenderCopy(renderer, meteors_, nullptr, &offset);
}


void SpaceShooter::Quit()
{
	SDL_DestroyTexture(meteors_);
	SDL_DestroyTexture(planets_);
	SDL_DestroyTexture(stars_);
	SDL_DestroyTexture(background_);
}