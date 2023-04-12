#include <iostream>
#include <sstream>

#include <SDL_image.h>

#include "Game.h"


// engine config
const int UPDATES_PER_SECOND = 60;
const int MS_PER_UPDATE = 1000 / UPDATES_PER_SECOND;

const float DELTA_TIME = MS_PER_UPDATE / 1000.0f;  // == 0.016f @ 60Hz

const int MAX_RENDERS_PER_SECOND = 240;
const int MIN_MS_PER_RENDER = 1000 / MAX_RENDERS_PER_SECOND;


bool Game::Init()
{
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result != 0)
    {
        // TODO: Use custom log system?
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
            renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);  // TODO: Enable vsync?
            if (!renderer_)
            {
                SDL_Log("SDL_CreateRenderer() Error: %s", SDL_GetError());
                return false;
            }
            else
            {
                int flags = IMG_INIT_PNG;
                int all_flags = IMG_Init(flags);  // load png support
                if ((flags & all_flags) != flags)
                {
                    SDL_Log("IMG_Init() Error: %s", IMG_GetError());
                    return false;
                }
                else
                {
                    // TODO: Log debug messages to file.
                    std::cout << "Success! Engine initialized.\n" << std::endl;
                    
                    space_shooter_.LoadData(renderer_);
                }
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
            Update(DELTA_TIME);  // convert dt to seconds, pass as float
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
    space_shooter_.Quit();

    IMG_Quit();
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

    const Uint8* key_states = SDL_GetKeyboardState(nullptr);

    if (key_states[SDL_SCANCODE_ESCAPE])
    {
        quit_ = true;
    }
    else
    {
        // process keyboard input
        space_shooter_.ProcessInput(key_states);
    }
}


void Game::Update(float delta_time)
{
    space_shooter_.Update(delta_time);
}


void Game::Render()
{
    // SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);  // clear back buffer to current draw color

    space_shooter_.Render(renderer_);  // NOTE: Render the scene here!

    SDL_RenderPresent(renderer_);
}
