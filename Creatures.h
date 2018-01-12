#pragma once

#include "main.h"
#include "Environment.h"

class Creatures {
public:
	//Konstruktor
	Creatures();
	Creatures(float x, float y, int w, int h, int hea, int str, float spe, float jSpe, float fSpe, int jumpH);
	//FrameVariabeln
	int walkFrame;
	int idleFrame;
	int jumpFrame;
	int fallFrame;
	int glideFrame;
	SDL_RendererFlip flipType;
	SDL_Rect currentFrame;

	bool sprintFlag;
	
	//Kollisionserkennung
	bool col;
	bool colLeft;
	bool colRight;
	bool colUp;
	bool colDown;
	bool checkCollision(Creatures obj);
	bool checkCollision(SDL_Rect rct);
    bool checkWhatCollision(Environment obj);
	void resetCollision();

	//State
	enum State { sIdle, sWalking, sFalling, sGliding, sJumping, sDucking, sPunching};
	enum Facing { fUp, fDown, fLeft, fRight };
	Facing facing;
	State state;

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

	//Charakterstats
	void setHealth(int h), 
		setStrength(int s), 
		setSpeed(float s), 
		setFallingSpeed(float s);
	int getHealth(), 
		getStrength(), 
		getJumpHeight();
	float getSpeed(), 
		getFallingSpeed(), 
		getJumpSpeed();
	void kill(), 
		revive();

	//Bewegung
	void moveUp(float s, bool r),
		moveDown(float s, bool r),
		moveLeft(float s, bool r),
		moveRight(float s, bool r);
	void setJumpHeightFlag(int x);
	int getJumpHeightFlag();

	//Hitbox
	void setHitbox(SDL_Rect hb);
	void setPunchHB(SDL_Rect hb);
	SDL_Rect getPunchHB();
	void setHitboxX(float x);
	void setHitboxY(float y);
	void setPunchHBX(int x);
	void setPunchHBY(int y);
	SDL_Rect getHitbox();

	//Texture
	void setTexture(SDL_Texture *t);
	//Rendern
	void render(float x, float y, int w, int h, SDL_Rect *clip, SDL_RendererFlip flip);

protected:	
	//Position
	float posX, posY;

	//Dimensionen
	int width, height;
	
	//Charakterstats
	int health, 
		strength, 
		jumpHeight, 
		jumpHeightFlag;
	float speed, 
		fallingSpeed, 
		jumpSpeed;

	//State
	bool alive;

	//Textur
	SDL_Texture *texture;
	
	//Hitbox
	SDL_Rect hitbox;
	SDL_Rect punchHB;
};
class Player : public Creatures {
public:
	Player(float x, float y, int w, int h, int hea, int str, float spe, float jSpe, float fSpe, int jumpH);

	static const int IDLE_FRAMES_COUNT = 2;
	static const int WALK_FRAMES_COUNT = 4;
	static const int JUMP_FRAMES_COUNT = 3;
	static const int FALL_FRAMES_COUNT = 1;
	static const int GLIDE_FRAMES_COUNT = 2;
	static const int DUCK_FRAMES_COUNT = 1;
	static const int PUNCH_FRAMES_COUNT = 1;
	
	static const int IDLE_FRAMES_SPEED = 40;
	static const int WALK_FRAMES_SPEED = 8;
	static const int JUMP_FRAMES_SPEED = 4;
	static const int FALL_FRAMES_SPEED = 14;
	static const int GLIDE_FRAMES_SPEED = 4;
	static const int DUCK_FRAMES_SPEED = 1;
	static const int PUNCH_FRAMES_SPEED = 1;

	SDL_Rect idleFrameclip[IDLE_FRAMES_COUNT];
	SDL_Rect walkFrameclip[WALK_FRAMES_COUNT];
	SDL_Rect jumpFrameclip[JUMP_FRAMES_COUNT];
	SDL_Rect fallFrameclip[FALL_FRAMES_COUNT];
	SDL_Rect glideFrameclip[GLIDE_FRAMES_COUNT];
	SDL_Rect duckFrameclip[DUCK_FRAMES_COUNT];
	SDL_Rect punchFrameclip[DUCK_FRAMES_COUNT];
	SDL_Rect dedFrameclip;

private:

};

class Bat : public Creatures {
public:
	Bat();

	static const int IDLE_FRAMES_COUNT = 2;
	static const int HIT_FRAMES_COUNT = 1;

	static const int IDLE_FRAMES_SPEED = 10;
	static const int WALK_FRAMES_SPEED = 1;

	SDL_Rect idleFrameclip[IDLE_FRAMES_COUNT];
	SDL_Rect hitFrameclip[HIT_FRAMES_COUNT];
	
	void followPlayer(Player player);
private:
	
};