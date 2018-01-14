#pragma once

#include "main.h"

class Texture {
public:
	//Constructor
	Texture();
	
	//Textur laden
	bool loadTexture(std::string path);
	//Textur aufrufen
	SDL_Texture *getTexture();
	//Textur freigeben
	void freeTexture();
	//Rendern
	void render(int x, int y);
	void render(int x, int y, int w, int h, SDL_Rect *clip, SDL_RendererFlip flip);

private:
	//Dimensionen
	int width,
		height;

	//Textur
	SDL_Texture *texture;
};