#include <cmath>

#include "GameObjects.h"


void BackgroundSprite::SetTexture(SDL_Texture* texture)
{
	texture_ = texture;

	// save texture width & height
	SDL_QueryTexture(texture_, nullptr, nullptr, &width_, &height_);
}

void BackgroundSprite::SetPosition(const Vector2& pos)
{
	position_ = pos;

	// set destination rect before rendering
	dest_rect_ = SDL_Rect{ 
		static_cast<int>(position_.x), static_cast<int>(position_.y), 
		width_, height_ 
	};
}

void BackgroundSprite::Update(float delta_time)
{
	position_.y += scroll_speed_ * delta_time;  // scroll the background

	if (position_.y > screen_height_)
	{
		position_.y = -(height_ - screen_height_);  // return to top
	}

	dest_rect_.y = std::round(position_.y);  // avoid loss of fractional data
}


void BackgroundSprite::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture_, nullptr, &dest_rect_);

	// draw offset texture to get infinite scrolling
	SDL_Rect offset_rect = dest_rect_;
	offset_rect.y -= height_;

	SDL_RenderCopy(renderer, texture_, nullptr, &offset_rect);
}