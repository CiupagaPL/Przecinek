/* Orginal {,} Made By Ciupaga
 * https://github.com/CiupagaPL/Przecinek
 * Make Sure to Read the License!
 *         |\_____/|
 *         |       |
 *  ___    | >   < |
 * /   \  _\ = , = /
 * \__  \/        |
 *    \_         /
 *      \ \ \--\ \
 *      {,{,} {,},}
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <windows.h>

/* |\____/| [pSize], [pPosition], [pColor] Structure
 * |  o o |
 */
typedef struct{ unsigned int width, height; } pSize;
typedef struct{ int x, y; } pPosition;
typedef struct{ unsigned int r, g, b; } pColor;

/* |\____/| [pWindow] Structure
 * |  o o |
 */
typedef struct{
  unsigned int ID;
  bool active;

  int x, y;
  unsigned int width, height, mode, frameLimit;
  char title[128];
} pWindow;

/* |\____/| [pBuildWin], [pWindowPointer] Structure and Variables
 * |  o o |
 */
typedef struct{
  int x, y;
  unsigned int width, height, mode;
  char title[128];

  HBITMAP hBitmap;
  HDC hdc;
  HDC hMemDC;
  HINSTANCE hInstance;
  WNDCLASS wClass;
  HWND hwnd;
  DWORD style;

  char class[8];

  bool DESTROY, MOUSEMOVE, FOCUSIN, FOCUSOUT;
 
  pSize SIZE;
  pPosition MOVE;

  bool KEYON;
  unsigned int KEY[256];

  DWORD frameStart;
  unsigned int frameCount;
} pBuildWin;

unsigned int activeWinID=0, currentWinID=0;

pBuildWin build[32];
pWindow currentWindow;
MSG message;
POINT cursor;
RECT rectangle;
PAINTSTRUCT paintStruct;
HBRUSH brush;

bool debug=false;
int windowCount=0;

typedef struct{
  unsigned int ID;

  HWND hwnd;
} pWindowPointer;

/* |\____/| [pEvent] Structure
 * |  o o |
 */
typedef struct{
  bool focus;
  unsigned int frameCount;

  unsigned int key[256];
  bool keyCaps;

  bool positionChange, sizeChange, cursorMove;
  // TODO: fullscreen, minimized, maximized

  pSize display;
  pPosition cursor;

  unsigned int windowCount, debug;
} pEvent;

/* |\____/| [pObject] Structure
 * |  o o |
 */
typedef struct{
  int x, y;
  unsigned int width, height;

  pColor color;
} pObject;

/* |\_____/| [pDebug()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pDebug(bool active){ debug=active; }

/* |\_____/| [pWindowReset()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pWindowReset(pWindow *window){
  /* |\____/| Reset [window]
   * |  o o |
   */
  window->active=false;

  window->x=0;
  window->y=0;

  window->width=0;
  window->height=0;
  window->mode=0;
  window->frameLimit=24;

  strncpy(window->title, "{,}", sizeof(window->title)-1);
  window->title[sizeof(window->title)-1]='\0';

  /* |\____/| Reset [build]
   * |  o o |
   */
  build[window->ID-1].x=0;
  build[window->ID-1].y=0;

  build[window->ID-1].width=0;
  build[window->ID-1].height=0;
  build[window->ID-1].mode=0;

  strncpy(build[window->ID-1].title, "{,}", sizeof(build[window->ID-1].title)-1);
  build[window->ID-1].title[sizeof(build[window->ID-1].title)-1]='\0';

  build[window->ID-1].hBitmap=NULL;
  build[window->ID-1].hdc=NULL;
  build[window->ID-1].hMemDC=NULL;
  build[window->ID-1].hInstance=NULL;
  build[window->ID-1].hwnd=NULL;
  build[window->ID-1].style=0;

  strncpy(build[window->ID-1].class, "", sizeof(build[window->ID-1].class)-1);
  build[window->ID-1].class[sizeof(build[window->ID-1].class)-1]='\0';

  build[window->ID-1].DESTROY=false;
  build[window->ID-1].MOUSEMOVE=false;
  build[window->ID-1].FOCUSIN=false;
  build[window->ID-1].FOCUSOUT=false;

  build[window->ID-1].SIZE.width=0;
  build[window->ID-1].SIZE.height=0;
  build[window->ID-1].MOVE.x=0;
  build[window->ID-1].MOVE.y=0;

  build[window->ID-1].KEYON=false;
  for(unsigned int current=0; current<256; current++){
    build[window->ID-1].KEY[current]=0;
  }

  build[window->ID-1].frameStart=0;
  build[window->ID-1].frameCount=0;

  windowCount--;

  return;
}

/* |\_____/| [pWindowProc()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
LRESULT CALLBACK pWindowProc(HWND hwnd, UINT uMessage, WPARAM wParameter, LPARAM lParameter){
  pWindowPointer *windowPointer=(pWindowPointer*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  /* |\____/| [window] Create Signal
   * |  o o |
   */
  if(uMessage==WM_CREATE){
    pWindowPointer *newWindowPointer=(pWindowPointer*)malloc(sizeof(pWindowPointer));
    newWindowPointer->ID=currentWindow.ID;
    newWindowPointer->hwnd=hwnd;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)newWindowPointer);
  }

  /* |\____/| [window] Close Signal
   * |  o o |
   */
  if(uMessage==WM_DESTROY){
    build[currentWinID-1].DESTROY=true;
    hwnd=NULL;

    return 0;
  }

  /* |\____/| [window] Focus Signal
   * |  o o |
   */
  if(uMessage==WM_ACTIVATE){
    if(wParameter==WA_ACTIVE || wParameter==WA_CLICKACTIVE){
      build[windowPointer->ID-1].FOCUSIN=true;
    }
    else{ build[windowPointer->ID-1].FOCUSOUT=false; }
  }

  /* |\____/| [window] Mouse Move Signal
   * |  o o |
   */
  if(uMessage==WM_MOUSEMOVE){ build[windowPointer->ID-1].MOUSEMOVE=true; }

  /* |\____/| [window] Position Change Signal
   * |  o o |
   */
  if(uMessage==WM_MOVE){
    build[windowPointer->ID-1].MOVE.x=LOWORD(lParameter);
    build[windowPointer->ID-1].MOVE.y=HIWORD(lParameter);
  }

  /* |\____/| [window] Size Change Signal
   * |  o o |
   */
  if(uMessage==WM_SIZE){
    build[windowPointer->ID-1].SIZE.width=LOWORD(lParameter);
    build[windowPointer->ID-1].SIZE.height=HIWORD(lParameter);
  }

  /* |\____/| [window] Key Down Signal
   * |  o o |
   */
  if(uMessage==WM_KEYDOWN){
    build[windowPointer->ID-1].KEY[MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)]=1;
    build[windowPointer->ID-1].KEYON=true;
  }

  /* |\____/| [window] Key Up Signal
   * |  o o |
   */
  if(uMessage==WM_KEYUP){
    build[windowPointer->ID-1].KEY[MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)]=2;
    build[windowPointer->ID-1].KEYON=true;
  }

  return DefWindowProc(hwnd, uMessage, wParameter, lParameter);
}

/* |\_____/| [pWindowCreate()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
pWindow pWindowCreate(unsigned int width, unsigned int height, unsigned int mode){
  if(GetConsoleWindow()==NULL && debug==true){
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
  }
  else if(GetConsoleWindow()!=NULL && debug==false){ FreeConsole(); }

  pWindow window;
  windowCount++;

  for(unsigned int current=1; current<=32; current++){
    if(build[current-1].width==0 && build[current-1].height==0){
      window.ID=current;
      break;
    }
    else if(current==32){
      if(debug==true){
        printf("[Error] Too many Windows active,\n");
        fflush(stdout);
      }

      pWindowReset(&window);
      return window;
    }
  }

  /* |\____/| Change [window] Variables Values
   * |  o o | TODO: Minimal [window] Size
   */
  window.active=true;

  window.x=(GetSystemMetrics(SM_CXSCREEN)-window.width)/2;
  window.y=(GetSystemMetrics(SM_CYSCREEN)-window.height)/2;

  window.width=width;
  window.height=height;
  window.mode=mode;
  window.frameLimit=24;

  currentWindow=window;

  /* |\____/| Change [build] Variables Values
   * |  o o |
   */
  build[window.ID-1].x=window.x;
  build[window.ID-1].y=window.y;

  build[window.ID-1].width=width;
  build[window.ID-1].height=height;
  build[window.ID-1].mode=mode;

  /* |\____/| Setup [build] hInstance and wClass
   * |  o o |
   */
  build[window.ID-1].hInstance=GetModuleHandle(NULL);
  sprintf(build[window.ID-1].class, "pClass%i", window.ID);

  build[window.ID-1].wClass.lpfnWndProc=pWindowProc;
  build[window.ID-1].wClass.hInstance=build[window.ID-1].hInstance;
  build[window.ID-1].wClass.lpszClassName=build[window.ID-1].class;
  build[window.ID-1].wClass.hCursor=LoadCursor(NULL, IDC_ARROW);
  if(!RegisterClass(&build[window.ID-1].wClass)){
    if(debug==true){
      printf("[Error] Could not register Window Class,\n");
      fflush(stdout);
    }

    pWindowReset(&window);
    return window;
  }

  /* |\____/| Setup [build] Hints
   * |  o o |
   */
  build[window.ID-1].style=WS_OVERLAPPEDWINDOW;
  if(mode==1){ build[window.ID-1].style=WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX; }
  if(mode==2){ build[window.ID-1].style=WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU; }

  /* |\____/| Setup [build] HWND
   * |  o o |
   */
  build[window.ID-1].hwnd=CreateWindowEx(
    0, build[window.ID-1].class, "{,}", build[window.ID-1].style,
    CW_USEDEFAULT, CW_USEDEFAULT, width, height,
    NULL, NULL, build[window.ID-1].hInstance, NULL
  );
  if(build[window.ID-1].hwnd==NULL){
    if(debug==true){
      printf("[Error] Could not create Window,\n");
      fflush(stdout);
    }

    pWindowReset(&window);
    return window;
  }

  /* |\____/| Setup [build] Buffers
   * |  o o |
   */
  build[window.ID-1].hMemDC=CreateCompatibleDC(build[window.ID-1].hdc);
  build[window.ID-1].hBitmap=CreateCompatibleBitmap(build[window.ID-1].hdc, width, height);
  SelectObject(build[window.ID-1].hMemDC, build[window.ID-1].hBitmap);
  ReleaseDC(NULL, build[window.ID-1].hdc);

  /* |\____/| Initialize and Show [window]
   * |  o o |
   */
  InvalidateRect(build[window.ID-1].hwnd, NULL, TRUE);
  ShowWindow(build[window.ID-1].hwnd, SW_SHOW);
  SetWindowPos(build[window.ID-1].hwnd, NULL, window.x, window.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
  SetForegroundWindow(build[window.ID-1].hwnd);
  build[window.ID-1].frameStart=GetTickCount();
  timeBeginPeriod(1);

  return window;
}

/* |\_____/| [pWindowDraw()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pWindowDraw(pWindow *window, pObject *object){
  if(window->active==true){
    /* |\____/| Change [brush] Color
     * |  o o |
    */
    brush=CreateSolidBrush(RGB(object->color.r, object->color.g, object->color.b));
    SelectObject(build[window->ID-1].hMemDC, brush);

    /* |\____/| Draw on [build] Buffer
     * |  o o |
    */
    rectangle.left=object->x;
    rectangle.top=object->y;
    rectangle.right=object->x+object->width;
    rectangle.bottom=object->y+object->height;

    FillRect(build[window->ID-1].hMemDC, &rectangle, brush);
  }
  else if(debug==true){
    printf("[Error] Could not Draw Object,\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| [pWindowClear()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pWindowClear(pWindow* window){
  if(window->active==true){
    /* |\____/| Change [brush] Color
     * |  o o |
    */
    brush=CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(build[window->ID-1].hMemDC, brush);

    /* |\____/| Clean [build] Buffer
     * |  o o |
    */
    rectangle.left=0;
    rectangle.top=0;
    rectangle.right=window->width;
    rectangle.bottom=window->height;

    FillRect(build[window->ID-1].hMemDC, &rectangle, brush);
  }
  else if(debug==true){
    printf("[Error] Could not Clear Window,\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| [pWindowClose()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pWindowClose(pWindow* window){
  if(window->active){
    /* |\____/| Send [window] Kill Event
     * |  o o |
    */

    PostMessage(build[window->ID-1].hwnd, WM_CLOSE, 0, 0);
  }
  else if(debug==true){
    printf("[Warning] Window is already closed,\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| [pEventReset()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pEventReset(pEvent *event){
  /* |\____/| Reset [event]
   * |  o o |
   */
  event->focus=false;
  event->frameCount=0;

  for(unsigned int current=0; current<256; current++){
    event->key[current]=0;
  }
  event->keyCaps=false;

  event->positionChange=false;
  event->sizeChange=false;
  event->cursorMove=false;

  event->display.width=0;
  event->display.height=0;
  event->cursor.x=0;
  event->cursor.y=0;

  event->windowCount=0;
  event->debug=false;

  return;
}

/* |\_____/| [pEventCreate()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
pEvent pEventCreate(){
  /* |\____/| Create and Set [event] Values
   * |  o o |
   */
  pEvent event;

  event.focus=false;
  event.frameCount=0;

  for(unsigned int current=0; current<256; current++){
    event.key[current]=0;
  }
  event.keyCaps=false;

  event.positionChange=false;
  event.sizeChange=false;
  event.cursorMove=false;

  event.display.width=0;
  event.display.height=0;
  event.cursor.x=0;
  event.cursor.y=0;

  event.windowCount=0;
  event.debug=0;

  return event;
}

/* |\_____/| [pEventHandle()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pEventHandle(pWindow *window, pEvent *event){
  if(window->active){
    currentWinID=window->ID;
    while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    /* |\____/| Change [window] Buffers
     * |  o o |
     */
    InvalidateRect(build[window->ID-1].hwnd, NULL, TRUE);

    build[window->ID-1].hdc=BeginPaint(build[window->ID-1].hwnd, &paintStruct);
    BitBlt(build[window->ID-1].hdc, 0, 0, window->width, window->height,
      build[window->ID-1].hMemDC, 0, 0, SRCCOPY
    );
    EndPaint(build[window->ID-1].hwnd, &paintStruct);

    /* |\____/| Reset [event] Keys
     * |  o o |
     */
    for(unsigned int current=0; current<256; current++){
      if(event->key[current]==1){ event->key[current]=2; }
    }

    /* |\____/| Update [event] Variables
     * |  o o |
     */
    event->windowCount=windowCount;
    event->debug=debug;

    /* |\____/| Check for Close [currentEvent]
     * |  o o |
     */
    if(build[currentWinID-1].DESTROY==true){
      pWindowReset(window);
      return;
    }
    else{ window->active=true; }

    /* |\____/| Manage [window] position change
     * |  o o |
     */
    if(window->x!=build[window->ID-1].x || window->y!=build[window->ID-1].y){
      build[window->ID-1].x=window->x;
      build[window->ID-1].y=window->y;

      SetWindowPos(build[window->ID-1].hwnd, NULL, window->x, window->y,
        0, 0, SWP_NOZORDER | SWP_NOSIZE
      );

      build[currentWinID-1].MOVE.x=0;
      build[currentWinID-1].MOVE.y=0;
      event->positionChange=true;
    }
    else if(build[currentWinID-1].MOVE.x!=0 || build[currentWinID-1].MOVE.y!=0){
      window->x=build[currentWinID-1].MOVE.x;
      window->y=build[currentWinID-1].MOVE.y;
      build[window->ID-1].x=build[currentWinID-1].MOVE.x;
      build[window->ID-1].y=build[currentWinID-1].MOVE.y;

      build[currentWinID-1].MOVE.x=0;
      build[currentWinID-1].MOVE.y=0;
      event->positionChange=true;
    }
    else{ event->positionChange=false; }

    /* |\____/| Manage [window] size change
     * |  o o |
     */
    if(window->width!=build[window->ID-1].width || window->height!=build[window->ID-1].height){
      build[window->ID-1].width=window->width;
      build[window->ID-1].height=window->height;

      SetWindowPos(build[window->ID-1].hwnd, NULL, build[window->ID-1].x, build[window->ID-1].y,
        window->width, window->height, SWP_NOZORDER
      );

      build[window->ID-1].hdc=GetDC(build[window->ID-1].hwnd);
      build[window->ID-1].hBitmap=CreateCompatibleBitmap(build[window->ID-1].hdc,
        window->width, window->height
      );
      SelectObject(build[window->ID-1].hMemDC, build[window->ID-1].hBitmap);
      ReleaseDC(build[window->ID-1].hwnd, build[window->ID-1].hdc);

      build[currentWinID-1].SIZE.width=0;
      build[currentWinID-1].SIZE.height=0;
      event->sizeChange=true;
    }
    else if(build[currentWinID-1].SIZE.width!=0 || build[currentWinID-1].SIZE.height!=0){
      window->width=build[currentWinID-1].SIZE.width;
      window->height=build[currentWinID-1].SIZE.height;
      build[window->ID-1].width=build[currentWinID-1].SIZE.width;
      build[window->ID-1].height=build[currentWinID-1].SIZE.height;

      build[window->ID-1].hdc=GetDC(build[window->ID-1].hwnd);
      build[window->ID-1].hBitmap=CreateCompatibleBitmap(build[window->ID-1].hdc,
        window->width, window->height
      );
      SelectObject(build[window->ID-1].hMemDC, build[window->ID-1].hBitmap);
      ReleaseDC(build[window->ID-1].hwnd, build[window->ID-1].hdc);

      build[currentWinID-1].SIZE.width=0;
      build[currentWinID-1].SIZE.height=0;
      event->sizeChange=true;
    }
    else{ event->sizeChange=false; }

    if(build[currentWinID-1].KEYON==true){
      for(unsigned int current=0; current<256; current++){
        /* |\____/| Check for Key Press [currentEvent]
         * |  o o |
         */
        if(build[currentWinID-1].KEY[current]==1){
          if(event->key[current]==0){ event->key[current]=1; }
          else{ event->key[current]=2; }
        }

        /* |\____/| Check for Key Release [currentEvent]
         * |  o o |
         */
        if(build[currentWinID-1].KEY[current]==2){
          event->key[current]=0;
        }
      }

      build[currentWinID-1].KEYON=false;
    }

    /* |\____/| Check for Cursor Move [currentEvent]
     * |  o o |
     */
    if(build[currentWinID-1].MOUSEMOVE==true){
      GetCursorPos(&cursor);
      event->cursor.x=cursor.x;
      event->cursor.y=cursor.y;

      build[currentWinID-1].MOUSEMOVE=false;
      event->cursorMove=true;
    }
    else{ event->cursorMove=false; }

    /* |\____/| Check for Window Focus In [currentEvent]
     * |  o o |
     */
    if(build[currentWinID-1].FOCUSIN==true){
      event->focus=true;
      activeWinID=window->ID;

      build[currentWinID-1].FOCUSIN=false;
    }

    /* |\____/| Check for Window Focus Out [currentEvent]
     * |  o o |
     */
    if(build[currentWinID-1].FOCUSOUT==true){
      event->focus=false;
      if(activeWinID==window->ID){ activeWinID=0; }

      build[currentWinID-1].FOCUSOUT=false;
    }

    /* |\____/| Update [event] Display Size
     * |  o o |
     */
    event->display.width=GetSystemMetrics(SM_CXSCREEN);
    event->display.height=GetSystemMetrics(SM_CYSCREEN);

    /* |\____/| Update [event] Key Caps Status
     * |  o o |
     */
    event->keyCaps=GetKeyState(VK_CAPITAL)&0x0001;

      /* |\____/| Update [window] Title
       * |  o o |
       */
    if(window->title!=build[window->ID-1].title){
      if(strlen(window->title)>128){
        if(debug==true){
          printf("[Error] Given Title is too Long,\n");
          fflush(stdout);

          strncpy(window->title, build[window->ID-1].title, sizeof(window->title)-1);
          window->title[sizeof(window->title)-1]='\0';
        }
      }
      else{
        strncpy(build[window->ID-1].title, window->title,
          sizeof(build[window->ID-1].title)-1
        );
        build[window->ID-1].title[sizeof(build[window->ID-1].title)-1]='\0';

        SetWindowText(build[window->ID-1].hwnd,window->title);
      }
    }

    /* |\____/| Update [window] Mode
     * |  o o |
     */
    window->mode=build[window->ID-1].mode;

    /* |\____/| Limit and Count [window] Frames
     * |  o o |
     */
    build[window->ID-1].frameCount++;
    Sleep((1000/windowCount)/window->frameLimit);

    if(GetTickCount()-build[window->ID-1].frameStart>=1000){
      event->frameCount=build[window->ID-1].frameCount;

      build[window->ID-1].frameCount=0;
      build[window->ID-1].frameStart=GetTickCount();
    }
  }
  else if(debug==true){
    printf("[Error] Could not handle Last Event,\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| [pObjectCreate()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
pObject pObjectCreate(unsigned int width, unsigned int height){
  /* |\____/| Create and Set [object] Values
   * |  o o |
   */
  pObject object;

  object.x=0;
  object.y=0;

  object.width=width;
  object.height=height;

  object.color.r=0;
  object.color.g=0;
  object.color.b=0;

  return object;
}

/* |\____/| Declare [pKey] Values
 * |  o o |
*/
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

/* |\_____/| [pKeyConvert()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
unsigned int pKeyConvert(const char *key){
  if(strcmp(key, "ESC")==0 || strcmp(key, "esc")==0 || strcmp(key, "Esc")==0){
    return MapVirtualKey(VK_ESCAPE, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "TAB")==0 || strcmp(key, "tab")==0 || strcmp(key, "Tab")==0){
    return MapVirtualKey(VK_TAB, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "CAPS")==0 || strcmp(key, "caps")==0 || strcmp(key, "Caps")==0){
    return MapVirtualKey(VK_CAPITAL, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "LSHIFT")==0 || strcmp(key, "lshift")==0 || strcmp(key, "LShift")==0){
    return MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "LCTRL")==0 || strcmp(key, "lctrl")==0 || strcmp(key, "LCtrl")==0){
    return MapVirtualKey(VK_LCONTROL, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "LMOD")==0 || strcmp(key, "lmod")==0 || strcmp(key, "LMod")==0){
    return MapVirtualKey(VK_LWIN, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "LALT")==0 || strcmp(key, "lalt")==0 || strcmp(key, "LAlt")==0){
    return MapVirtualKey(VK_LMENU, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "SPACE")==0 || strcmp(key, "space")==0 || strcmp(key, "Space")==0){
    return MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC);
  }

  else if(strcmp(key, "F1")==0 || strcmp(key, "f1")==0){ return MapVirtualKey(VK_F1, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F2")==0 || strcmp(key, "f2")==0){ return MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F3")==0 || strcmp(key, "f3")==0){ return MapVirtualKey(VK_F3, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F4")==0 || strcmp(key, "f4")==0){ return MapVirtualKey(VK_F4, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F5")==0 || strcmp(key, "f5")==0){ return MapVirtualKey(VK_F5, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F6")==0 || strcmp(key, "f6")==0){ return MapVirtualKey(VK_F6, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F7")==0 || strcmp(key, "f7")==0){ return MapVirtualKey(VK_F7, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F8")==0 || strcmp(key, "f8")==0){ return MapVirtualKey(VK_F8, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F9")==0 || strcmp(key, "f9")==0){ return MapVirtualKey(VK_F9, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F10")==0 || strcmp(key, "f10")==0){ return MapVirtualKey(VK_F10, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F11")==0 || strcmp(key, "f11")==0){ return MapVirtualKey(VK_F11, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F12")==0 || strcmp(key, "f12")==0){ return MapVirtualKey(VK_F12, MAPVK_VK_TO_VSC); }

  else if(strcmp(key, "RALT")==0 || strcmp(key, "ralt")==0 || strcmp(key, "RAlt")==0){
    return MapVirtualKey(VK_RMENU, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "RWIN")==0 || strcmp(key, "rwin")==0 || strcmp(key, "RWin")==0){
    return MapVirtualKey(VK_RWIN, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "MENU")==0 || strcmp(key, "menu")==0 || strcmp(key, "Menu")==0){
    return MapVirtualKey(VK_APPS, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "RCTRL")==0 || strcmp(key, "rctrl")==0 || strcmp(key, "RCtrl")==0){
    return MapVirtualKey(VK_RCONTROL, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "RSHIFT")==0 || strcmp(key, "rshift")==0 || strcmp(key, "RShift")==0){
    return MapVirtualKey(VK_RSHIFT, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "ENTER")==0 || strcmp(key, "enter")==0 || strcmp(key, "Enter")==0){
    return MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "BACKSPACE")==0 || strcmp(key, "backspace")==0 || strcmp(key, "Backspace")==0){
    return MapVirtualKey(VK_BACK, MAPVK_VK_TO_VSC);
  }

  else if(strcmp(key, "LARROW")==0 || strcmp(key, "larrow")==0 || strcmp(key, "LArrow")==0){
    return MapVirtualKey(VK_LEFT, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "DARROW")==0 || strcmp(key, "darrow")==0 || strcmp(key, "DArrow")==0){
    return MapVirtualKey(VK_DOWN, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "RARROW")==0 || strcmp(key, "rarrow")==0 || strcmp(key, "RArrow")==0){
    return MapVirtualKey(VK_RIGHT, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "UARROW")==0 || strcmp(key, "uarrow")==0 || strcmp(key, "UArrow")==0){
    return MapVirtualKey(VK_UP, MAPVK_VK_TO_VSC);
  }

  else if(strcmp(key, "PRINTSCRN")==0 || strcmp(key, "printscrn")==0 || strcmp(key, "PrintScrn")==0){
    return MapVirtualKey(VK_SNAPSHOT, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "SCROLLLOCK")==0 || strcmp(key, "scrolllock")==0 || strcmp(key, "ScrollLock")==0){
    return MapVirtualKey(VK_SCROLL, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "PAUSEBREAK")==0 || strcmp(key, "pausebreak")==0 || strcmp(key, "PauseBreak")==0){
    return MapVirtualKey(VK_PAUSE, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "INS")==0 || strcmp(key, "ins")==0 || strcmp(key, "Ins")==0){
    return MapVirtualKey(VK_INSERT, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "HOME")==0 || strcmp(key, "home")==0 || strcmp(key, "Home")==0){
    return MapVirtualKey(VK_HOME, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "PAGEU")==0 || strcmp(key, "pageu")==0 || strcmp(key, "PageU")==0){
    return MapVirtualKey(VK_PRIOR, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "DEL")==0 || strcmp(key, "del")==0 || strcmp(key, "Del")==0){
    return MapVirtualKey(VK_DELETE, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "END")==0 || strcmp(key, "end")==0 || strcmp(key, "End")==0){
    return MapVirtualKey(VK_END, MAPVK_VK_TO_VSC);
  }
  else if(strcmp(key, "PAGED")==0 || strcmp(key, "paged")==0 || strcmp(key, "PageD")==0){
    return MapVirtualKey(VK_NEXT, MAPVK_VK_TO_VSC);
  }

  else if(strcmp(key, "Q")==0 || strcmp(key, "q")==0){ return MapVirtualKey(VK_Q, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "W")==0 || strcmp(key, "w")==0){ return MapVirtualKey(VK_W, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "E")==0 || strcmp(key, "e")==0){ return MapVirtualKey(VK_E, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "R")==0 || strcmp(key, "r")==0){ return MapVirtualKey(VK_R, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "T")==0 || strcmp(key, "t")==0){ return MapVirtualKey(VK_T, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "Y")==0 || strcmp(key, "y")==0){ return MapVirtualKey(VK_Y, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "U")==0 || strcmp(key, "u")==0){ return MapVirtualKey(VK_U, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "I")==0 || strcmp(key, "i")==0){ return MapVirtualKey(VK_I, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "O")==0 || strcmp(key, "o")==0){ return MapVirtualKey(VK_O, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "P")==0 || strcmp(key, "p")==0){ return MapVirtualKey(VK_P, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "A")==0 || strcmp(key, "a")==0){ return MapVirtualKey(VK_A, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "S")==0 || strcmp(key, "s")==0){ return MapVirtualKey(VK_S, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "D")==0 || strcmp(key, "d")==0){ return MapVirtualKey(VK_D, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "F")==0 || strcmp(key, "f")==0){ return MapVirtualKey(VK_F, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "G")==0 || strcmp(key, "g")==0){ return MapVirtualKey(VK_G, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "H")==0 || strcmp(key, "h")==0){ return MapVirtualKey(VK_H, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "J")==0 || strcmp(key, "j")==0){ return MapVirtualKey(VK_J, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "K")==0 || strcmp(key, "k")==0){ return MapVirtualKey(VK_K, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "L")==0 || strcmp(key, "l")==0){ return MapVirtualKey(VK_L, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "Z")==0 || strcmp(key, "z")==0){ return MapVirtualKey(VK_Z, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "X")==0 || strcmp(key, "x")==0){ return MapVirtualKey(VK_X, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "C")==0 || strcmp(key, "c")==0){ return MapVirtualKey(VK_C, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "V")==0 || strcmp(key, "v")==0){ return MapVirtualKey(VK_V, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "B")==0 || strcmp(key, "b")==0){ return MapVirtualKey(VK_B, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "N")==0 || strcmp(key, "n")==0){ return MapVirtualKey(VK_N, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "M")==0 || strcmp(key, "m")==0){ return MapVirtualKey(VK_M, MAPVK_VK_TO_VSC); }

  else if(strcmp(key, "1")==0){ return MapVirtualKey(VK_1, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "2")==0){ return MapVirtualKey(VK_2, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "3")==0){ return MapVirtualKey(VK_3, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "4")==0){ return MapVirtualKey(VK_4, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "5")==0){ return MapVirtualKey(VK_5, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "6")==0){ return MapVirtualKey(VK_6, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "7")==0){ return MapVirtualKey(VK_7, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "8")==0){ return MapVirtualKey(VK_8, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "9")==0){ return MapVirtualKey(VK_9, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "0")==0){ return MapVirtualKey(VK_0, MAPVK_VK_TO_VSC); }

  else if(strcmp(key, "`")==0){ return MapVirtualKey(VK_OEM_3, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, ",")==0){ return MapVirtualKey(VK_OEM_COMMA, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, ".")==0){ return MapVirtualKey(VK_OEM_PERIOD, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "/")==0){ return MapVirtualKey(VK_OEM_2, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, ";")==0){ return MapVirtualKey(VK_OEM_SEMICOLON, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "'")==0){ return MapVirtualKey(VK_OEM_QUOTE, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "\\")==0){ return MapVirtualKey(VK_OEM_5, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "[")==0){ return MapVirtualKey(VK_OEM_4, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "]")==0){ return MapVirtualKey(VK_OEM_6, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "-")==0){ return MapVirtualKey(VK_OEM_MINUS, MAPVK_VK_TO_VSC); }
  else if(strcmp(key, "=")==0){ return MapVirtualKey(VK_OEM_PLUS, MAPVK_VK_TO_VSC); }

  return 0;
}
