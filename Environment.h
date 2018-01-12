#pragma once

#include "main.h"

class Environment {
public:
	//Key für Renderflip
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	//Box zum Rendern
	SDL_Rect Box;

	//Konstruktor
	Environment();
	Environment(int x, int y, int w, int h);
	
	//Position
	float getPosX(),
		getPosY();
	void setPosX(float x),
		setPosY(float y);

	//Dimensionen
	int getWidth(),
		getHeight();
	void setWidth(int w),
		setHeight(int h);

	//Bewegung
	void moveLeft(float s);
	void moveRight(float s);
	
	//Textur
	void setTexture(SDL_Texture *t);

	//Rendern
	void render(float x, float y, int w, int h, SDL_Rect *clip, SDL_RendererFlip flip);

protected:
	//Position
	float posX,
		posY;

	//Dimensionen
	int width,
		height;
	
	//Textur
	SDL_Texture *texture;
};