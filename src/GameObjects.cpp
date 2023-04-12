#include <cmath>

#include "GameObjects.h"


void BackgroundSprite::SetTexture(SDL_Texture* texture)
{
	texture_ = texture;
	SDL_QueryTexture(texture_, nullptr, nullptr, nullptr, &tex_height_);  // save texture height
}


void BackgroundSprite::Update(float delta_time)
{
	// scroll the background
	position_.y += scroll_speed_ * delta_time;

	if (position_.y > screen_size_.y)
	{
		position_.y = -(tex_height_ - screen_size_.y);  // return to top
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