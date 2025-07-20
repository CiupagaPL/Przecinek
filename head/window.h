/*Orginal {,} Made By Ciupaga*/
/*https://github.com/CiupagaPL/Przecinek*/

#include<stdbool.h>

#ifndef WINDOW_H
#define WINDOW_H

#ifdef __cplusplus
extern "C"{
  #endif

  typedef struct{ unsigned int width,height; } pScreen;
  typedef struct{ unsigned int x,y; } pCursor;

  typedef struct{
    pScreen display;
    pCursor cursor;
  } pStatus;

  typedef struct{
    unsigned int ID;
    bool on,active;

    unsigned int x,y,width,height,mode;
    char title[256];
    unsigned int red,green,blue;
  } pWindow;

  extern pStatus przecinek;

  void pDebug(bool on);

  pWindow pWindowCreate(unsigned int width,unsigned int height,unsigned int mode);
  void pWindowSetPosition(pWindow* window,unsigned int x,unsigned int y);
  void pWindowSetTitle(pWindow* window,const char* title);
  void pWindowSetBackground(pWindow* window,unsigned int red,unsigned int green,unsigned int blue);
  void pWindowHandle(pWindow* window);
  void pWindowClose(pWindow* window);

  bool pKeyPress(const char* key);
  bool pKeyHold(const char* key);
  bool pKeyCaps();

  #ifdef __cplusplus
}
#endif
#endif
