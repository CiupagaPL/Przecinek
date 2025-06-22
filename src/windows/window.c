/*Orginal {,} Made By Ciupaga*/
/*https://github.com/CiupagaPL/Przecinek*/

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include<windows.h>

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

static COLORREF backgroundColor=RGB(0,0,0);
char CLASS_NAME[256];
HWND hwnd;
MSG message;
unsigned int current=0;
unsigned int keyPress[256]={ 0 };
bool keyHold[256]={ false };
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

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMessage,WPARAM wParameter,LPARAM lParameter){
if(uMessage==WM_DESTROY){
    windowBackgroundRed=0;
    windowBackgroundGreen=0;
    windowBackgroundBlue=0;
    strncpy(windowTitle,"{,}",sizeof(windowTitle)-1);
    windowTitle[sizeof(windowTitle)-1]='\0';
    keyFirst=0;
    keySecond=0;

    windowOn=false;
    FreeConsole();
    if(hwnd){
      DestroyWindow(hwnd);
      hwnd=NULL;
    }
    PostQuitMessage(0);
    return 0;
  } else if(uMessage==WM_PAINT){
    PAINTSTRUCT ps;
    HDC hdc=BeginPaint(hwnd,&ps);
    HBRUSH blackBrush=CreateSolidBrush(backgroundColor);
    FillRect(hdc,&ps.rcPaint,blackBrush);
    DeleteObject(blackBrush);
    EndPaint(hwnd,&ps);
  } else if(uMessage==WM_SIZE){
    windowWidth=LOWORD(lParameter);
    windowHeight=HIWORD(lParameter);
  } else if(uMessage==WM_KEYDOWN){
    if(keyPress[MapVirtualKey(wParameter,MAPVK_VK_TO_VSC)]!=2){
      keyPress[MapVirtualKey(wParameter,MAPVK_VK_TO_VSC)]=1;
      keyHold[MapVirtualKey(wParameter,MAPVK_VK_TO_VSC)]=true;
      keySecond=keyFirst;
      keyFirst=MapVirtualKey(wParameter,MAPVK_VK_TO_VSC);
    }
  } else if(uMessage==WM_KEYUP){
    keyPress[MapVirtualKey(wParameter,MAPVK_VK_TO_VSC)]=0;
    keyHold[MapVirtualKey(wParameter,MAPVK_VK_TO_VSC)]=false;
  }

  return DefWindowProc(hwnd,uMessage,wParameter,lParameter);
}

void pWindowCreate(unsigned int pWidth,unsigned int pHeight,unsigned int pMode,bool pDebugOn){
  debugOn=pDebugOn;
  current++;

  if(pDebugOn){
    AllocConsole();
    freopen("CONOUT$","w",stdout);
  } else{ FreeConsole(); }

  HINSTANCE hInstance=GetModuleHandle(NULL);
  WNDCLASS wc={0};
  wc.lpfnWndProc=WindowProc;
  wc.hInstance=hInstance;
  sprintf(CLASS_NAME,"PrzecinekClass%i",current);
  wc.lpszClassName=CLASS_NAME;
  wc.hCursor=LoadCursor(NULL,IDC_ARROW);
  if(!RegisterClass(&wc)){
    windowOn=false;
    if(debugOn){
      printf("[Error] Could not register Class,\n");
      fflush(stdout);
    }
    return;
  }

  if(pMode==0){
    hwnd=CreateWindowEx(
      0,CLASS_NAME,"{,}",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,CW_USEDEFAULT,
      pWidth,pHeight,
      NULL,NULL,hInstance,NULL
    );
  } else if(pMode==1){
    hwnd=CreateWindowEx(
      0,CLASS_NAME,"{,}",
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT,CW_USEDEFAULT,
      pWidth,pHeight,
      NULL,NULL,hInstance,NULL
    );
  } else{
    hwnd=CreateWindowEx(
      0,CLASS_NAME,"{,}",
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
      CW_USEDEFAULT,CW_USEDEFAULT,
      pWidth,pHeight,
      NULL,NULL,hInstance,NULL
    );
  }

  if(hwnd==NULL){
    windowOn=false;
    if(debugOn){
      printf("[Error] Could not create Window,\n");
      fflush(stdout);
    }
    return;
  } else{
    backgroundColor=RGB(windowBackgroundRed,windowBackgroundGreen,windowBackgroundBlue);
    InvalidateRect(hwnd,NULL,TRUE);

    windowOn=true;
    windowWidth=pWidth;
    windowHeight=pHeight;
    windowMode=pMode;
    ShowWindow(hwnd,SW_SHOW);
  }
}

void pWindowSetTitle(const char* pTitle){
  if(windowOn){
    strncpy(windowTitle,pTitle,sizeof(windowTitle)-1);
    windowTitle[sizeof(windowTitle)-1]='\0';
    SetWindowText(hwnd,pTitle);
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
    backgroundColor=RGB(pRed,pGreen,pBlue);
    windowBackgroundRed=pRed;
    windowBackgroundGreen=pGreen;
    windowBackgroundBlue=pBlue;
    InvalidateRect(hwnd,NULL,TRUE);
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
    if(GetMessage(&message,NULL,0,0)){
      TranslateMessage(&message);
      DispatchMessage(&message);
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
  if(strcmp(pKey,"ESC")==0||strcmp(pKey,"esc")==0||strcmp(pKey,"Esc")==0){ return MapVirtualKey(VK_ESCAPE,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"TAB")==0||strcmp(pKey,"tab")==0||strcmp(pKey,"Tab")==0){ return MapVirtualKey(VK_TAB,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"CAPS")==0||strcmp(pKey,"caps")==0||strcmp(pKey,"Caps")==0){ return MapVirtualKey(VK_CAPITAL,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"LSHIFT")==0||strcmp(pKey,"lshift")==0||strcmp(pKey,"LShift")==0){ return MapVirtualKey(VK_LSHIFT,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"LCTRL")==0||strcmp(pKey,"lctrl")==0||strcmp(pKey,"LCtrl")==0){ return MapVirtualKey(VK_LCONTROL,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"LMOD")==0||strcmp(pKey,"lmod")==0||strcmp(pKey,"LMod")==0){ return MapVirtualKey(VK_LWIN,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"LALT")==0||strcmp(pKey,"lalt")==0||strcmp(pKey,"LAlt")==0){ return MapVirtualKey(VK_LMENU,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"SPACE")==0||strcmp(pKey,"space")==0||strcmp(pKey,"Space")==0){ return MapVirtualKey(VK_SPACE,MAPVK_VK_TO_VSC); }

  else if(strcmp(pKey,"F1")==0||strcmp(pKey,"f1")==0){ return MapVirtualKey(VK_F1,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F2")==0||strcmp(pKey,"f2")==0){ return MapVirtualKey(VK_F2,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F3")==0||strcmp(pKey,"f3")==0){ return MapVirtualKey(VK_F3,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F4")==0||strcmp(pKey,"f4")==0){ return MapVirtualKey(VK_F4,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F5")==0||strcmp(pKey,"f5")==0){ return MapVirtualKey(VK_F5,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F6")==0||strcmp(pKey,"f6")==0){ return MapVirtualKey(VK_F6,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F7")==0||strcmp(pKey,"f7")==0){ return MapVirtualKey(VK_F7,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F8")==0||strcmp(pKey,"f8")==0){ return MapVirtualKey(VK_F8,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F9")==0||strcmp(pKey,"f9")==0){ return MapVirtualKey(VK_F9,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F10")==0||strcmp(pKey,"f10")==0){ return MapVirtualKey(VK_F10,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F11")==0||strcmp(pKey,"f11")==0){ return MapVirtualKey(VK_F11,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F12")==0||strcmp(pKey,"f12")==0){ return MapVirtualKey(VK_F12,MAPVK_VK_TO_VSC); }

  else if(strcmp(pKey,"RALT")==0||strcmp(pKey,"ralt")==0||strcmp(pKey,"RAlt")==0){ return MapVirtualKey(VK_RMENU,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"RWIN")==0||strcmp(pKey,"rwin")==0||strcmp(pKey,"RWin")==0){ return MapVirtualKey(VK_RWIN,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"MENU")==0||strcmp(pKey,"menu")==0||strcmp(pKey,"Menu")==0){ return MapVirtualKey(VK_APPS,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"RCTRL")==0||strcmp(pKey,"rctrl")==0||strcmp(pKey,"RCtrl")==0){ return MapVirtualKey(VK_RCONTROL,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"RSHIFT")==0||strcmp(pKey,"rshift")==0||strcmp(pKey,"RShift")==0){ return MapVirtualKey(VK_RSHIFT,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"ENTER")==0||strcmp(pKey,"enter")==0||strcmp(pKey,"Enter")==0){ return MapVirtualKey(VK_RETURN,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"BACKSPACE")==0||strcmp(pKey,"backspace")==0||strcmp(pKey,"Backspace")==0){ return MapVirtualKey(VK_BACK,MAPVK_VK_TO_VSC); }

  else if(strcmp(pKey,"LARROW")==0||strcmp(pKey,"larrow")==0||strcmp(pKey,"LArrow")==0){ return MapVirtualKey(VK_LEFT,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"DARROW")==0||strcmp(pKey,"darrow")==0||strcmp(pKey,"DArrow")==0){ return MapVirtualKey(VK_DOWN,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"RARROW")==0||strcmp(pKey,"rarrow")==0||strcmp(pKey,"RArrow")==0){ return MapVirtualKey(VK_RIGHT,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"UARROW")==0||strcmp(pKey,"uarrow")==0||strcmp(pKey,"UArrow")==0){ return MapVirtualKey(VK_UP,MAPVK_VK_TO_VSC); }

  else if(strcmp(pKey,"PRINTSCRN")==0||strcmp(pKey,"printscrn")==0||strcmp(pKey,"PrintScrn")==0){ return MapVirtualKey(VK_SNAPSHOT,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"SCROLLLOCK")==0||strcmp(pKey,"scrolllock")==0||strcmp(pKey,"ScrollLock")==0){ return MapVirtualKey(VK_SCROLL,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"PAUSEBREAK")==0||strcmp(pKey,"pausebreak")==0||strcmp(pKey,"PauseBreak")==0){ return MapVirtualKey(VK_PAUSE,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"INS")==0||strcmp(pKey,"ins")==0||strcmp(pKey,"Ins")==0){ return MapVirtualKey(VK_INSERT,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"HOME")==0||strcmp(pKey,"home")==0||strcmp(pKey,"Home")==0){ return MapVirtualKey(VK_HOME,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"PAGEU")==0||strcmp(pKey,"pageu")==0||strcmp(pKey,"PageU")==0){ return MapVirtualKey(VK_PRIOR,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"DEL")==0||strcmp(pKey,"del")==0||strcmp(pKey,"Del")==0){ return MapVirtualKey(VK_DELETE,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"END")==0||strcmp(pKey,"end")==0||strcmp(pKey,"End")==0){ return MapVirtualKey(VK_END,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"PAGED")==0||strcmp(pKey,"paged")==0||strcmp(pKey,"PageD")==0){ return MapVirtualKey(VK_NEXT,MAPVK_VK_TO_VSC); }

  else if(strcmp(pKey,"Q")==0||strcmp(pKey,"q")==0){ return MapVirtualKey(VK_Q,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"W")==0||strcmp(pKey,"w")==0){ return MapVirtualKey(VK_W,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"E")==0||strcmp(pKey,"e")==0){ return MapVirtualKey(VK_E,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"R")==0||strcmp(pKey,"r")==0){ return MapVirtualKey(VK_R,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"T")==0||strcmp(pKey,"t")==0){ return MapVirtualKey(VK_T,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"Y")==0||strcmp(pKey,"y")==0){ return MapVirtualKey(VK_Y,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"U")==0||strcmp(pKey,"u")==0){ return MapVirtualKey(VK_U,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"I")==0||strcmp(pKey,"i")==0){ return MapVirtualKey(VK_I,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"O")==0||strcmp(pKey,"o")==0){ return MapVirtualKey(VK_O,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"P")==0||strcmp(pKey,"p")==0){ return MapVirtualKey(VK_P,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"A")==0||strcmp(pKey,"a")==0){ return MapVirtualKey(VK_A,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"S")==0||strcmp(pKey,"s")==0){ return MapVirtualKey(VK_S,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"D")==0||strcmp(pKey,"d")==0){ return MapVirtualKey(VK_D,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"F")==0||strcmp(pKey,"f")==0){ return MapVirtualKey(VK_F,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"G")==0||strcmp(pKey,"g")==0){ return MapVirtualKey(VK_G,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"H")==0||strcmp(pKey,"h")==0){ return MapVirtualKey(VK_H,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"J")==0||strcmp(pKey,"j")==0){ return MapVirtualKey(VK_J,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"K")==0||strcmp(pKey,"k")==0){ return MapVirtualKey(VK_K,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"L")==0||strcmp(pKey,"l")==0){ return MapVirtualKey(VK_L,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"Z")==0||strcmp(pKey,"z")==0){ return MapVirtualKey(VK_Z,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"X")==0||strcmp(pKey,"x")==0){ return MapVirtualKey(VK_X,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"C")==0||strcmp(pKey,"c")==0){ return MapVirtualKey(VK_C,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"V")==0||strcmp(pKey,"v")==0){ return MapVirtualKey(VK_V,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"B")==0||strcmp(pKey,"b")==0){ return MapVirtualKey(VK_B,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"N")==0||strcmp(pKey,"n")==0){ return MapVirtualKey(VK_N,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"M")==0||strcmp(pKey,"m")==0){ return MapVirtualKey(VK_M,MAPVK_VK_TO_VSC); }

  else if(strcmp(pKey,"1")==0){ return MapVirtualKey(VK_1,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"2")==0){ return MapVirtualKey(VK_2,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"3")==0){ return MapVirtualKey(VK_3,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"4")==0){ return MapVirtualKey(VK_4,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"5")==0){ return MapVirtualKey(VK_5,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"6")==0){ return MapVirtualKey(VK_6,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"7")==0){ return MapVirtualKey(VK_7,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"8")==0){ return MapVirtualKey(VK_8,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"9")==0){ return MapVirtualKey(VK_9,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"0")==0){ return MapVirtualKey(VK_0,MAPVK_VK_TO_VSC); }

  else if(strcmp(pKey,"`")==0){ return MapVirtualKey(VK_OEM_3,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,",")==0){ return MapVirtualKey(VK_OEM_COMMA,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,".")==0){ return MapVirtualKey(VK_OEM_PERIOD,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"/")==0){ return MapVirtualKey(VK_OEM_2,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,";")==0){ return MapVirtualKey(VK_OEM_SEMICOLON,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"'")==0){ return MapVirtualKey(VK_OEM_QUOTE,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"\\")==0){ return MapVirtualKey(VK_OEM_5,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"[")==0){ return MapVirtualKey(VK_OEM_4,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"]")==0){ return MapVirtualKey(VK_OEM_6,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"-")==0){ return MapVirtualKey(VK_OEM_MINUS,MAPVK_VK_TO_VSC); }
  else if(strcmp(pKey,"=")==0){ return MapVirtualKey(VK_OEM_PLUS,MAPVK_VK_TO_VSC); }
  else{ return (-1); }
}

bool pEventKeyPress(const char *pKey){
  if(windowOn){
    if(modifyKey(pKey)>=0&&modifyKey(pKey)<256){
      if(keyPress[modifyKey(pKey)]==1){
        keyPress[modifyKey(pKey)]=2;
        return true;

      } else{ return false; }
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
      if(keyHold[modifyKey(pKey)]){ return true; }
      else{ return false; }
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
  if(GetKeyState(VK_CAPITAL)&0x0001){ return true; }
  else{ return false; }
}
