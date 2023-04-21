#include <iostream>
#include <sstream>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Engine.h"


// engine config
const int UPDATES_PER_SECOND = 60;
const int MS_PER_UPDATE = 1000 / UPDATES_PER_SECOND;

const float DELTA_TIME = MS_PER_UPDATE / 1000.0f;  // == 0.016f @ 60Hz

const int MAX_RENDERS_PER_SECOND = 120;
const int MIN_MS_PER_RENDER = 1000 / MAX_RENDERS_PER_SECOND;


const SDL_Color WHITE = { 255, 255, 255, 255 };

SDL_Texture* get_text_texture(TTF_Font* font, const char* text, 
                              SDL_Renderer* renderer)
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, WHITE);
    if (!surface)
    {
        SDL_Log("TTF_RenderText_Solid() Error: %s", TTF_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
    }

    SDL_FreeSurface(surface);

    return texture;
}


bool Engine::Init()
{
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result != 0)
    {
        // TODO: Use custom log system.
        SDL_Log("SDL_Init() Error: %s", SDL_GetError());
        return false;
    }

    // TODO: Test positioning on dual-monitor PC.
    // create window at top left corner of screen
    // 1 px for left window border, 32 px for window title bar  
    window_ = SDL_CreateWindow("SDL Game", 1, 32, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window_)
    {
        SDL_Log("SDL_CreateWindow() Error: %s", SDL_GetError());
        return false;
    }
        
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);  // TODO: Enable vsync?
    if (!renderer_)
    {
        SDL_Log("SDL_CreateRenderer() Error: %s", SDL_GetError());
        return false;
    }

    int flags = IMG_INIT_PNG;
    int all_flags = IMG_Init(flags);  // load png support
    if ((flags & all_flags) != flags)
    {
        SDL_Log("IMG_Init() Error: %s", IMG_GetError());
        return false;
    }

    if (TTF_Init() < 0)  // init font rendering
    {
        SDL_Log("TTF_Init() Error: %s", SDL_GetError());
        return false;
    }
    else
    {
        // load font and render text surface
        font_ = TTF_OpenFont("data/Fonts/CascadiaMono.ttf", 16);
        text_ = get_text_texture(font_, "Hello, World!  Cascadia Mono - 16pt.", 
                                 renderer_);
    }
               
    std::cout << "Success! Engine initialized." << std::endl;

    space_shooter_.LoadData(renderer_);  // initialize the game state

    return true;
}


void Engine::Run()
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
            Update(DELTA_TIME);  // convert dt to seconds (fraction), pass as float
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
            SDL_Delay(MIN_MS_PER_RENDER - render_timer);  // TODO: Why sleep?!
        }

        self_timer += elapsed_time;
        if (self_timer > 1000)
        {
            // update fps text
            std::ostringstream oss;
            oss << render_count << "FPS @ " << update_count << "Hz";
            text_ = get_text_texture(font_, oss.str().c_str(), renderer_);

            update_count = 0;
            render_count = 0;
            self_timer -= 1000;
        }
    }
}


void Engine::Quit()
{
    space_shooter_.Quit();

    SDL_DestroyTexture(text_);
    TTF_CloseFont(font_);

    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}


void Engine::ProcessEvents()
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
        space_shooter_.ProcessInput(key_states);  // process keyboard input
    }
}


void Engine::Update(float delta_time)
{
    space_shooter_.Update(delta_time);
}


void Engine::Render()
{
    SDL_RenderClear(renderer_);  // begin draw (clear back buffer to current draw color)

    space_shooter_.Render();  // NOTE: Render the scene here!

    // draw debug text
    SDL_Rect dstrect{ 4, 4, 0, 0 };  // 4 px border padding
    SDL_QueryTexture(text_, nullptr, nullptr, &dstrect.w, &dstrect.h);
    SDL_RenderCopy(renderer_, text_, nullptr, &dstrect);

    SDL_RenderPresent(renderer_);  // end draw
}
