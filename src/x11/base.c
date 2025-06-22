/*Orginal {,} Made By Ciupaga*/
/*https://github.com/CiupagaPL/Przecinek*/

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include<X11/Xlib.h>
#include<X11/Xatom.h>
#include<X11/Xutil.h>
#include<X11/keysym.h>

Display *display;
Window window;
XEvent event,report;
Atom wmDelete;
int screen;
bool keyPress[256]={ false },keyHold[256]={ false };
unsigned int keyFirst=0,keySecond=0;

bool debugOn=false;
bool pDebugOn(){ return debugOn; }

bool windowOn=false;
bool pWindowOn(){ return windowOn; }
unsigned int windowWidth=0,windowHeight=0,windowMode=0;
unsigned int pWindowWidth(){
  if(windowOn){ return windowWidth; }
  else{
    if(debugOn){
      printf("[Warning] Could not find any Window,\n");
      fflush(stdout);
    }
    return 0;
  }
} unsigned int pWindowHeight(){
  if(windowOn){ return windowHeight; }
  else{
    if(debugOn){
      printf("[Warning] Could not find any Window,\n");
      fflush(stdout);
    }
    return 0;
  }
} unsigned int pWindowMode(){
  if(windowOn){ return windowMode; }
  else{
    if(debugOn){
      printf("[Warning] Could not find any Window,\n");
      fflush(stdout);
    }
    return 0;
  }
}
char windowTitle[256]="{,}";
char* pWindowTitle(){
  if(windowOn){ return windowTitle; }
  else{
    if(debugOn){
      printf("[Warning] Could not find any Window,\n");
      fflush(stdout);
    }
    return "";
  }
}

unsigned int windowBackgroundRed=0,windowBackgroundGreen=0,windowBackgroundBlue=0;
unsigned int pWindowBackgroundRed(){
  if(windowOn){ return windowBackgroundRed; }
  else{
    if(debugOn){
      printf("[Warning] Could not find any Window,\n");
      fflush(stdout);
    }
    return 0;
  }
} unsigned int pWindowBackgroundGreen(){
  if(windowOn){ return windowBackgroundGreen; }
  else{
    if(debugOn){
      printf("[Warning] Could not find any Window,\n");
      fflush(stdout);
    }
    return 0;
  }
} unsigned int pWindowBackgroundBlue(){
  if(windowOn){ return windowBackgroundBlue; }
  else{
    if(debugOn){
      printf("[Warning] Could not find any Window,\n");
      fflush(stdout);
    }
    return 0;
  }
}

void pWindowCreate(unsigned int pWidth,unsigned int pHeight,unsigned int pMode,bool pDebugOn){
  debugOn=pDebugOn;

  display=XOpenDisplay(NULL);
  if(display==NULL){
    windowOn=false;
    if(debugOn){
      printf("[Error] Could not open Display,\n");
      fflush(stdout);
    }
    return;
  }

  screen=DefaultScreen(display);
  window=XCreateSimpleWindow(
    display,RootWindow(display,screen), 
    0,0,pWidth,pHeight,1,
    BlackPixel(display,screen), 
    BlackPixel(display,screen)
  );

  if(window==0){
    if(debugOn){
      printf("[Error] Could not create Window,\n");
      fflush(stdout);
    }
    XCloseDisplay(display);
    return;
  } else{
    windowOn=true;
    windowWidth=pWidth;
    windowHeight=pHeight;
    windowMode=pMode;
  }
  XStoreName(display,window,"{,}");

  wmDelete=XInternAtom(display,"WM_DELETE_WINDOW",False);
  XSetWMProtocols(display,window,&wmDelete,1);

  Atom wmHints=XInternAtom(display,"_NET_WM_HINTS",False);
  if(wmHints!=None){
    long hints[5]={ 1,0,0,0,0 };
    if(pMode==0){ hints[1] = 0; }
    else if(pMode==1){
      hints[1]=1;
      hints[2]=0;
    } else{
      hints[1]=1;
      hints[2]=1;
      hints[3]=1;
    }

    XChangeProperty(display,window,wmHints,wmHints,32,PropModeReplace,(unsigned char*)hints,5);
  }

  XSelectInput(display,window,ExposureMask|KeyPressMask|StructureNotifyMask);
  XMapWindow(display,window);
}

void pWindowSetTitle(const char* pTitle){
  if(windowOn){
    strncpy(windowTitle,pTitle,sizeof(windowTitle)-1);
    windowTitle[sizeof(windowTitle)-1]='\0';
    XStoreName(display,window,pTitle);
    XFlush(display);
  } else{
    if(debugOn){
      printf("[Error] Could not set Title,\n");
      fflush(stdout);
    }
    return;
  }
}

void pWindowSetBackground(unsigned int pRed,unsigned int pGreen,unsigned int pBlue){
  if(windowOn){
    if(pRed>255){ pRed=255; }
    if(pGreen>255){ pGreen=255; }
    if(pBlue>255){ pBlue=255; }
    windowBackgroundRed=pRed;
    windowBackgroundGreen=pGreen;
    windowBackgroundBlue=pBlue;
    XSetWindowBackground(display,window,((pRed<<16)|(pGreen<<8)|pBlue));
    XClearWindow(display,window);
    XFlush(display);
  } else{
    if(debugOn){
      printf("[Error] Could not set Background Color,\n");
      fflush(stdout);
    }
    return;
  }
}

void pEventHandle(){
  if(windowOn){
    if(XPending(display)>0){
      XNextEvent(display,&event);
      if(event.type==Expose){ XClearWindow(display,window); }
      else if(event.type==ClientMessage){
        if((Atom)event.xclient.data.l[0]==wmDelete){
          windowBackgroundRed=0;
          windowBackgroundGreen=0;
          windowBackgroundBlue=0;
          strncpy(windowTitle,"{,}",sizeof(windowTitle)-1);
          windowTitle[sizeof(windowTitle)-1]='\0';
          keyFirst=0;
          keySecond=0;

          windowOn=false;
          XDestroyWindow(display,window);
          XCloseDisplay(display);
          return;
        }
      } else if(event.type==ConfigureNotify){
        windowWidth=event.xconfigure.width;
        windowHeight=event.xconfigure.height;
      } else if(event.type==KeyPress){
        keyPress[event.xkey.keycode]=true;
        keyHold[event.xkey.keycode]=true;
        keySecond=keyFirst;
        keyFirst=event.xkey.keycode;
      } else if(event.type==KeyRelease){
        if(XEventsQueued(display,QueuedAfterReading)){
          XPeekEvent(display,&report);
          if(report.type==KeyPress&&report.xkey.time==event.xkey.time&&report.xkey.keycode==event.xkey.keycode){
            keyPress[event.xkey.keycode]=false;
            keyHold[event.xkey.keycode]=true;
            XNextEvent(display,&event);
          }
        }
      }
    }
  } else{
    if(debugOn){
      printf("[Error] Could not handle Base Event,\n");
      fflush(stdout);
    }
    return;
  }
}

int modifyKey(const char *pKey){
  if(strcmp(pKey,"ESC")==0||strcmp(pKey,"esc")==0||strcmp(pKey,"Esc")==0){ return 9; }
  else if(strcmp(pKey,"TAB")==0||strcmp(pKey,"tab")==0||strcmp(pKey,"Tab")==0){ return 23; }
  else if(strcmp(pKey,"CAPS")==0||strcmp(pKey,"caps")==0||strcmp(pKey,"Caps")==0){ return 66; }
  else if(strcmp(pKey,"LSHIFT")==0||strcmp(pKey,"lshift")==0||strcmp(pKey,"LShift")==0){ return 50; }
  else if(strcmp(pKey,"LCTRL")==0||strcmp(pKey,"lctrl")==0||strcmp(pKey,"LCtrl")==0){ return 37; }
  else if(strcmp(pKey,"LMOD")==0||strcmp(pKey,"lmod")==0||strcmp(pKey,"LMod")==0){ return 133; }
  else if(strcmp(pKey,"LALT")==0||strcmp(pKey,"lalt")==0||strcmp(pKey,"LAlt")==0){ return 64; }
  else if(strcmp(pKey,"SPACE")==0||strcmp(pKey,"space")==0||strcmp(pKey,"Space")==0){ return 65; }

  else if(strcmp(pKey,"F1")==0||strcmp(pKey,"f1")==0){ return 67; }
  else if(strcmp(pKey,"F2")==0||strcmp(pKey,"f2")==0){ return 68; }
  else if(strcmp(pKey,"F3")==0||strcmp(pKey,"f3")==0){ return 69; }
  else if(strcmp(pKey,"F4")==0||strcmp(pKey,"f4")==0){ return 70; }
  else if(strcmp(pKey,"F5")==0||strcmp(pKey,"f5")==0){ return 71; }
  else if(strcmp(pKey,"F6")==0||strcmp(pKey,"f6")==0){ return 72; }
  else if(strcmp(pKey,"F7")==0||strcmp(pKey,"f7")==0){ return 73; }
  else if(strcmp(pKey,"F8")==0||strcmp(pKey,"f8")==0){ return 74; }
  else if(strcmp(pKey,"F9")==0||strcmp(pKey,"f9")==0){ return 75; }
  else if(strcmp(pKey,"F10")==0||strcmp(pKey,"f10")==0){ return 76; }
  else if(strcmp(pKey,"F11")==0||strcmp(pKey,"f11")==0){ return 95; }
  else if(strcmp(pKey,"F12")==0||strcmp(pKey,"f12")==0){ return 96; }

  else if(strcmp(pKey,"RALT")==0||strcmp(pKey,"ralt")==0||strcmp(pKey,"RAlt")==0){ return 108; }
  else if(strcmp(pKey,"RWIN")==0||strcmp(pKey,"rwin")==0||strcmp(pKey,"RWin")==0){ return 134; }
  else if(strcmp(pKey,"MENU")==0||strcmp(pKey,"menu")==0||strcmp(pKey,"Menu")==0){ return 135; }
  else if(strcmp(pKey,"RCTRL")==0||strcmp(pKey,"rctrl")==0||strcmp(pKey,"RCtrl")==0){ return 105; }
  else if(strcmp(pKey,"RSHIFT")==0||strcmp(pKey,"rshift")==0||strcmp(pKey,"RShift")==0){ return 62; }
  else if(strcmp(pKey,"ENTER")==0||strcmp(pKey,"enter")==0||strcmp(pKey,"Enter")==0){ return 36; }
  else if(strcmp(pKey,"BACKSPACE")==0||strcmp(pKey,"backspace")==0||strcmp(pKey,"Backspace")==0){ return 22; }

  else if(strcmp(pKey,"LARROW")==0||strcmp(pKey,"larrow")==0||strcmp(pKey,"LArrow")==0){ return 113; }
  else if(strcmp(pKey,"DARROW")==0||strcmp(pKey,"darrow")==0||strcmp(pKey,"DArrow")==0){ return 116; }
  else if(strcmp(pKey,"RARROW")==0||strcmp(pKey,"rarrow")==0||strcmp(pKey,"RArrow")==0){ return 114; }
  else if(strcmp(pKey,"UARROW")==0||strcmp(pKey,"uarrow")==0||strcmp(pKey,"UArrow")==0){ return 111; }

  else if(strcmp(pKey,"PRINTSCRN")==0||strcmp(pKey,"printscrn")==0||strcmp(pKey,"PrintScrn")==0){ return 107; }
  else if(strcmp(pKey,"SCROLLLOCK")==0||strcmp(pKey,"scrolllock")==0||strcmp(pKey,"ScrollLock")==0){ return 78; }
  else if(strcmp(pKey,"PAUSEBREAK")==0||strcmp(pKey,"pausebreak")==0||strcmp(pKey,"PauseBreak")==0){ return 127; }
  else if(strcmp(pKey,"INS")==0||strcmp(pKey,"ins")==0||strcmp(pKey,"Ins")==0){ return 118; }
  else if(strcmp(pKey,"HOME")==0||strcmp(pKey,"home")==0||strcmp(pKey,"Home")==0){ return 110; }
  else if(strcmp(pKey,"PAGEU")==0||strcmp(pKey,"pageu")==0||strcmp(pKey,"PageU")==0){ return 112; }
  else if(strcmp(pKey,"DEL")==0||strcmp(pKey,"del")==0||strcmp(pKey,"Del")==0){ return 119; }
  else if(strcmp(pKey,"END")==0||strcmp(pKey,"end")==0||strcmp(pKey,"End")==0){ return 115; }
  else if(strcmp(pKey,"PAGED")==0||strcmp(pKey,"paged")==0||strcmp(pKey,"PageD")==0){ return 117; }

  else if(strcmp(pKey,"Q")==0||strcmp(pKey,"q")==0){ return 24; }
  else if(strcmp(pKey,"W")==0||strcmp(pKey,"w")==0){ return 25; }
  else if(strcmp(pKey,"E")==0||strcmp(pKey,"e")==0){ return 26; }
  else if(strcmp(pKey,"R")==0||strcmp(pKey,"r")==0){ return 27; }
  else if(strcmp(pKey,"T")==0||strcmp(pKey,"t")==0){ return 28; }
  else if(strcmp(pKey,"Y")==0||strcmp(pKey,"y")==0){ return 29; }
  else if(strcmp(pKey,"U")==0||strcmp(pKey,"u")==0){ return 30; }
  else if(strcmp(pKey,"I")==0||strcmp(pKey,"i")==0){ return 31; }
  else if(strcmp(pKey,"O")==0||strcmp(pKey,"o")==0){ return 32; }
  else if(strcmp(pKey,"P")==0||strcmp(pKey,"p")==0){ return 33; }
  else if(strcmp(pKey,"A")==0||strcmp(pKey,"a")==0){ return 38; }
  else if(strcmp(pKey,"S")==0||strcmp(pKey,"s")==0){ return 39; }
  else if(strcmp(pKey,"D")==0||strcmp(pKey,"d")==0){ return 40; }
  else if(strcmp(pKey,"F")==0||strcmp(pKey,"f")==0){ return 41; }
  else if(strcmp(pKey,"G")==0||strcmp(pKey,"g")==0){ return 42; }
  else if(strcmp(pKey,"H")==0||strcmp(pKey,"h")==0){ return 43; }
  else if(strcmp(pKey,"J")==0||strcmp(pKey,"j")==0){ return 44; }
  else if(strcmp(pKey,"K")==0||strcmp(pKey,"k")==0){ return 45; }
  else if(strcmp(pKey,"L")==0||strcmp(pKey,"l")==0){ return 46; }
  else if(strcmp(pKey,"Z")==0||strcmp(pKey,"z")==0){ return 52; }
  else if(strcmp(pKey,"X")==0||strcmp(pKey,"x")==0){ return 53; }
  else if(strcmp(pKey,"C")==0||strcmp(pKey,"c")==0){ return 54; }
  else if(strcmp(pKey,"V")==0||strcmp(pKey,"v")==0){ return 55; }
  else if(strcmp(pKey,"B")==0||strcmp(pKey,"b")==0){ return 56; }
  else if(strcmp(pKey,"N")==0||strcmp(pKey,"n")==0){ return 57; }
  else if(strcmp(pKey,"M")==0||strcmp(pKey,"m")==0){ return 58; }

  else if(strcmp(pKey,"1")==0){ return 10; }
  else if(strcmp(pKey,"2")==0){ return 11; }
  else if(strcmp(pKey,"3")==0){ return 12; }
  else if(strcmp(pKey,"4")==0){ return 13; }
  else if(strcmp(pKey,"5")==0){ return 14; }
  else if(strcmp(pKey,"6")==0){ return 15; }
  else if(strcmp(pKey,"7")==0){ return 16; }
  else if(strcmp(pKey,"8")==0){ return 17; }
  else if(strcmp(pKey,"9")==0){ return 18; }
  else if(strcmp(pKey,"0")==0){ return 19; }

  else if(strcmp(pKey,"`")==0){ return 49; }
  else if(strcmp(pKey,",")==0){ return 59; }
  else if(strcmp(pKey,".")==0){ return 60; }
  else if(strcmp(pKey,"/")==0){ return 61; }
  else if(strcmp(pKey,";")==0){ return 47; }
  else if(strcmp(pKey,"'")==0){ return 48; }
  else if(strcmp(pKey,"\\")==0){ return 51; }
  else if(strcmp(pKey,"[")==0){ return 34; }
  else if(strcmp(pKey,"]")==0){ return 35; }
  else if(strcmp(pKey,"-")==0){ return 20; }
  else if(strcmp(pKey,"=")==0){ return 21; }
  else{ return (-1); }
}

bool pEventKeyPress(const char *pKey){
  if(windowOn){
    if(modifyKey(pKey)>=0&&modifyKey(pKey)<256){
      if(keyPress[modifyKey(pKey)]){
        keyPress[modifyKey(pKey)]=false;
        return true;
      }
    } else{
      if(debugOn){
        printf("[Warning] Invalid Key,\n");
        fflush(stdout);
      }
      return false;
    }
  } else{
    if(debugOn){
      printf("[Error] Could not handle Key Event,\n");
      fflush(stdout);
    }
    return false;
  }
}

bool pEventKeyHold(const char *pKey){
  if(windowOn){
    if(modifyKey(pKey)>=0&&modifyKey(pKey)<256){
      if(keyHold[modifyKey(pKey)]){
        keyHold[modifyKey(pKey)]=false;
        return true;
      }
    } else{
      if(debugOn){
        printf("[Warning] Invalid Key,\n");
        fflush(stdout);
      }
      return false;
    }
  } else{
    if(debugOn){
      printf("[Error] Could not handle Key Event,\n");
      fflush(stdout);
    }
    return false;
  }
}

bool pEventKeyLast(const char* pKey){
  if(keySecond==modifyKey(pKey)){ return true; }
  else{ return false; }
}

bool pEventCapsOn(){
  XKeyboardState keyboardState;
  XGetKeyboardControl(display,&keyboardState);
  if(keyboardState.led_mask&(1<<1)!=0){ return true; }
  else{ return false; }
}

void pClean(){

}
