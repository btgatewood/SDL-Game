#pragma once

#include <SDL.h>


struct Vector2
{
	int x;
	int y;
};


class Game
{
public:
	Game();
	bool Init();
	void Run();
	void Quit();

private:
	void ProcessEvents();
	void Update(double delta_time);
	void Render();

	SDL_Window* window_;
	SDL_Renderer* renderer_;
	bool quit_;

	Vector2 player_pos_;
	int player_dir_;

	Vector2 ball_pos_;
	Vector2 ball_vel_;
};
