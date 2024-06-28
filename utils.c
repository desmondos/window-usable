#include "game.h"

void	toLowerCase(char *str)
{
	while (*str)
	{
		*str = tolower((unsigned char)*str);
		str++;
	}
}

int	initSDL(SDLApp *app, const char *title, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		return 0;
	}
    if (TTF_Init() != 0)
	{
		SDL_Quit();
		return 0;
	}
	app->font = NULL; // Assurez-vous que app->font est initialisée à NULL
	app->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
								   SDL_WINDOWPOS_CENTERED, width, height,
								   SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (app->window == NULL)
	{
		SDL_Quit();
		return 0;
	}
	app->renderer = SDL_CreateRenderer(app->window, -1,
									   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (app->renderer == NULL)
	{
		SDL_DestroyWindow(app->window);
		SDL_Quit();
		return 0;
	}
	app->font = TTF_OpenFont("JetBrainsMonoNerdFont-Medium.ttf", 24);
	if (app->font == NULL)
	{
		SDL_DestroyRenderer(app->renderer);
		SDL_DestroyWindow(app->window);
		SDL_Quit();
		return 0;
	}
	app->width = width;
	app->height = height;
	SDL_RenderSetLogicalSize(app->renderer, app->width, app->height);
	return 1;
}

void	cleanupSDL(SDLApp *app)
{
	if (app->font)
		TTF_CloseFont(app->font);
	if (app->renderer)
		SDL_DestroyRenderer(app->renderer);
	if (app->window)
		SDL_DestroyWindow(app->window);
	TTF_Quit();
	SDL_Quit();
}

void	mainloop(SDLApp *app)
{
    int			running = 1;
    SDL_Event	event;
    char		inputText[256] = "";
    int			textLength = 0;
    int			enterPressed = 1; // Indicateur pour la validation par Entrée
    SDL_Color	leftColor = {255, 255, 255, 255}; // Couleur de départ de la partie gauche
    char		inputLower[256];

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = 0;
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
					running = 0;
				else if (event.key.keysym.sym == SDLK_BACKSPACE && textLength > 0)
					inputText[--textLength] = '\0';
				else if (event.key.keysym.sym == SDLK_RETURN)
				{
					if (strlen(inputText) > 0 && enterPressed)
					{
						strcpy(inputLower, inputText);
						toLowerCase(inputLower);

                    // Validation par Entrée
						if (strcmp(inputLower, "voay") == 0)
						{                              // Action pour bonne réponse
							leftColor.r = 0;
							leftColor.g = 255;
							leftColor.b = 0;
							enterPressed = 0; // Désactiver la validation par Entrée jusqu'à la prochaine bonne réponse
							textLength = 0; // Réinitialiser l'inputText pour la prochaine saisie
						}
						else
						{
                        // Action pour mauvaise réponse
							leftColor.r = 255;
							leftColor.g = 0;
							leftColor.b = 0;
						}
					}
				}
			}
			else if (event.type == SDL_TEXTINPUT)
			{
				if (textLength < sizeof(inputText) - 1)
				{
					strcat(inputText, event.text.text);
					textLength += strlen(event.text.text);
				}
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					app->width = event.window.data1;
					app->height = event.window.data2;
					SDL_RenderSetLogicalSize(app->renderer, app->width, app->height);
				}
			}
		}

        // Clear the renderer
		SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
		SDL_RenderClear(app->renderer);

        // Draw the left half
		SDL_Rect leftRect = {0, 0, app->width / 2, app->height};
		SDL_SetRenderDrawColor(app->renderer, leftColor.r, leftColor.g, leftColor.b, 255);
		SDL_RenderFillRect(app->renderer, &leftRect);

        // Draw the right half
		SDL_Rect rightRect = {app->width / 2, 0, app->width / 2, app->height};
		SDL_SetRenderDrawColor(app->renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(app->renderer, &rightRect);

        // Render the input text in the right half
		if (app->font)
			renderText(app->renderer, app->font, inputText, app->width / 2 + 10, 10);

        // Present the renderer
		SDL_RenderPresent(app->renderer);
	}
}

void	renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y)
{
	SDL_Surface	*surface = NULL;
    SDL_Texture	*texture = NULL;
    SDL_Color	color = {255, 255, 255, 255}; // White color

    // Render text to surface
	surface = TTF_RenderText_Blended(font, text, color);
	if (surface == NULL)
	{
		return;
	}

    // Create texture from surface
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
	{
		SDL_FreeSurface(surface);
		return;
	}

    // Query texture for dimensions
	int	texW = 0;
	int	texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    // Render texture to screen
	SDL_Rect dest = {x, y, texW, texH};
	SDL_RenderCopy(renderer, texture, NULL, &dest);

    // Cleanup
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}
