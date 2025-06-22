/*Copyright 2025 Ciupaga*/
#include<stdbool.h>

#ifndef BASE_H
#define BASE_H

#ifdef __cplusplus
extern "C" {
#endif

bool pDebugOn();

void pWindowCreate(unsigned int pWidth,unsigned int pHeight,unsigned int pMode,bool pDebugOn);
bool pWindowOn();
unsigned int pWindowWidth();
unsigned int pWindowHeight();
unsigned int pWindowMode();
char* pWindowTitle();

void pWindowSetTitle(const char* pTitle);

void pWindowSetBackground(unsigned int pRed,unsigned int pGreen,unsigned int pBlue);
unsigned int pWindowBackgroundRed();
unsigned int pWindowBackgroundGreen();
unsigned int pWindowBackgroundBlue();

void pEventHandle();
bool pEventKeyPress(const char* pKey);
bool pEventKeyHold(const char* pKey);

bool pEventKeyLast(const char* pKey);
bool pEventCapsOn();

#ifdef __cplusplus
}
#endif

#endif
