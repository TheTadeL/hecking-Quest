#include "main.h"
#include "Textures.h"
#include "Creatures.h"

Creatures::Creatures(){
	walkFrame = 0;
	idleFrame = 0;
	sprintFlag = false;

	posX = 0;
	posY = 0;

	width = 0;
	height = 0;

	health = 1;
	strength = 1;
	speed = 1;

	alive = true;
}

Creatures::Creatures(Textures tex, int x, int y, int w, int h, int hea, int str, int spe) {
	//Variabeln initialisieren
	walkFrame = 0;
	idleFrame = 0;
	sprintFlag = false;
	alive = true;
	//Position
	posX = x;
	posY = y;
	//Dimensionen
	width = w;
	height = h;
	//Charakterstats
	health = hea;
	strength = str;
	speed = spe;
}

//Position
int Creatures::getPosX(){
	return posX;
}
int Creatures::getPosY(){
	return posY;
}


void Creatures::setPosX(int x) {
	posX = x;
}
void Creatures::setPosY(int y) {
	posX = y;
}
//

//Dimensionen
int Creatures::getHeight(){
	return height;
}
int Creatures::getWidth(){
	return width;
}

void Creatures::setWidth(int w){
	width = w;
}
void Creatures::setHeight(int h){
	height = h;
}
//

//Charakterstats
void Creatures::setHealth(int h){
	health = h;
}
void Creatures::setStrength(int s) {
	strength = s;
}
//

//Speed
void Creatures::setSpeed(int s) {
	speed = s;
}
int Creatures::getSpeed()
{
	return speed;
}
//

//Bewegung
void Creatures::moveUp(int s, bool r){
	if(hitbox.y >= 0){
		posY -= s;
		if (sprintFlag) {
			posY -= s / 2;
		}
	}
}
void Creatures::moveDown(int s, bool r) {
	if (hitbox.y + hitbox.w <= SCREEN_HEIGHT) {
		posY += s;
		if (sprintFlag) {
			posY += s / 2;
		}
	}
}
void Creatures::moveLeft(int s, bool r) {
	posX -= s;
	if (sprintFlag) {
		posX -= s/2;
	}
	if (posX < 0) {
		posX = 0;
	}
}
void Creatures::moveRight(int s, bool r) {
	posX += s;
	if (sprintFlag) {
		posX += s/2;
	}
	if (posX > SCREEN_WIDTH-width) {
		posX = SCREEN_WIDTH - width;
	}
}

//Hitbox
void Creatures::setHitbox(SDL_Rect hb) {
	hitbox = hb;
}
void Creatures::setHitboxX(int x) {
	hitbox.x = x;
}
void Creatures::setHitboxY(int y) {
	hitbox.y = y;
}
SDL_Rect Creatures::getHitbox() {
	return hitbox;
}
//

//Textur setzen
void Creatures::setTexture(SDL_Texture *t){
	texture = t;
}

//Rendern
void Creatures::render(int x, int y, int w, int h, SDL_Rect *clip, SDL_RendererFlip flip){
	//Renderposition & Dimension
	SDL_Rect renderQuad = { x, y, width, height };	//Renderquad wird generiert. Wenn w & h leer bleiben, wird width/height der Textur gebraucht

	//Wenn ein Rect angegeben wird, wird deren Dimension übernommen
	if(clip != NULL){
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

/* === === ==== === Player === === === === */

Player::Player(int x, int y, int w, int h, int hea, int str, int spe) {
	//Variabeln initialisieren
	walkFrame = 0;
	idleFrame = 0;
	sprintFlag = false;
	alive = true;
	//Position
	posX = x;
	posY = y;
	//Dimensionen
	width = w;
	height = h;
	//Charakterstats
	health = hea;
	strength = str;
	speed = spe;
}

/* === === ==== === ==== === === === === */

/* === === ==== === BAT === === === === */
Bat::Bat( int x, int y, int w, int h, int hea, int str, int spe) {
	//Variabeln initialisieren
	walkFrame = 0;
	idleFrame = 0;
	sprintFlag = false;
	alive = true;
	//Position
	posX = x;
	posY = y;
	//Dimensionen
	width = w;
	height = h;
	//Charakterstats
	health = hea;
	strength = str;
	speed = spe;
}

void Bat::followPlayer(Player player){
	if (player.getHitbox().x - player.getWidth() / 2 < posX) {
		moveLeft(speed, false);
	}
	if (player.getHitbox().x - player.getWidth() /2 > posX) {
		moveRight(speed, false);
	}
	if (player.getHitbox().y + player.getHeight() / 4 < posY) {
		moveUp(speed, false);
	}
	if (player.getHitbox().y - player.getHeight() / 4 > posY) {
		moveDown(speed, false);
	}
}

/* === === ==== === ==== === === === === */