/*Orginal {,} Made By Ciupaga*/
/*https://github.com/CiupagaPL/Przecinek*/

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include<X11/Xlib.h>
#include<X11/Xatom.h>
#include<X11/Xutil.h>
#include<X11/keysym.h>

bool debug=false;
void pDebug(bool on){ debug=on; }

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

typedef struct{
  bool on,active;

  unsigned int x,y,width,height,mode;
  char title[256];
  unsigned int red,green,blue;

  Display *display;
  int screen;
  Window base;
  Atom wmDelete;
  XEvent event,report;

  bool W_DESTROY;

  unsigned int keyPress;
  bool keyHold[256];
} pWindowX11;

unsigned int activeWinID=0;
pWindowX11 windowX11[64];
pStatus przecinek;

void pWindowReset(pWindow* window);

pWindow pWindowCreate(unsigned int width,unsigned int height,unsigned int mode){
  pWindow window;
  for(int c=1;c<=64;c++){
    if(!windowX11[c-1].on){
      window.ID=c;
      break;
    } else if(c==64){
      if(debug){
        printf("[Error] Too many Windows active,\n");
        fflush(stdout);
      }
      pWindowReset(&window);
      return window;
    }
  }

  windowX11[window.ID-1].display=XOpenDisplay(NULL);
  if(windowX11[window.ID-1].display==NULL){
    if(debug){
      printf("[Error] Could not open Display,\n");
      fflush(stdout);
    }
    pWindowReset(&window);
    return window;
  }

  windowX11[window.ID-1].screen=DefaultScreen(windowX11[window.ID-1].display);
  windowX11[window.ID-1].base=XCreateSimpleWindow(
    windowX11[window.ID-1].display,
    RootWindow(windowX11[window.ID-1].display,windowX11[window.ID-1].screen),
    0,0,width,height,1,
    BlackPixel(windowX11[window.ID-1].display,windowX11[window.ID-1].screen),
    BlackPixel(windowX11[window.ID-1].display,windowX11[window.ID-1].screen)
  );

  if(windowX11[window.ID-1].base==0){
    if(debug){
      printf("[Error] Could not create Window,\n");
      fflush(stdout);
    }
    pWindowReset(&window);
    XCloseDisplay(windowX11[window.ID-1].display);
    return window;
  }

  window.on=true;
  windowX11[window.ID-1].on=true;
  window.width=width;
  window.height=height;
  window.x=(DisplayWidth(windowX11[window.ID-1].display,windowX11[window.ID-1].screen)-window.width)/2;
  window.y=(DisplayHeight(windowX11[window.ID-1].display,windowX11[window.ID-1].screen)-window.height)/2;
  window.mode=mode;
  XStoreName(windowX11[window.ID-1].display,windowX11[window.ID-1].base,"{,}");

  windowX11[window.ID-1].wmDelete=XInternAtom(windowX11[window.ID-1].display,"WM_DELETE_WINDOW",False);
  XSetWMProtocols(windowX11[window.ID-1].display,windowX11[window.ID-1].base,&windowX11[window.ID-1].wmDelete,1);

  Atom wmHints=XInternAtom(windowX11[window.ID-1].display,"_NET_WM_HINTS",False);
  if(wmHints!=None){
    long hints[5]={ 1,0,0,0,0 };
    if(mode==0){ hints[1] = 0; }
    else if(mode==1){
      hints[1]=1;
      hints[2]=0;
    } else{
      hints[1]=1;
      hints[2]=1;
      hints[3]=1;
    }
    XChangeProperty(windowX11[window.ID-1].display,windowX11[window.ID-1].base,wmHints,wmHints,32,PropModeReplace,(unsigned char*)hints,5);
  }

  XSelectInput(windowX11[window.ID-1].display,windowX11[window.ID-1].base,ExposureMask|KeyPressMask|KeyReleaseMask|StructureNotifyMask|FocusChangeMask|PointerMotionMask);
  XMapWindow(windowX11[window.ID-1].display,windowX11[window.ID-1].base);
  XMoveWindow(windowX11[window.ID-1].display,windowX11[window.ID-1].base,window.x,window.y);
  XFlush(windowX11[window.ID-1].display);

  unsigned int currentX=0,currentY=0,windowX=0,windowY=0;
  unsigned int state;
  Window child=None;
  XQueryPointer(windowX11[window.ID-1].display,RootWindow(windowX11[window.ID-1].display,windowX11[window.ID-1].screen),
    &child,&child,&currentX,&currentY,&windowX,&windowY,&state);
  przecinek.cursor.x=currentX;
  przecinek.cursor.y=currentY;
  przecinek.display.width=DisplayWidth(windowX11[window.ID-1].display,windowX11[window.ID-1].screen);
  przecinek.display.height=DisplayHeight(windowX11[window.ID-1].display,windowX11[window.ID-1].screen);

  return window;
}

void pWindowSetPosition(pWindow* window,unsigned int x,unsigned int y){
  if(window->on){
    window->x=x;
    window->y=y;

    XMoveWindow(windowX11[window->ID-1].display,windowX11[window->ID-1].base,x,y);
    XFlush(windowX11[window->ID-1].display);
  } else{
    printf("[Error] Could not set Window Position,\n");
    fflush(stdout);
  }
  return;
}

void pWindowSetTitle(pWindow* window,const char* title){
  if(window->on){
    strncpy(window->title,title,sizeof(window->title)-1);
    window->title[sizeof(window->title)-1]='\0';
    strncpy(windowX11[window->ID-1].title,title,sizeof(windowX11[window->ID-1].title)-1);
    windowX11[window->ID-1].title[sizeof(windowX11[window->ID-1].title)-1]='\0';

    XStoreName(windowX11[window->ID-1].display,windowX11[window->ID-1].base,window->title);
    XFlush(windowX11[window->ID-1].display);
  } else if(debug){
    printf("[Error] Could not set Window Title,\n");
    fflush(stdout);
  }
  return;
}

void pWindowSetBackground(pWindow* window,unsigned int red,unsigned int green,unsigned int blue){
  if(window->on){
    if(red>255){ red=255; }
    if(green>255){ green=255; }
    if(blue>255){ blue=255; }
    window->red=red;
    window->green=green;
    window->blue=blue;
    windowX11[window->ID-1].red=red;
    windowX11[window->ID-1].green=green;
    windowX11[window->ID-1].blue=blue;

    XSetWindowBackground(windowX11[window->ID-1].display,windowX11[window->ID-1].base,((window->red<<16)|(window->green<<8)|window->blue));
    XClearWindow(windowX11[window->ID-1].display,windowX11[window->ID-1].base);
    XFlush(windowX11[window->ID-1].display);
  } else if(debug){
    printf("[Error] Could not set Window Background,\n");
    fflush(stdout);
  }
  return;
}

void pWindowHandle(pWindow* window){
  if(window->on){
    if(XPending(windowX11[window->ID-1].display)>0){
      XNextEvent(windowX11[window->ID-1].display,&windowX11[window->ID-1].event);
      if(windowX11[window->ID-1].event.type==Expose){ XClearWindow(windowX11[window->ID-1].display,windowX11[window->ID-1].base); }
      else if(windowX11[window->ID-1].event.type==ClientMessage&&(Atom)windowX11[window->ID-1].event.xclient.data.l[0]==windowX11[window->ID-1].wmDelete||
          windowX11[window->ID-1].W_DESTROY){
        pWindowReset(window);
        XDestroyWindow(windowX11[window->ID-1].display,windowX11[window->ID-1].base);
        XCloseDisplay(windowX11[window->ID-1].display);
        return;
      } else if(windowX11[window->ID-1].event.type==ConfigureNotify){
        windowX11[window->ID-1].x=windowX11[window->ID-1].event.xconfigure.x;
        windowX11[window->ID-1].y=windowX11[window->ID-1].event.xconfigure.y;
        windowX11[window->ID-1].width=windowX11[window->ID-1].event.xconfigure.width;
        windowX11[window->ID-1].height=windowX11[window->ID-1].event.xconfigure.height;
      } else if(windowX11[window->ID-1].event.type==MotionNotify){
        przecinek.cursor.x=windowX11[window->ID-1].event.xmotion.x_root;
        przecinek.cursor.y=windowX11[window->ID-1].event.xmotion.y_root;
      } else if(windowX11[window->ID-1].event.type==KeyPress){
        windowX11[window->ID-1].keyPress=windowX11[window->ID-1].event.xkey.keycode;
        windowX11[window->ID-1].keyHold[windowX11[window->ID-1].event.xkey.keycode]=true;
      } else if(windowX11[window->ID-1].event.type==KeyRelease){
        windowX11[window->ID-1].keyHold[windowX11[window->ID-1].event.xkey.keycode]=false;

        if(XEventsQueued(windowX11[window->ID-1].display,QueuedAfterReading)){
          XPeekEvent(windowX11[window->ID-1].display,&windowX11[window->ID-1].report);
          if(windowX11[window->ID-1].report.type==KeyPress&&windowX11[window->ID-1].report.xkey.time==windowX11[window->ID-1].event.xkey.time&&
              windowX11[window->ID-1].report.xkey.keycode==windowX11[window->ID-1].event.xkey.keycode){
            windowX11[window->ID-1].keyPress=0;
            windowX11[window->ID-1].keyHold[windowX11[window->ID-1].event.xkey.keycode]=true;
            XNextEvent(windowX11[window->ID-1].display,&windowX11[window->ID-1].event);
          }
        }
      } else if(windowX11[window->ID-1].event.type==FocusIn){
        windowX11[window->ID-1].keyPress=0;
        for(int c=0;c<256;c++){ windowX11[window->ID-1].keyHold[c]=false; }

        activeWinID=window->ID;
        windowX11[window->ID-1].active=true;
      } else if(windowX11[window->ID-1].event.type==FocusOut&&activeWinID==window->ID){
        windowX11[window->ID-1].keyPress=0;
        for(int c=0;c<256;c++){ windowX11[window->ID-1].keyHold[c]=false; }

        activeWinID=0;
        windowX11[window->ID-1].active=false;
      }
    }

    przecinek.display.width=DisplayWidth(windowX11[window->ID-1].display,windowX11[window->ID-1].screen);
    przecinek.display.height=DisplayHeight(windowX11[window->ID-1].display,windowX11[window->ID-1].screen);

    window->on=windowX11[window->ID-1].on;
    window->active=windowX11[window->ID-1].active;

    window->x=windowX11[window->ID-1].x;
    window->y=windowX11[window->ID-1].y;
    window->width=windowX11[window->ID-1].width;
    window->height=windowX11[window->ID-1].height;
    window->mode=windowX11[window->ID-1].mode;
    strncpy(window->title,windowX11[window->ID-1].title,sizeof(window->title)-1);
    window->title[sizeof(window->title)-1]='\0';
    window->red=windowX11[window->ID-1].red;
    window->green=windowX11[window->ID-1].green;
    window->blue=windowX11[window->ID-1].blue;
  } else if(debug){
    printf("[Error] Could not handle Window,\n");
    fflush(stdout);
  }
  return;
}

void pWindowReset(pWindow* window){
  window->on=false;
  window->active=false;

  window->x=0;
  window->y=0;
  window->width=0;
  window->height=0;
  window->mode=0;
  strncpy(window->title,"{,}",sizeof(window->title)-1);
  window->title[sizeof(window->title)-1]='\0';
  window->red=0;
  window->green=0;
  window->blue=0;

  windowX11[window->ID-1].on=false;
  windowX11[window->ID-1].active=false;

  windowX11[window->ID-1].x=0;
  windowX11[window->ID-1].y=0;
  windowX11[window->ID-1].width=0;
  windowX11[window->ID-1].height=0;
  windowX11[window->ID-1].mode=0;
  strncpy(windowX11[window->ID-1].title,"{,}",sizeof(windowX11[window->ID-1].title)-1);
  windowX11[window->ID-1].title[sizeof(windowX11[window->ID-1].title)-1]='\0';
  windowX11[window->ID-1].red=0;
  windowX11[window->ID-1].green=0;
  windowX11[window->ID-1].blue=0;

  windowX11[window->ID-1].W_DESTROY=false;
  windowX11[window->ID-1].keyPress=0;
  for(int c=0;c<256;c++){ windowX11[window->ID-1].keyHold[c]=false; }
  return;
}

void pWindowClose(pWindow* window){
  if(window->on){ windowX11[window->ID-1].W_DESTROY=true; }
  else if(debug){
    printf("[Warning] Window is already closed,\n");
    fflush(stdout);
  }
  return;
}

unsigned int pModifyKey(const char* key);

bool pKeyPress(const char* key){
  if(pModifyKey(key)>0&&pModifyKey(key)<256){
    if(activeWinID!=0&&pModifyKey(key)==windowX11[activeWinID-1].keyPress){
      windowX11[activeWinID-1].keyPress=0;
      return true;
    }
  } else if(debug){
    printf("[Warning] Invalid Key,\n");
    fflush(stdout);
  }
  return false;
}

bool pKeyHold(const char* key){
  if(pModifyKey(key)>0&&pModifyKey(key)<256){
    if(activeWinID!=0){ return windowX11[activeWinID-1].keyHold[pModifyKey(key)]; }
  } else if(debug){
    printf("[Warning] Invalid Key,\n");
    fflush(stdout);
  }
  return false;
}

bool pKeyCaps(){
  if(activeWinID!=0){
    XKeyboardState keyboardState;
    XGetKeyboardControl(windowX11[activeWinID-1].display,&keyboardState);
    return keyboardState.led_mask&(1<<1)!=0;
  } else if(debug){
    printf("[Warning] Could not check Caps State,\n");
    fflush(stdout);
  }
  return false;
}

unsigned int pModifyKey(const char* key){
  if(strcmp(key,"ESC")==0||strcmp(key,"esc")==0||strcmp(key,"Esc")==0){ return 9; }
  else if(strcmp(key,"TAB")==0||strcmp(key,"tab")==0||strcmp(key,"Tab")==0){ return 23; }
  else if(strcmp(key,"CAPS")==0||strcmp(key,"caps")==0||strcmp(key,"Caps")==0){ return 66; }
  else if(strcmp(key,"LSHIFT")==0||strcmp(key,"lshift")==0||strcmp(key,"LShift")==0){ return 50; }
  else if(strcmp(key,"LCTRL")==0||strcmp(key,"lctrl")==0||strcmp(key,"LCtrl")==0){ return 37; }
  else if(strcmp(key,"LMOD")==0||strcmp(key,"lmod")==0||strcmp(key,"LMod")==0){ return 133; }
  else if(strcmp(key,"LALT")==0||strcmp(key,"lalt")==0||strcmp(key,"LAlt")==0){ return 64; }
  else if(strcmp(key,"SPACE")==0||strcmp(key,"space")==0||strcmp(key,"Space")==0){ return 65; }

  else if(strcmp(key,"F1")==0||strcmp(key,"f1")==0){ return 67; }
  else if(strcmp(key,"F2")==0||strcmp(key,"f2")==0){ return 68; }
  else if(strcmp(key,"F3")==0||strcmp(key,"f3")==0){ return 69; }
  else if(strcmp(key,"F4")==0||strcmp(key,"f4")==0){ return 70; }
  else if(strcmp(key,"F5")==0||strcmp(key,"f5")==0){ return 71; }
  else if(strcmp(key,"F6")==0||strcmp(key,"f6")==0){ return 72; }
  else if(strcmp(key,"F7")==0||strcmp(key,"f7")==0){ return 73; }
  else if(strcmp(key,"F8")==0||strcmp(key,"f8")==0){ return 74; }
  else if(strcmp(key,"F9")==0||strcmp(key,"f9")==0){ return 75; }
  else if(strcmp(key,"F10")==0||strcmp(key,"f10")==0){ return 76; }
  else if(strcmp(key,"F11")==0||strcmp(key,"f11")==0){ return 95; }
  else if(strcmp(key,"F12")==0||strcmp(key,"f12")==0){ return 96; }

  else if(strcmp(key,"RALT")==0||strcmp(key,"ralt")==0||strcmp(key,"RAlt")==0){ return 108; }
  else if(strcmp(key,"RWIN")==0||strcmp(key,"rwin")==0||strcmp(key,"RWin")==0){ return 134; }
  else if(strcmp(key,"MENU")==0||strcmp(key,"menu")==0||strcmp(key,"Menu")==0){ return 135; }
  else if(strcmp(key,"RCTRL")==0||strcmp(key,"rctrl")==0||strcmp(key,"RCtrl")==0){ return 105; }
  else if(strcmp(key,"RSHIFT")==0||strcmp(key,"rshift")==0||strcmp(key,"RShift")==0){ return 62; }
  else if(strcmp(key,"ENTER")==0||strcmp(key,"enter")==0||strcmp(key,"Enter")==0){ return 36; }
  else if(strcmp(key,"BACKSPACE")==0||strcmp(key,"backspace")==0||strcmp(key,"Backspace")==0){ return 22; }

  else if(strcmp(key,"LARROW")==0||strcmp(key,"larrow")==0||strcmp(key,"LArrow")==0){ return 113; }
  else if(strcmp(key,"DARROW")==0||strcmp(key,"darrow")==0||strcmp(key,"DArrow")==0){ return 116; }
  else if(strcmp(key,"RARROW")==0||strcmp(key,"rarrow")==0||strcmp(key,"RArrow")==0){ return 114; }
  else if(strcmp(key,"UARROW")==0||strcmp(key,"uarrow")==0||strcmp(key,"UArrow")==0){ return 111; }

  else if(strcmp(key,"PRINTSCRN")==0||strcmp(key,"printscrn")==0||strcmp(key,"PrintScrn")==0){ return 107; }
  else if(strcmp(key,"SCROLLLOCK")==0||strcmp(key,"scrolllock")==0||strcmp(key,"ScrollLock")==0){ return 78; }
  else if(strcmp(key,"PAUSEBREAK")==0||strcmp(key,"pausebreak")==0||strcmp(key,"PauseBreak")==0){ return 127; }
  else if(strcmp(key,"INS")==0||strcmp(key,"ins")==0||strcmp(key,"Ins")==0){ return 118; }
  else if(strcmp(key,"HOME")==0||strcmp(key,"home")==0||strcmp(key,"Home")==0){ return 110; }
  else if(strcmp(key,"PAGEU")==0||strcmp(key,"pageu")==0||strcmp(key,"PageU")==0){ return 112; }
  else if(strcmp(key,"DEL")==0||strcmp(key,"del")==0||strcmp(key,"Del")==0){ return 119; }
  else if(strcmp(key,"END")==0||strcmp(key,"end")==0||strcmp(key,"End")==0){ return 115; }
  else if(strcmp(key,"PAGED")==0||strcmp(key,"paged")==0||strcmp(key,"PageD")==0){ return 117; }

  else if(strcmp(key,"Q")==0||strcmp(key,"q")==0){ return 24; }
  else if(strcmp(key,"W")==0||strcmp(key,"w")==0){ return 25; }
  else if(strcmp(key,"E")==0||strcmp(key,"e")==0){ return 26; }
  else if(strcmp(key,"R")==0||strcmp(key,"r")==0){ return 27; }
  else if(strcmp(key,"T")==0||strcmp(key,"t")==0){ return 28; }
  else if(strcmp(key,"Y")==0||strcmp(key,"y")==0){ return 29; }
  else if(strcmp(key,"U")==0||strcmp(key,"u")==0){ return 30; }
  else if(strcmp(key,"I")==0||strcmp(key,"i")==0){ return 31; }
  else if(strcmp(key,"O")==0||strcmp(key,"o")==0){ return 32; }
  else if(strcmp(key,"P")==0||strcmp(key,"p")==0){ return 33; }
  else if(strcmp(key,"A")==0||strcmp(key,"a")==0){ return 38; }
  else if(strcmp(key,"S")==0||strcmp(key,"s")==0){ return 39; }
  else if(strcmp(key,"D")==0||strcmp(key,"d")==0){ return 40; }
  else if(strcmp(key,"F")==0||strcmp(key,"f")==0){ return 41; }
  else if(strcmp(key,"G")==0||strcmp(key,"g")==0){ return 42; }
  else if(strcmp(key,"H")==0||strcmp(key,"h")==0){ return 43; }
  else if(strcmp(key,"J")==0||strcmp(key,"j")==0){ return 44; }
  else if(strcmp(key,"K")==0||strcmp(key,"k")==0){ return 45; }
  else if(strcmp(key,"L")==0||strcmp(key,"l")==0){ return 46; }
  else if(strcmp(key,"Z")==0||strcmp(key,"z")==0){ return 52; }
  else if(strcmp(key,"X")==0||strcmp(key,"x")==0){ return 53; }
  else if(strcmp(key,"C")==0||strcmp(key,"c")==0){ return 54; }
  else if(strcmp(key,"V")==0||strcmp(key,"v")==0){ return 55; }
  else if(strcmp(key,"B")==0||strcmp(key,"b")==0){ return 56; }
  else if(strcmp(key,"N")==0||strcmp(key,"n")==0){ return 57; }
  else if(strcmp(key,"M")==0||strcmp(key,"m")==0){ return 58; }

  else if(strcmp(key,"1")==0){ return 10; }
  else if(strcmp(key,"2")==0){ return 11; }
  else if(strcmp(key,"3")==0){ return 12; }
  else if(strcmp(key,"4")==0){ return 13; }
  else if(strcmp(key,"5")==0){ return 14; }
  else if(strcmp(key,"6")==0){ return 15; }
  else if(strcmp(key,"7")==0){ return 16; }
  else if(strcmp(key,"8")==0){ return 17; }
  else if(strcmp(key,"9")==0){ return 18; }
  else if(strcmp(key,"0")==0){ return 19; }

  else if(strcmp(key,"`")==0){ return 49; }
  else if(strcmp(key,",")==0){ return 59; }
  else if(strcmp(key,".")==0){ return 60; }
  else if(strcmp(key,"/")==0){ return 61; }
  else if(strcmp(key,";")==0){ return 47; }
  else if(strcmp(key,"'")==0){ return 48; }
  else if(strcmp(key,"\\")==0){ return 51; }
  else if(strcmp(key,"[")==0){ return 34; }
  else if(strcmp(key,"]")==0){ return 35; }
  else if(strcmp(key,"-")==0){ return 20; }
  else if(strcmp(key,"=")==0){ return 21; }
  else{ return 0; }
}
