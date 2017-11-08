#include "../SDL2-2.0.6/include/SDL.h"
#include "../SDL2_image-2.0.1/include/SDL_image.h"
#include <iostream>
#include <vector>

enum DmgMode;

/* Fensterdimensionen */
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

/* globale SDL-Variabeln */
extern SDL_Window* mainWindow;		//Hauptfenster
extern SDL_Surface* mainWinSurface;	//Surface das an mainWindow gebunden ist
extern SDL_Renderer* mainRenderer;	//Hauptrenderer
