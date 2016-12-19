#include "SDLWrapper.h"

bool SDLWrapper::Initialize(int width, int height, int up) {
	upscale = up;
	screenWidth = width * upscale;
	screenHeight = height * upscale;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	} else {
		// Create window
		window = SDL_CreateWindow("StoveNES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		} else {
			// Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return true;
}

void SDLWrapper::DrawPixel(unsigned short x, unsigned short y, SDL_Color color) {

	SDL_Rect rect;
	rect.x = x * upscale;
	rect.y = y * upscale;
	rect.w = upscale;
	rect.h = upscale;

	SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, color.r, color.g, color.b));
}

void SDLWrapper::UpdateWindowSurface() {
	SDL_UpdateWindowSurface(window);
}

void SDLWrapper::ShutDown() {
	SDL_DestroyWindow(window);

	SDL_Quit();
}