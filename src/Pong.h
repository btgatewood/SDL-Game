#pragma once

#include <SDL.h>


// game settings
const int SCREEN_WIDTH = 1024;  // 4:3 * 64?
const int SCREEN_HEIGHT = 768;
const int TILE_SIZE = 16;

const int PLAYER_SPEED = 400;  // pixels per second
const int PLAYER_HEIGHT = 100;

const int BALL_SPEED_X = 600;
const int BALL_SPEED_Y = 300;


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
	void Update(double delta_time);
	void Render(SDL_Renderer* renderer);

private:
	Vector2 player_pos_;
	int player_dir_;

	Vector2 ball_pos_;
	Vector2 ball_vel_;
};

