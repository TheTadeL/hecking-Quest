#include "main.h"
#include "Texture.h"
#include "Creature.h"
#include "Environment.h"

Creature::Creature(){
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

Creature::Creature(float x, float y, int w, int h, int hea, int str, float spe, float jSpe, float fSpe, int jumpH) {
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
bool Creature::checkCollision(Creature obj){
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
bool Creature::checkCollision(SDL_Rect rct) {
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

bool Creature::checkWhatCollision(Environment obj){
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


void Creature::resetCollision() {
	colUp = false;
	colDown = false;
	colLeft = false;
	colRight = false;
}

//

//Position
float Creature::getPosX(){
	return posX;
}
float Creature::getPosY(){
	return posY;
}


void Creature::setPosX(float x) {
	posX = x;
}
void Creature::setPosY(float y) {
	posY = y;
}
//

//Dimensionen
int Creature::getHeight(){
	return height;
}
int Creature::getWidth(){
	return width;
}

void Creature::setWidth(int w){
	width = w;
}
void Creature::setHeight(int h){
	height = h;
}
//

//Charakterstats
void Creature::kill() {
	alive = false;
}
void Creature::revive() {
	alive = true;
}
bool Creature::isAlive() {
	return alive;
}
void Creature::setHealth(int h){
	health = h;
}
void Creature::setStrength(int s) {
	strength = s;
}
//

//Speed
void Creature::setSpeed(float s) {
	speed = s;
}
float Creature::getSpeed(){
	return speed;
}
void Creature::setFallingSpeed(float s) {
	fallingSpeed = s;
}
float Creature::getFallingSpeed() {
	return fallingSpeed;
}
float Creature::getJumpSpeed() {
	return jumpSpeed;
}
int Creature::getJumpHeight() {
	return jumpHeight;
}
//

//Bewegung
void Creature::moveUp(float s, bool r){
	if (!colUp) {
		posY -= s;
		if (sprintFlag) {
			posY -= s / 2;
		}
	}
}
void Creature::moveDown(float s, bool r) {
	if(!colDown){
		if (hitbox.y + hitbox.h <= LEVEL_HEIGHT) {
			posY += s;
			if (sprintFlag) {
				posY += s / 2;
			}
		}
	}
}
void Creature::moveLeft(float s, bool r) {
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
void Creature::moveRight(float s, bool r) {
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

void Creature::setJumpHeightFlag(int x) {
	jumpHeightFlag = x;
}
int Creature::getJumpHeightFlag() {
	return jumpHeightFlag;
}

//Hitbox
void Creature::setHitbox(SDL_Rect hb) {
	hitbox = hb;
}
void Creature::setPunchHB(SDL_Rect hb) {
	punchHB = hb;
}
SDL_Rect Creature::getPunchHB(){
	return punchHB;
}
void Creature::setHitboxX(float x) {
	hitbox.x = x;
}
void Creature::setHitboxY(float y) {
	hitbox.y = y;
}
void Creature::setPunchHBX(int x) {
	punchHB.x = x;
}
void Creature::setPunchHBY(int y) {
	punchHB.y = y;
}
SDL_Rect Creature::getHitbox() {
	return hitbox;
}
//

//Textur setzen
void Creature::setTexture(SDL_Texture *t){
	texture = t;
}

//Rendern
void Creature::render(float x, float y, int w, int h, SDL_Rect *clip, SDL_RendererFlip flip){
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
Wimp::Wimp(){
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

void Wimp::followPlayer(Player player){
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