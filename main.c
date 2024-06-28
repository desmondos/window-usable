#include "game.h"

int	main(int argc, char* argv[])
{
	SDLApp app;

	if (!initSDL(&app, "window", WIDTH, HEIGHT))
	{
		return 1;
	}

    // Start accepting text input events
    SDL_StartTextInput();

    mainloop(&app);

    // Stop accepting text input events
    SDL_StopTextInput();

    cleanupSDL(&app);

    return 0;
}
