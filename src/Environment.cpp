#include "Environment.h"

Environment::Environment(){
	posX = 0;
	posY = 0;

	width = 0;
	height = 0;
}
Environment::Environment(int x, int y, int w, int h) {
	posX = x;
	posY = y;

	width = w;
	height = h;
	
	Box.x = 0;
	Box.y = 0;
	Box.w = w;
	Box.h = h;
}


//Position
float Environment::getPosX(){
	return posX;
}
float Environment::getPosY(){
	return posY;
}
void Environment::setPosX(float x){
	posX = x;
}
void Environment::setPosY(float y){
	posY = y;
}

//Dimensionen
int Environment::getWidth(){
	return width;
}
int Environment::getHeight(){
	return height;
}
void Environment::setWidth(int w) {
	width = w;
}
void Environment::setHeight(int h) {
	height = h;
}

//Bewegung
void Environment::moveLeft(float s) {
	posX -= s;
}
void Environment::moveRight(float s) {
	posX += s;
}

//Textur
void Environment::setTexture(SDL_Texture *t) {
	texture = t;
}

//Rendern
void Environment::render(float x, float y, int w, int h, SDL_Rect *clip, SDL_RendererFlip flip) {
	//Renderposition & Dimension
	SDL_Rect renderQuad = { x - camera.x, y, width, height };	//Renderquad wird generiert. Wenn w & h leer bleiben, wird width/height der Textur gebraucht

	//Wenn ein Rect angegeben wird, wird deren Dimension übernommen
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Wenn w & h angegeben werden, werden deren Dimensionen übernommen
	if (w != NULL && h != NULL) {
		renderQuad.w = w;
		renderQuad.h = h;
	}

	//definitiv aufs Bild rendern
	SDL_RenderCopyEx(mainRenderer, texture, clip, &renderQuad, NULL, NULL, flip);
}