#pragma once

#include <vector>  // temp import for ship animation?

#include <SDL.h>


// TODO: Create AnimatedSprite and Player (Ship) classes.
// NOTE: Actor -> Ship, SpriteComponent -> AnimSpriteComponent

// NOTE: Maybe an inheritance hierarchy of game objects is better for learning?
//	Component architecture is confusing...or is inheritance confusing?!


struct Vector2
{
	float x;
	float y;
};


class Actor
{
public:
	void SetPosition(const Vector2& pos) { position_ = pos; }

protected:
	Vector2 position_ = { 0.0f, 0.0f };
	float scale_ = 0.0f;
	float rotation_ = 0.0f;
};


class Sprite : public Actor
{
public:
	void SetTexture(SDL_Texture* texture);
	void Draw(SDL_Renderer* renderer);

protected:
	SDL_Texture* texture_ = nullptr;
	int tex_width_ = 0;
	int tex_height_ = 0;
};


class AnimatedSprite : public Sprite
{
public:
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	void Update(float delta_time) {}

protected:
	std::vector<SDL_Texture*> anim_textures_;
	float current_frame_ = 0.0f;  // cast to int when setting texture index
	float anim_fps_ = 0.0f;
};


class PlayerShip : public AnimatedSprite
{
	// TODO:
	// load textures
	// set position to center of screen
	// draw ship
	// animate exhuast
};


/* -------------------------------------------------------------------------- */  // NOTE: length == 80
class BackgroundSprite
// Draws a repeating, scrolling background texture.
// Currently only scrolls down on the y-axis.
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
