#include "header.h"
#include "Core.h"
#include "IMG.h"
#include "CFG.h"
#include "Text.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* ******************************************** */

Map* CCore::oMap = new Map();
bool CCore::mouseLeftPressed = false;
bool CCore::mouseRightPressed = false;
int CCore::mouseX = 0;
int CCore::mouseY = 0;
bool CCore::quitGame = false;

bool CCore::movePressed = false;
bool CCore::keyMenuPressed = false;
bool CCore::keyS = false;
bool CCore::keyW = false;
bool CCore::keyA = false;
bool CCore::keyD = false;
bool CCore::keyShift = false;
bool CCore::keyAPressed = false;
bool CCore::keyDPressed = false;

//pipe//
int fd;

//gestures//
bool gestureDown = false;
bool gestureUp=false;
bool useKeys = true;
int gestureKey;
int gestureKeyUp;
int keyPress;
int keyPressUp;

CCore::CCore(void) {
	this->quitGame = false;
	this->iFPS = 0;
	this->iNumOfFPS = 0;
	this->lFPSTime = 0;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

	window = SDL_CreateWindow("uMario - www.LukaszJakowski.pl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CCFG::GAME_WIDTH, CCFG::GAME_HEIGHT, SDL_WINDOW_SHOWN);

	if(window == NULL) {
		quitGame = true;
	}

	rR = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// ----- ICO
	std::string fileName = "files/images/ico.bmp";
	SDL_Surface* loadedSurface = SDL_LoadBMP(fileName.c_str());
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));

	SDL_SetWindowIcon(window, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	mainEvent = new SDL_Event();
	// ----- ICO

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	oMap = new Map(rR);
	CCFG::getMM()->setActiveOption(rR);
	CCFG::getSMBLOGO()->setIMG("super_mario_bros", rR);

	CCFG::getMusic()->PlayMusic();

	this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;

	this->keyAPressed = this->keyDPressed = this->firstDir = false;

	this->mouseX = this->mouseY = 0;

	CCFG::keyIDA = SDLK_a;
	CCFG::keyIDS = SDLK_s;
	CCFG::keyIDD = SDLK_d;
	CCFG::keyIDSpace = SDLK_SPACE;
	CCFG::keyIDShift = SDLK_LSHIFT;

    //open test pipe
    fd = open("/tmp/testpipe", O_RDONLY|O_NONBLOCK);
    // printf("gothere\n", );
}

CCore::~CCore(void) {
	delete oMap;
	delete mainEvent;
	SDL_DestroyRenderer(rR);
	SDL_DestroyWindow(window);
    close(fd);
}

/* ******************************************** */

void CCore::mainLoop() {
    // printf("fuckf\n", );
	lFPSTime = SDL_GetTicks();

	while(!quitGame && mainEvent->type != SDL_QUIT) {
        // printf("ugh\n", );
        char line[128] = {0};
        if (read(fd, line, sizeof(line)) > 0){
            if (useKeys){
                useKeys = false;
            }
            // printf("%s", line);
            if (strcmp(line, "sd\n")== 0){
                gestureUp = true;
                gestureKeyUp = SDLK_d;
                printf("recognized stop right\n");
            }
            else if (strcmp(line, "sb\n")== 0){
                gestureUp = true;
                gestureKeyUp = SDLK_SPACE;
                printf("recognized stop jump\n");
            }
            else if (strcmp(line, "sa\n")== 0){
                gestureUp = true;
                gestureKeyUp = SDLK_a;
                printf("recognized stop left\n");
            }
            else if (strcmp(line, "sshift\n")==0){
                gestureUp = true;
                gestureKeyUp = SDLK_LSHIFT;
                printf("recognized stop shift\n");
            }
            else{
                gestureDown = true;
                if (strcmp(line, "enter\n") == 0){
                    gestureKey = SDLK_RETURN;
                    printf("recognized enter\n");
                }
                else if (strcmp(line, "up\n") == 0){
                    gestureKey = SDLK_UP;
                    printf("recognized up\n");
                }
                else if (strcmp(line, "down\n")== 0){
                    gestureKey = SDLK_DOWN;
                    printf("recognized down\n");
                }
                else if (strcmp(line, "right\n")== 0){
                    gestureKey = SDLK_RIGHT;
                    printf("recognized right\n");
                }
                else if (strcmp(line, "left\n")== 0){
                    gestureKey = SDLK_LEFT;
                    printf("recognized left\n");
                }
                else if (strcmp(line, "a\n") == 0){
                    gestureKey = SDLK_a;
                    printf("recognized a\n");
                }
                else if (strcmp(line, "d\n") == 0){
                    gestureKey = SDLK_d;
                    printf("recognized d\n");
                }
                else if (strcmp(line, "b\n") == 0){
                    gestureKey = SDLK_SPACE;
                    printf("recognized jump\n");
                }
                else if (strcmp(line, "shift\n") == 0){
                    gestureKey = SDLK_LSHIFT;
                    printf("recognized shift\n");
                }
            }
        }
		frameTime = SDL_GetTicks();
		SDL_PollEvent(mainEvent);
		SDL_RenderClear(rR);

		CCFG::getMM()->setBackgroundColor(rR);
		SDL_RenderFillRect(rR, NULL);

		Input();
		MouseInput();
		Update();
		Draw();

		/*CCFG::getText()->Draw(rR, "FPS:" + std::to_string(iNumOfFPS), CCFG::GAME_WIDTH - CCFG::getText()->getTextWidth("FPS:" + std::to_string(iNumOfFPS), 8) - 8, 5, 8);

		if(SDL_GetTicks() - 1000 >= lFPSTime) {
			lFPSTime = SDL_GetTicks();
			iNumOfFPS = iFPS;
			iFPS = 0;
		}

		++iFPS;*/

		SDL_RenderPresent(rR);

		if(SDL_GetTicks() - frameTime < MIN_FRAME_TIME) {
			SDL_Delay(MIN_FRAME_TIME - (SDL_GetTicks () - frameTime));
		}
	}
}

void CCore::Input() {
	switch(CCFG::getMM()->getViewID()) {
		case 2: case 7:
			if(!oMap->getInEvent()) {
				InputPlayer();
			} else {
				resetMove();
			}
			break;
		default:
			InputMenu();
			break;
	}
}

void CCore::InputMenu() {
    if (gestureDown){
        keyPress = gestureKey;
        printf("happened\n");
    }
    else{
        keyPress = mainEvent -> key.keysym.sym;
    }
    // printf("%d\n", gestureDown);
	if((mainEvent->type == SDL_KEYDOWN)|gestureDown) {
        if (gestureDown){
            gestureDown = false;
            gestureUp = true;
        }
		CCFG::getMM()->setKey(keyPress);

		switch(keyPress) {
			case SDLK_s: case SDLK_DOWN:
				if(!keyMenuPressed) {
					CCFG::getMM()->keyPressed(2);
					keyMenuPressed = true;
				}
				break;
			case SDLK_w: case SDLK_UP:
				if(!keyMenuPressed) {
					CCFG::getMM()->keyPressed(0);
					keyMenuPressed = true;
				}
				break;
			case SDLK_KP_ENTER: case SDLK_RETURN:
                printf("executed enter\n");
				if(!keyMenuPressed) {
					CCFG::getMM()->enter();
					keyMenuPressed = true;
				}
				break;
			case SDLK_ESCAPE:
				if(!keyMenuPressed) {
					CCFG::getMM()->escape();
					keyMenuPressed = true;
				}
				break;
			case SDLK_LEFT: case SDLK_d:
				if(!keyMenuPressed) {
					CCFG::getMM()->keyPressed(3);
					keyMenuPressed = true;
				}
				break;
			case SDLK_RIGHT: case SDLK_a:
				if(!keyMenuPressed) {
					CCFG::getMM()->keyPressed(1);
					keyMenuPressed = true;
				}
				break;
		}
	}

    // bool keyUpSwich;
    // if (useKeys){
    //     keyUpSwich = (mainEvent->type == SDL_KEYUP);
    // }
    // else{
    //     keyUpSwich = (mainEvent->type == SDL_KEYUP)||gestureDown == false;
    // }

	if((mainEvent->type == SDL_KEYUP)|gestureUp) {
        gestureUp = false;
		switch(keyPress) {
			case SDLK_s: case SDLK_DOWN: case SDLK_w: case SDLK_UP: case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE: case SDLK_a: case SDLK_RIGHT: case SDLK_LEFT: case SDLK_d:
				keyMenuPressed = false;
				break;
			default:
				break;
		}
	}
}

void CCore::InputPlayer() {
	if(mainEvent->type == SDL_WINDOWEVENT) {
		switch(mainEvent->window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:
				// // CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePasue);
				// // CCFG::getMM()->setViewID(CCFG::getMM()->ePasue);
				// CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
				// CCFG::getMusic()->PauseMusic();
				break;
		}
	}
    if (gestureUp){
        keyPressUp = gestureKeyUp;
        // printf("happened\n");
    }
    else{
        keyPressUp = mainEvent -> key.keysym.sym;
    }

	if((mainEvent->type == SDL_KEYUP) | gestureUp) {
        gestureUp = false;
		if(keyPressUp == CCFG::keyIDD) {
                printf("processed stop right\n");

				if(firstDir) {
					firstDir = false;
				}

				keyDPressed = false;
			}
			if(keyPressUp == CCFG::keyIDS) {
				oMap->getPlayer()->setSquat(false);
				keyS = false;
			}

			if(keyPressUp == CCFG::keyIDA) {
				if(!firstDir) {
					firstDir = true;
				}

				keyAPressed = false;
			}

			if(keyPressUp == CCFG::keyIDSpace) {
				CCFG::keySpace = false;
                printf("processed stop jump\n");
			}

			if(keyPressUp == CCFG::keyIDShift) {
				if(keyShift) {
					oMap->getPlayer()->resetRun();
					keyShift = false;
				}
                printf("processed stop shift\n");
			}
		switch(keyPressUp) {
			case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE:
				keyMenuPressed = false;
				break;
		}
	}
    if (gestureDown){
        keyPress = gestureKey;
        // printf("happened\n");
    }
    else{
        keyPress = mainEvent -> key.keysym.sym;
    }
	if((mainEvent->type == SDL_KEYDOWN) | gestureDown) {
        gestureDown = false;
		if(keyPress == CCFG::keyIDD) {
            printf("processed start right\n");
			keyDPressed = true;
			if(!keyAPressed) {
				firstDir = true;
			}
		}

		if(keyPress == CCFG::keyIDS) {
			if(!keyS) {
				keyS = true;
				if(!oMap->getUnderWater() && !oMap->getPlayer()->getInLevelAnimation()) oMap->getPlayer()->setSquat(true);
			}
		}

		if(keyPress == CCFG::keyIDA) {
			keyAPressed = true;
			if(!keyDPressed) {
				firstDir = false;
			}
		}

		if(keyPress == CCFG::keyIDSpace) {
			if(!CCFG::keySpace) {
                printf("processed start jump\n");
				oMap->getPlayer()->jump();
				CCFG::keySpace = true;
			}
		}

		if(keyPress == CCFG::keyIDShift) {
			if(!keyShift) {
				oMap->getPlayer()->startRun();
				keyShift = true;
			}
		}

		switch(keyPress) {
			case SDLK_KP_ENTER: case SDLK_RETURN:
				if(!keyMenuPressed) {
					CCFG::getMM()->enter();
					keyMenuPressed = true;
				}
			case SDLK_ESCAPE:
				if(!keyMenuPressed && CCFG::getMM()->getViewID() == CCFG::getMM()->eGame) {
					CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePasue);
					CCFG::getMM()->setViewID(CCFG::getMM()->ePasue);
					CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
					CCFG::getMusic()->PauseMusic();
					keyMenuPressed = true;
				}
				break;
		}
	}

	if(keyAPressed) {
		if(!oMap->getPlayer()->getMove() && firstDir == false && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
			oMap->getPlayer()->startMove();
			oMap->getPlayer()->setMoveDirection(false);
		} else if(!keyDPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
			oMap->getPlayer()->setChangeMoveDirection();
		}
	}

	if(keyDPressed) {
		if(!oMap->getPlayer()->getMove() && firstDir == true && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
			oMap->getPlayer()->startMove();
			oMap->getPlayer()->setMoveDirection(true);
		} else if(!keyAPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
			oMap->getPlayer()->setChangeMoveDirection();
		}
	}

	if(oMap->getPlayer()->getMove() && !keyAPressed && !keyDPressed) {
		oMap->getPlayer()->resetMove();
	}
}

void CCore::MouseInput() {
	switch(mainEvent->type) {
		case SDL_MOUSEBUTTONDOWN: {
			switch (mainEvent->button.button) {
				case SDL_BUTTON_LEFT:
					mouseLeftPressed = true;
					break;
				case SDL_BUTTON_RIGHT:
					mouseRightPressed = true;
					break;
			}
			break;
		}
		case SDL_MOUSEMOTION: {

			SDL_GetMouseState(&mouseX, &mouseY);
			//CCFG::getMM()->getConsole()->print("x:" + std::to_string(mouseX));
			//CCFG::getMM()->getConsole()->print("y:" + std::to_string(mouseY));
			break;
		}
		case SDL_MOUSEBUTTONUP: {
			switch (mainEvent->button.button) {
				case SDL_BUTTON_LEFT:
					mouseLeftPressed = false;
					break;
				case SDL_BUTTON_RIGHT:
					mouseRightPressed = false;
					break;
			}
			break;
		}
		case SDL_MOUSEWHEEL:
			if(mainEvent->wheel.timestamp > SDL_GetTicks() - 2) {
				//CCFG::getMM()->getLE()->mouseWheel(mainEvent->wheel.y);
			}
			break;
	}
}

void CCore::resetKeys() {
	movePressed = keyMenuPressed = keyS = keyW = keyA = keyD = CCFG::keySpace = keyShift = keyAPressed = keyDPressed = false;
}

void CCore::Update() {
	CCFG::getMM()->Update();
}


void CCore::Draw() {
	CCFG::getMM()->Draw(rR);
}

/* ******************************************** */

void CCore::resetMove() {
	this->keyAPressed = this->keyDPressed = false;
}

Map* CCore::getMap() {
	return oMap;
}
