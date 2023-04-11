#include "Pong.h"


// game settings
const int PLAYER_HEIGHT = 100;
const int PLAYER_SPEED = 400;  // pixels per second

const int BALL_SPEED_X = 600;
const int BALL_SPEED_Y = 300;


Pong::Pong()
{
    // NOTE: Positions are rect's top left corners.
    player_pos_.x = TILE_SIZE * 3;  // 4th column
    player_pos_.y = (SCREEN_HEIGHT / 2) - (PLAYER_HEIGHT / 2);  // screen_center.y - player_center.y
    player_dir_ = 0;

    ball_pos_.x = (SCREEN_WIDTH / 2) - (TILE_SIZE / 2);  // screen_center.x - tile_center
    ball_pos_.y = (SCREEN_HEIGHT / 2) - (TILE_SIZE / 2);  // screen_center.y - tile_center
    ball_vel_.x = BALL_SPEED_X;
    ball_vel_.y = BALL_SPEED_Y;
}


void Pong::ProcessInput(const Uint8* key_states)
{
    player_dir_ = 0;
    if (key_states[SDL_SCANCODE_W] || key_states[SDL_SCANCODE_UP])
    {
        player_dir_ -= 1;
    }
    if (key_states[SDL_SCANCODE_S] || key_states[SDL_SCANCODE_DOWN])
    {
        player_dir_ += 1;
    }
}


void Pong::Update(double delta_time)
{
    // move player
    if (player_dir_ != 0)
    {
        player_pos_.y += static_cast<int>(player_dir_ * PLAYER_SPEED * delta_time);
    }

    // clamp player to screen
    const int MAX_PLAYER_POS_Y = SCREEN_HEIGHT - TILE_SIZE - PLAYER_HEIGHT;

    if (player_pos_.y <= TILE_SIZE)
    {
        player_pos_.y = TILE_SIZE;
    }
    else if (player_pos_.y >= MAX_PLAYER_POS_Y)
    {
        player_pos_.y = MAX_PLAYER_POS_Y;
    }

    // move ball
    ball_pos_.x += static_cast<int>(ball_vel_.x * delta_time);
    ball_pos_.y += static_cast<int>(ball_vel_.y * delta_time);

    // check wall collisions
    if (ball_pos_.y <= TILE_SIZE ||  // top wall
        ball_pos_.y + TILE_SIZE >= SCREEN_HEIGHT - TILE_SIZE)  // bottom wall
    {
        ball_vel_.y *= -1;
    }

    if (ball_pos_.x + TILE_SIZE >= SCREEN_WIDTH - TILE_SIZE) // right wall
    {
        ball_vel_.x *= -1;
    }

    // check player collision
    if (ball_pos_.x <= player_pos_.x + TILE_SIZE        // ball.left <= player.right
        && ball_pos_.x > player_pos_.x                  // ball.left > player.left
        && ball_pos_.y + TILE_SIZE > player_pos_.y      // ball.bottom > player.top
        && ball_pos_.y < player_pos_.y + PLAYER_HEIGHT  // ball.top < player.bottom
        && ball_vel_.x < 0)
    {
        ball_vel_.x *= -1;
    }

    // check ai goal scored
    if (ball_pos_.x < 0)
    {
        // reset ball position
        ball_pos_.x = SCREEN_WIDTH / 2;
        ball_pos_.y = SCREEN_HEIGHT / 2;
    }
}


void Pong::Render(SDL_Renderer* renderer)
{
    // walls
    SDL_Rect left_wall{ 0, 0, TILE_SIZE, SCREEN_HEIGHT };
    SDL_Rect right_wall{ SCREEN_WIDTH - TILE_SIZE, 0, TILE_SIZE, SCREEN_HEIGHT };
    SDL_Rect top_wall{ 0, 0, SCREEN_WIDTH, TILE_SIZE };
    SDL_Rect bottom_wall{ 0, SCREEN_HEIGHT - TILE_SIZE, SCREEN_WIDTH, TILE_SIZE };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  // red
    SDL_RenderFillRect(renderer, &left_wall);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);  // green
    SDL_RenderFillRect(renderer, &right_wall);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);  // blue
    SDL_RenderFillRect(renderer, &top_wall);
    SDL_RenderFillRect(renderer, &bottom_wall);

    // player and ball
    SDL_Rect player{ player_pos_.x, player_pos_.y, TILE_SIZE, PLAYER_HEIGHT };
    SDL_Rect ball{ ball_pos_.x, ball_pos_.y, TILE_SIZE, TILE_SIZE };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white
    SDL_RenderFillRect(renderer, &player);
    SDL_RenderFillRect(renderer, &ball);
}
