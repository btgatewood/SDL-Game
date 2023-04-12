#pragma once

#include <SDL.h>


class Actor
{};

class Component
{};


struct Vector2
{
	float x;
	float y;
};


// TODO: Create AnimatedSprite and Player (Ship) classes.


class BackgroundSprite
{
public:
	void SetTexture(SDL_Texture* texture);
	void SetPosition(const Vector2& pos);

	void SetScrollSpeed(float speed) { scroll_speed_ = speed; }
	void SetScreenHeight(int height) { screen_height_ = height; }

	void Update(float delta_time);
	void Draw(SDL_Renderer* renderer);

private:
	SDL_Texture* texture_ = nullptr;
	int width_ = 0;
	int height_ = 0;

	Vector2 position_ = { 0.0f, 0.0f };
	SDL_Rect dest_rect_ = { 0, 0, 0, 0 };

	float scroll_speed_ = 0.0f;
	int screen_height_ = 0;
};


