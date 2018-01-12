#pragma once

#include "main.h"
#include "Textures.h"

class Projectile {
protected:
	//Dimensionen
	int width, height;

	//Position
	int posX, posY;

	//Stats
	int speed;
	SDL_Texture *texture;

public:
	//Dimensionen
	int getWidth(),
		getHeight();

	//Position
	void setPosX(int x),
		setPosY(int y);
	int getPosX(),
		getposY();
};