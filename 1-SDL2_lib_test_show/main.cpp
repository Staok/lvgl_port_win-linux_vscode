// 参考 https://my.oschina.net/u/5732601/blog/5516455

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

#include <iostream>
#include <thread>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
    //The window we'll be rendering to
    SDL_Window* window = NULL;

    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            std::thread t( [&]() -> void {
                for(unsigned char i = 0; i < 20; i++)
                {
                    //Get window surface
                    screenSurface = SDL_GetWindowSurface( window );

                    //Fill the surface white
                    SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x01 + i * 10, 0xAA, 0xBB ) );
                    
                    //Update the surface
                    SDL_UpdateWindowSurface( window );
                    
                    //Wait two seconds
                    SDL_Delay( 200 );
                }
            });
            t.join();
            
        }
    }

    std::cout << "quit..." << std::endl;

    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}