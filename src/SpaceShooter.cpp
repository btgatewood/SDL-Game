#include <cmath>
#include <iostream>
#include <sstream>

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
			textures_.insert({ file, texture });  // add texture to map for deletion later
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

		bg_sprites_.push_back(sprite);  // add sprite to list
	}

	// TODO: Load ship textures, create ship sprite.
	for (int i = 0; i < 10; ++i)
	{
		std::ostringstream oss;
		oss << "data/Ship_01/Exhaust/Exhaust_1_1_00" << i << ".png";
		ship_textures_.push_back(GetTexture(oss.str()));  // using GetTexture(filename) to ensure deletion
	}

	// TODO:  Init player ship.
	// PlayerShip player_;
	player_.SetAnimTextures(ship_textures_);
	// TODO: Set position to center screen.
	player_.SetPosition(Vector2{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
}


void SpaceShooter::Update(float delta_time)
{
	for (BackgroundSprite& sprite : bg_sprites_)
	{
		sprite.Update(delta_time);
	}

	// TODO: Animate ship.
}


const int SHIP_WIDTH = 1063;
const int SHIP_HEIGHT = 1192;

void SpaceShooter::Render(SDL_Renderer* renderer)  // TODO: Remove renderer param?
{
	for (BackgroundSprite& sprite : bg_sprites_)
	{
		sprite.Draw(renderer_);  // NOTE: Passing class member, not method arg.
	}

	// scale ship texture and draw at screen center
	//SDL_Rect dstrect = { SCREEN_WIDTH / 2 - SHIP_WIDTH / 16, 
	//					 SCREEN_HEIGHT / 2 - SHIP_HEIGHT / 16, 
	//					 SHIP_WIDTH / 8, SHIP_HEIGHT / 8 };  // center the ship!?
	SDL_Rect dstrect = { 0, 0, SHIP_WIDTH / 8, SHIP_HEIGHT / 8 };
	SDL_RenderCopy(renderer_, ship_textures_[0], nullptr, &dstrect);

	// TODO: Draw ship (player).
	player_.Draw(renderer_);
}


void SpaceShooter::Quit()
{
	// destroy textures in texture map
	for (auto& i : textures_)
	{
		SDL_DestroyTexture(i.second);
	}
}
