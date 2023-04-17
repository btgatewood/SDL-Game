#pragma once

#include <vector>  // temp import for ship animation?

#include <SDL.h>


// NOTE: Maybe an inheritance hierarchy of game objects is better for learning?
//	Component architecture is confusing...or is inheritance confusing?!


struct Vector2
{
	float x;
	float y;
};


class Sprite
{
public:
	void SetTexture(SDL_Texture* texture);

	void SetPosition(const Vector2& pos) { position_ = pos; }
	void SetScale(const float& scale)	 { scale_ = scale; }

	void Draw(SDL_Renderer* renderer);

	const int& GetHeight() const  { return tex_height_; }
	const float& GetScale() const { return scale_; }

protected:
	SDL_Texture* texture_ = nullptr;
	int tex_width_ = 0;
	int tex_height_ = 0;

	Vector2 position_ = { 0.0f, 0.0f };  // center position ("origin" in 3d)
	float scale_ = 0.0f;
};


class AnimatedSprite : public Sprite
{
public:
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	void SetAnimFPS(float fps) { anim_fps_ = fps; }

	void Update(float delta_time);

protected:
	std::vector<SDL_Texture*> anim_textures_;
	float current_frame_ = 0.0f;  // cast to int when setting texture index
	float anim_fps_ = 0.0f;
};


/* -------------------------------------------------------------------------- */  // NOTE: length == 80
class BackgroundSprite
// Draws a repeating, scrolling background texture.
// Currently only scrolls down on the y-axis.
{
public:
	void SetTexture(SDL_Texture* texture);

	void SetPosition(const Vector2& pos);  // top left corner

	void SetScrollSpeed(float speed) { scroll_speed_ = speed; }
	void SetScreenHeight(int height) { screen_height_ = height; }

	void Update(float delta_time);
	void Draw(SDL_Renderer* renderer);

private:
	SDL_Texture* texture_ = nullptr;
	int tex_width_ = 0;
	int tex_height_ = 0;

	Vector2 position_ = { 0.0f, 0.0f };  // TODO: Set pos to sprite's center.
	SDL_Rect dest_rect_ = { 0, 0, 0, 0 };  // TODO: Calc rect in draw method.

	float scroll_speed_ = 0.0f;
	int screen_height_ = 0;
};
