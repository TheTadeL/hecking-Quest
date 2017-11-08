#include "main.h"
#include "Textures.h"
#include "Creatures.h"

/* Fensterdimensionen */
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

/* globale SDL-Variabeln */
SDL_Window* mainWindow = NULL;		//Hauptfenster
SDL_Surface* mainWinSurface = NULL;	//Surface das an mainWindow gebunden ist
SDL_Renderer* mainRenderer = NULL;	//Hauptrenderer

/* globale SDL-Funktionen */
bool init();		//Initialisiert alle SDL- und SDL-Sub-Systeme
bool loadMedia();	//lädt alle Medien
void close();		//beendet SDL und alle Subsysteme

/* Texturen Objekte */
Textures p1Tex;
Textures batTex;

/* Creatures Objekte */
Player p1 = {10, 10, 45, 100, 1000, 5, 4 };
void p1_init();
void p1_setSprites();
//
Bat bat1 = { 10, 10, 45, 100, 1000, 5, 4 };
void bat_init();
void bat_setSprites();

int main(int argc, char* args[]) {	
	//SDL initialisieren
	if (!init()) {
		std::cerr << "init() = false" << std::endl;
	}
	else {
		//Medien laden
		if (!loadMedia()) {
			std::cerr << "loadMedia() = false" << std::endl;
		}
	}
	
	bool quit = false;
	SDL_Event e;

	/* === MAIN LOOP === */
	while (!quit) {
		//Eventhandler
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				//User requested quit
			case SDL_QUIT:
				quit = true;
				break;
			//wurde Taste gedrückt?
			case SDL_KEYDOWN:
				//welche Taste?
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
			}
		}
		/* === === Tastaturinput lesen === === */
		
		//Flag für Tastaturinput
		const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
		//Sprinten
		if (currentKeyState[SDL_SCANCODE_LSHIFT] && p1.state != p1.sIdle) {
			p1.sprintFlag = true;
			p1.walkFrame++;
		}
		else {
			p1.sprintFlag = false;
		}
		//
		if (currentKeyState[SDL_SCANCODE_UP]) {
			p1.facing = p1.fUp;
			p1.state = p1.sWalkingU;
			p1.moveUp(p1.getSpeed(), p1.sprintFlag);
		}
		if (currentKeyState[SDL_SCANCODE_DOWN]) {
			p1.facing = p1.fDown;
			p1.state = p1.sWalkingD;
			p1.moveDown(p1.getSpeed(), p1.sprintFlag);
		}
		if (currentKeyState[SDL_SCANCODE_LEFT]) {
			p1.facing = p1.fLeft;
			p1.state = p1.sWalkingL;
			p1.moveLeft(p1.getSpeed(), p1.sprintFlag);
		}
		if (currentKeyState[SDL_SCANCODE_RIGHT]) {
			p1.facing = p1.fRight;
			p1.state = p1.sWalkingR;
			p1.moveRight(p1.getSpeed(), p1.sprintFlag);
		}
		//Wenn nicht gelaufen wird
		if (!currentKeyState[SDL_SCANCODE_UP] && !currentKeyState[SDL_SCANCODE_DOWN] && !currentKeyState[SDL_SCANCODE_LEFT] && !currentKeyState[SDL_SCANCODE_RIGHT]) {
			p1.state = p1.sIdle;
		}

		/* === === === === === === === === === */
		/* === === Animationen loopen === === */
		
		if ((p1.state == p1.sWalkingD||p1.state == p1.sWalkingU) && p1.walkFrame >= p1.WALK_FRAMES_COUNT*p1.WALK_FRAMES_SPEED) {
			p1.walkFrame = 0;
		}
		if ((p1.state == p1.sWalkingL || p1.state == p1.sWalkingR) && p1.walkFrame >= p1.WALK_FRAMES_COUNT*p1.WALK_FRAMES_SPEED_S) {
			p1.walkFrame = 0;
		}
		if (p1.idleFrame >= p1.IDLE_FRAMES_COUNT*p1.IDLE_FRAMES_SPEED) {
			p1.idleFrame = 0;
		}
		//Bat1
		bat1.currentFrame = bat1.idleDownFrameclip[bat1.idleFrame / bat1.IDLE_FRAMES_SPEED];
		if (bat1.idleFrame >= bat1.IDLE_FRAMES_COUNT*bat1.IDLE_FRAMES_SPEED-1) {
			bat1.idleFrame = 0;
		}

		/* === === === === === === === === === */		
		/* === === State Event === === */
		
		switch (p1.state) {
		//Laufen
		case p1.sWalkingD:
			p1.flipType = SDL_FLIP_NONE;
			p1.idleFrame = 0;
			p1.currentFrame = p1.walkDownFrameclip[p1.walkFrame/p1.WALK_FRAMES_SPEED];
			break;
		case p1.sWalkingU:
			p1.flipType = SDL_FLIP_NONE;
			p1.idleFrame = 0;
			p1.currentFrame = p1.walkUpFrameclip[p1.walkFrame / p1.WALK_FRAMES_SPEED];
			break;
		case p1.sWalkingL:
			p1.flipType = SDL_FLIP_NONE;
			p1.idleFrame = 0;
			p1.currentFrame = p1.walkSideFrameclip[p1.walkFrame / p1.WALK_FRAMES_SPEED_S];
			break;
		case p1.sWalkingR:
			p1.flipType = SDL_FLIP_HORIZONTAL;
			p1.idleFrame = 0;
			p1.currentFrame = p1.walkSideFrameclip[p1.walkFrame / p1.WALK_FRAMES_SPEED_S];
			break;
		//Idle
		case p1.sIdle:
			p1.walkFrame = 0;
			//In welche richtung wird geschaut?
			switch (p1.facing) {
			case p1.fUp:
				p1.currentFrame = p1.idleUpFrameclip[p1.idleFrame / p1.IDLE_FRAMES_SPEED];
				break;
			case p1.fDown:
				p1.currentFrame = p1.idleDownFrameclip[p1.idleFrame / p1.IDLE_FRAMES_SPEED];
				break;
			}
			break;
		}

		//Animation cyclen
		//player1
		p1.walkFrame++;
		p1.idleFrame++;
		//bat1
		bat1.idleFrame++;

		//Hitbox versetzen
		p1.setHitboxX(p1.getPosX());
		p1.setHitboxY(p1.getPosY() + p1.getHeight()/3);
		bat1.setHitboxX(bat1.getPosX() + bat1.getWidth()/4);
		bat1.setHitboxY(bat1.getPosY());
		/* === === === === === === ===*/

		/*====================TEST====================*/
		
		bat1.followPlayer(p1);

		/*============================================*/

		/* === RenderVorgang === */		
		SDL_SetRenderDrawColor(mainRenderer, 42, 142, 0, 0xFF);
		SDL_RenderClear(mainRenderer);
		bat1.render(bat1.getPosX(), bat1.getPosY(), bat1.getWidth(), bat1.getHeight(), &bat1.currentFrame, bat1.flipType);
		p1.render(p1.getPosX(), p1.getPosY(), p1.getWidth(), p1.getHeight(), &p1.currentFrame, p1.flipType);
		SDL_SetRenderDrawColor(mainRenderer, 255, 0, 0, 0xFF);
		SDL_RenderDrawRect(mainRenderer, &bat1.getHitbox());
		SDL_SetRenderDrawColor(mainRenderer, 0, 0, 255, 0xFF);
		SDL_RenderDrawRect(mainRenderer, &p1.getHitbox());

		//Bild updaten
		SDL_RenderPresent(mainRenderer);
		/* === === === === === === */
	}	//Ende main-loop

	//Ressourcen leeren und SDL-Subsysteme herunterfahren
	close();
	return 0;
}	//Ende main-funktion


bool init() {
	bool success = true;

	//SDL initialisieren
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL konnte nicht initialisiert werden! SDL_Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			std::cerr << "Warnung: lineare Texturfilterung ist nicht aktiviert!" << std::endl;
		}
		else{
			//Fenster erstellen
			mainWindow = SDL_CreateWindow("=+=+=+=+=+= TadeL's Game =+=+=+=+=+=", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if (mainWindow == NULL) {
				std::cerr << "Fenster konnte nicht erstellt werden! SDL_Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else {
				//Renderer erstellen
				mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (mainRenderer == NULL) {
					std::cerr << "Renderer konnte nicht erstellt werden! SDL_Error: " << SDL_GetError() << std::endl;
					success = false;
				}
				else {
					//Rendererfarbe festlegen
					SDL_SetRenderDrawColor(mainRenderer, 42, 142, 0, 0xFF);

					//PNG-loader initialisieren
					int imgFlags = IMG_INIT_PNG;
					if (!(IMG_Init(imgFlags) & imgFlags)) {
						std::cerr << "SDL_image konnte nicht initialisiert werden! Error: " << IMG_GetError() << std::endl;
						success = false;
					}
				}
			}
		}
	}
	//3 Sekunden Delay um Error-Meldungen lesen zu können
	if (!success) {
		SDL_Delay(3 * 1000);
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	//Player 1 initialisieren
	if (!p1Tex.loadTexture("Unbenannt.png")) {
		std::cerr << "p1 Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
	}
	p1_setSprites();	//Sprites setzen
	/* === */
	if (!batTex.loadTexture("eyyy.png")) {
		std::cerr << "bat Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
	}
	p1_init();
	bat_init();		//Werte setzen
	bat_setSprites();
	return success;
}

void p1_init() {
	p1.setTexture(p1Tex.getTexture());
	SDL_Rect newHitbox;
	newHitbox.w = bat1.getWidth();
	newHitbox.h = bat1.getHeight()/2;
	newHitbox.x = bat1.getPosX() + bat1.getWidth() - newHitbox.w;
	newHitbox.y = bat1.getPosY();
	p1.setHitbox(newHitbox);
}

void bat_init() {
	bat1.setTexture(batTex.getTexture());
	bat1.setHeight(40);
	bat1.setWidth(90);
	//Charakterstats
	bat1.setSpeed(1);
	//Hitbox
	SDL_Rect newHitbox;
	newHitbox.w = bat1.getWidth()/2;
	newHitbox.h = bat1.getHeight();
	newHitbox.x = bat1.getPosX() + bat1.getWidth() -newHitbox.w;
	newHitbox.y = bat1.getPosY();
	bat1.setHitbox(newHitbox);
}

void p1_setSprites() {
	//IdleUp
	//
	p1.idleUpFrameclip[0].x = 0;
	p1.idleUpFrameclip[0].y = 100;
	p1.idleUpFrameclip[0].w = 45;
	p1.idleUpFrameclip[0].h = 100;

	p1.idleUpFrameclip[1].x = 45;
	p1.idleUpFrameclip[1].y = 100;
	p1.idleUpFrameclip[1].w = 45;
	p1.idleUpFrameclip[1].h = 100;
	//
	//IdleDown
	//
	p1.idleDownFrameclip[0].x = 0;
	p1.idleDownFrameclip[0].y = 0;
	p1.idleDownFrameclip[0].w = 45;
	p1.idleDownFrameclip[0].h = 100;

	p1.idleDownFrameclip[1].x = 45;
	p1.idleDownFrameclip[1].y = 0;
	p1.idleDownFrameclip[1].w = 45;
	p1.idleDownFrameclip[1].h = 100;
	//
	//WalkDown
	//
	p1.walkDownFrameclip[0].x = 90;
	p1.walkDownFrameclip[0].y = 0;
	p1.walkDownFrameclip[0].w = 45;
	p1.walkDownFrameclip[0].h = 100;

	p1.walkDownFrameclip[1].x = 135;
	p1.walkDownFrameclip[1].y = 0;
	p1.walkDownFrameclip[1].w = 45;
	p1.walkDownFrameclip[1].h = 100;

	p1.walkDownFrameclip[2].x = 90;
	p1.walkDownFrameclip[2].y = 0;
	p1.walkDownFrameclip[2].w = 45;
	p1.walkDownFrameclip[2].h = 100;

	p1.walkDownFrameclip[3].x = 135;
	p1.walkDownFrameclip[3].y = 0;
	p1.walkDownFrameclip[3].w = 45;
	p1.walkDownFrameclip[3].h = 100;
	//
	//WalkUp
	//
	p1.walkUpFrameclip[0].x = 90;
	p1.walkUpFrameclip[0].y = 100;
	p1.walkUpFrameclip[0].w = 45;
	p1.walkUpFrameclip[0].h = 100;

	p1.walkUpFrameclip[1].x = 135;
	p1.walkUpFrameclip[1].y = 100;
	p1.walkUpFrameclip[1].w = 45;
	p1.walkUpFrameclip[1].h = 100;

	p1.walkUpFrameclip[2].x = 90;
	p1.walkUpFrameclip[2].y = 100;
	p1.walkUpFrameclip[2].w = 45;
	p1.walkUpFrameclip[2].h = 100;

	p1.walkUpFrameclip[3].x = 135;
	p1.walkUpFrameclip[3].y = 100;
	p1.walkUpFrameclip[3].w = 45;
	p1.walkUpFrameclip[3].h = 100;
	/*=========*/

	//
	//WalkSide
	//
	p1.walkSideFrameclip[0].x = 90;
	p1.walkSideFrameclip[0].y = 200;
	p1.walkSideFrameclip[0].w = 45;
	p1.walkSideFrameclip[0].h = 100;

	p1.walkSideFrameclip[1].x = 135;
	p1.walkSideFrameclip[1].y = 200;
	p1.walkSideFrameclip[1].w = 45;
	p1.walkSideFrameclip[1].h = 100;

	p1.walkSideFrameclip[2].x = 180;
	p1.walkSideFrameclip[2].y = 200;
	p1.walkSideFrameclip[2].w = 45;
	p1.walkSideFrameclip[2].h = 100;

	p1.walkSideFrameclip[3].x = 225;
	p1.walkSideFrameclip[3].y = 200;
	p1.walkSideFrameclip[3].w = 45;
	p1.walkSideFrameclip[3].h = 100;

}
void bat_setSprites() {
	//
	//IdleDown
	//
	bat1.idleDownFrameclip[0].x = 0;
	bat1.idleDownFrameclip[0].y = 0;
	bat1.idleDownFrameclip[0].w = 90;
	bat1.idleDownFrameclip[0].h = 40;

	bat1.idleDownFrameclip[1].x = 0;
	bat1.idleDownFrameclip[1].y = 40;
	bat1.idleDownFrameclip[1].w = 90;
	bat1.idleDownFrameclip[1].h = 40;

	bat1.idleDownFrameclip[2].x = 0;
	bat1.idleDownFrameclip[2].y = 80;
	bat1.idleDownFrameclip[2].w = 90;
	bat1.idleDownFrameclip[2].h = 40;

	bat1.idleDownFrameclip[3].x = 0;
	bat1.idleDownFrameclip[3].y = 0;
	bat1.idleDownFrameclip[3].w = 90;
	bat1.idleDownFrameclip[3].h = 40;
	//
}

void close() {
	//geladene Texturen freigeben
	//

	//Fenster und Renderer freigeben
	SDL_DestroyRenderer(mainRenderer);
	mainRenderer = NULL;
	SDL_DestroyWindow(mainWindow);
	mainWindow = NULL;
}