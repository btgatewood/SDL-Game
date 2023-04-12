#pragma once

#include <SDL.h>


// game settings
const int SCREEN_WIDTH = 1024;  // 4:3 * 64?
const int SCREEN_HEIGHT = 768;
const int TILE_SIZE = 16;


struct Vector2
{
	int x;
	int y;
};


class Pong
{
public:
	Pong();
	void ProcessInput(const Uint8* key_states);
	void Update(float delta_time);
	void Render(SDL_Renderer* renderer);

private:
	Vector2 player_pos_;
	int player_dir_;

	Vector2 ball_pos_;
	Vector2 ball_vel_;
};

