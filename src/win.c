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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <windows.h>

/* |\____/| Define Default Values
 * |  o o |
 */
#define WINDOW_MAX 16
#define WINDOW_X_DEF 64
#define WINDOW_Y_DEF 64
#define WINDOW_POS_MAX 32768
#define WINDOW_POS_CHANGE 65536
#define WINDOW_WIDTH_MIN 256
#define WINDOW_HEIGHT_MIN 256
#define WINDOW_WIDTH_MAX 7680
#define WINDOW_HEIGHT_MAX 4320

#define TITLE_DEF "{,}"
#define TITLE_MAX 255

#define KEY_MAX 255
#define FRAME_DEF 24
#define FRAME_MAX 1024

#define FONT_MAX 32
#define FONT_SIZE_MAX 512
#define FONT_DIR_MAX 255
#define TEXT_MAX 8192

/* |\____/| Define [pKey] Values
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

#define VK_LBUTTON 0x02
#define VK_RBUTTON 0x03
#define VK_MBUTTON 0x04
#define VK_XBUTTON1 0x05
#define VK_XBUTTON2 0xFD

#define VK_OEM_SEMICOLON 0xBA
#define VK_OEM_QUOTE 0xDE

/* |\____/| [pSize], [pPosition], [pColor] Structure
 * |  o o |
 */
typedef struct{ unsigned short int width, height; } pSize;
typedef struct{ int x, y; } pPosition;
typedef struct{ unsigned short int r, g, b; } pColor;

/* |\____/| [pPrzecinek] Structure and Variable
 * |  o o |
 */
typedef struct{
  bool debug;
  unsigned short int windowCount, frameLimit;

  pSize display;
  pPosition cursor;
} pPrzecinek;

pPrzecinek przecinek={ false, 0, FRAME_DEF };

/* |\____/| [pWindow] Structure
 * |  o o |
 */
typedef struct{
  unsigned short int ID;
  bool active;

  int x, y;
  unsigned short int width, height;
  unsigned short int widthMin, heightMin, widthMax, heightMax;

  bool resize;
  char title[TITLE_MAX];
  unsigned short int border;
  bool fullScreen;
} pWindow;

/* |\____/| [pBuildWin], [pWindowPointer] Structure and Variables
 * |  o o |
 */
typedef struct{
  int x, y;
  int xBac, yBac;

  unsigned short int width, height;
  unsigned short int widthMin, heightMin, widthMax, heightMax;
  unsigned short int widthBac, heightBac, widthFix, heightFix;

  bool resize;
  char title[TITLE_MAX];
  unsigned short int border;
  bool fullScreen;

  HBITMAP hBitmap;
  HDC hdc;
  HDC hMemDC;
  HINSTANCE hInstance;
  WNDCLASS wClass;
  HWND hwnd;
  DWORD style;

  char class[8];

  bool DESTROY, FOCUSIN, FOCUSOUT;
 
  pSize SIZE;
  pPosition MOVE;

  bool KEYON;
  unsigned short int KEY[KEY_MAX];

  DWORD frameStart;
  unsigned short int frameCount;
} pBuildWin;

unsigned short int activeWinID=0, currentWinID=0, createWinID=0;
unsigned short int winCount=0;

pBuildWin build[WINDOW_MAX];
MSG message;
POINT cursor;
RECT rectangle, fix;
PAINTSTRUCT paintStruct;
HBRUSH brush;

typedef struct{
  unsigned short int ID;

  HWND hwnd;
} pWindowPointer;

pWindowPointer *windowPointer, *newWindowPointer;
RECT *dpi;
WINDOWPOS *limit;

/* |\____/| [pEvent] Structure and Variables
 * |  o o |
 */
typedef struct{
  bool focus;
  unsigned short int frameCount;

  unsigned short int key[KEY_MAX];
  bool keyCaps;
} pEvent;

pEvent change;

/* |\____/| [pObject] Structure
 * |  o o |
 */
typedef struct{
  int x, y;
  unsigned short int width, height;

  pColor color;
} pObject;

/* |\____/| [pFont] Structure
 * |  o o |
 */
typedef struct{
  unsigned int ID;

  unsigned short int size;
  char name[FONT_DIR_MAX];

  pColor color;
} pFont;

/* |\____/| [pFontWin] Structure and Variables
 * |  o o |
 */
//pFontWin

/* |\____/| [pText] Structure
 * |  o o |
 */
typedef struct{
  int x, y;

  char value[TEXT_MAX];
} pText;

/* |\____/| Base Function List
 * |  o o |
 */
void pWindowReset(pWindow *window);
pWindow pWindowCreate(unsigned short int width, unsigned short int height, bool resize);
void pWindowDrawObject(pWindow *window, pObject *object);
void pWindowDrawText(pWindow *window, pFont *font, pText *text);
void pWindowClear(pWindow *window);
void pWindowClose(pWindow *window);
pEvent pEventCreate();
void pEventHandle(pWindow *window, pEvent *event);
pObject pObjectCreate(unsigned short int width, unsigned short int height);
bool pObjectCollision(pObject object1, pObject object2);
void pFontReset(pFont *font);
pFont pFontCreate(const char *name, unsigned short int size);
void pFontClose(pFont *font);
pText pTextCreate(const char *value);

/* |\_____/| pSetup() Function
 * |       | Used for initialization of the library
 * | o   o | [debug] (true/false), [frameLimit] (1:MAX)
 * \ = , = / Returns nothing
 */
void pSetup(bool debug, unsigned short int frameLimit){
  // Update [przecinek] [debug] value
  przecinek.debug=debug;

  // Update [przecinek] [frameLimit] value
  if(frameLimit==0){
    if(przecinek.debug==true){
      printf("[pWarning, E001] \"Frame limit value is too low\" (changing from: 0 to: 1),\n");
      fflush(stdout);
    }

    frameLimit=1;
  }
  else if(frameLimit>FRAME_MAX){
    if(przecinek.debug==true){
printf(
  "[pWarning, E002] \"Frame limit value is too big\" (changing from: %i to: %i),\n",
  frameLimit, FRAME_MAX
);
      fflush(stdout);
    }

    frameLimit=FRAME_MAX;
  }
  przecinek.frameLimit=frameLimit;

  // Update [przecinek] [display] values
  przecinek.display.width=GetSystemMetrics(SM_CXSCREEN);
  przecinek.display.height=GetSystemMetrics(SM_CYSCREEN);

  // Update [przecinek] [cursor] values
  if(GetCursorPos(&cursor)){
    przecinek.cursor.x=cursor.x;
    przecinek.cursor.y=cursor.y;
  }
  else{
    przecinek.cursor.x=0;
    przecinek.cursor.y=0;
  }
}

/* |\_____/| pClear() Function
 * |       | Used for clearing console
 * | o   o | None
 * \ = , = / Returns nothing
 */
void pClear(){
  // Clear console
  system("cls");

  return;
}

/* |\_____/| pWindowReset() Function
 * |       | Used for resetting window
 * | o   o | [window]
 * \ = , = / Returns nothing
 */
void pWindowReset(pWindow *window){
  // Reset [window] values
  window->active=false;

  window->x=0;
  window->y=0;

  window->width=0;
  window->height=0;

  window->widthMin=0;
  window->heightMin=0;
  window->widthMax=0;
  window->heightMax=0;

  window->resize=false;
  strncpy(window->title, "", sizeof(window->title)-1);
  window->title[sizeof(window->title)-1]='\0';
  window->border=0;
  window->fullScreen=false;

  // Reset [build] values
  build[window->ID-1].x=0;
  build[window->ID-1].y=0;

  build[window->ID-1].xBac=0;
  build[window->ID-1].yBac=0;

  build[window->ID-1].width=0;
  build[window->ID-1].height=0;

  build[window->ID-1].widthMin=0;
  build[window->ID-1].heightMin=0;
  build[window->ID-1].widthMax=0;
  build[window->ID-1].heightMax=0;

  build[window->ID-1].widthBac=0;
  build[window->ID-1].heightBac=0;
  build[window->ID-1].widthFix=0;
  build[window->ID-1].heightFix=0;

  build[window->ID-1].resize=false;
  strncpy(build[window->ID-1].title, "", sizeof(build[window->ID-1].title)-1);
  build[window->ID-1].title[sizeof(build[window->ID-1].title)-1]='\0';
  build[window->ID-1].border=0;
  build[window->ID-1].fullScreen=false;

  build[window->ID-1].hBitmap=NULL;
  build[window->ID-1].hdc=NULL;
  build[window->ID-1].hMemDC=NULL;
  build[window->ID-1].hInstance=NULL;
  build[window->ID-1].hwnd=NULL;
  build[window->ID-1].style=0;

  strncpy(build[window->ID-1].class, "", sizeof(build[window->ID-1].class)-1);
  build[window->ID-1].class[sizeof(build[window->ID-1].class)-1]='\0';

  build[window->ID-1].DESTROY=false;
  build[window->ID-1].FOCUSIN=false;
  build[window->ID-1].FOCUSOUT=false;

  build[window->ID-1].SIZE.width=0;
  build[window->ID-1].SIZE.height=0;
  build[window->ID-1].MOVE.x=0;
  build[window->ID-1].MOVE.y=0;

  build[window->ID-1].KEYON=false;
  for(unsigned short int current=0; current<KEY_MAX; current++){
    build[window->ID-1].KEY[current]=0;
  }

  //build[window->ID-1].frameStart=0;
  build[window->ID-1].frameCount=0;

  // Change [winCount]
  winCount--;

  return;
}

/* |\_____/| pWindowProc() Function
 * |       | Used for managing WIN windows
 * | o   o | [hwnd], [uMessage], [wParameter], [lParameter]
 * \ = , = / Returns `0`
 */
LRESULT CALLBACK pWindowProc(HWND hwnd, UINT uMessage, WPARAM wParameter, LPARAM lParameter){
  // Load current [windowPointer]
  windowPointer=(pWindowPointer*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  // Manage [window] create signal
  if(uMessage==WM_CREATE){
    // Create [newWindowPointer]
    newWindowPointer=(pWindowPointer*)malloc(sizeof(pWindowPointer));
    newWindowPointer->ID=createWinID;
    newWindowPointer->hwnd=hwnd;

    // Send [newWindowPointer] to memory
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)newWindowPointer);
  }

  // Manage [window] close signal
  if(uMessage==WM_DESTROY){
    // Send kill signal and destroy [hwnd]
    build[currentWinID-1].DESTROY=true;
    hwnd=NULL;

    return 0;
  }

  // Manage [window] size limit change signal
  if(uMessage==WM_WINDOWPOSCHANGING){
    limit=(WINDOWPOS*)lParameter;

    if(build[windowPointer->ID-1].resize==true){
      // Update [window] limits
      if(limit->cx<build[windowPointer->ID-1].widthMin+build[windowPointer->ID-1].widthFix){
        limit->cx=build[windowPointer->ID-1].widthMin+build[windowPointer->ID-1].widthFix;
      }
      if(limit->cy<build[windowPointer->ID-1].heightMin+build[windowPointer->ID-1].heightFix){
        limit->cy=build[windowPointer->ID-1].heightMin+build[windowPointer->ID-1].heightFix;
      }

      if(limit->cx>build[windowPointer->ID-1].widthMax){
        limit->cx=build[windowPointer->ID-1].widthMax;
      }
      if(limit->cy>build[windowPointer->ID-1].heightMax){
        limit->cy=build[windowPointer->ID-1].heightMax;
      }
    }
  }

  // Manage [window] focus change signal
  if(uMessage==WM_ACTIVATE){
    if(wParameter==WA_ACTIVE || wParameter==WA_CLICKACTIVE){
      build[windowPointer->ID-1].FOCUSIN=true;
    }
    else{ build[windowPointer->ID-1].FOCUSOUT=false; }
  }

  // Manage [window] position change signal
  if(uMessage==WM_MOVE){
    build[windowPointer->ID-1].MOVE.x=LOWORD(lParameter);
    build[windowPointer->ID-1].MOVE.y=HIWORD(lParameter);

    // Correct [MOVE] values
    if(build[windowPointer->ID-1].MOVE.x>32768){ build[windowPointer->ID-1].MOVE.x-=65536; }
    if(build[windowPointer->ID-1].MOVE.y>32768){ build[windowPointer->ID-1].MOVE.y-=65536; }
  }

  // Manage [window] size change signal
  if(uMessage==WM_SIZE){
    // Configure shape
    rectangle.left=0;
    rectangle.top=0;
    rectangle.right=LOWORD(lParameter);
    rectangle.bottom=HIWORD(lParameter);

    // Set [brush] color
    brush=CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(build[windowPointer->ID-1].hMemDC, brush);

    // Draw on [build] buffer
    FillRect(build[windowPointer->ID-1].hMemDC, &rectangle, brush);

    InvalidateRect(hwnd, NULL, TRUE);
    build[windowPointer->ID-1].hdc=BeginPaint(hwnd, &paintStruct);

    // Switch [window] buffer
    BitBlt(
      build[windowPointer->ID-1].hdc, 0, 0, LOWORD(lParameter), HIWORD(lParameter),
      build[windowPointer->ID-1].hMemDC, 0, 0, SRCCOPY
    );

    // Set [brush] color
    brush=CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(build[windowPointer->ID-1].hdc, brush);

    // Draw on [window]
    FillRect(build[windowPointer->ID-1].hdc, &rectangle, brush);
    EndPaint(hwnd, &paintStruct);

    // Clear [brush]
    DeleteObject(brush);

    // Refresh [window]
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);

    build[windowPointer->ID-1].SIZE.width=LOWORD(lParameter);
    build[windowPointer->ID-1].SIZE.height=HIWORD(lParameter);
  }

  // Manage [window] dpi change signal
  if(uMessage==WM_DPICHANGED){
    dpi=(RECT*)lParameter;

    build[windowPointer->ID-1].SIZE.width=dpi->right-dpi->left;
    build[windowPointer->ID-1].SIZE.height=dpi->bottom-dpi->top;
  }

  // Manage [window] key down signal
  if(uMessage==WM_KEYDOWN){
    build[windowPointer->ID-1].KEY[MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)]=1;
    build[windowPointer->ID-1].KEYON=true;
  }

  // Manage [window] key up signal
  if(uMessage==WM_KEYUP){
    build[windowPointer->ID-1].KEY[MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)]=2;
    build[windowPointer->ID-1].KEYON=true;
  }

  // Manage [window] `lmouse` key down
  if(uMessage==WM_LBUTTONDOWN){
    build[windowPointer->ID-1].KEY[VK_LBUTTON]=1;
    build[windowPointer->ID-1].KEYON=true;
  }
  // Manage [window] `lmouse` key up
  if(uMessage==WM_LBUTTONUP){
    build[windowPointer->ID-1].KEY[VK_LBUTTON]=2;
    build[windowPointer->ID-1].KEYON=true;
  }

  // Manage [window] `rmouse` key down
  if(uMessage==WM_RBUTTONDOWN){
    build[windowPointer->ID-1].KEY[VK_RBUTTON]=1;
    build[windowPointer->ID-1].KEYON=true;
  }
  // Manage [window] `rmouse` key up
  if(uMessage==WM_RBUTTONUP){
    build[windowPointer->ID-1].KEY[VK_RBUTTON]=2;
    build[windowPointer->ID-1].KEYON=true;
  }

  // Manage [window] `mmouse` key down
  if(uMessage==WM_MBUTTONDOWN){
    build[windowPointer->ID-1].KEY[VK_MBUTTON]=1;
    build[windowPointer->ID-1].KEYON=true;
  }
  // Manage [window] `mmouse` key up
  if(uMessage==WM_MBUTTONUP){
    build[windowPointer->ID-1].KEY[VK_MBUTTON]=2;
    build[windowPointer->ID-1].KEYON=true;
  }

  // Manage [window] `back` and `forward` key down
  if(uMessage==WM_XBUTTONDOWN){
    if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON1){ build[windowPointer->ID-1].KEY[VK_XBUTTON1]=1; }
    if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON2){ build[windowPointer->ID-1].KEY[VK_XBUTTON2]=1; }
    build[windowPointer->ID-1].KEYON=true;
  }
  // Manage [window] `back` and `forward` key up
  if(uMessage==WM_XBUTTONUP){
    if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON1){ build[windowPointer->ID-1].KEY[VK_XBUTTON1]=2; }
    if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON2){ build[windowPointer->ID-1].KEY[VK_XBUTTON2]=2; }
    build[windowPointer->ID-1].KEYON=true;
  }

  // Return current [hwnd] values
  return DefWindowProc(hwnd, uMessage, wParameter, lParameter);
}

/* |\_____/| pWindowCreate() Function
 * |       | Used for creating windows
 * | o   o | [width], [height] (MIN:MAX), [resize] (true/false)
 * \ = , = / Returns [window]
 */
pWindow pWindowCreate(unsigned short int width, unsigned short int height, bool resize){
  // Manage console
  if(GetConsoleWindow()==NULL && przecinek.debug==true){
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
  }
  else if(GetConsoleWindow()!=NULL && przecinek.debug==false){ FreeConsole(); }

  // Create local [window]
  pWindow window;

  // Change [winCount]
  winCount++;

  for(unsigned short int current=0; current<WINDOW_MAX; current++){
    if(build[current].width==0 && build[current].height==0){
      // Set [window] [ID] and reset [window]
      window.ID=current+1;
      pWindowReset(&window);

      // Change [winCount]
      winCount++;
      break;
    }
    else if(current==WINDOW_MAX-1){
      if(przecinek.debug==true){
printf(
  "[pError, W001] \"Too many windows were created\" (limit: %i),\n",
  WINDOW_MAX
);
        fflush(stdout);
      }

      // Reset and return [window]
      pWindowReset(&window);
      return window;
    }
  }

  // Check [width] value
  if(width<WINDOW_WIDTH_MIN){
    if(przecinek.debug==true){
printf(
  "[pWarning, W001] \"Window width value is too low\" (changing from: %i to %i),\n",
  width, WINDOW_WIDTH_MIN
);
      fflush(stdout);
    }

    // Change [width] value
    width=WINDOW_WIDTH_MIN;
  }
  if(width>WINDOW_WIDTH_MAX){
    if(przecinek.debug==true){
printf(
  "[pWarning, W002] \"Window width value is too big\" (changing from: %i to %i),\n",
  width, WINDOW_WIDTH_MAX
);
      fflush(stdout);
    }

    // Change [width] value
    width=WINDOW_WIDTH_MAX;
  }

  // Check [height] value
  if(height<WINDOW_HEIGHT_MIN){
    if(przecinek.debug==true){
printf(
  "[pWarning, W003] \"Window height value is too low\" (changing from: %i to %i),\n",
  height, WINDOW_HEIGHT_MIN
);
      fflush(stdout);
    }

    // Change [height] value
    height=WINDOW_HEIGHT_MIN;
  }
  if(height>WINDOW_HEIGHT_MAX){
    if(przecinek.debug==true){
printf(
  "[pWarning, W004] \"Window height value is too big\" (changing from: %i to %i),\n",
  height, WINDOW_HEIGHT_MAX
);
      fflush(stdout);
    }

    // Change [height] value
    height=WINDOW_HEIGHT_MAX;
  }

  // Update [window] values
  window.active=true;

  window.x=WINDOW_X_DEF;
  window.y=WINDOW_Y_DEF;

  window.width=width;
  window.height=height;

  if(resize==true){
    window.widthMin=WINDOW_WIDTH_MIN;
    window.heightMin=WINDOW_HEIGHT_MIN;
    window.widthMax=WINDOW_WIDTH_MAX;
    window.heightMax=WINDOW_HEIGHT_MAX;
  }
  else{
    window.widthMin=width;
    window.heightMin=height;
    window.widthMax=width;
    window.heightMax=height;
  }

  window.resize=resize;
  createWinID=window.ID;

  // Update [build] values
  build[window.ID-1].x=window.x;
  build[window.ID-1].y=window.y;

  build[window.ID-1].width=width;
  build[window.ID-1].height=height;

  build[window.ID-1].widthMin=window.widthMin;
  build[window.ID-1].heightMin=window.heightMin;
  build[window.ID-1].widthMax=window.widthMax;
  build[window.ID-1].heightMax=window.heightMax;

  build[window.ID-1].resize=resize;

  // Setup [build] [hInstance] and [class]
  build[window.ID-1].hInstance=GetModuleHandle(NULL);
  sprintf(build[window.ID-1].class, "pClass%i", window.ID);

  // Create [build] [wClass]
  build[window.ID-1].wClass.lpfnWndProc=pWindowProc;
  build[window.ID-1].wClass.hInstance=build[window.ID-1].hInstance;

  // Check if [class] already exists
  if(GetClassInfo(build[window.ID-1].hInstance, build[window.ID-1].class, &build[window.ID-1].wClass)){
    sprintf(build[window.ID-1].class, "pClass%i", window.ID+WINDOW_MAX);
  }

  // Setup [build] [wClass]
  build[window.ID-1].wClass.lpszClassName=build[window.ID-1].class;
  build[window.ID-1].wClass.hCursor=LoadCursor(NULL, IDC_ARROW);
  if(!RegisterClass(&build[window.ID-1].wClass)){
    if(przecinek.debug==true){
printf(
  "[pError, W101] \"Could not register WIN class\" (tried: %s),\n",
  build[window.ID-1].class
);
      fflush(stdout);
    }

    // Reset and return [window]
    pWindowReset(&window);
    return window;
  }

  // Set [build] [style]
  if(resize==true){ build[window.ID-1].style=WS_OVERLAPPEDWINDOW; }
  else{ build[window.ID-1].style=WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX; }

  // Check for [window] dpi
  SetProcessDPIAware();
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  // Calculate [window] actual size
  rectangle.left=0;
  rectangle.top=0;
  rectangle.right=width;
  rectangle.bottom=height;
  AdjustWindowRectEx(&rectangle, build[window.ID-1].style, FALSE, 0);

  // Create [hwnd] for [build]
  build[window.ID-1].hwnd=CreateWindowEx(
    0, build[window.ID-1].class, TITLE_DEF, build[window.ID-1].style,
    CW_USEDEFAULT, CW_USEDEFAULT, rectangle.right-rectangle.left, rectangle.bottom-rectangle.top,
    NULL, NULL, build[window.ID-1].hInstance, NULL
  );
  if(build[window.ID-1].hwnd==NULL){
    if(przecinek.debug==true){
      printf("[pError, W102] \"Could not create WIN hwnd\",\n");
      fflush(stdout);
    }

    // Reset and return [window]
    pWindowReset(&window);
    return window;
  }

  // Set [title] value
  strncpy(window.title, TITLE_DEF, sizeof(window.title)-1);
  window.title[sizeof(window.title)-1]='\0';
  strncpy(build[window.ID-1].title, TITLE_DEF, sizeof(build[window.ID-1].title)-1);
  build[window.ID-1].title[sizeof(build[window.ID-1].title)-1]='\0';

  // Calculate [window] offset
  GetWindowRect(build[window.ID-1].hwnd, &rectangle);
  GetClientRect(build[window.ID-1].hwnd, &fix);

  // Set [window] and [build] [border] value
  window.border=((rectangle.right-rectangle.left)-(fix.right-fix.left))/2;
  build[window.ID-1].border=window.border;

  // Set [build] [widthFix] and [heightFix] values
  build[window.ID-1].widthFix=(rectangle.right-rectangle.left)-(fix.right-fix.left);
  build[window.ID-1].heightFix=(rectangle.bottom-rectangle.top)-(fix.bottom-fix.top);

  // Create [build] buffer
  build[window.ID-1].hMemDC=CreateCompatibleDC(build[window.ID-1].hdc);
  build[window.ID-1].hBitmap=CreateCompatibleBitmap(
    build[window.ID-1].hdc, rectangle.right-rectangle.left, rectangle.bottom-rectangle.top
  );
  SelectObject(build[window.ID-1].hMemDC, build[window.ID-1].hBitmap);
  ReleaseDC(NULL, build[window.ID-1].hdc);

  // Refresh and show [window]
  InvalidateRect(build[window.ID-1].hwnd, NULL, TRUE);
  ShowWindow(build[window.ID-1].hwnd, SW_SHOW);

  // Move and resize [window]
  SetWindowPos(
    build[window.ID-1].hwnd, NULL, window.x, window.y,
    0, 0, SWP_NOZORDER | SWP_NOSIZE
  );
  SetForegroundWindow(build[window.ID-1].hwnd);

  // Set timer start value
  timeBeginPeriod(1);
  build[window.ID-1].frameStart=GetTickCount();

  // Return local [window]
  return window;
}

/* |\_____/| pWindowDrawObject() Function
 * |       | Used for drawing objects on window
 * | o   o | [window], [object]
 * \ = , = / Returns nothing
 */
void pWindowDrawObject(pWindow *window, pObject *object){
  if(window->active==true){
    // Set [brush] color
    brush=CreateSolidBrush(RGB(object->color.r, object->color.g, object->color.b));
    SelectObject(build[window->ID-1].hMemDC, brush);

    // Configure shape
    rectangle.left=object->x;
    rectangle.top=object->y;
    rectangle.right=object->x+object->width;
    rectangle.bottom=object->y+object->height;

    // Draw on [build] buffer
    FillRect(build[window->ID-1].hMemDC, &rectangle, brush);

    // Clear [brush]
    DeleteObject(brush);
  }
  else if(przecinek.debug==true){
    printf("[pError, W301] \"Could not draw object\" (window is closed),\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| WindowClear() Function
 * |       | Used for clearing window
 * | o   o | [window]
 * \ = , = / Returns nothing
 */
void pWindowClear(pWindow* window){
  if(window->active==true){
    // Set [brush] color
    brush=CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(build[window->ID-1].hMemDC, brush);

    // Configure shape
    rectangle.left=0;
    rectangle.top=0;
    rectangle.right=window->width;
    rectangle.bottom=window->height;

    // Draw on [build] buffer
    FillRect(build[window->ID-1].hMemDC, &rectangle, brush);

    // Clear [brush]
    DeleteObject(brush);
  }
  else if(przecinek.debug==true){
    printf("[pError, W302] \"Could not clear screen\" (window is closed),\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| WindowClose() Function
 * |       | Used for closing window
 * | o   o | [window]
 * \ = , = / Returns nothing
 */
void pWindowClose(pWindow* window){
  if(window->active==true){
    // Send kill event
    PostMessage(build[window->ID-1].hwnd, WM_CLOSE, 0, 0);
  }
  else if(przecinek.debug==true){
    printf("[pWarning, W101] \"Window is already closed\",\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| pEventCreate() Function
 * |       | Used for creating events
 * | o   o | None
 * \ = , = / Returns [event]
 */
pEvent pEventCreate(){
  // Create local [event]
  pEvent event;

  // Set [event] values
  event.focus=false;
  event.frameCount=0;

  for(unsigned short int current=0; current<KEY_MAX; current++){
    event.key[current]=0;
  }
  event.keyCaps=false;

  // Return local [event]
  return event;
}

/* |\_____/| pEventHandle() Function
 * |       | Used for handling window events
 * | o   o | [window] [event]
 * \ = , = / Returns nothing
 */
void pEventHandle(pWindow *window, pEvent *event){
  if(window->active==true){
    // Manage console
    if(GetConsoleWindow()!=NULL && przecinek.debug==false){ FreeConsole(); }

    // Manage pending [message]
    currentWinID=window->ID;
    while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    // Switch [window] buffer
    InvalidateRect(build[window->ID-1].hwnd, NULL, TRUE);
    build[window->ID-1].hdc=BeginPaint(build[window->ID-1].hwnd, &paintStruct);
    BitBlt(
      build[window->ID-1].hdc, 0, 0, window->width, window->height,
      build[window->ID-1].hMemDC, 0, 0, SRCCOPY
    );
    EndPaint(build[window->ID-1].hwnd, &paintStruct);

    // Change [event] [key] values from 1 to 2
    for(unsigned short int current=0; current<KEY_MAX; current++){
      if(event->key[current]==1){ event->key[current]=2; }
    }

    // Update [przecinek] [display] values
    przecinek.display.width=GetSystemMetrics(SM_CXSCREEN);
    przecinek.display.height=GetSystemMetrics(SM_CYSCREEN);

    // Update [przecinek] [cursor] values
    if(GetCursorPos(&cursor)){
      przecinek.cursor.x=cursor.x;
      przecinek.cursor.y=cursor.y;
    }
    else{
      przecinek.cursor.x=0;
      przecinek.cursor.y=0;
    }

    // Update [przecinek] [windowCount]
    przecinek.windowCount=winCount;

    // Calculate [window] offset
    GetWindowRect(build[window->ID-1].hwnd, &rectangle);
    GetClientRect(build[window->ID-1].hwnd, &fix);

    // Update [window] and [build] [border] value
    window->border=((rectangle.right-rectangle.left)-(fix.right-fix.left))/2;
    build[window->ID-1].border=window->border;

    // Update [build] [widthFix] and [heightFix] values
    if(build[window->ID-1].fullScreen==false){
      build[window->ID-1].widthFix=(rectangle.right-rectangle.left)-(fix.right-fix.left);
      build[window->ID-1].heightFix=(rectangle.bottom-rectangle.top)-(fix.bottom-fix.top);
    }

    // Update [window] [resize] and [border]
    window->resize=build[window->ID-1].resize;
    window->border=build[window->ID-1].border;

    // Update [event] [keyCaps]
    event->keyCaps=GetKeyState(VK_CAPITAL)&0x0001;

    // Manage close [message]
    if(build[window->ID-1].DESTROY==true){
      // Reset [window]
      pWindowReset(window);

      return;
    }
    else{ window->active=true; }

    // Manage fullscreen change [message]
    if(window->fullScreen==true && build[window->ID-1].fullScreen==false){
      // Backup [build] parameters
      build[window->ID-1].xBac=build[window->ID-1].x;
      build[window->ID-1].yBac=build[window->ID-1].y;
      build[window->ID-1].widthBac=build[window->ID-1].width;
      build[window->ID-1].heightBac=build[window->ID-1].height;

      // Update [window] and [build] size parameters
      window->width=przecinek.display.width;
      window->height=przecinek.display.height;
      build[window->ID-1].width=window->width;
      build[window->ID-1].height=window->height;

      // Update [window] and [build] position parameters
      window->x=0;
      window->y=0;
      build[window->ID-1].x=window->x;
      build[window->ID-1].y=window->y;

      // Update [window] mode
      SetWindowLong(build[window->ID-1].hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

      // Resize and move [window]
      SetWindowPos(
        build[window->ID-1].hwnd, HWND_TOP, window->x, window->y,
        window->width, window->height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED
      );

      // Update buffer
      build[window->ID-1].hdc=GetDC(build[window->ID-1].hwnd);
      build[window->ID-1].hBitmap=CreateCompatibleBitmap(
        build[window->ID-1].hdc, window->width, window->height
      );
      SelectObject(build[window->ID-1].hMemDC, build[window->ID-1].hBitmap);
      ReleaseDC(build[window->ID-1].hwnd, build[window->ID-1].hdc);

      build[window->ID-1].fullScreen=true;
    }
    else if(window->fullScreen==false && build[window->ID-1].fullScreen==true){
      // Change [window] and [build] size parameters to the backuped ones
      window->width=build[window->ID-1].widthBac+build[window->ID-1].widthFix;
      window->height=build[window->ID-1].heightBac+build[window->ID-1].heightFix;
      build[window->ID-1].width=window->width;
      build[window->ID-1].height=window->height;

      // Change [window] and [build] position parameters to the backuped ones
      window->x=build[window->ID-1].xBac-(build[window->ID-1].widthFix/2);
      window->y=build[window->ID-1].yBac-build[window->ID-1].heightFix+(build[window->ID-1].widthFix/2);
      build[window->ID-1].x=window->x;
      build[window->ID-1].y=window->y;
      if(window->x<0){ build[window->ID-1].x+=WINDOW_POS_CHANGE; }
      if(window->y<0){ build[window->ID-1].y+=WINDOW_POS_CHANGE; }

      // Set [build] [style]
      if(build[window->ID-1].resize==true){ build[window->ID-1].style=WS_OVERLAPPEDWINDOW | WS_VISIBLE; }
      else{ build[window->ID-1].style=WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; }

      // Update [window] mode
      SetWindowLong(build[window->ID-1].hwnd, GWL_STYLE, build[window->ID-1].style);
      ShowWindow(build[window->ID-1].hwnd, SW_RESTORE);

      // Resize and move [window]
      SetWindowPos(
        build[window->ID-1].hwnd, NULL, window->x, window->y,
        window->width, window->height, SWP_NOZORDER | SWP_FRAMECHANGED
      );

      // Update [build] position parameters
      build[window->ID-1].x=window->x;
      build[window->ID-1].y=window->y;

      // Update buffer
      build[window->ID-1].hdc=GetDC(build[window->ID-1].hwnd);
      build[window->ID-1].hBitmap=CreateCompatibleBitmap(
        build[window->ID-1].hdc, window->width, window->height
      );
      SelectObject(build[window->ID-1].hMemDC, build[window->ID-1].hBitmap);
      ReleaseDC(build[window->ID-1].hwnd, build[window->ID-1].hdc);

      build[window->ID-1].fullScreen=false;
    }

    if(window->fullScreen==false){
      // Manage position change [message]
      if(window->x!=build[window->ID-1].x || window->y!=build[window->ID-1].y){
        // Check [window] [x] value
        if(window->x<-WINDOW_POS_MAX){
          if(przecinek.debug==true){
printf(
  "[pWarning, W006] \"Window x value is too low\" (changing from: %i to %i),\n",
  window->x, -WINDOW_POS_MAX
);
            fflush(stdout);
          }

          // Change [window] [x] value
          window->x=-WINDOW_POS_MAX;
        }
        if(window->x>WINDOW_POS_MAX){
          if(przecinek.debug==true){
printf(
  "[pWarning, W007] \"Window x value is too big\" (changing from: %i to %i),\n",
  window->x, WINDOW_POS_MAX
);
            fflush(stdout);
          }

          // Change [window] [x] value
          window->x=WINDOW_POS_MAX;
        }

        // Check [window] [y] value
        if(window->y<-WINDOW_POS_MAX){
          if(przecinek.debug==true){
printf(
  "[pWarning, W008] \"Window y value is too low\" (changing from: %i to %i),\n",
  window->y, -WINDOW_POS_MAX
);
            fflush(stdout);
          }

          // Change [window] [y] value
          window->y=-WINDOW_POS_MAX;
        }
        if(window->y>WINDOW_POS_MAX){
          if(przecinek.debug==true){
printf(
  "[pWarning, W009] \"Window y value is too big\" (changing from: %i to %i),\n",
  window->y, WINDOW_POS_MAX
);
            fflush(stdout);
          }

          // Change [window] [y] value
          window->y=WINDOW_POS_MAX;
        }

        // Update [build] position parameters
        build[window->ID-1].x=window->x;
        build[window->ID-1].y=window->y;

        // Change [event] [key] values to 0
        for(unsigned short int current=0; current<KEY_MAX; current++){
          event->key[current]=0;
        }

        // Move [window]
        SetWindowPos(
          build[window->ID-1].hwnd, NULL, build[window->ID-1].x, build[window->ID-1].y,
          0, 0, SWP_NOZORDER | SWP_NOSIZE
        );

        // Update [build] position parameters
        build[window->ID-1].x=window->x;
        build[window->ID-1].y=window->y;

        // Reset [build] [message] values
        build[window->ID-1].MOVE.x=0;
        build[window->ID-1].MOVE.y=0;
      }
      else if(build[window->ID-1].MOVE.x!=0 || build[window->ID-1].MOVE.y!=0){
        // Update [build] and [window] position parameters
        window->x=build[window->ID-1].MOVE.x;
        window->y=build[window->ID-1].MOVE.y;
        build[window->ID-1].x=build[window->ID-1].MOVE.x;
        build[window->ID-1].y=build[window->ID-1].MOVE.y;

        // Change [event] [key] values to 0
        for(unsigned short int current=0; current<KEY_MAX; current++){
          event->key[current]=0;
        }

        // Reset [build] [message] values
        build[window->ID-1].MOVE.x=0;
        build[window->ID-1].MOVE.y=0;
      }

      // Manage size change [message]
      if(window->width!=build[window->ID-1].width || window->height!=build[window->ID-1].height){
          // Check [window] [width] value
          if(window->width<WINDOW_WIDTH_MIN){
            if(przecinek.debug==true){
  printf(
    "[pWarning, W001] \"Window width value is too low\" (changing from: %i to %i),\n",
    window->width, WINDOW_WIDTH_MIN
  );
            fflush(stdout);
          }

          // Change [window] [width] value
          window->width=WINDOW_WIDTH_MIN;
        }
        if(window->width>WINDOW_WIDTH_MAX){
          if(przecinek.debug==true){
  printf(
    "[pWarning, W002] \"Window width value is too big\" (changing from: %i to %i),\n",
    window->width, WINDOW_WIDTH_MAX
  );
            fflush(stdout);
          }

          // Change [window] [width] value
          window->width=WINDOW_WIDTH_MAX;
        }

        // Check [window] [height] value
        if(window->height<WINDOW_HEIGHT_MIN){
          if(przecinek.debug==true){
  printf(
    "[pWarning, W003] \"Window height value is too low\" (changing from: %i to %i),\n",
    window->height, WINDOW_HEIGHT_MIN
  );
            fflush(stdout);
          }

          // Change [window] [height] value
          window->height=WINDOW_HEIGHT_MIN;
        }
        if(window->height>WINDOW_HEIGHT_MAX){
          if(przecinek.debug==true){
  printf(
    "[pWarning, W004] \"Window height value is too big\" (changing from: %i to %i),\n",
    window->height, WINDOW_HEIGHT_MAX
  );
            fflush(stdout);
          }

          // Change [window] [height] value
          window->height=WINDOW_HEIGHT_MAX;
        }

        // Update [build] size parameters
        build[window->ID-1].width=window->width;
        build[window->ID-1].height=window->height;

        // Resize [window]
        SetWindowPos(
          build[window->ID-1].hwnd, NULL, 0, 0,
          window->width, window->height, SWP_NOZORDER | SWP_NOMOVE
        );

        // Change [event] [key] values to 0
        for(unsigned short int current=0; current<KEY_MAX; current++){
          event->key[current]=0;
        }

        // Update buffer
        build[window->ID-1].hdc=GetDC(build[window->ID-1].hwnd);
        build[window->ID-1].hBitmap=CreateCompatibleBitmap(
          build[window->ID-1].hdc, window->width, window->height
        );
        SelectObject(build[window->ID-1].hMemDC, build[window->ID-1].hBitmap);
        ReleaseDC(build[window->ID-1].hwnd, build[window->ID-1].hdc);

        // Reset [build] [message] values
        build[window->ID-1].SIZE.width=0;
        build[window->ID-1].SIZE.height=0;
      }
      else if(build[window->ID-1].SIZE.width!=0 || build[window->ID-1].SIZE.height!=0){
        // Update [window] and [build] size parameters
        window->width=build[window->ID-1].SIZE.width;
        window->height=build[window->ID-1].SIZE.height;
        build[window->ID-1].width=window->width;
        build[window->ID-1].height=window->height;

        // Change [event] [key] values to 0
        for(unsigned short int current=0; current<KEY_MAX; current++){
          event->key[current]=0;
        }

        // Update buffer
        build[window->ID-1].hdc=GetDC(build[window->ID-1].hwnd);
        build[window->ID-1].hBitmap=CreateCompatibleBitmap(
          build[window->ID-1].hdc, window->width, window->height
        );
        SelectObject(build[window->ID-1].hMemDC, build[window->ID-1].hBitmap);
        ReleaseDC(build[window->ID-1].hwnd, build[window->ID-1].hdc);

        // Reset [build] [message] values
        build[window->ID-1].SIZE.width=0;
        build[window->ID-1].SIZE.height=0;
      }
    }

    if(build[window->ID-1].KEYON==true){
      for(unsigned short int current=0; current<KEY_MAX; current++){
        // Manage key press [message]
        if(build[window->ID-1].KEY[current]==1){
          if(event->key[current]==0){ event->key[current]=1; }
          else{ event->key[current]=2; }
        }

        // Manage key release [message]
        if(build[window->ID-1].KEY[current]==2){ event->key[current]=0; }
      }

      // Reset [build] [message] values
      build[window->ID-1].KEYON=false;
    }

    // Manage focus in [message]
    if(build[window->ID-1].FOCUSIN==true){
      activeWinID=window->ID;
      event->focus=true;

      // Reset [build] [message] values
      build[window->ID-1].FOCUSIN=false;
    }

    // Manage focus out [message]
    if(build[window->ID-1].FOCUSOUT==true && activeWinID==window->ID){
      activeWinID=0;
      event->focus=false;

      // Change [event] [key] values to 0
      for(unsigned short int current=0; current<KEY_MAX; current++){
        event->key[current]=0;
      }

      // Reset [build] [message] values
      build[window->ID-1].FOCUSOUT=false;
    }

    // Update [window] [title]
    if(strcmp(window->title, build[window->ID-1].title)!=0){
      if(strlen(window->title)>TITLE_MAX){
        if(przecinek.debug==true){
printf(
  "[pWarning, W005] \"Window title is too long\" (changing back to: %s),\n",
  build[window->ID-1].title
);
          fflush(stdout);
        }

        // Reset [window] [title]
        strncpy(window->title, build[window->ID-1].title, sizeof(window->title)-1);
        window->title[sizeof(window->title)-1]='\0';
      }
      else{
        // Update [build] [title]
        strncpy(build[window->ID-1].title, window->title, sizeof(build[window->ID-1].title)-1);
        build[window->ID-1].title[sizeof(build[window->ID-1].title)-1]='\0';

        // Change [window] [title]
        SetWindowText(build[window->ID-1].hwnd, window->title);
      }
    }

    if(window->resize==true){
      // Check [window] [widthMin] value
      if(build[window->ID-1].widthMin!=window->widthMin){
        if(window->widthMin<WINDOW_WIDTH_MIN){
          if(przecinek.debug==true){
printf(
  "[pWarning, W010] \"Window widthMin value is too low\" (changing from: %i to %i),\n",
  window->widthMin, WINDOW_WIDTH_MIN
);
            fflush(stdout);
          }

          // Change [window] [widthMin] value
          window->widthMin=WINDOW_WIDTH_MIN;
        }
        else if(window->widthMin>WINDOW_WIDTH_MAX){
          if(przecinek.debug==true){
printf(
  "[pWarning, W011] \"Window widthMin value is too big\" (changing from: %i to %i),\n",
  window->widthMin, WINDOW_WIDTH_MAX
);
            fflush(stdout);
          }

          // Change [window] [widthMin] value
          window->widthMin=WINDOW_WIDTH_MAX;
        }

        // Update [build] [widthMin] value
        build[window->ID-1].widthMin=window->widthMin;
      }

      // Check [window] [heightMin] value
      if(build[window->ID-1].heightMin!=window->heightMin){
        if(window->heightMin<WINDOW_HEIGHT_MIN){
          if(przecinek.debug==true){
printf(
  "[pWarning, W012] \"Window heightMin value is too low\" (changing from: %i to %i),\n",
  window->heightMin, WINDOW_HEIGHT_MIN
);
            fflush(stdout);
          }

          // Change [window] [heightMin] value
          window->heightMin=WINDOW_HEIGHT_MIN;
        }
        else if(window->heightMin>WINDOW_HEIGHT_MAX){
          if(przecinek.debug==true){
printf(
  "[pWarning, W013] \"Window heightMin value is too big\" (changing from: %i to %i),\n",
  window->heightMin, WINDOW_HEIGHT_MAX
);
            fflush(stdout);
          }

          // Change [window] [heightMin] value
          window->heightMin=WINDOW_HEIGHT_MAX;
        }

        // Update [build] [heightMin] value
        build[window->ID-1].heightMin=window->heightMin;
      }

      // Check [window] [widthMax] value
      if(build[window->ID-1].widthMax!=window->widthMax){
        if(window->widthMax<WINDOW_WIDTH_MIN){
          if(przecinek.debug==true){
printf(
  "[pWarning, W014] \"Window widthMax value is too low\" (changing from: %i to %i),\n",
  window->widthMax, WINDOW_WIDTH_MIN
);
            fflush(stdout);
          }

          // Change [window] [widthMax] value
          window->widthMax=WINDOW_WIDTH_MIN;
        }
        else if(window->widthMax>WINDOW_WIDTH_MAX){
          if(przecinek.debug==true){
printf(
  "[pWarning, W015] \"Window widthMax value is too big\" (changing from: %i to %i),\n",
  window->widthMax, WINDOW_WIDTH_MAX
);
            fflush(stdout);
          }

          // Change [window] [widthMax] value
          window->widthMax=WINDOW_WIDTH_MAX;
        }

        // Update [build] [widthMax] value
        build[window->ID-1].widthMax=window->widthMax;
      }

      // Check [window] [heightMax] value
      if(build[window->ID-1].heightMax!=window->heightMax){
        if(window->heightMax<WINDOW_HEIGHT_MIN){
          if(przecinek.debug==true){
printf(
  "[pWarning, W016] \"Window heightMax value is too low\" (changing from: %i to %i),\n",
  window->heightMax, WINDOW_HEIGHT_MIN
);
            fflush(stdout);
          }

          // Change [window] [heightMax] value
          window->heightMax=WINDOW_HEIGHT_MIN;
        }
        else if(window->heightMax>WINDOW_HEIGHT_MAX){
          if(przecinek.debug==true){
printf(
  "[pWarning, W017] \"Window heightMax value is too big\" (changing from: %i to %i),\n",
  window->heightMax, WINDOW_HEIGHT_MAX
);
            fflush(stdout);
          }

          // Change [window] [heightMax] value
          window->heightMax=WINDOW_HEIGHT_MAX;
        }

        // Update [build] [heightMax] value
        build[window->ID-1].heightMax=window->heightMax;
      }

      // Check [window] [widthMin] and [widthMax] values
      if(build[window->ID-1].widthMin>build[window->ID-1].widthMax){
        if(przecinek.debug==true){
printf(
  "[pWarning, W018] \"Window widthMin value is bigger than widthMax\" (changing from: %i to %i),\n",
  window->widthMin, window->widthMax
);
          fflush(stdout);
        }

          // Change [window] and [build] [widthMin] value
        window->widthMin=window->widthMax;
        build[window->ID-1].widthMin=build[window->ID-1].widthMax;
      }

      // Check [window] [heightMin] and [heightMax] values
      if(build[window->ID-1].heightMin>build[window->ID-1].heightMax){
        if(przecinek.debug==true){
printf(
  "[pWarning, W019] \"Window heightMin value is bigger than heightMax\" (changing from: %i to %i),\n",
  window->heightMin, window->heightMax
);
          fflush(stdout);
        }

        // Change [window] and [build] [heightMin] value
        window->heightMin=window->heightMax;
        build[window->ID-1].heightMin=build[window->ID-1].heightMax;
      }

      /* |\____/| Correct [window] Width
       * |  o o |
       */
      if(build[window->ID-1].widthMin==build[window->ID-1].widthMax &&
          build[window->ID-1].width!=build[window->ID-1].widthMin){
        window->width=build[window->ID-1].widthMin;
        build[window->ID-1].width=build[window->ID-1].widthMin;

        // Resize [window]
        SetWindowPos(
          build[window->ID-1].hwnd, NULL, 0, 0,
          window->width, window->height, SWP_NOZORDER | SWP_NOMOVE
        );

        // Change [event] [key] values to 0
        for(unsigned short int current=0; current<KEY_MAX; current++){
          event->key[current]=0;
        }

        // Update buffer
        build[window->ID-1].hdc=GetDC(build[window->ID-1].hwnd);
        build[window->ID-1].hBitmap=CreateCompatibleBitmap(
          build[window->ID-1].hdc, window->width, window->height
        );
        SelectObject(build[window->ID-1].hMemDC, build[window->ID-1].hBitmap);
        ReleaseDC(build[window->ID-1].hwnd, build[window->ID-1].hdc);
      }

      /* |\____/| Correct [window] Height
       * |  o o |
       */
      if(build[window->ID-1].heightMin==build[window->ID-1].heightMax &&
          build[window->ID-1].height!=build[window->ID-1].heightMin){
        window->height=build[window->ID-1].heightMin;
        build[window->ID-1].height=build[window->ID-1].heightMin;

        // Resize [window]
        SetWindowPos(
          build[window->ID-1].hwnd, NULL, 0, 0,
          window->width, window->height, SWP_NOZORDER | SWP_NOMOVE
        );

        // Change [event] [key] values to 0
        for(unsigned short int current=0; current<KEY_MAX; current++){
          event->key[current]=0;
        }

        // Update buffer
        build[window->ID-1].hdc=GetDC(build[window->ID-1].hwnd);
        build[window->ID-1].hBitmap=CreateCompatibleBitmap(
          build[window->ID-1].hdc, window->width, window->height
        );
        SelectObject(build[window->ID-1].hMemDC, build[window->ID-1].hBitmap);
        ReleaseDC(build[window->ID-1].hwnd, build[window->ID-1].hdc);
      }
    }
    else{
      window->widthMin=window->width;
      window->heightMin=window->height;
      window->widthMax=window->width;
      window->heightMax=window->height;
    }

    // Update [frameCount] and sleep
    build[window->ID-1].frameCount++;
    Sleep((1000/winCount)/przecinek.frameLimit);

    // Calculate current time
    if(GetTickCount()-build[window->ID-1].frameStart>=1000){
      // Set [event] [frameCount] and reset loop
      event->frameCount=build[window->ID-1].frameCount;

      build[window->ID-1].frameCount=0;
      build[window->ID-1].frameStart=GetTickCount();
    }
  }
  else if(przecinek.debug==true){
    printf("[pError, E001] \"Could not handle event\" (window is closed),\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| pObjectCreate() Function
 * |       | Used for creating objects
 * | o   o | [width], [height] (0:s_int)
 * \ = , = / Returns [object]
 */
pObject pObjectCreate(unsigned short int width, unsigned short int height){
  // Create local [object]
  pObject object;

  // Set [object] values
  object.x=0;
  object.y=0;

  object.width=width;
  object.height=height;

  object.color.r=0;
  object.color.g=0;
  object.color.b=0;

  // Return local [object]
  return object;
}

/* |\_____/| pObjectCollision() Function
 * |       | Used for checking collisions between objects
 * | o   o | [object1], [object2]
 * \ = , = / Returns (true/false)
 */
bool pObjectCollision(pObject object1, pObject object2){
  // Return collision
  return (object1.x<object2.x+object2.width &&
    object1.x+object1.width>object2.x &&
    object1.y<object2.y+object2.height &&
    object1.y+object1.height>object2.y
  );
}

/* |\_____/| pKey() Function
 * |       | Used for converting key names into codes
 * | o   o | [key]
 * \ = , = / Returns (0:255)
 */
unsigned short int pKey(const char *key){
  if(strcmp(key, "LMOUSE")==0 || strcmp(key, "LMouse")==0 || strcmp(key, "lmouse")==0){
    return VK_LBUTTON;
  }
  if(strcmp(key, "MMOUSE")==0 || strcmp(key, "MMouse")==0 || strcmp(key, "mmouse")==0){
    return VK_MBUTTON;
  }
  if(strcmp(key, "RMOUSE")==0 || strcmp(key, "RMouse")==0 || strcmp(key, "rmouse")==0){
    return VK_RBUTTON;
  }
  if(strcmp(key, "BACK")==0 || strcmp(key, "Back")==0 || strcmp(key, "back")==0){
    return VK_XBUTTON1;
  }
  if(strcmp(key, "FORWARD")==0 || strcmp(key, "Forward")==0 || strcmp(key, "forward")==0){
    return VK_XBUTTON2;
  }

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
