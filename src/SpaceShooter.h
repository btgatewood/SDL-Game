#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL.h>

#include "GameObjects.h"


// 16:9 * 64 == 1024x576
// 4:3 * 256 == 1024x768

// 16:9 * 120 == 1920x1080
// 16:9 * 90  == 1440x810
// 16:9 * 75  == 1200x675
// 16:9 * 60  == 960x540

// 1080x1920 / 2 == 540x960
// 1080x1920 / 3 == 360x640

// NOTE: 1920x1080 / 2 == 16:9 * 60 == 960x540
// const int SCREEN_WIDTH = 960;
// const int SCREEN_HEIGHT = 540;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;


class SpaceShooter
{
public:
	void LoadData(SDL_Renderer* renderer);
	void Quit();

	void ProcessInput(const Uint8* key_states) {}
	void Update(float delta_time);
	void Render();

private:
	SDL_Texture* GetTexture(const std::string& file);

	SDL_Renderer* renderer_ = nullptr;  // ptr to engine's renderer

	typedef std::unordered_map<std::string, SDL_Texture*> TextureMap;
	TextureMap textures_;  // map of all loaded textures

	std::vector<BackgroundSprite> bg_sprites_;
	AnimatedSprite player_;
};