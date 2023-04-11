#include <sstream>

#include "Game.h"


// game settings
const int SCREEN_WIDTH = 1024;  // 4:3 * 64?
const int SCREEN_HEIGHT = 768;
const int TILE_SIZE = 16;

const int PLAYER_SPEED = 400;  // pixels per second
const int PLAYER_HEIGHT = 100;

const int BALL_SPEED_X = 400;
const int BALL_SPEED_Y = 200;


// engine config
const int UPDATES_PER_SECOND = 60;
const int MS_PER_UPDATE = 1000 / UPDATES_PER_SECOND;

const double DELTA_TIME = MS_PER_UPDATE / 1000.0;  // 0.016 @ 60 UPS

const int MAX_RENDERS_PER_SECOND = 240;
const int MIN_MS_PER_RENDER = 1000 / MAX_RENDERS_PER_SECOND;


Game::Game()
    : window_(nullptr)
    , renderer_(nullptr)
    , quit_(false)
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


bool Game::Init()
{
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result != 0)
    {
        // TODO: Use custom log system.
        SDL_Log("SDL_Init() Error: %s", SDL_GetError());
        return false;
    }
    else
    {
        window_ = SDL_CreateWindow("SDL Game", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (!window_)
        {
            SDL_Log("SDL_CreateWindow() Error: %s", SDL_GetError());
            return false;
        }
        else
        {
            // TODO: Enable vsync and test effects on framerate.  // NOTE: Limits fps to 60.
            // renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
            if (!renderer_)
            {
                SDL_Log("SDL_CreateRenderer() Error: %s", SDL_GetError());
                return false;
            }
        }
    }

    return true;
}


void Game::Run()
{
    Uint64 last_time = SDL_GetTicks64();  // ms since sdl init

    // NOTE: We don't need 64-bit integers for timer variables.
    Uint64 update_timer = 0;
    Uint64 render_timer = 0;
    Uint64 self_timer = 0;

    int update_count = 0;
    int render_count = 0;  // TODO: Calculate and display fps as float.

    while (!quit_)
    {
        ProcessEvents();

        Uint64 this_time = SDL_GetTicks64();
        Uint64 elapsed_time = this_time - last_time;  // elapsed real time in ms
        last_time = this_time;

        update_timer += elapsed_time;
        while (update_timer >= MS_PER_UPDATE)
        {
            Update(MS_PER_UPDATE / 1000.0);  // convert dt to seconds, pass as double value
            update_timer -= MS_PER_UPDATE;
            ++update_count;
        }

        render_timer += elapsed_time;
        if (render_timer >= MIN_MS_PER_RENDER)
        {
            Render();
            render_timer -= MIN_MS_PER_RENDER;
            ++render_count;;
        }
        else
        {
            // TODO: What is the benefit of sleeping? 
            SDL_Delay(MIN_MS_PER_RENDER - render_timer);  // sleep
        }

        self_timer += elapsed_time;
        if (self_timer > 1000)
        {
            // print fps data to window title
            std::ostringstream oss;
            oss << render_count << "FPS @ " << update_count << "Hz" << std::endl;
            SDL_SetWindowTitle(window_, oss.str().c_str());  // convert to const char *

            update_count = 0;
            render_count = 0;
            self_timer -= 1000;
        }
    }
}


void Game::Quit()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}


void Game::ProcessEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            quit_ = true;
        }
    }

    // process and handle keyboard state
    const Uint8* key_states = SDL_GetKeyboardState(nullptr);

    if (key_states[SDL_SCANCODE_ESCAPE])
    {
        quit_ = true;
    }

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


void Game::Update(double delta_time)
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

void Game::Render()
{
    // wall rects
    SDL_Rect left_wall{ 0, 0, TILE_SIZE, SCREEN_HEIGHT };
    SDL_Rect right_wall{ SCREEN_WIDTH - TILE_SIZE, 0, TILE_SIZE, SCREEN_HEIGHT };
    SDL_Rect top_wall{ 0, 0, SCREEN_WIDTH, TILE_SIZE };
    SDL_Rect bottom_wall{ 0, SCREEN_HEIGHT - TILE_SIZE, SCREEN_WIDTH, TILE_SIZE };

    // player rects
    SDL_Rect player{ player_pos_.x, player_pos_.y, TILE_SIZE, PLAYER_HEIGHT };

    // ball rect
    SDL_Rect ball{ ball_pos_.x, ball_pos_.y, TILE_SIZE, TILE_SIZE };


    // TODO: Simplify r, g, b params with a color struct and helper function?
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);  // background color

    // start render scene
    SDL_RenderClear(renderer_);  // clear back buffer to current draw color
    
    // NOTE: Render the scene here!

    // walls
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, SDL_ALPHA_OPAQUE);  // red
    SDL_RenderFillRect(renderer_, &left_wall);

    SDL_SetRenderDrawColor(renderer_, 0, 255, 0, SDL_ALPHA_OPAQUE);  // green
    SDL_RenderFillRect(renderer_, &right_wall);

    SDL_SetRenderDrawColor(renderer_, 0, 0, 255, SDL_ALPHA_OPAQUE);  // blue
    SDL_RenderFillRect(renderer_, &top_wall);
    SDL_RenderFillRect(renderer_, &bottom_wall);

    // players and ball
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white
    SDL_RenderFillRect(renderer_, &player);
    SDL_RenderFillRect(renderer_, &ball);

    // stop render scene
    SDL_RenderPresent(renderer_);
}
