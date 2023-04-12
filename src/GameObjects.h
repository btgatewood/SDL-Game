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

class BackgroundSprite
{
public:
	void SetTexture(SDL_Texture* texture);

	void Update(float delta_time);
	void Draw(SDL_Renderer* renderer);

	void SetPosition(const Vector2& pos) { position_ = pos; }
	void SetSpeed(float speed) { scroll_speed_ = speed; }
	void SetScreenSize(const Vector2& size) { screen_size_ = size; }

private:
	SDL_Texture* texture_ = nullptr;
	int tex_height_ = 0;
	Vector2 position_ = { 0.0f, 0.0f };
	float scroll_speed_ = 0.0f;
	SDL_Rect dest_rect_ = { 0, 0, 0, 0 };
	Vector2 screen_size_ = { 0.0f, 0.0f };
};


