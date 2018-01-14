#include "main.h"
#include "Texture.h"
#include "Creature.h"
#include "Environment.h"

Texture::Texture(){
	width = 100;
	height = 100;

	texture = NULL;
}

//Textur laden
bool Texture::loadTexture(std::string path){
	//Die finale Textur
	SDL_Texture* newTexture = NULL;	

	//die bereits existierende Textur freigeben
	freeTexture();

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cerr << "!=!FEHLER!=! Bild konnte nicht geladen werden! error: " << IMG_GetError() << std::endl;
	}
	else {
		//colorKey der Textur
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 150, 170, 190));

		//Textur aus den Pixeln des Surfaces erstellen
		newTexture = SDL_CreateTextureFromSurface(mainRenderer, loadedSurface);
		if (newTexture == NULL) {
			std::cerr << "!=!FEHLER!=! Spielertextur konnte nicht erstellt werden! Error: " << SDL_GetError() << std::endl;
		}
		else {
			//Bilddimensionen aufrufen
			if (width == 0) {
				width = loadedSurface->w;
			}
			if (height == 0) {
				height = loadedSurface->h;
			}
		}
		//Surface wieder freigeben
		SDL_FreeSurface(loadedSurface);
	}

	texture = newTexture;
	return texture != NULL;
}

//Textur aufrufen
SDL_Texture *Texture::getTexture(){
	return texture;
}

//Rendern
void Texture::render(int x, int y){
	SDL_Rect renderQuad = { x - camera.x, y, 10, 10};

	SDL_RenderCopy(mainRenderer, texture, NULL, &renderQuad);
}
void Texture::render(int x, int y, int w, int h, SDL_Rect *clip, SDL_RendererFlip flip) {
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

//Textur freigeben
void Texture::freeTexture() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}
