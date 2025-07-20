/*Orginal {,} Made By Ciupaga*/
/*https://github.com/CiupagaPL/Przecinek*/

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include<windows.h>

bool debug=false;
void pDebug(bool on){
  debug=on;
  if(debug){
    AllocConsole();
    freopen("CONOUT$","w",stdout);
  } else{ FreeConsole(); }
}

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
  bool on;

  WPARAM wParameter;
} pWindowProcW;

typedef struct{
  bool on;

  LPARAM lParameter;
} pWindowProcL;

typedef struct{
  bool on,active;

  unsigned int x,y,width,height,mode;
  char title[256];
  unsigned int red,green,blue;

  char CLASS[16];
  HWND hwnd;
  bool W_DESTROY,W_MOUSEMOVE;
  pWindowProcW W_ACTIVATE,W_KEYDOWN;
  pWindowProcL W_SIZE,W_MOVE;

  unsigned int keyPress,keyHold[256];
} pWindowWin;

typedef struct{
  unsigned int ID;

  unsigned int x,y,width,height;

  HWND hwnd;
} pWindowPointer;

unsigned int activeWinID=0,currentWinID=0;
pWindowWin windowWin[64];
pWindow currentWindow;
MSG message;
pStatus przecinek;

void pWindowReset(pWindow* window);

LRESULT CALLBACK pWindowProc(HWND hwnd,UINT uMessage,WPARAM wParameter,LPARAM lParameter){
  pWindowPointer* windowPointer=(pWindowPointer*)GetWindowLongPtr(hwnd,GWLP_USERDATA);

  if(uMessage==WM_CREATE){
    pWindowPointer* newWindowPointer=(pWindowPointer*)malloc(sizeof(pWindowPointer));
    newWindowPointer->ID=currentWindow.ID;
    newWindowPointer->width=currentWindow.width;
    newWindowPointer->height=currentWindow.height;
    newWindowPointer->hwnd=hwnd;
    SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)newWindowPointer);
  } else if(uMessage==WM_DESTROY){
    windowWin[currentWinID-1].W_DESTROY=true;
    hwnd=NULL;
    return 0;
  } else if(uMessage==WM_PAINT){
    PAINTSTRUCT paintStruct;
    HDC hdc=BeginPaint(hwnd,&paintStruct);
    HBRUSH blackBrush=CreateSolidBrush(RGB(windowWin[windowPointer->ID-1].red,windowWin[windowPointer->ID-1].green,windowWin[windowPointer->ID-1].blue));
    FillRect(hdc,&paintStruct.rcPaint,blackBrush);
    DeleteObject(blackBrush);
    EndPaint(hwnd,&paintStruct);
  } else if(uMessage==WM_ACTIVATE){
    for(int c=0;c<256;c++){ windowWin[windowPointer->ID-1].keyHold[c]=0; }
    windowWin[windowPointer->ID-1].keyPress=0;
    windowWin[windowPointer->ID-1].W_ACTIVATE.on=true;
    windowWin[windowPointer->ID-1].W_ACTIVATE.wParameter=wParameter;
  } else if(uMessage==WM_MOUSEMOVE){ windowWin[windowPointer->ID-1].W_MOUSEMOVE=true; }
  else if(uMessage==WM_SIZE){
    windowWin[windowPointer->ID-1].W_SIZE.on=true;
    windowWin[windowPointer->ID-1].W_SIZE.lParameter=lParameter;
  } else if(uMessage==WM_MOVE){
    windowWin[windowPointer->ID-1].W_MOVE.on=true;
    windowWin[windowPointer->ID-1].W_MOVE.lParameter=lParameter;
  } else if(uMessage==WM_KEYDOWN){
    if(windowWin[windowPointer->ID-1].keyHold[MapVirtualKey(wParameter,MAPVK_VK_TO_VSC)]==0){
      windowWin[windowPointer->ID-1].W_KEYDOWN.on=true;
      windowWin[windowPointer->ID-1].W_KEYDOWN.wParameter=wParameter;
    }
  } else if(uMessage==WM_KEYUP){ windowWin[windowPointer->ID-1].keyHold[MapVirtualKey(wParameter,MAPVK_VK_TO_VSC)]=0; }

  return DefWindowProc(hwnd,uMessage,wParameter,lParameter);
}

pWindow pWindowCreate(unsigned int width,unsigned int height,unsigned int mode){
  pWindow window;
  for(int c=1;c<=64;c++){
    if(!windowWin[c-1].on){
      window.ID=c;
      break;
    } else if(c==64){
      if(debug){
        printf("[Error] Too many Windows active,\n");
        fflush(stdout);
      }
      window.on=false;
      windowWin[window.ID-1].on=false;
      return window;
    }
  }

  window.on=true;
  windowWin[window.ID-1].on=true;
  window.width=width;
  window.height=height;
  window.x=(GetSystemMetrics(SM_CXSCREEN)-window.width)/2;
  window.y=(GetSystemMetrics(SM_CYSCREEN)-window.height)/2;
  window.mode=mode;
  currentWindow=window;

  HINSTANCE hInstance=GetModuleHandle(NULL);
  WNDCLASS wClass={0};
  wClass.lpfnWndProc=pWindowProc;
  wClass.hInstance=hInstance;
  sprintf(windowWin[window.ID-1].CLASS,"PrzecinekClass%i",window.ID);
  wClass.lpszClassName=windowWin[window.ID-1].CLASS;
  wClass.hCursor=LoadCursor(NULL,IDC_ARROW);
  if(!RegisterClass(&wClass)){
    if(debug){
      printf("[Error] Could not register Window Class,\n");
      fflush(stdout);
    }
    pWindowReset(&window);
    return window;
  }

  DWORD style=WS_OVERLAPPEDWINDOW;
  if(window.mode==1){ style=WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX; }
  else if(window.mode==2){ style=WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU; }

  windowWin[window.ID-1].hwnd=CreateWindowEx(
    0,windowWin[window.ID-1].CLASS,"{,}",
    style,
    CW_USEDEFAULT,CW_USEDEFAULT,
    width,height,
    NULL,NULL,hInstance,NULL
  );

  if(windowWin[window.ID-1].hwnd==NULL){
    if(debug){
      printf("[Error] Could not create Window,\n");
      fflush(stdout);
    }
    pWindowReset(&window);
  } else{
    window.on=true;
    windowWin[window.ID-1].on=true;

    POINT cursor;
    if(GetCursorPos(&cursor)){
      przecinek.cursor.x=cursor.x;
      przecinek.cursor.y=cursor.y;
    }
    przecinek.display.width=GetSystemMetrics(SM_CXSCREEN);
    przecinek.display.height=GetSystemMetrics(SM_CYSCREEN);

    InvalidateRect(windowWin[window.ID-1].hwnd,NULL,TRUE);
    ShowWindow(windowWin[window.ID-1].hwnd,SW_SHOW);
    SetWindowPos(windowWin[window.ID-1].hwnd,NULL,window.x,window.y,0,0,SWP_NOZORDER|SWP_NOSIZE);
    SetForegroundWindow(windowWin[window.ID-1].hwnd);
  }
  return window;
}

void pWindowSetPosition(pWindow* window,unsigned int x,unsigned int y){
  if(window->on){
    window->x=x;
    window->y=y;

    SetWindowPos(windowWin[window->ID-1].hwnd,NULL,window->x,window->y,0,0,SWP_NOZORDER|SWP_NOSIZE);
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
    strncpy(windowWin[window->ID-1].title,title,sizeof(windowWin[window->ID-1].title)-1);
    windowWin[window->ID-1].title[sizeof(windowWin[window->ID-1].title)-1]='\0';

    SetWindowText(windowWin[window->ID-1].hwnd,window->title);
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
    windowWin[window->ID-1].red=red;
    windowWin[window->ID-1].green=green;
    windowWin[window->ID-1].blue=blue;

    InvalidateRect(windowWin[window->ID-1].hwnd,NULL,TRUE);
  } else if(debug){
    printf("[Error] Could not set Window Background,\n");
    fflush(stdout);
  }
  return;
}

void pWindowHandle(pWindow* window){
  if(window->on){
    currentWinID=window->ID;
    if(GetMessage(&message,NULL,0,0)){
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    if(windowWin[currentWinID-1].W_DESTROY){ pWindowReset(window); }
    if(windowWin[currentWinID-1].W_ACTIVATE.on){
      if(windowWin[currentWinID-1].W_ACTIVATE.wParameter==WA_ACTIVE||windowWin[currentWinID-1].W_ACTIVATE.wParameter==WA_CLICKACTIVE){
        windowWin[window->ID-1].active=true;
        activeWinID=window->ID;
      } else{
        windowWin[window->ID-1].active=false;
        if(activeWinID==window->ID){ activeWinID=0; }
      }
      windowWin[currentWinID-1].W_ACTIVATE.on=false;
    }
    if(windowWin[currentWinID-1].W_MOUSEMOVE){
      POINT cursor;
      if(GetCursorPos(&cursor)){
        przecinek.cursor.x=cursor.x;
        przecinek.cursor.y=cursor.y;
      }
      windowWin[currentWinID-1].W_MOUSEMOVE=false;
    } if(windowWin[currentWinID-1].W_SIZE.on){
      windowWin[window->ID-1].width=LOWORD(windowWin[currentWinID-1].W_SIZE.lParameter);
      windowWin[window->ID-1].height=HIWORD(windowWin[currentWinID-1].W_SIZE.lParameter);
      windowWin[currentWinID-1].W_SIZE.on=false;
    } if(windowWin[currentWinID-1].W_MOVE.on){
      windowWin[window->ID-1].x=LOWORD(windowWin[currentWinID-1].W_MOVE.lParameter);
      windowWin[window->ID-1].y=HIWORD(windowWin[currentWinID-1].W_MOVE.lParameter);
      windowWin[currentWinID-1].W_MOVE.on=false;
    } if(windowWin[currentWinID-1].W_KEYDOWN.on){
      if(activeWinID==currentWinID){
        if(windowWin[window->ID-1].keyHold[MapVirtualKey(windowWin[currentWinID-1].W_KEYDOWN.wParameter,MAPVK_VK_TO_VSC)]==0){
          windowWin[window->ID-1].keyPress=MapVirtualKey(windowWin[currentWinID-1].W_KEYDOWN.wParameter,MAPVK_VK_TO_VSC);
          windowWin[window->ID-1].keyHold[windowWin[window->ID-1].keyPress]=1;
        }
      }
      windowWin[currentWinID-1].W_KEYDOWN.on=false;
    }

    przecinek.display.width=GetSystemMetrics(SM_CXSCREEN);
    przecinek.display.height=GetSystemMetrics(SM_CYSCREEN);

    window->on=windowWin[window->ID-1].on;
    window->active=windowWin[window->ID-1].active;

    window->x=windowWin[window->ID-1].x;
    window->y=windowWin[window->ID-1].y;
    window->width=windowWin[window->ID-1].width;
    window->height=windowWin[window->ID-1].height;
    window->mode=windowWin[window->ID-1].mode;
    strncpy(window->title,windowWin[window->ID-1].title,sizeof(window->title)-1);
    window->title[sizeof(window->title)-1]='\0';
    window->red=windowWin[window->ID-1].red;
    window->green=windowWin[window->ID-1].green;
    window->blue=windowWin[window->ID-1].blue;
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

  windowWin[window->ID-1].on=false;
  windowWin[window->ID-1].active=false;

  windowWin[window->ID-1].x=0;
  windowWin[window->ID-1].y=0;
  windowWin[window->ID-1].width=0;
  windowWin[window->ID-1].height=0;
  windowWin[window->ID-1].mode=0;
  strncpy(windowWin[window->ID-1].title,"{,}",sizeof(windowWin[window->ID-1].title)-1);
  windowWin[window->ID-1].title[sizeof(windowWin[window->ID-1].title)-1]='\0';
  windowWin[window->ID-1].red=0;
  windowWin[window->ID-1].green=0;
  windowWin[window->ID-1].blue=0;

  windowWin[window->ID-1].hwnd=NULL;
  windowWin[window->ID-1].W_DESTROY=false;
  windowWin[window->ID-1].W_MOUSEMOVE=false;
  windowWin[window->ID-1].W_ACTIVATE.on=false;
  windowWin[window->ID-1].W_KEYDOWN.on=false;
  windowWin[window->ID-1].W_SIZE.on=false;
  windowWin[window->ID-1].W_MOVE.on=false;

  windowWin[window->ID-1].keyPress=0;
  for(int c=0;c<256;c++){ windowWin[window->ID-1].keyHold[c]=0; }
  return;
}

void pWindowClose(pWindow* window){
  if(window->on){ PostMessage(windowWin[window->ID-1].hwnd,WM_CLOSE,0,0); }
  else if(debug){
    printf("[Warning] Window is already closed,\n");
    fflush(stdout);
  }
  return;
}

unsigned int pModifyKey(const char* key);

bool pKeyPress(const char *key){
  if(pModifyKey(key)>0&&pModifyKey(key)<256){
    if(activeWinID!=0&&windowWin[activeWinID-1].keyHold[pModifyKey(key)]==1){
      windowWin[activeWinID-1].keyHold[pModifyKey(key)]=2;
      windowWin[activeWinID-1].keyPress=0;
      return true;
    }
  } else if(debug){
    printf("[Warning] Invalid Key,\n");
    fflush(stdout);
  }
  return false;
}

bool pKeyHold(const char *key){
  if(activeWinID!=0){
    if(pModifyKey(key)>0&&pModifyKey(key)<256){
      if(activeWinID!=0){ return windowWin[activeWinID-1].keyHold[pModifyKey(key)]!=0; }
    } else if(debug){
      printf("[Warning] Invalid Key,\n");
      fflush(stdout);
    }
  }
  return false;
}

bool pKeyCaps(){
  if(activeWinID!=0){ return GetKeyState(VK_CAPITAL)&0x0001; }
  else if(debug){
    printf("[Warning] Could not check Caps State,\n");
    fflush(stdout);
  }
  return false;
}

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_0 0x30

#define VK_Q 0x51
#define VK_W 0x57
#define VK_E 0x45
#define VK_R 0x52
#define VK_T 0x54
#define VK_Y 0x59
#define VK_U 0x55
#define VK_I 0x49
#define VK_O 0x4F
#define VK_P 0x50
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_Z 0x5A
#define VK_X 0x58
#define VK_C 0x43
#define VK_V 0x56
#define VK_B 0x42
#define VK_N 0x4E
#define VK_M 0x4D

#define VK_OEM_SEMICOLON 0xBA
#define VK_OEM_QUOTE 0xDE

unsigned int pModifyKey(const char *key){
  if(strcmp(key,"ESC")==0||strcmp(key,"esc")==0||strcmp(key,"Esc")==0){ return MapVirtualKey(VK_ESCAPE,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"TAB")==0||strcmp(key,"tab")==0||strcmp(key,"Tab")==0){ return MapVirtualKey(VK_TAB,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"CAPS")==0||strcmp(key,"caps")==0||strcmp(key,"Caps")==0){ return MapVirtualKey(VK_CAPITAL,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"LSHIFT")==0||strcmp(key,"lshift")==0||strcmp(key,"LShift")==0){ return MapVirtualKey(VK_LSHIFT,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"LCTRL")==0||strcmp(key,"lctrl")==0||strcmp(key,"LCtrl")==0){ return MapVirtualKey(VK_LCONTROL,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"LMOD")==0||strcmp(key,"lmod")==0||strcmp(key,"LMod")==0){ return MapVirtualKey(VK_LWIN,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"LALT")==0||strcmp(key,"lalt")==0||strcmp(key,"LAlt")==0){ return MapVirtualKey(VK_LMENU,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"SPACE")==0||strcmp(key,"space")==0||strcmp(key,"Space")==0){ return MapVirtualKey(VK_SPACE,MAPVK_VK_TO_VSC); }

  else if(strcmp(key,"F1")==0||strcmp(key,"f1")==0){ return MapVirtualKey(VK_F1,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F2")==0||strcmp(key,"f2")==0){ return MapVirtualKey(VK_F2,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F3")==0||strcmp(key,"f3")==0){ return MapVirtualKey(VK_F3,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F4")==0||strcmp(key,"f4")==0){ return MapVirtualKey(VK_F4,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F5")==0||strcmp(key,"f5")==0){ return MapVirtualKey(VK_F5,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F6")==0||strcmp(key,"f6")==0){ return MapVirtualKey(VK_F6,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F7")==0||strcmp(key,"f7")==0){ return MapVirtualKey(VK_F7,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F8")==0||strcmp(key,"f8")==0){ return MapVirtualKey(VK_F8,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F9")==0||strcmp(key,"f9")==0){ return MapVirtualKey(VK_F9,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F10")==0||strcmp(key,"f10")==0){ return MapVirtualKey(VK_F10,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F11")==0||strcmp(key,"f11")==0){ return MapVirtualKey(VK_F11,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F12")==0||strcmp(key,"f12")==0){ return MapVirtualKey(VK_F12,MAPVK_VK_TO_VSC); }

  else if(strcmp(key,"RALT")==0||strcmp(key,"ralt")==0||strcmp(key,"RAlt")==0){ return MapVirtualKey(VK_RMENU,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"RWIN")==0||strcmp(key,"rwin")==0||strcmp(key,"RWin")==0){ return MapVirtualKey(VK_RWIN,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"MENU")==0||strcmp(key,"menu")==0||strcmp(key,"Menu")==0){ return MapVirtualKey(VK_APPS,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"RCTRL")==0||strcmp(key,"rctrl")==0||strcmp(key,"RCtrl")==0){ return MapVirtualKey(VK_RCONTROL,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"RSHIFT")==0||strcmp(key,"rshift")==0||strcmp(key,"RShift")==0){ return MapVirtualKey(VK_RSHIFT,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"ENTER")==0||strcmp(key,"enter")==0||strcmp(key,"Enter")==0){ return MapVirtualKey(VK_RETURN,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"BACKSPACE")==0||strcmp(key,"backspace")==0||strcmp(key,"Backspace")==0){ return MapVirtualKey(VK_BACK,MAPVK_VK_TO_VSC); }

  else if(strcmp(key,"LARROW")==0||strcmp(key,"larrow")==0||strcmp(key,"LArrow")==0){ return MapVirtualKey(VK_LEFT,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"DARROW")==0||strcmp(key,"darrow")==0||strcmp(key,"DArrow")==0){ return MapVirtualKey(VK_DOWN,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"RARROW")==0||strcmp(key,"rarrow")==0||strcmp(key,"RArrow")==0){ return MapVirtualKey(VK_RIGHT,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"UARROW")==0||strcmp(key,"uarrow")==0||strcmp(key,"UArrow")==0){ return MapVirtualKey(VK_UP,MAPVK_VK_TO_VSC); }

  else if(strcmp(key,"PRINTSCRN")==0||strcmp(key,"printscrn")==0||strcmp(key,"PrintScrn")==0){ return MapVirtualKey(VK_SNAPSHOT,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"SCROLLLOCK")==0||strcmp(key,"scrolllock")==0||strcmp(key,"ScrollLock")==0){ return MapVirtualKey(VK_SCROLL,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"PAUSEBREAK")==0||strcmp(key,"pausebreak")==0||strcmp(key,"PauseBreak")==0){ return MapVirtualKey(VK_PAUSE,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"INS")==0||strcmp(key,"ins")==0||strcmp(key,"Ins")==0){ return MapVirtualKey(VK_INSERT,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"HOME")==0||strcmp(key,"home")==0||strcmp(key,"Home")==0){ return MapVirtualKey(VK_HOME,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"PAGEU")==0||strcmp(key,"pageu")==0||strcmp(key,"PageU")==0){ return MapVirtualKey(VK_PRIOR,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"DEL")==0||strcmp(key,"del")==0||strcmp(key,"Del")==0){ return MapVirtualKey(VK_DELETE,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"END")==0||strcmp(key,"end")==0||strcmp(key,"End")==0){ return MapVirtualKey(VK_END,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"PAGED")==0||strcmp(key,"paged")==0||strcmp(key,"PageD")==0){ return MapVirtualKey(VK_NEXT,MAPVK_VK_TO_VSC); }

  else if(strcmp(key,"Q")==0||strcmp(key,"q")==0){ return MapVirtualKey(VK_Q,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"W")==0||strcmp(key,"w")==0){ return MapVirtualKey(VK_W,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"E")==0||strcmp(key,"e")==0){ return MapVirtualKey(VK_E,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"R")==0||strcmp(key,"r")==0){ return MapVirtualKey(VK_R,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"T")==0||strcmp(key,"t")==0){ return MapVirtualKey(VK_T,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"Y")==0||strcmp(key,"y")==0){ return MapVirtualKey(VK_Y,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"U")==0||strcmp(key,"u")==0){ return MapVirtualKey(VK_U,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"I")==0||strcmp(key,"i")==0){ return MapVirtualKey(VK_I,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"O")==0||strcmp(key,"o")==0){ return MapVirtualKey(VK_O,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"P")==0||strcmp(key,"p")==0){ return MapVirtualKey(VK_P,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"A")==0||strcmp(key,"a")==0){ return MapVirtualKey(VK_A,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"S")==0||strcmp(key,"s")==0){ return MapVirtualKey(VK_S,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"D")==0||strcmp(key,"d")==0){ return MapVirtualKey(VK_D,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"F")==0||strcmp(key,"f")==0){ return MapVirtualKey(VK_F,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"G")==0||strcmp(key,"g")==0){ return MapVirtualKey(VK_G,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"H")==0||strcmp(key,"h")==0){ return MapVirtualKey(VK_H,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"J")==0||strcmp(key,"j")==0){ return MapVirtualKey(VK_J,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"K")==0||strcmp(key,"k")==0){ return MapVirtualKey(VK_K,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"L")==0||strcmp(key,"l")==0){ return MapVirtualKey(VK_L,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"Z")==0||strcmp(key,"z")==0){ return MapVirtualKey(VK_Z,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"X")==0||strcmp(key,"x")==0){ return MapVirtualKey(VK_X,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"C")==0||strcmp(key,"c")==0){ return MapVirtualKey(VK_C,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"V")==0||strcmp(key,"v")==0){ return MapVirtualKey(VK_V,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"B")==0||strcmp(key,"b")==0){ return MapVirtualKey(VK_B,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"N")==0||strcmp(key,"n")==0){ return MapVirtualKey(VK_N,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"M")==0||strcmp(key,"m")==0){ return MapVirtualKey(VK_M,MAPVK_VK_TO_VSC); }

  else if(strcmp(key,"1")==0){ return MapVirtualKey(VK_1,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"2")==0){ return MapVirtualKey(VK_2,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"3")==0){ return MapVirtualKey(VK_3,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"4")==0){ return MapVirtualKey(VK_4,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"5")==0){ return MapVirtualKey(VK_5,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"6")==0){ return MapVirtualKey(VK_6,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"7")==0){ return MapVirtualKey(VK_7,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"8")==0){ return MapVirtualKey(VK_8,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"9")==0){ return MapVirtualKey(VK_9,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"0")==0){ return MapVirtualKey(VK_0,MAPVK_VK_TO_VSC); }

  else if(strcmp(key,"`")==0){ return MapVirtualKey(VK_OEM_3,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,",")==0){ return MapVirtualKey(VK_OEM_COMMA,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,".")==0){ return MapVirtualKey(VK_OEM_PERIOD,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"/")==0){ return MapVirtualKey(VK_OEM_2,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,";")==0){ return MapVirtualKey(VK_OEM_SEMICOLON,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"'")==0){ return MapVirtualKey(VK_OEM_QUOTE,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"\\")==0){ return MapVirtualKey(VK_OEM_5,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"[")==0){ return MapVirtualKey(VK_OEM_4,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"]")==0){ return MapVirtualKey(VK_OEM_6,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"-")==0){ return MapVirtualKey(VK_OEM_MINUS,MAPVK_VK_TO_VSC); }
  else if(strcmp(key,"=")==0){ return MapVirtualKey(VK_OEM_PLUS,MAPVK_VK_TO_VSC); }
  return 0;
}
