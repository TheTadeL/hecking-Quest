#include "Projectile.h"

//Dimensionen
int Projectile::getWidth() {
	return width;
}
int Projectile::getHeight() {
	return height;
}
//

//Position
void Projectile::setPosX(int x) {
	posX = x;
}
void Projectile::setPosY(int y) {
	posY = y;
}

int Projectile::getPosX() {
	return posX;
}
int Projectile::getposY() {
	return posY;
}
//
