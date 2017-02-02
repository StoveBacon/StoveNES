#include "SDLWrapper.h"

bool SDLWrapper::Initialize(int width, int height, int upscale) {
	this->upscale = upscale;
	screenWidth = width * upscale;
	screenHeight = height * upscale;

	a = b = up = down = left = right = start = select = false;

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
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

unsigned short SDLWrapper::GetKeys() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_a:
				left = true;
				break;
			case SDLK_s:
				down = true;
				break;
			case SDLK_d:
				right = true;
				break;
			case SDLK_w:
				up = true;
				break;
			case SDLK_j:
				a = true;
				break;
			case SDLK_k:
				b = true;
				break;
			case SDLK_l:
				start = true;
				break;
			case SDLK_ESCAPE:
				select = true;
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_a:
				left = false;
				break;
			case SDLK_s:
				down = false;
				break;
			case SDLK_d:
				right = false;
				break;
			case SDLK_w:
				up = false;
				break;
			case SDLK_j:
				a = false;
				break;
			case SDLK_k:
				b = false;
				break;
			case SDLK_l:
				start = false;
				break;
			case SDLK_ESCAPE:
				select = false;
				break;
			}
			break;
		}
	}
	unsigned short keys = 0;
	keys |= a << 7;
	keys |= b << 6;
	keys |= select << 5;
	keys |= start << 4;
	keys |= up << 3;
	keys |= down << 2;
	keys |= left << 1;
	keys |= right;
	return keys;
}

void SDLWrapper::ShutDown() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}