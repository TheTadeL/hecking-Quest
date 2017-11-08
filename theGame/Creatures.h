#include "main.h"

class Creatures {
public:
	Creatures();
	Creatures(Textures tex, int x, int y, int w, int h, int hea, int str, int spe);
	//FrameVariabeln
	int walkFrame;
	int idleFrame;
	SDL_RendererFlip flipType;
	SDL_Rect currentFrame;

	bool sprintFlag;

	//Frames	
	static const int IDLE_FRAMES_COUNT = 1;
	static const int WALK_FRAMES_COUNT = 1;
	static const int HIT_FRAMES_COUNT = 1;
	static const int DUCK_FRAMES_COUNT = 1;

	static const int IDLE_FRAMES_SPEED = 1;
	static const int WALK_FRAMES_SPEED = 1;
	static const int WALK_FRAMES_SPEED_S = 1;
	static const int HIT_FRAMES_SPEED = 1;
	static const int DUCK_FRAMES_SPEED = 1;

	SDL_Rect idleUpFrameclip[IDLE_FRAMES_COUNT];
	SDL_Rect idleDownFrameclip[IDLE_FRAMES_COUNT];
	SDL_Rect idleSideFrameclip[IDLE_FRAMES_COUNT];

	SDL_Rect walkUpFrameclip[WALK_FRAMES_COUNT];
	SDL_Rect walkDownFrameclip[WALK_FRAMES_COUNT];
	SDL_Rect walkSideFrameclip[WALK_FRAMES_COUNT];

	SDL_Rect hitFrameclip[HIT_FRAMES_COUNT];
	SDL_Rect duckFrameclip[DUCK_FRAMES_COUNT];

	//State
	enum State { sIdle, sWalkingU, sWalkingD, sWalkingL, sWalkingR };
	enum Facing { fUp, fDown, fLeft, fRight };
	Facing facing;
	State state;

	//Position
	int getPosX(),
		getPosY();
	void setPosX(int x),
		 setPosY(int y);
	
	//Dimensionen
	int getWidth(),
		getHeight();
	void setWidth(int w),
		setHeight(int h);

	//Charakterstats
	void setHealth(int h),
		setStrength(int s),
		setSpeed(int s);
	int getHealth(),
		getStrength(),
		getSpeed();
	void kill(),
		revive();

	//Bewegung
	void moveUp(int s, bool r),
		moveDown(int s, bool r),
		moveLeft(int s, bool r),
		moveRight(int s, bool r);

	//Hitbox
	void setHitbox(SDL_Rect hb);
	void setHitboxX(int x);
	void setHitboxY(int y);
	SDL_Rect getHitbox();

	//Texture
	void setTexture(SDL_Texture *t);
	//Rendern
	void render(int x, int y, int w, int h, SDL_Rect *clip, SDL_RendererFlip flip);

protected:	
	//Position
	int posX,
		posY;

	//Dimensionen
	int width,
		height;
	
	//Charakterstats
	int health,
		strength,
		speed;

	//State
	bool alive;

	//Textur
	SDL_Texture *texture;
	
	//Hitbox
	SDL_Rect hitbox;
};

class Player : public Creatures {
public:
	Player(int x, int y, int w, int h, int hea, int str, int spe);
	
	static const int IDLE_FRAMES_COUNT = 2;
	static const int WALK_FRAMES_COUNT = 4;
	static const int HIT_FRAMES_COUNT = 1;
	static const int DUCK_FRAMES_COUNT = 1;
	
	static const int IDLE_FRAMES_SPEED = 26;
	static const int WALK_FRAMES_SPEED = 20;
	static const int WALK_FRAMES_SPEED_S = 8;
	static const int HIT_FRAMES_SPEED = 1;
	static const int DUCK_FRAMES_SPEED = 1;

	SDL_Rect idleUpFrameclip[IDLE_FRAMES_COUNT];
	SDL_Rect idleDownFrameclip[IDLE_FRAMES_COUNT];
	SDL_Rect idleSideFrameclip[IDLE_FRAMES_COUNT];

	SDL_Rect walkUpFrameclip[WALK_FRAMES_COUNT];
	SDL_Rect walkDownFrameclip[WALK_FRAMES_COUNT];
	SDL_Rect walkSideFrameclip[WALK_FRAMES_COUNT];

	SDL_Rect hitFrameclip[HIT_FRAMES_COUNT];
	SDL_Rect duckFrameclip[DUCK_FRAMES_COUNT];
};

class Bat : public Creatures {
public:
	Bat( int x, int y, int w, int h, int hea, int str, int spe);
	
	static const int IDLE_FRAMES_COUNT = 4;
	static const int HIT_FRAMES_COUNT = 1;
	static const int DUCK_FRAMES_COUNT = 1;

	static const int IDLE_FRAMES_SPEED = 10;
	static const int WALK_FRAMES_SPEED = 1;
	static const int WALK_FRAMES_SPEED_S = 1;
	static const int HIT_FRAMES_SPEED = 1;
	static const int DUCK_FRAMES_SPEED = 1;

	SDL_Rect idleUpFrameclip[IDLE_FRAMES_COUNT];
	SDL_Rect idleDownFrameclip[IDLE_FRAMES_COUNT];
	SDL_Rect idleSideFrameclip[IDLE_FRAMES_COUNT];

	SDL_Rect walkUpFrameclip[WALK_FRAMES_COUNT];
	SDL_Rect walkDownFrameclip[WALK_FRAMES_COUNT];
	SDL_Rect walkSideFrameclip[WALK_FRAMES_COUNT];

	SDL_Rect hitFrameclip[HIT_FRAMES_COUNT];
	SDL_Rect duckFrameclip[DUCK_FRAMES_COUNT];
	
	void followPlayer(Player player);
private:
	
};