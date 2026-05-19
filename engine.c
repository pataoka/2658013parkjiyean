#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>			
#include "engine.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
TTF_Font* font;				// 폰트 객체를 가리키는 포인터

bool engine_init()
{
	// initialize SDL and create window
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return 0;
	// initialize SDL_ttf
	if (TTF_Init() != 0)
		return 0;
	//Create window
	window = SDL_CreateWindow("raisingFish", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);		//우리말로 렌더러는 화면에 그리는 도구라고 생각하면 된다. 창과 렌더러는 세트로 생각하면 된다.
	if (!window)
		return 0;
	// initialize other engine resources
	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24); // 폰트 파일과 크기 설정
	if (!font) {
		printf("Failed to load font: %s\n", TTF_GetError());
		return 0;
	}
	// ...
	return 1;
}

void engine_update()
{
	// Update game logic and other engine resources
	// ...
}

void draw_text(const char* text, int x, int y)
{
	SDL_Color color = { 255, 255, 255, 255 };   // 흰색 글자

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);		// 텍스트를 렌더링하여 표면(surface) 생성

	if (!surface)
		return;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);	// 표면을 텍스처로 변환

	if (!texture) {
		SDL_FreeSurface(surface);
		return;
	}

	SDL_Rect dstRect;				// 텍스처를 그릴 위치와 크기를 정의하는 사각형 구조체
	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = surface->w;			// 텍스처의 너비와 높이를 표면의 너비와 높이로 설정
	dstRect.h = surface->h;			//

	SDL_RenderCopy(renderer, texture, NULL, &dstRect);		// 텍스처를 렌더러에 복사하여 화면에 그리기

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void engine_render()
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 검은색으로 설정
	SDL_RenderClear(renderer);
	// Render game objects and other engine resources
	// 사각형 그리기
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 빨간색으로 설정
	SDL_Rect rect = { 100, 100, 200, 200 }; // 사각형 위치와 크기 설정
	SDL_RenderFillRect(renderer, &rect); // 사각형 그리기

	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 50); // 파란색으로 설정
	SDL_Rect rect1 = { 400, 300, 100, 200 }; // 사각형 위치와 크기 설정
	SDL_RenderFillRect(renderer, &rect1); // 사각형 그리기
	
	SDL_SetRenderDrawColor(renderer, 0, 205, 100, 0); // 초록색으로 설정
	SDL_Rect rect2 = { 90, 370, 300, 150 }; // 사각형 위치와 크기 설정
	SDL_RenderDrawRect(renderer, &rect2); // 사각형 그리기

	//별
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

	SDL_RenderDrawLine(renderer, 300, 170, 359, 351);
	SDL_RenderDrawLine(renderer, 359, 351, 205, 239);
	SDL_RenderDrawLine(renderer, 205, 239, 395, 239);
	SDL_RenderDrawLine(renderer, 395, 239, 241, 351);
	SDL_RenderDrawLine(renderer, 241, 351, 300, 170);

	draw_text("2658013 parkjiyean", 300, 40);

	// Update the window
	SDL_RenderPresent(renderer);
}

void engine_handle_event(SDL_Event* e)
{
	// Handle input events and other engine events
	// ...
}

void engine_cleanup()
{
	// Clean up engine resources
	// ...
	// Destroy renderer
	SDL_DestroyRenderer(renderer);
	// Destroy window
	SDL_DestroyWindow(window);
	// Quit SDL subsytems
	SDL_Quit();
}