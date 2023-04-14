#include <cmath>
#include <iostream>
#include <sstream>

#include <SDL_image.h>

#include "SpaceShooter.h"


SDL_Texture* SpaceShooter::GetTexture(const std::string& file)
// Creates a texture if it has not already been loaded and 
// saves a ptr so we can destroy it later.
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
	renderer_ = renderer;  // save ptr to engine's renderer object

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
		sprite.SetPosition(position);  // TODO: Vector2{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }
		sprite.SetScrollSpeed(60.0f * (i + 1) * 2);  // 120, 240, 360, 480
		sprite.SetScreenHeight(SCREEN_HEIGHT);
		bg_sprites_.push_back(sprite);  // add sprite to list
	}

	// load ship textures
	for (int i = 0; i < 10; ++i)
	{
		std::ostringstream oss;
		oss << "data/Ship_01/Exhaust/Exhaust_1_1_00" << i << ".png";
		ship_textures_.push_back(GetTexture(oss.str()));  // using GetTexture(filename) to ensure deletion
	}

	// init player ship
	player_.SetAnimTextures(ship_textures_);
	player_.SetAnimFPS(10.0f);  // TODO: Double check this.  Tests, tests, tests.
	player_.SetPosition(Vector2{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	player_.SetScale(1.0f / 8.0f);  // TODO: Use this, check math.
}


void SpaceShooter::Update(float delta_time)
{
	for (BackgroundSprite& sprite : bg_sprites_)
	{
		sprite.Update(delta_time);
	}

	player_.Update(delta_time);  // TODO: Animate ship.
}


void SpaceShooter::Render(SDL_Renderer* renderer)  // TODO: Remove renderer param?
{
	for (BackgroundSprite& bg_sprite : bg_sprites_)
	{
		bg_sprite.Draw(renderer_);  // NOTE: Passing class member, not method arg.
	}

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
