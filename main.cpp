#include <SDL.h>
#undef main
#include <SDL_events.h>
#include <iostream>


SDL_Window* wind;
SDL_Event event;
SDL_Renderer* renderer;
bool running = true;

int space[100][100];
int spacebuffer[100][100];//each grain is 5 pixels
bool msedown = false;
int msex = 0;
int msey = 0;
int brushsize = 5;
void clearspace();
int initiate_sdl (){
	int i = SDL_Init(SDL_INIT_EVERYTHING);
	wind = SDL_CreateWindow("Sand Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);
	renderer = SDL_CreateRenderer(wind, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	return i;
}

void handle_events() {
	while (SDL_PollEvent(&event) > 0) {
		if (event.type == SDL_QUIT) {
			running = false;
			SDL_Quit();
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				msedown = true;
			}
			
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == 21) {
				clearspace();
			}else if (event.key.keysym.scancode == 79) {
				if (brushsize < 20) {
					brushsize++;
				}
			}
			else if (event.key.keysym.scancode == 80) {
				if (brushsize > 1) {
					brushsize--;
				}
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				msedown = false;
			}
		}


	}
}


void renderspace() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_Rect r;
	r.w = 5;
	r.h = 5;
    
	
	
	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			if (space[x][y] == 1) {
				r.x = x * 5;
				r.y = y * 5;
				SDL_SetRenderDrawColor(renderer, 255, 100, 0, 0);
				SDL_RenderFillRect(renderer, &r);
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
				SDL_RenderDrawRect(renderer, &r);
			}
		}
	}

	SDL_RenderPresent(renderer);
}
void clearspace() {
	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			space[x][y] = 0;
		}
	}
}
void clearbuffer() {
	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			spacebuffer[x][y] = 0;
		}
	}
}

void processphysics() {
	clearbuffer();
	for (int xc = 0; xc < 100; xc++) {
		for (int yc = 0; yc < 100; yc++) {
			if (space[xc][yc] == 1) {
				
				if (yc < 99) {
					
					if (space[xc][yc + 1] == 1) {
						if (xc > 0 && (xc + 1) < 100) {
							if (space[xc][yc + 2] == 1 && (space[xc + 1][yc + 1] == 0 || space[xc - 1][yc + 1] == 0)) {
								if (space[xc + 1][yc + 1] == 0) {
									spacebuffer[xc + 1][yc + 1] = 1;
									spacebuffer[xc][yc] = 0;
								}
								else if (space[xc - 1][yc + 1] == 0) {
									spacebuffer[xc - 1][yc + 1] = 1;
									spacebuffer[xc][yc] = 0;
								}
							}
							else {
								spacebuffer[xc][yc] = 1;
							}
						}
						else {
							spacebuffer[xc][yc] = 1;
						}
						
					}
					else {
						spacebuffer[xc][yc] = 0;
						spacebuffer[xc][yc + 1] = 1;
					}

				}
				else {
					spacebuffer[xc][yc] = 1;
				}


			}
		}
	}
	for (int xin = 0; xin < 100; xin++) {
		for (int yin = 0; yin < 100; yin++) {
			space[xin][yin] = spacebuffer[xin][yin];
		}
	}
}

int main() {
	initiate_sdl();
	clearspace();
	SDL_Delay(10);
	while(running){
		
		handle_events(); //check for quit, mouse buttons down or keys


		

		if (msedown) {
			SDL_GetMouseState(&msex, &msey);//get mouse x and y
			if (msex < 500 && msex > 0 && msey < 500 && msey > 0) {
				
				int halfbrush = brushsize / 2;
				int newx;
				int newy;
				for (int brushx = 0; brushx < brushsize; brushx++) {
					for (int brushy = 0; brushy < brushsize; brushy++) {
						
						newx = ((msex / 5) - halfbrush) + brushx;
						newy = ((msey / 5) - halfbrush) + brushy;
						if (newx > 0 && newy > 0 && newx < 100 && newy < 100) {
							space[newx][newy] = 1;
						}
					}
				}
				
			}
		}

		processphysics();
		renderspace();

	}
	return 0;
}
