#include "main.h"
#include "Textures.h"
#include "Creatures.h"
#include "Environment.h"

Creatures::Creatures(){
	//Kollisionserkennung
	colUp = false;
	colDown = false;
	colLeft = false;
	colRight = false;

	walkFrame = 0;
	idleFrame = 0;
	jumpFrame = 0;
	fallFrame = 0;
	glideFrame = 0;
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

Creatures::Creatures(float x, float y, int w, int h, int hea, int str, float spe, float jSpe, float fSpe, int jumpH) {
	//Kollisionserkennung
	colUp = false;
	colDown = false;
	colLeft = false;
	colRight = false;
	//Variabeln initialisieren
	walkFrame = 0;
	idleFrame = 0;
	jumpFrame = 0;
	fallFrame = 0;
	glideFrame = 0;
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
	fallingSpeed = fSpe;
	jumpHeight = jumpH;
}

//Kollisionserkennung
bool Creatures::checkCollision(Creatures obj){
	bool collisionX = false;
	bool collisionY = false;
	bool collision = false;
	if (hitbox.x <= obj.hitbox.x + obj.hitbox.w && hitbox.x + hitbox.w >= obj.hitbox.x) {
		collisionX = true;
	}
	if (hitbox.y <= obj.hitbox.y + obj.hitbox.h && hitbox.y + hitbox.h >= obj.hitbox.y) {
		collisionY = true;
	}
	if (collisionX && collisionY) {
		collision = true;
	}
	return collision;
}
bool Creatures::checkCollision(SDL_Rect rct) {
	bool collisionX = false;
	bool collisionY = false;
	bool collision = false;
	if (hitbox.x <= rct.x + rct.w && hitbox.x + hitbox.w >= rct.x) {
		collisionX = true;
	}
	if (hitbox.y <= rct.y + rct.h && hitbox.y + hitbox.h >= rct.y) {
		collisionY = true;
	}
	if (collisionX && collisionY) {
		collision = true;
	}
	return collision;
}

bool Creatures::checkWhatCollision(Environment obj){
	bool collisionX = false;
	bool collisionY = false;
	bool collision = false;
	if (hitbox.x <= (obj.getPosX() + obj.getWidth()) && hitbox.x + hitbox.w >= obj.getPosX()) {
		collisionX = true;
	}
	if (hitbox.y <= obj.getPosY() + obj.getHeight() && hitbox.y + hitbox.h >= obj.getPosY()) {
		collisionY = true;
	}
	if (collisionX && collisionY) {
		if (state == sFalling || state == sJumping || state == sGliding) {
			if (hitbox.x <= (obj.getPosX() + 4) + (obj.getWidth() - 8) && (hitbox.x + hitbox.w) >= (obj.getPosX() + 4)) {
				if ((hitbox.y + hitbox.h) - fallingSpeed < obj.getPosY()) {
					colDown = true;
					posY -= ((posY + height) - obj.getPosY());
				}
			}
		}
		else {
			if ((hitbox.y + hitbox.h) - fallingSpeed < obj.getPosY()) {
				colDown = true;
				posY -= ((posY + height) - obj.getPosY());
			}
		}
		if (hitbox.y + jumpSpeed > obj.getPosY() + obj.getHeight()) {
			colUp = true;
		}
		if ((hitbox.x + hitbox.w) - speed < obj.getPosX()) {
			colRight = true;
		} 
		if (hitbox.x + speed > obj.getPosX() + obj.getWidth()) {
			colLeft = true;
		}
		collision = true;
	}
	return collision;
} 


void Creatures::resetCollision() {
	colUp = false;
	colDown = false;
	colLeft = false;
	colRight = false;
}

//

//Position
float Creatures::getPosX(){
	return posX;
}
float Creatures::getPosY(){
	return posY;
}


void Creatures::setPosX(float x) {
	posX = x;
}
void Creatures::setPosY(float y) {
	posY = y;
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
void Creatures::setSpeed(float s) {
	speed = s;
}
float Creatures::getSpeed(){
	return speed;
}
void Creatures::setFallingSpeed(float s) {
	fallingSpeed = s;
}
float Creatures::getFallingSpeed() {
	return fallingSpeed;
}
float Creatures::getJumpSpeed() {
	return jumpSpeed;
}
int Creatures::getJumpHeight() {
	return jumpHeight;
}
//

//Bewegung
void Creatures::moveUp(float s, bool r){
	if (!colUp) {
		posY -= s;
		if (sprintFlag) {
			posY -= s / 2;
		}
	}
}
void Creatures::moveDown(float s, bool r) {
	if(!colDown){
		if (hitbox.y + hitbox.h <= LEVEL_HEIGHT) {
			posY += s;
			if (sprintFlag) {
				posY += s / 2;
			}
		}
	}
}
void Creatures::moveLeft(float s, bool r) {
	if (!colLeft) {
		posX -= s;
		if (sprintFlag) {
			posX -= s / 2;
		}
		if (posX < 0) {
			posX = 0;
		}
	}
}
void Creatures::moveRight(float s, bool r) {
	if (!colRight) {
		posX += s;
		if (sprintFlag) {
			posX += s / 2;
		}
		if (posX > LEVEL_WIDTH - width) {
			posX = LEVEL_WIDTH - width;
		}
	}
}

void Creatures::setJumpHeightFlag(int x) {
	jumpHeightFlag = x;
}
int Creatures::getJumpHeightFlag() {
	return jumpHeightFlag;
}

//Hitbox
void Creatures::setHitbox(SDL_Rect hb) {
	hitbox = hb;
}
void Creatures::setPunchHB(SDL_Rect hb) {
	punchHB = hb;
}
SDL_Rect Creatures::getPunchHB(){
	return punchHB;
}
void Creatures::setHitboxX(float x) {
	hitbox.x = x;
}
void Creatures::setHitboxY(float y) {
	hitbox.y = y;
}
void Creatures::setPunchHBX(int x) {
	punchHB.x = x;
}
void Creatures::setPunchHBY(int y) {
	punchHB.y = y;
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
void Creatures::render(float x, float y, int w, int h, SDL_Rect *clip, SDL_RendererFlip flip){
	//Renderposition & Dimension
	SDL_Rect renderQuad = { x - camera.x, y, width, height };	//Renderquad wird generiert. Wenn w & h leer bleiben, wird width/height der Textur gebraucht

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

Player::Player(float x, float y, int w, int h, int hea, int str, float spe, float jSpe, float fSpe, int jumpH) {
	//Kollisionserkennung
	colUp = false;
	colDown = false;
	colLeft = false;
	colRight = false;
	//Variabeln initialisieren
	walkFrame = 0;
	idleFrame = 0;
	jumpFrame = 0;
	fallFrame = 0;
	glideFrame = 0;
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
	fallingSpeed = fSpe;
	jumpSpeed = jSpe;
	jumpHeight = jumpH;
	jumpHeightFlag = jumpH;
}

/* === === ==== === ==== === === === === */

/* === === ==== === BAT === === === === */
Bat::Bat(){
	//Kollisionserkennung
	colUp = false;
	colDown = false;
	colLeft = false;
	colRight = false;
	//Variabeln initialisieren
	idleFrame = 0;
	alive = true;
	//Position
	posX = rand()%LEVEL_WIDTH+200;
	posY = rand()%LEVEL_HEIGHT+200;
	//Dimensionen
	width = 90;
	height = 45;
	//Charakterstats
	health = 1;
	speed = 1;
}

void Bat::followPlayer(Player player){
	if (player.getHitbox().x < posX ) {
		moveLeft(speed, false);
	}
	if (player.getHitbox().x > posX) {
		moveRight(speed, false);
	}
	if (player.getHitbox().y + player.getHeight() / 2 < posY) {
		moveUp(speed, false);
	}
	if (player.getHitbox().y + player.getHeight() / 6 > posY) {
		moveDown(speed, false);
	}
}

/* === === ==== === ==== === === === === */