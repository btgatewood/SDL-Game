#include <cmath>
#include <iostream>

#include "GameObjects.h"


/* -------------------------------------------------------------------------- */
void Sprite::SetTexture(SDL_Texture* texture)
{
	texture_ = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &tex_width_, &tex_height_);
}

void Sprite::Draw(SDL_Renderer* renderer)
{
	SDL_Rect r;
	// calc width & height based on texture size & scale
	r.w = static_cast<int>(tex_width_ * scale_);
	r.h = static_cast<int>(tex_height_ * scale_);

	// set position to top left corner using center pos & 1/2 width/height
	r.x = static_cast<int>(position_.x - r.w / 2);
	r.y = static_cast<int>(position_.y - r.h / 2);

	SDL_RenderCopy(renderer, texture_, nullptr, &r);
}


/* -------------------------------------------------------------------------- */
void AnimatedSprite::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	anim_textures_ = textures;
	current_frame_ = 0.0f;
	SetTexture(anim_textures_[0]);
}


/* -------------------------------------------------------------------------- */
void BackgroundSprite::SetTexture(SDL_Texture* texture)
{
	texture_ = texture;

	// store texture width & height
	SDL_QueryTexture(texture_, nullptr, nullptr, &tex_width_, &tex_height_);
}

void BackgroundSprite::SetPosition(const Vector2& pos)
{
	position_ = pos;

	// set destination rect before rendering
	dest_rect_ = SDL_Rect{ 
		static_cast<int>(position_.x), static_cast<int>(position_.y), 
		tex_width_, tex_height_ 
	};
}

void BackgroundSprite::Update(float delta_time)
{
	position_.y += scroll_speed_ * delta_time;  // scroll the background

	if (position_.y > screen_height_)
	{
		position_.y = -(tex_height_ - screen_height_);  // return to top
	}

	dest_rect_.y = std::round(position_.y);  // avoid loss of fractional data
}

void BackgroundSprite::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture_, nullptr, &dest_rect_);

	// draw offset texture to get infinite scrolling
	SDL_Rect offset_rect = dest_rect_;
	offset_rect.y -= tex_height_;

	SDL_RenderCopy(renderer, texture_, nullptr, &offset_rect);
}
