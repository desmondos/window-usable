#ifndef GAME_H
# define GAME_H

# define WIDTH 1920
# define HEIGHT 1080

# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>

typedef struct {
    SDL_Window		*window;
    SDL_Renderer	*renderer;
    int				width;
    int				height;
	TTF_Font		*font;
}				SDLApp;

void	toLowerCase(char *str);
int		initSDL(SDLApp *app, const char *title, int width, int height);
int		main(int argc, char* argv[]);
void	cleanupSDL(SDLApp *app);
void	mainloop(SDLApp *app);
void	renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);

#endif
