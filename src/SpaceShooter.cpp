#include <cmath>
#include <iostream>

#include <SDL_image.h>

#include "SpaceShooter.h"


SDL_Texture* SpaceShooter::GetTexture(const std::string& file)
// Creates a texture if it has not already been loaded and 
// saves a ptr so we can delete it later.
{
	SDL_Texture* texture = nullptr;

	auto iter = textures_.find(file);
	if (iter != textures_.end())
	{
		texture = iter->second;
	}
	else
	{
		// load the image and create the texture
		SDL_Surface* surface = IMG_Load(file.c_str());
		if (!surface)
		{
			SDL_Log("IMG_Load() Error: %s", IMG_GetError());
			return nullptr;
		}

		texture = SDL_CreateTextureFromSurface(renderer_, surface);
		SDL_FreeSurface(surface);  // release the surface object
		if (!texture)
		{
			SDL_Log("SDL_CreateTextureFromSurface(%s) Error: %s", file.c_str(), SDL_GetError());
			return nullptr;
		}
		else
		{
			// textures_.emplace(file, texture);
			textures_.insert({ file, texture });  // add texture to map
		}
	}

	return texture;
}


void SpaceShooter::LoadData(SDL_Renderer* renderer)
{
	// save ptr to engine's renderer object
	renderer_ = renderer;

	// load background textures
	std::vector<SDL_Texture*> layers = {
		GetTexture("data/Space_BG_01/Layers/BG.png"),
		GetTexture("data/Space_BG_01/Layers/Stars.png"),
		GetTexture("data/Space_BG_01/Layers/Planets.png"),
		GetTexture("data/Space_BG_01/Layers/Meteors.png")
	};

	// calculate initial background position
	float x = -(1080 - SCREEN_WIDTH) / 2.0f;  // -(bg_width - 960) / 2 = -60
	float y = -(1920 - SCREEN_HEIGHT);		  // -(bg_height - 540) = -1380
	Vector2 position{ x, y };

	// create background sprites
	for (int i = 0; i < layers.size(); ++i)
	{
		BackgroundSprite sprite;

		sprite.SetTexture(layers[i]);
		sprite.SetPosition(position);

		sprite.SetScrollSpeed(60.0f * (i + 1) * 2);  // 120, 240, 360, 480
		sprite.SetScreenHeight(SCREEN_HEIGHT);

		sprites_.push_back(sprite);  // add sprite to list
	}
}


void SpaceShooter::Update(float delta_time)
{
	for (BackgroundSprite& sprite : sprites_)
	{
		sprite.Update(delta_time);
	}
}


void SpaceShooter::Render(SDL_Renderer* renderer)
{
	for (BackgroundSprite& sprite : sprites_)
	{
		sprite.Draw(renderer_);  // NOTE: Passing class member, not function param.
	}
}


void SpaceShooter::Quit()
{
	// destroy textures in texture map
	for (auto& i : textures_)
	{
		SDL_DestroyTexture(i.second);
	}
}