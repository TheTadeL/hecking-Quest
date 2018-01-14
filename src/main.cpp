#include "main.h"
#include "Texture.h"
#include "Creature.h"
#include "Environment.h"

/* Fensterdimensionen */
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

/* Leveldimensionen */
const int LEVEL_WIDTH = 2400;
const int LEVEL_HEIGHT = 800;

/* globale SDL-Variabeln */
SDL_Window* mainWindow = NULL;		//Hauptfenster
SDL_Surface* mainWinSurface = NULL;	//Surface das an mainWindow gebunden ist
SDL_Renderer* mainRenderer = NULL;	//Hauptrenderer
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; //Kamera für Levelscroll

/* globale SDL-Funktionen */
bool init();		//Initialisiert alle SDL- und SDL-Sub-Systeme
bool loadMedia();	//lädt alle Medien
void moveCamera();	//Kamera bewegen
void close();		//beendet SDL und alle Subsysteme

/* Texturen Objekte */
Texture p1Tex;
Texture wimpTex;
Texture wallTex;
Texture BG1Tex;
Texture BG2Tex;
Texture BG3Tex;
Texture dedTex;

/* Creature Objekte */
Player p1 = {10, 550, 45, 100, 1000, 5, 4, 7, 7, 21};
void p1_init();
void p1_setSprites();

/* Environment Objekte */
//Boden
Environment ground = {0, LEVEL_HEIGHT-50, LEVEL_WIDTH, 50};
//Backgrounds
Environment bg1 = { -50, 0, LEVEL_WIDTH+100, LEVEL_HEIGHT };
Environment bg2 = { -50, 0, LEVEL_WIDTH+100, LEVEL_HEIGHT };
Environment bg3 = { -50, 0, LEVEL_WIDTH+100, LEVEL_HEIGHT };
//Todesscreen (für Testzwecke)
Environment ded = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//Block-Array
std::vector<Environment> Wall(0);
//Array für Wimps
std::vector<Wimp> Wimps(1);
void wimp_init();
void wimp_setSprites();

int main(int argc, char* args[]) {	
	//Leveldesign
	//				X	|  		Y			 | W |  H |
	Wall.push_back({ 150, LEVEL_HEIGHT - 110, 50, 60 });
	Wall.push_back({ 270, LEVEL_HEIGHT - 170, 50, 50 });
	Wall.push_back({ 800, LEVEL_HEIGHT - 240, 400, 50 });
	Wall.push_back({ 1150, LEVEL_HEIGHT - 300, 50, 60 });
	Wall.push_back({ 1800, LEVEL_HEIGHT - 350, 70, 30 });
	Wall.push_back({ 1950, LEVEL_HEIGHT - 450, 70, 30 });
	Wall.push_back({ 1700, LEVEL_HEIGHT - 550, 110, 30 });
	Wall.push_back({ 1300, LEVEL_HEIGHT - 650, 100, 30 });

	
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

		/* === === Kollisionserkennung === === */
		//Walls
		p1.checkWhatCollision(ground);
		for (int i = 0; i < Wall.size(); i++) {
			p1.checkWhatCollision(Wall.at(i));
		}
		//Wimps

		/* === === === === === === === === === */

		/* === === Tastaturinput lesen === === */

		//Flag für Tastaturinput
		const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
		//Sprinten
		if (currentKeyState[SDL_SCANCODE_W] && p1.state == p1.sFalling) {
			p1.state = p1.sGliding;
		}
		
		if (!p1.colDown && p1.state != p1.sJumping && !currentKeyState[SDL_SCANCODE_W]) {
			p1.state = p1.sFalling;
		}

		if (currentKeyState[SDL_SCANCODE_LSHIFT] && p1.state != p1.sIdle) {
			p1.sprintFlag = true;
			p1.walkFrame++;
			if (p1.colDown) {
				p1.state = p1.sWalking;
			}
		}
		else {
			p1.sprintFlag = false;
		}
		if (currentKeyState[SDL_SCANCODE_LEFT]||currentKeyState[SDL_SCANCODE_A]) {
			p1.facing = p1.fLeft;
			p1.moveLeft(p1.getSpeed(), p1.sprintFlag);
			//Backgrounds relativ zum Spieler bewegen
			if (camera.x > 0 && camera.x + camera.w < LEVEL_WIDTH && !p1.colLeft && !p1.colRight) {
				if (bg1.getPosX() < 0) {bg1.moveRight(p1.getSpeed());}
				if (bg2.getPosX() < 0) { bg2.moveRight(2); }
				if (bg3.getPosX() < 0) { bg3.moveRight(1.5); }
			}
			if (p1.colDown) {
				p1.state = p1.sWalking;
			}
		}
		if (currentKeyState[SDL_SCANCODE_RIGHT] || currentKeyState[SDL_SCANCODE_D]) {
			p1.facing = p1.fRight;
			p1.moveRight(p1.getSpeed(), p1.sprintFlag);
			//Backgrounds relativ zum Spieler bewegen
			if (camera.x > 0 && camera.x + camera.w < LEVEL_WIDTH && !p1.colLeft && !p1.colRight) {
				bg1.moveLeft(p1.getSpeed());
				bg2.moveLeft(2);
				bg3.moveLeft(1.5);
			}
			if (p1.colDown) {
				p1.state = p1.sWalking;
			}
		}
		if (currentKeyState[SDL_SCANCODE_SPACE] && p1.colDown) {
			p1.state = p1.sJumping;
		}
		//Wenn nicht gelaufen wird
		if (!currentKeyState[SDL_SCANCODE_LEFT] && !currentKeyState[SDL_SCANCODE_RIGHT] && !currentKeyState[SDL_SCANCODE_A] && !currentKeyState[SDL_SCANCODE_D] && p1.state != p1.sJumping && p1.colDown) {
			p1.state = p1.sIdle;
		}
		//Ducken
		if (currentKeyState[SDL_SCANCODE_S] && p1.colDown) {
			p1.state = p1.sDucking;
		}

		/*TEST INPUT*/
		if (currentKeyState[SDL_SCANCODE_P]) {
			p1.state = p1.sPunching;
		}
		//
		
		/* === === === === === === === === === */

		//Wimps animationen
		for (int i = 0; i < Wimps.size(); i++) {
			Wimps.at(i).currentFrame = Wimps.at(i).idleFrameclip[Wimps.at(i).idleFrame / Wimps.at(i).IDLE_FRAMES_SPEED];
			if (Wimps.at(i).idleFrame >= Wimps.at(i).IDLE_FRAMES_COUNT*Wimps.at(i).IDLE_FRAMES_SPEED - 1) {
				Wimps.at(i).idleFrame = 0;
			}
		}
		//frames++
		for (int i = 0; i < Wimps.size(); i++) {
			Wimps.at(i).idleFrame++;
		}
		/* === === === === === === === === === */		

		/* === === === === State Event === === === === */
		switch (p1.state) {
		//Laufen
		case p1.sWalking:
			//Animation loopen
			if (p1.state == p1.sWalking && p1.walkFrame >= p1.WALK_FRAMES_COUNT*p1.WALK_FRAMES_SPEED) {
				p1.walkFrame = 0;
			}
			if (p1.facing == p1.fRight) {
				p1.flipType = SDL_FLIP_HORIZONTAL;
			}
			else {
				p1.flipType = SDL_FLIP_NONE;
			}
			p1.jumpFrame = 0;
			p1.idleFrame = 0;
			p1.currentFrame = p1.walkFrameclip[p1.walkFrame / p1.WALK_FRAMES_SPEED];
			p1.walkFrame++;
			break;
		//Idle
		case p1.sIdle:
			//Animation loopen
			if (p1.idleFrame >= p1.IDLE_FRAMES_COUNT*p1.IDLE_FRAMES_SPEED) {
				p1.idleFrame = 0;
			}
			if (p1.facing == p1.fRight) {
				p1.flipType = SDL_FLIP_HORIZONTAL;
			}
			else {
				p1.flipType = SDL_FLIP_NONE;
			}
			p1.jumpFrame = 0;
			p1.walkFrame = 0;
			p1.currentFrame = p1.idleFrameclip[p1.idleFrame / p1.IDLE_FRAMES_SPEED];
			p1.idleFrame++;
			break;
		//Springen
		case p1.sJumping:
			//Animation loopen
			if (p1.colUp) {
				p1.state = p1.sFalling;
			}
			if (p1.jumpFrame >= p1.JUMP_FRAMES_COUNT*p1.JUMP_FRAMES_SPEED) {
				p1.jumpFrame--;
			}
			if (p1.getJumpHeightFlag() > 0) {
				p1.moveUp(p1.getJumpSpeed(), NULL);
				p1.setJumpHeightFlag(p1.getJumpHeightFlag() - 1);
			}
			else {
				p1.state = p1.sFalling;
				p1.setJumpHeightFlag(p1.getJumpHeight());
			}
			//RenderFlip
			if (p1.facing == p1.fRight) {
				p1.flipType = SDL_FLIP_HORIZONTAL;
			}
			else {
				p1.flipType = SDL_FLIP_NONE;
			}
			p1.walkFrame = 0;
			p1.currentFrame = p1.jumpFrameclip[p1.jumpFrame / p1.JUMP_FRAMES_SPEED];
			p1.jumpFrame++;
			break;
		//Fallen
		case p1.sFalling:
			p1.jumpFrame = 0;
			p1.setJumpHeightFlag(p1.getJumpHeight());
			//Animation loopen
			if (p1.fallFrame >= p1.FALL_FRAMES_COUNT*p1.FALL_FRAMES_SPEED) {
				p1.fallFrame = 0;
			}
			if (!p1.colDown) {
				p1.moveDown(p1.getFallingSpeed(), NULL);
			}
			//RenderFlip
			if (p1.facing == p1.fRight) {
				p1.flipType = SDL_FLIP_HORIZONTAL;
			}
			else {
				p1.flipType = SDL_FLIP_NONE;
			}
			p1.currentFrame = p1.fallFrameclip[0/*p1.fallFrame / p1.FALL_FRAMES_SPEED*/];
			p1.fallFrame++;
			break;
		//Gleiten
		case p1.sGliding:
			//Animation loopen
			if (p1.glideFrame >= p1.GLIDE_FRAMES_COUNT*p1.GLIDE_FRAMES_SPEED) {
				p1.glideFrame = 0;
			}
			if (!p1.colDown) {
				p1.moveDown(.7/*p1.getFallingSpeed()/3, NULL*/, NULL);
			}
			//RenderFlip
			if (p1.facing == p1.fRight) {
				p1.flipType = SDL_FLIP_HORIZONTAL;
			}
			else {
				p1.flipType = SDL_FLIP_NONE;
			}
			p1.currentFrame = p1.glideFrameclip[p1.glideFrame / p1.GLIDE_FRAMES_SPEED];
			p1.glideFrame++;
			break;
		//Ducken
		case p1.sDucking:
			p1.currentFrame = p1.duckFrameclip[0];
			break;
		//Schlagen
		case p1.sPunching:
			if (p1.facing == p1.fRight) {
				p1.flipType = SDL_FLIP_HORIZONTAL;
			}
			else {
				p1.flipType = SDL_FLIP_NONE;
			}
			if (p1.facing == p1.fLeft) {
				p1.setPunchHBX(p1.getPosX());
				p1.setPunchHBY(p1.getPosY()+10);
			}
			else {
				p1.setPunchHBX(p1.getPosX()+p1.getWidth()-p1.getPunchHB().w);
				p1.setPunchHBY(p1.getPosY() + 10);
			}			
			p1.currentFrame = p1.punchFrameclip[0];
		}
		//Hitbox versetzen
		p1.setHitboxX(p1.getPosX()+p1.getWidth()/4);
		p1.setHitboxY(p1.getPosY());
		if (p1.state != p1.sPunching) {
			p1.setPunchHBX(0);
			p1.setPunchHBY(0);
		}
		for (int i = 0; i < Wimps.size(); i++) {
			Wimps.at(i).setHitboxX(Wimps.at(i).getPosX() + Wimps.at(i).getWidth() / 4);
			Wimps.at(i).setHitboxY(Wimps.at(i).getPosY());
		}
		/* === === === === === === ===*/

		/*====================TEST====================*/
		p1.revive();
		for (int i = 0; i < Wimps.size(); i++) {
			if (p1.checkCollision(Wimps.at(i))) {
				//Placeholder zum Testen
				p1.kill();
				p1.currentFrame = p1.dedFrameclip;
			}
			
			if (Wimps.at(i).checkCollision(p1.getPunchHB())) {
				//Wimps.at(i).setPosX(-100);
				//Wimps.at(i).setPosY(-100);
				if (Wimps.at(i).flipType == SDL_FLIP_NONE) {
					Wimps.at(i).flipType = SDL_FLIP_VERTICAL;
				}
				else {
					Wimps.at(i).flipType = SDL_FLIP_NONE;
				}
			}
			else {
				Wimps.at(i).followPlayer(p1);
			}
		}	
		moveCamera();	//Kamera bewegen
		
		//SDL_Delay(100);

		/*============================================*/
		


		/* === RenderVorgang === */		
		SDL_RenderClear(mainRenderer);
		
		bg3.render(bg3.getPosX() + camera.x, bg3.getPosY(), bg3.getWidth(), bg3.getHeight(), NULL, SDL_FLIP_NONE);
		bg2.render(bg2.getPosX() + camera.x, bg2.getPosY()-200, bg2.getWidth(), bg2.getHeight(), NULL, SDL_FLIP_NONE);
		for (int i = 0; i < Wall.size(); i++) {
			Wall.at(i).render(Wall.at(i).getPosX(), Wall.at(i).getPosY(), Wall.at(i).getWidth(), Wall.at(i).getHeight(), &Wall.at(i).Box, Wall.at(i).flipType);
		}
		p1.render(p1.getPosX(), p1.getPosY(), p1.getWidth(), p1.getHeight(), &p1.currentFrame, p1.flipType);
		bg1.render(bg1.getPosX() + camera.x, bg1.getPosY(), bg1.getWidth(), bg1.getHeight(), NULL, SDL_FLIP_NONE);
		for (int i = 0; i < Wimps.size(); i++) {
			Wimps.at(i).render(Wimps.at(i).getPosX(), Wimps.at(i).getPosY(), Wimps.at(i).getWidth(), Wimps.at(i).getHeight(), &Wimps.at(i).currentFrame, Wimps.at(i).flipType);
		}
		if (!p1.isAlive()) {
			ded.render(ded.getPosX() + camera.x, ded.getPosY(), ded.getWidth(), ded.getHeight(), NULL, SDL_FLIP_NONE);
		}
		//Bild updaten
		SDL_RenderPresent(mainRenderer);
		/* === === === === === === */
		//Collision zurücksetzen
		p1.resetCollision();
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
	if (!p1Tex.loadTexture("sprites/player1.png")) {
		std::cerr << "p1 Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
	}
	p1_init();
	p1_setSprites();	//Sprites setzen
	/* === */
	if (!wimpTex.loadTexture("sprites/eyyy_red.png")) {
		std::cerr << "wimp Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
	}
	wimp_init();		//Werte setzen
	wimp_setSprites();
	if (!BG1Tex.loadTexture("sprites/Background1.png")) {
		std::cerr << "BG1-Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
	}
	if (!BG2Tex.loadTexture("sprites/Background2.png")) {
		std::cerr << "BG2-Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
	}
	if (!BG3Tex.loadTexture("sprites/newBackground3.png")) {
		std::cerr << "BG3-Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
	}
	if (!dedTex.loadTexture("sprites/ded.png")) {
		std::cerr << "Ded-Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
	}
	
	bg1.setTexture(BG1Tex.getTexture());
	bg2.setTexture(BG2Tex.getTexture());
	bg3.setTexture(BG3Tex.getTexture());
	ded.setTexture(dedTex.getTexture());

	if (!wallTex.loadTexture("sprites/wall.png")) {
		std::cerr << "Wall-Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
	}
	for (int i = 0; i < Wall.size(); i++) {
		Wall.at(i).setTexture(wallTex.getTexture());
	}

	return success;
}

void p1_init() {
	p1.setTexture(p1Tex.getTexture());
	SDL_Rect newHitbox;
	newHitbox.w = p1.getWidth()/2;
	newHitbox.h = p1.getHeight();
	newHitbox.x = p1.getPosX();
	newHitbox.y = p1.getPosY();
	p1.setHitbox(newHitbox);
	
	//punchHB
	newHitbox.x = 0;
	newHitbox.y = 0;
	newHitbox.w = 20;
	newHitbox.h = 60;
	p1.setPunchHB(newHitbox);
}

void wimp_init() {
	for (int i = 0; i < Wimps.size(); i++) {
		Wimps.at(i).setTexture(wimpTex.getTexture());
		Wimps.at(i).setHeight(45);
		Wimps.at(i).setWidth(45);
		//Charakterstats
		Wimps.at(i).setSpeed(1);
		//Hitbox
		SDL_Rect newHitbox;
		newHitbox.w = Wimps.at(i).getWidth() / 2;
		newHitbox.h = Wimps.at(i).getHeight();
		newHitbox.x = Wimps.at(i).getPosX() + Wimps.at(i).getWidth() - newHitbox.w;
		newHitbox.y = Wimps.at(i).getPosY();
		Wimps.at(i).setHitbox(newHitbox);
	}
}

void p1_setSprites() {
	//
	//Idle
	//
	p1.idleFrameclip[0].x = 0;
	p1.idleFrameclip[0].y = 0;
	p1.idleFrameclip[0].w = 45;
	p1.idleFrameclip[0].h = 100;

	p1.idleFrameclip[1].x = 45;
	p1.idleFrameclip[1].y = 0;
	p1.idleFrameclip[1].w = 45;
	p1.idleFrameclip[1].h = 100;

	//
	//Walk
	//
	p1.walkFrameclip[0].x = 90;
	p1.walkFrameclip[0].y = 0;
	p1.walkFrameclip[0].w = 45;
	p1.walkFrameclip[0].h = 100;

	p1.walkFrameclip[1].x = 135;
	p1.walkFrameclip[1].y = 0;
	p1.walkFrameclip[1].w = 45;
	p1.walkFrameclip[1].h = 100;

	p1.walkFrameclip[2].x = 180;
	p1.walkFrameclip[2].y = 0;
	p1.walkFrameclip[2].w = 45;
	p1.walkFrameclip[2].h = 100;

	p1.walkFrameclip[3].x = 225;
	p1.walkFrameclip[3].y = 0;
	p1.walkFrameclip[3].w = 45;
	p1.walkFrameclip[3].h = 100;

	//
	//Jump
	//
	p1.jumpFrameclip[0].x = 0;
	p1.jumpFrameclip[0].y = 100;
	p1.jumpFrameclip[0].w = 45;
	p1.jumpFrameclip[0].h = 115;

	p1.jumpFrameclip[1].x = 45;
	p1.jumpFrameclip[1].y = 100;
	p1.jumpFrameclip[1].w = 45;
	p1.jumpFrameclip[1].h = 115;

	p1.jumpFrameclip[2].x = 90;
	p1.jumpFrameclip[2].y = 100;
	p1.jumpFrameclip[2].w = 45;
	p1.jumpFrameclip[2].h = 115;


	//
	//Fall
	//
	p1.fallFrameclip[0].x = 225;
	p1.fallFrameclip[0].y = 115;
	p1.fallFrameclip[0].w = 45;
	p1.fallFrameclip[0].h = 100;
		
	p1.fallFrameclip[1].x = 90;
	p1.fallFrameclip[1].y = 115;
	p1.fallFrameclip[1].w = 45;
	p1.fallFrameclip[1].h = 100;

	//
	//Glide
	//
	p1.glideFrameclip[0].x = 135;
	p1.glideFrameclip[0].y = 100;
	p1.glideFrameclip[0].w = 45;
	p1.glideFrameclip[0].h = 115;

	p1.glideFrameclip[1].x = 180;
	p1.glideFrameclip[1].y = 100;
	p1.glideFrameclip[1].w = 45;
	p1.glideFrameclip[1].h = 115;

	//
	//Duck
	//
	p1.duckFrameclip[0].x = 0;
	p1.duckFrameclip[0].y = 215;
	p1.duckFrameclip[0].w = 45;
	p1.duckFrameclip[0].h = 100;

	p1.dedFrameclip.x = 45;
	p1.dedFrameclip.y = 215;
	p1.dedFrameclip.w = 45;
	p1.dedFrameclip.h = 100;

	//
	//Punch
	//
	p1.punchFrameclip[0].x = 225;
	p1.punchFrameclip[0].y = 215;
	p1.punchFrameclip[0].w = 45;
	p1.punchFrameclip[0].h = 100;
}
void wimp_setSprites() {
	//
	//Idle
	//
	for (int i = 0; i < Wimps.size(); i++) {
		Wimps.at(i).idleFrameclip[0].x = 0;
		Wimps.at(i).idleFrameclip[0].y = 0;
		Wimps.at(i).idleFrameclip[0].w = 45;
		Wimps.at(i).idleFrameclip[0].h = 45;

		Wimps.at(i).idleFrameclip[1].x = 45;
		Wimps.at(i).idleFrameclip[1].y = 0;
		Wimps.at(i).idleFrameclip[1].w = 45;
		Wimps.at(i).idleFrameclip[1].h = 45;
		/*
		Wimps.at(i).idleFrameclip[2].x = 0;
		Wimps.at(i).idleFrameclip[2].y = 80;
		Wimps.at(i).idleFrameclip[2].w = 90;
		Wimps.at(i).idleFrameclip[2].h = 40;

		Wimps.at(i).idleFrameclip[3].x = 0;
		Wimps.at(i).idleFrameclip[3].y = 0;
		Wimps.at(i).idleFrameclip[3].w = 90;
		Wimps.at(i).idleFrameclip[3].h = 40;
		*/
	}
	//
}

//Kamera bewegen
void moveCamera() {
	camera.x = (p1.getPosX() + p1.getWidth() / 2) - SCREEN_WIDTH / 2;
	camera.y = (p1.getPosY() + p1.getHeight() / 2) - SCREEN_HEIGHT / 2;
	if (camera.x < 0) {
		camera.x = 0; 
	} 
	if (camera.y < 0) {
		camera.y = 0; 
	} 
	if (camera.x > LEVEL_WIDTH - camera.w) { 
		camera.x = LEVEL_WIDTH - camera.w; 
	}
	if (camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h; 
	}
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