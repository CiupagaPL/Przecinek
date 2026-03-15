/****************************************************************
 *         |\_____/|  Orginal {,} made by Ciupaga
 *         |       |  Copyright 2025-2026
 *  ___    | >   < |
 * /   \  _\ = , = /  https://github.com/CiupagaPL/Przecinek
 * \__  \/        |   Make sure to read the License and Manual!
 *    \_         /
 *      \ \----\ \
 *       {,{,} {,},}
 ****************************************************************/

// Przecinek header
#include "../przecinek.h"

// Standard C libraries
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <locale.h>

// WIN and GDI+ libraries
#include <windows.h>
#include <gdiplus.h>

// GL and GLU libraries
#include <GL/gl.h>
#include <GL/glu.h>

/********************************
 *  ,______,  Define [pKey]
 *  |      |  values [DEBUG]
 *  |______|
 * (--------)
 ********************************/
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

/********************************
 *  ,______,  Define GL
 *  |      |  values [DEBUG]
 *  |______|
 * (--------)
 ********************************/
#ifndef GL_BGRA
  #define GL_BGRA 0x80E1
#endif

#ifndef GL_CLAMP_TO_EDGE
  #define GL_CLAMP_TO_EDGE 0x812F
#endif

/********************************
 *  ,______,  Define [pBuildWIN],
 *  |      |  [pWindowPointWIN]
 *  |______|  structure [DEBUG]
 * (--------)
 ********************************/
typedef struct{
  int x, y;
  int xBac, yBac;

  unsigned short int width, height;
  unsigned short int widthMin, heightMin, widthMax, heightMax;
  unsigned short int widthBac, heightBac, widthFix, heightFix;

  wchar_t title[WINDOW_TITLE_CHAR];
  DWORD frameStart;
  unsigned short int frameCount;

  bool resizable, focus, fullScreen;

  wchar_t class[8];
  HBITMAP hBitmap;
  HDC hdc;
  HGLRC buffer;
  HWND hwnd;
  DWORD style;
  GpGraphics *graphics;

  bool DESTROY, FOCUSIN, FOCUSOUT;
  pSize SIZE;
  pPosition MOVE;

  bool exist;
} pBuildWIN;

typedef struct{
  unsigned short int ID;

  HWND hwnd;
} pWindowPointWIN;

/********************************
 *  ,______,  Define [pFigureWIN]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  int x, y;
  unsigned short int width, height;
  unsigned short int vertice, rotation;

  GpPoint point[OBJECT_VERTICE_MAX];
  BYTE type[OBJECT_VERTICE_MAX];
  pPosition center;
  float sourceX[OBJECT_VERTICE_MAX], sourceY[OBJECT_VERTICE_MAX];
  unsigned short int rotationFix;

  int position;
  bool change;

  bool exist;
} pFigureWIN;

/********************************
 *  ,______,  Define [pViewWIN]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  unsigned short int size;
  short int letterSpacing, spaceSpacing, lineSpacing;
  wchar_t directory[FONT_DIRECTORY_CHAR];

  GpFontCollection *collection;
  GpFontFamily *fontFamily;
  GpFont *base;

  bool change;

  bool exist;
} pViewWIN;

/********************************
 *  ,______,  Define [pCodeWIN]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  wchar_t value[TEXT_CHAR];

  short int x[FONT_MAX], y[FONT_MAX];

  GLuint source[FONT_MAX][TEXT_CHAR];
  pSize sourceSize[FONT_MAX][TEXT_CHAR];
  pPosition sourcePosition[FONT_MAX][TEXT_CHAR];

  bool change;

  bool exist;
} pCodeWIN;

/********************************
 *  ,______,  Define [pTextureWIN]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  wchar_t directory[IMAGE_DIRECTORY_CHAR];

  GLuint source;

  bool exist;
} pTextureWIN;

// Przecinek
pPrzecinek przecinek={ true, 0, 0 };
bool setup=false;

ULONG_PTR gdiToken;
GdiplusStartupInput gdiInput;

// Window
unsigned short int windowMainID=0, windowCreateID=0;
unsigned short int windowCount=0;

pBuildWIN build[WINDOW_MAX];

HDC hdc;
GpGraphics* graphics;

WNDCLASSW wClass;
HINSTANCE hInstance;
MSG message;
POINT cursor;
RECT rectangle, fix;
RectF shape;
PAINTSTRUCT paintStruct;
GpSolidFill *fill;
GpBrush *broom;
ARGB argb;
HBRUSH brush;
PIXELFORMATDESCRIPTOR pixel;

unsigned short int currentFrameLimit;
short int frameOverHead;

unsigned short int input[PRZECINEK_KEY_MAX];
bool inputChange;

// Window Pointer
pWindowPointWIN *windowPoint, *newWindowPoint;
RECT *dpi;
WINDOWPOS *limit;

// Object
pFigureWIN figure[OBJECT_MAX];

GpPath *path;

float widthScale, heightScale, ratio, distance;
int distanceMinX, distanceMinY, distanceMaxX, distanceMaxY;
int edgeX, edgeY, projectionX, projectionY;
pPosition centerA, centerB;
int pointA[5], pointB[5];

// Font
pViewWIN view[FONT_MAX];

GpFontFamily** family;
int familyCount;
wchar_t familyName[FONT_DIRECTORY_CHAR];

// Text
pCodeWIN code[TEXT_MAX];

unsigned short int line;
short int xFix;

GpGraphics* charGraphics;
GpStringFormat* stringFormat;
GpRectF checkBox;
BYTE* charData;

// Image
pTextureWIN texture[IMAGE_MAX];

unsigned int imageWidth, imageHeight;
float tempImageHeight;

GpRect picture;
GpBitmap* imageBuffer;
BitmapData imageBufferData;

/****************************************************************
 * |\_____/| pDebugWindowReset() [DEBUG]
 * | .     |
 * |     . | In: pWindow* [window]
 * \ = , = / Out:
 *
 * This function resets [window] and [build] values.
 * Cleared variables depend on [window] [ID].
 ****************************************************************/
void pDebugWindowReset(pWindow *window){
  // Reset [window] values
  window->x=0;
  window->y=0;

  window->width=0;
  window->height=0;

  window->widthMin=0;
  window->heightMin=0;
  window->widthMax=0;
  window->heightMax=0;

  memset(window->title, 0, sizeof(window->title));
  window->frameCount=0;

  window->resizable=false;
  window->focus=false;
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

  memset(build[window->ID-1].title, 0, sizeof(build[window->ID-1].title));
  build[window->ID-1].frameCount=0;

  build[window->ID-1].resizable=false;
  build[window->ID-1].focus=false;
  build[window->ID-1].fullScreen=false;

  memset(build[window->ID-1].class, 0, sizeof(build[window->ID-1].class));
  if(build[window->ID-1].hBitmap!=NULL){
    DeleteObject(build[window->ID-1].hBitmap);
    build[window->ID-1].hBitmap=NULL;
  }
  if(build[window->ID-1].hdc!=NULL){
    wglMakeCurrent(build[window->ID-1].hdc, build[window->ID-1].buffer);
    wglDeleteContext(build[window->ID-1].buffer);
    build[window->ID-1].buffer=NULL;
    DeleteDC(build[window->ID-1].hdc);
    build[window->ID-1].hdc=NULL;
  }
  if(build[window->ID-1].hwnd!=NULL){
    DestroyWindow(build[window->ID-1].hwnd);
    build[window->ID-1].hwnd=NULL;
  }
  build[window->ID-1].style=0;
  if(build[window->ID-1].graphics!=NULL){
    GdipDeleteGraphics(build[window->ID-1].graphics);
    build[window->ID-1].graphics=NULL;
  }

  build[window->ID-1].DESTROY=false;
  build[window->ID-1].FOCUSIN=false;
  build[window->ID-1].FOCUSOUT=false;

  build[window->ID-1].SIZE.width=0;
  build[window->ID-1].SIZE.height=0;
  build[window->ID-1].MOVE.x=0;
  build[window->ID-1].MOVE.y=0;

  build[window->ID-1].exist=false;

  // Change [windowCount]
  windowCount-=1;

  return;
}

/****************************************************************
 * |\_____/| pDebugWindowProc() [DEBUG]
 * | .     | In: HWND [hwnd], UINT [uMessage],
 * |     . |     WPARAM [wParameter], LPARAM [lParameter]
 * \ = , = / Out: LRESULT
 *
 * This function handles all WIN signals.
 * It manages window creation, window size change,
 * window focus change, window position change,
 * and input. [windowPoint] holds [window] [ID].
 ****************************************************************/
LRESULT CALLBACK pDebugWindowProc(HWND hwnd, UINT uMessage, WPARAM wParameter, LPARAM lParameter){
  // Load current [windowPoint]
  windowPoint=(pWindowPointWIN *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  // Manage [window] create signal
  if(uMessage==WM_CREATE){
    // Create [newWindowPoint]
    newWindowPoint=(pWindowPointWIN *)malloc(sizeof(pWindowPointWIN));
    newWindowPoint->ID=windowCreateID;
    newWindowPoint->hwnd=hwnd;

    // Send [newWindowPoint] to memory
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)newWindowPoint);
  }

  // Manage [window] close signal
  if(uMessage==WM_DESTROY){
    // Send kill signal and destroy [hwnd]
    build[windowPoint->ID-1].DESTROY=true;
    hwnd=NULL;

    return 0;
  }

  // Manage [window] size limit change signal
  if(uMessage==WM_WINDOWPOSCHANGING){
    limit=(WINDOWPOS*)lParameter;

    if(build[windowPoint->ID-1].resizable==true){
      // Update [window] limits
      if(limit->cx<build[windowPoint->ID-1].widthMin+build[windowPoint->ID-1].widthFix){
        limit->cx=build[windowPoint->ID-1].widthMin+build[windowPoint->ID-1].widthFix;
      }
      if(limit->cy<build[windowPoint->ID-1].heightMin+build[windowPoint->ID-1].heightFix){
        limit->cy=build[windowPoint->ID-1].heightMin+build[windowPoint->ID-1].heightFix;
      }

      if(limit->cx>build[windowPoint->ID-1].widthMax+build[windowPoint->ID-1].widthFix){
        limit->cx=build[windowPoint->ID-1].widthMax+build[windowPoint->ID-1].widthFix;
      }
      if(limit->cy>build[windowPoint->ID-1].heightMax+build[windowPoint->ID-1].heightFix){
        limit->cy=build[windowPoint->ID-1].heightMax+build[windowPoint->ID-1].heightFix;
      }
    }
  }

  // Manage [window] focus change signal
  if(uMessage==WM_ACTIVATE){
    if(wParameter==WA_ACTIVE || wParameter==WA_CLICKACTIVE){
      build[windowPoint->ID-1].FOCUSIN=true;
    }
    else{ build[windowPoint->ID-1].FOCUSOUT=false; }

    // Change [przecinek] [key] values to `0`
    for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
      if(input[button]!=0){
        przecinek.key[input[button]]=0;
        input[button]=0;
      }
      else{ break; }
    }
  }

  // Manage [window] position change signal
  if(uMessage==WM_MOVE){
    build[windowPoint->ID-1].MOVE.x=LOWORD(lParameter);
    build[windowPoint->ID-1].MOVE.y=HIWORD(lParameter);

    // Correct [MOVE] values
    if(build[windowPoint->ID-1].MOVE.x>32768){ build[windowPoint->ID-1].MOVE.x-=65536; }
    if(build[windowPoint->ID-1].MOVE.y>32768){ build[windowPoint->ID-1].MOVE.y-=65536; }
  }

  // Manage [window] size change signal
  if(uMessage==WM_SIZE){
    // Refresh [window]
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);

    build[windowPoint->ID-1].SIZE.width=LOWORD(lParameter);
    build[windowPoint->ID-1].SIZE.height=HIWORD(lParameter);
  }

  // Manage [window] dpi change signal
  if(uMessage==WM_DPICHANGED){
    dpi=(RECT*)lParameter;

    build[windowPoint->ID-1].SIZE.width=dpi->right-dpi->left;
    build[windowPoint->ID-1].SIZE.height=dpi->bottom-dpi->top;
  }

  // Manage [window] key down signal
  if(uMessage==WM_KEYDOWN){
    przecinek.key[MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)]=1;

    for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
      if(input[button]==0 || input[button]==MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)){
        input[button]=MapVirtualKey(wParameter, MAPVK_VK_TO_VSC);

        break;
      }
    }
  }

  // Manage [window] key up signal
  if(uMessage==WM_KEYUP){
    przecinek.key[MapVirtualKey(wParameter, MAPVK_VK_TO_VSC)]=0;

    inputChange=false;
    for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
      if(input[button]==MapVirtualKey(wParameter, MAPVK_VK_TO_VSC) || inputChange==true){
        input[button]=input[button+1];
        inputChange=true;

        if(input[button]==0){ break; }
      }
    }
  }

  // Manage [window] `lmouse` key down
  if(uMessage==WM_LBUTTONDOWN){
    przecinek.key[VK_LBUTTON]=1;

    for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
      if(input[button]==0 || input[button]==VK_LBUTTON){
        input[button]=VK_LBUTTON;

        break;
      }
    }
  }

  // Manage [window] `lmouse` key up
  if(uMessage==WM_LBUTTONUP){
    przecinek.key[VK_LBUTTON]=0;

    inputChange=false;
    for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
      if(input[button]==VK_LBUTTON || inputChange==true){
        input[button]=input[button+1];
        inputChange=true;

        if(input[button]==0){ break; }
      }
    }
  }

  // Manage [window] `rmouse` key down
  if(uMessage==WM_RBUTTONDOWN){
    przecinek.key[VK_RBUTTON]=1;

    for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
      if(input[button]==0 || input[button]==VK_RBUTTON){
        input[button]=VK_RBUTTON;

        break;
      }
    }
  }

  // Manage [window] `rmouse` key up
  if(uMessage==WM_RBUTTONUP){
    przecinek.key[VK_RBUTTON]=0;

    inputChange=false;
    for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
      if(input[button]==VK_RBUTTON || inputChange==true){
        input[button]=input[button+1];
        inputChange=true;

        if(input[button]==0){ break; }
      }
    }
  }

  // Manage [window] `mmouse` key down
  if(uMessage==WM_MBUTTONDOWN){
    przecinek.key[VK_MBUTTON]=1;

    for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
      if(input[button]==0 || input[button]==VK_MBUTTON){
        input[button]=VK_MBUTTON;

        break;
      }
    }
  }

  // Manage [window] `mmouse` key up
  if(uMessage==WM_MBUTTONUP){
    przecinek.key[VK_MBUTTON]=0;

    inputChange=false;
    for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
      if(input[button]==VK_MBUTTON || inputChange==true){
        input[button]=input[button+1];
        inputChange=true;

        if(input[button]==0){ break; }
      }
    }
  }

  // Manage [window] `back` and `forward` key down
  if(uMessage==WM_XBUTTONDOWN){
    if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON1){
      przecinek.key[VK_XBUTTON1]=1;

      for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
        if(input[button]==0 || input[button]==VK_XBUTTON1){
          input[button]=VK_XBUTTON1;

          break;
        }
      }
    }
    if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON2){
      przecinek.key[VK_XBUTTON2]=1;

      for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
        if(input[button]==0 || input[button]==VK_XBUTTON2){
          input[button]=VK_XBUTTON2;

          break;
        }
      }
    }
  }

  // Manage [window] `back` and `forward` key up
  if(uMessage==WM_XBUTTONUP){
    if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON1){
      przecinek.key[VK_XBUTTON1]=0;

      inputChange=false;
      for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
        if(input[button]==VK_XBUTTON1 || inputChange==true){
          input[button]=input[button+1];
          inputChange=true;

          if(input[button]==0){ break; }
        }
      }
    }
    if(GET_XBUTTON_WPARAM(wParameter)==XBUTTON2){
      przecinek.key[VK_XBUTTON2]=0;

      inputChange=false;
      for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
        if(input[button]==VK_XBUTTON2 || inputChange==true){
          input[button]=input[button+1];
          inputChange=true;

          if(input[button]==0){ break; }
        }
      }
    }
  }

  // Return current [hwnd] values
  return DefWindowProcW(hwnd, uMessage, wParameter, lParameter);
}

/****************************************************************
 * |\_____/| pDebugObjectReset() [DEBUG]
 * | .     |
 * |     . | In: pObject* [object]
 * \ = , = / Out:
 *
 * This function resets all [object] and [figure] values.
 * Cleared variables depend on [object] [ID].
 ****************************************************************/
void pDebugObjectReset(pObject *object){
  // Reset [object] values
  object->x=0;
  object->y=0;
  object->width=0;
  object->height=0;

  object->vertice=0;
  object->rotation=0;

  // Reset [figure] values
  figure[object->ID-1].x=0;
  figure[object->ID-1].y=0;
  figure[object->ID-1].width=0;
  figure[object->ID-1].height=0;

  figure[object->ID-1].vertice=0;
  figure[object->ID-1].rotation=0;

  memset(figure[object->ID-1].point, 0, sizeof(figure[object->ID-1].point));
  memset(figure[object->ID-1].type, 0, sizeof(figure[object->ID-1].type));
  figure[object->ID-1].center.x=0;
  figure[object->ID-1].center.y=0;
  memset(figure[object->ID-1].sourceX, 0, sizeof(figure[object->ID-1].sourceX));
  memset(figure[object->ID-1].sourceY, 0, sizeof(figure[object->ID-1].sourceY));
  figure[object->ID-1].rotationFix=0;

  figure[object->ID-1].position=0;
  figure[object->ID-1].change=false;

  figure[object->ID-1].exist=false;

  return;
}

/****************************************************************
 * |\_____/| pDebugObjectCalculate() [DEBUG]
 * | .     |
 * |     . | In: pObject* [object]
 * \ = , = / Out:
 *
 * This function calculates all [object] vertices.
 * It firstly calculates [ratio] with `100`x`100` size.
 * Then based on calculated scale and other variables
 * it sets all [figure] [point] values.
 ****************************************************************/
void pDebugObjectCalculate(pObject *object){
  // Reset some variables
  distanceMinX=INT_MAX;
  distanceMinY=INT_MAX;
  distanceMaxX=INT_MIN;
  distanceMaxY=INT_MIN;

  for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
    // Calculate base [ratio]
    ratio=((current*360/figure[object->ID-1].vertice)+
      figure[object->ID-1].rotation+figure[object->ID-1].rotationFix)*(M_PI/180);

    // Calculate [figure] [point] [x]
    figure[object->ID-1].point[current].X=50+(sqrt(pow(100, 2)*2)/2)*cos(ratio);

    // Calculate [figure] [point] [y]
    figure[object->ID-1].point[current].Y=50+(sqrt(pow(100, 2)*2)/2)*sin(ratio);

    // Check for [distanceMinX] and [distanceMinY] values
    if(figure[object->ID-1].point[current].X<distanceMinX){
      distanceMinX=figure[object->ID-1].point[current].X;
    }
    if(figure[object->ID-1].point[current].Y<distanceMinY){
      distanceMinY=figure[object->ID-1].point[current].Y;
    }

    // Check for [distanceMaxX] and [distanceMaxY] values
    if(figure[object->ID-1].point[current].X>distanceMaxX){
      distanceMaxX=figure[object->ID-1].point[current].X;
    }
    if(figure[object->ID-1].point[current].Y>distanceMaxY){
      distanceMaxY=figure[object->ID-1].point[current].Y;
    }
  }

  // Calculate [widthScale] and [heightScale] values
  widthScale=(float)(distanceMaxX-distanceMinX)/100;
  heightScale=(float)(distanceMaxY-distanceMinY)/100;

  // Reset some variables
  distanceMinX=INT_MAX;
  distanceMinY=INT_MAX;
  distanceMaxX=INT_MIN;
  distanceMaxY=INT_MIN;

  for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
    // Calculate base [ratio]
    ratio=((current*360/figure[object->ID-1].vertice)+
      figure[object->ID-1].rotation+figure[object->ID-1].rotationFix)*(M_PI/180);

    // Calculate [figure] [point] [x]
    figure[object->ID-1].point[current].X=
      (figure[object->ID-1].width/2)+
      (sqrt(pow((float)figure[object->ID-1].width/widthScale, 2)*2)/2)*
      cos(ratio);

    // Calculate [figure] [point] [y]
    figure[object->ID-1].point[current].Y=
      (figure[object->ID-1].height/2)+
      (sqrt(pow((float)figure[object->ID-1].height/heightScale, 2)*2)/2)*
      sin(ratio);

    // Set [figure] [type] value
    figure[object->ID-1].type[current]=PathPointTypeLine;

    // Check for [distanceMinX] and [distanceMinY] values
    if(figure[object->ID-1].point[current].X<distanceMinX){
      distanceMinX=figure[object->ID-1].point[current].X;
    }
    if(figure[object->ID-1].point[current].Y<distanceMinY){
      distanceMinY=figure[object->ID-1].point[current].Y;
    }
  }

  for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
    // Correct [figure] [point] position
    figure[object->ID-1].point[current].X-=distanceMinX-figure[object->ID-1].x;
    figure[object->ID-1].point[current].Y-=distanceMinY-figure[object->ID-1].y;

    // Calculate [distanceMaxX] and [distanceMaxY]
    if(figure[object->ID-1].point[current].X>distanceMaxX){
      distanceMaxX=figure[object->ID-1].point[current].X;
    }
    if(figure[object->ID-1].point[current].Y>distanceMaxY){
      distanceMaxY=figure[object->ID-1].point[current].Y;
    }
  }

  for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
    // Calculate [figure] [sourceX] and [sourceY] values
    figure[object->ID-1].sourceX[current]=
      (float)(distanceMaxX-figure[object->ID-1].point[current].X)/
      (float)(distanceMaxX-figure[object->ID-1].x);
    figure[object->ID-1].sourceY[current]=
      (float)(distanceMaxY-figure[object->ID-1].point[current].Y)/
      (float)(distanceMaxY-figure[object->ID-1].y);

    figure[object->ID-1].sourceY[current]=1-figure[object->ID-1].sourceY[current];
    figure[object->ID-1].sourceX[current]=1-figure[object->ID-1].sourceX[current];
  }

  // Correct [figure] [type] values
  figure[object->ID-1].type[0]=PathPointTypeStart | PathPointTypeLine;
  figure[object->ID-1].type[figure[object->ID-1].vertice]=PathPointTypeCloseSubpath;

  return;
}

/****************************************************************
 * |\_____/| pDebugObjectTriangle() [DEBUG]
 * | .     |
 * |     . | In: pPosition 3x[v_A], 3x[v_B]
 * \ = , = / Out: bool
 *
 * This function checks collision between two triangles.
 * It simulates two diffrent triangle objects.
 * Then it checks and returns collision between them.
 ****************************************************************/
bool pDebugObjectTriangle(
  pPosition v1A, pPosition v2A, pPosition v3A,
  pPosition v1B, pPosition v2B, pPosition v3B
){

  // Simulate [triangleA] and [triangleB] objects
  pPosition triangleA[3]={ v1A, v2A, v3A };
  pPosition triangleB[3]={ v1B, v2B, v3B };

  for(unsigned short int current=0; current<6; current+=1){
    // Calculate [edge] values
    if(current<3){
      edgeX=triangleA[(current+1)%3].x-triangleA[current].x;
      edgeY=triangleA[(current+1)%3].y-triangleA[current].y;
    }
    else{
      edgeX=triangleB[(current-2)%3].x-triangleB[current-3].x;
      edgeY=triangleB[(current-2)%3].y-triangleB[current-3].y;
    }

    // Reset some variables
    distanceMinX=INT_MAX;
    distanceMinY=INT_MAX;
    distanceMaxX=INT_MIN;
    distanceMaxY=INT_MIN;

    for(unsigned short int loop=0; loop<3; loop+=1){
      // Caululate [projectionX] and [projectionY] values
      projectionX=(triangleA[loop].x*(-edgeY))+(triangleA[loop].y*edgeX);
      projectionY=(triangleB[loop].x*(-edgeY))+(triangleB[loop].y*edgeX);

      // Check for collision
      if(projectionX<distanceMinX){ distanceMinX=projectionX; }
      if(projectionX>distanceMaxX){ distanceMaxX=projectionX; }
      if(projectionY<distanceMinY){ distanceMinY=projectionY; }
      if(projectionY>distanceMaxY){ distanceMaxY=projectionY; }
    }

    if(distanceMaxX<distanceMinY || distanceMaxY<distanceMinX){
      // Return `false`, collision undetected
      return false;
    }
  }

  // Return `true`, collision detected
  return true;
}

/****************************************************************
 * |\_____/| pDebugFontReset() [DEBUG]
 * | .     |
 * |     . | In: pFont* [font]
 * \ = , = / Out:
 *
 * This function resets all [font] and [view] values.
 * Cleared variables depend on [font] [ID].
 ****************************************************************/
void pDebugFontReset(pFont *font){
  // Reset [font] values
  font->size=0;
  memset(font->directory, 0, sizeof(font->directory));
  font->letterSpacing=0;
  font->spaceSpacing=0;
  font->lineSpacing=0;

  // Reset [view] values
  view[font->ID-1].size=0;
  view[font->ID-1].letterSpacing=0;
  view[font->ID-1].spaceSpacing=0;
  view[font->ID-1].lineSpacing=0;
  memset(view[font->ID-1].directory, 0, sizeof(view[font->ID-1].directory));

  if(view[font->ID-1].collection!=NULL){
    GdipDeletePrivateFontCollection(&view[font->ID-1].collection);
    view[font->ID-1].collection=NULL;
  }
  if(view[font->ID-1].fontFamily!=NULL){
    GdipDeleteFontFamily(view[font->ID-1].fontFamily);
    view[font->ID-1].fontFamily=NULL;
  }
  if(view[font->ID-1].base!=NULL){
    GdipDeleteFont(view[font->ID-1].base);
    view[font->ID-1].base=NULL;
  }

  view[font->ID-1].change=false;

  view[font->ID-1].exist=false;

  return;
}

/****************************************************************
 * |\_____/| pDebugTextReset() [DEBUG]
 * | .     |
 * |     . | In: pText* [text]
 * \ = , = / Out:
 *
 * This function resets all [text] and [code] values.
 * Cleared variables depend on [text] [ID].
 ****************************************************************/
void pDebugTextReset(pText *text){
  // Reset [text] values
  text->x=0;
  text->y=0;

  memset(text->value, 0, sizeof(text->value));

  // Reset [code] values
  memset(code[text->ID-1].value, 0, sizeof(code[text->ID-1].value));

  memset(code[text->ID-1].x, 0, sizeof(code[text->ID-1].x));
  memset(code[text->ID-1].y, 0, sizeof(code[text->ID-1].y));

  for(unsigned short int font=0; font<FONT_MAX; font+=1){
    for(unsigned short int current=0; current<TEXT_CHAR; current+=1){
      glDeleteTextures(1, &code[text->ID-1].source[font][current]);
      code[text->ID-1].sourceSize[font][current]=(pSize){ 0, 0 };
      code[text->ID-1].sourcePosition[font][current]=(pPosition){ 0, 0 };
    }
  }

  code[text->ID-1].change=false;

  code[text->ID-1].exist=false;

  return;
}

/****************************************************************
 * |\_____/| pDebugTextSetup() [DEBUG]
 * | .     |
 * |     . | In: pText* [text], pFont* [font]
 * \ = , = / Out:
 *
 * This function setups [text] for current [font].
 * It creates textures used later for rendering.
 ****************************************************************/
void pDebugTextSetup(pText *text, pFont *font){
  // Reset [code] [source] for current [font]
  for(unsigned short int current=0; current<TEXT_CHAR; current+=1){
    glDeleteTextures(1, &code[text->ID-1].source[font->ID-1][current]);
    glGenTextures(1, &code[text->ID-1].source[font->ID-1][current]);
  }

  // Reset [line] and [xFix] values
  line=0;
  xFix=0;

  for(unsigned short int current=0; current<wcslen(code[text->ID-1].value); current+=1){
    // Add space to new [line]
    if(code[text->ID-1].value[current]==L'\n'){
      xFix=0;
      line+=1;

      continue;
    }
    // Add space to space O.O
    if(code[text->ID-1].value[current]==L' '){
      xFix-=view[font->ID-1].spaceSpacing*1.33;

      continue;
    }

    // Get [imageWidth] and [imageHeight] values
    GdipMeasureString(
      graphics, &code[text->ID-1].value[current], 1, view[font->ID-1].base,
      &(GpRectF){0, 0}, stringFormat, &checkBox, NULL, NULL
    );
    imageWidth=checkBox.Width;

    GdipGetFontHeight(view[font->ID-1].base, graphics, &tempImageHeight);
    imageHeight=tempImageHeight;

    // Create [imageBuffer] and [charGraphics] based on [charData]
    charData=(BYTE*)malloc(imageWidth*imageHeight*4);
    memset(charData, 0, imageWidth*imageHeight*4);
    GdipCreateBitmapFromScan0(
      imageWidth, imageHeight, imageWidth*4, PixelFormat32bppARGB,
      charData, &imageBuffer
    );
    GdipGetImageGraphicsContext(imageBuffer, &charGraphics);

    // Setup [argb] and [fill] for render
    argb=(
      (ARGB)(255<<24) |
      (255<<16) | (255<<8) | 255
    );
    GdipCreateSolidFill(argb, &fill);

    // Draw char on [charGraphics]
    GdipDrawString(
      charGraphics, &code[text->ID-1].value[current], 1, view[font->ID-1].base,
      &(GpRectF){0, 0}, stringFormat, fill
    );

    // Reset [fill]
    GdipDeleteBrush(fill);

    // Save [imageBuffer]
    GdipBitmapLockBits(
      imageBuffer, &(GpRect){0, 0, imageWidth, imageHeight},
      ImageLockModeRead, PixelFormat32bppARGB, &imageBufferData
    );

    // Setup [code] [source]
    glBindTexture(GL_TEXTURE_2D, code[text->ID-1].source[font->ID-1][current]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Initialize [code] [source]
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
      GL_BGRA, GL_UNSIGNED_BYTE, imageBufferData.Scan0
    );

    // Set [code] [sourceSize] values
    code[text->ID-1].sourceSize[font->ID-1][current].width=imageWidth;
    code[text->ID-1].sourceSize[font->ID-1][current].height=imageHeight;

    // Set [code] [sourcePosition] values
    code[text->ID-1].sourcePosition[font->ID-1][current].x=text->x-xFix;
    code[text->ID-1].sourcePosition[font->ID-1][current].y=
      text->y+(imageHeight*line)+(view[font->ID-1].lineSpacing*1.33*line);

    // Change [xFix] value
    xFix-=imageWidth+(view[font->ID-1].letterSpacing*1.33);

    // Reset some variables
    GdipBitmapUnlockBits(imageBuffer, &imageBufferData);
    GdipDisposeImage(imageBuffer);
    GdipDeleteGraphics(charGraphics);
    free(charData);
  }

  return;
}

/****************************************************************
 * |\_____/| pDebugImageReset() [DEBUG]
 * | .     |
 * |     . | In: pImage* [image]
 * \ = , = / Out:
 *
 * This function resets all [image] and [texture] values.
 * Cleared variables depend on [image] [ID].
 ****************************************************************/
void pDebugImageReset(pImage *image){
  // Reset [image] values
  memset(image->directory, 0, sizeof(image->directory));

  // Reset [texture] values
  memset(texture[image->ID-1].directory, 0, sizeof(texture[image->ID-1].directory));

  glDeleteTextures(1, &texture[image->ID-1].source);
  glGenTextures(1, &texture[image->ID-1].source);

  texture[image->ID-1].exist=false;

  return;
}

/****************************************************************
 * |\_____/| pDebugImageSetup() [DEBUG]
 * | .     |
 * |     . | In: pImage* [image]
 * \ = , = / Out:
 *
 * This function setups [texture] for current [image].
 * It loads image from given [directory].
 * It checks its parameters and initializes it.
 ****************************************************************/
void pDebugImageSetup(pImage *image){
  // Get [imageWidth] and [imageHeight] values
  GdipGetImageWidth((GpImage*)imageBuffer, &imageWidth);
  GdipGetImageHeight((GpImage*)imageBuffer, &imageHeight);

  // Create [picture]
  picture=(GpRect){0, 0, imageWidth, imageHeight};
  GdipBitmapLockBits(imageBuffer, &picture, ImageLockModeRead, PixelFormat32bppARGB, &imageBufferData);

  // Generate [texture] [source]
  glDeleteTextures(1, &texture[image->ID-1].source);
  glGenTextures(1, &texture[image->ID-1].source);
  glBindTexture(GL_TEXTURE_2D, texture[image->ID-1].source);

  // Fill [texture] [source]
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
    GL_BGRA, GL_UNSIGNED_BYTE, imageBufferData.Scan0
  );

  // Setup [texture] [source]
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Reset [imageBuffer] and [imageBufferData]
  GdipBitmapUnlockBits(imageBuffer, &imageBufferData);
  GdipDisposeImage(imageBuffer);

  return;
}

/****************************************************************
 * |\_____/| pSetup()
 * | .     |
 * |     . | In: bool [debug], us_int [frameLimit]
 * \ = , = / Out:
 *
 * This function initializes Przecinek library.
 * It sets global [debug] and [frameLimit] values.
 * It setups locale and libraries used later.
 * It checks current mouse position and display size.
 ****************************************************************/
void pSetup(bool debug, unsigned short int frameLimit){
  // Initialize locale
  setlocale(LC_ALL, "");

  // Initialize GDI+
  gdiInput.GdiplusVersion=1;
  gdiInput.DebugEventCallback=NULL;
  gdiInput.SuppressBackgroundThread=FALSE;
  gdiInput.SuppressExternalCodecs=FALSE;
  GdiplusStartup(&gdiToken, &gdiInput, NULL);

  // Update [przecinek] [debug] value
  przecinek.debug=debug;

  // Set [przecinek] [key] and [keyCaps] values
  memset(przecinek.key, 0, sizeof(przecinek.key));
  przecinek.keyCaps=false;

  // Update [przecinek] [frameLimit] value
  if(frameLimit<PRZECINEK_FRAME_MIN){
    if(przecinek.debug==true){
      printf("[pSetup() Warning]\n");
      printf("Value of przecinek.frameLimit is too small!\n");
      printf("Value of przecinek.frameLimit was changed from %i to %i,\n", frameLimit, PRZECINEK_FRAME_MIN);
      fflush(stdout);
    }

    frameLimit=PRZECINEK_FRAME_MIN;
  }
  else if(frameLimit>PRZECINEK_FRAME_MAX){
    if(przecinek.debug==true){
      printf("[pSetup() Warning]\n");
      printf("Value of przecinek.frameLimit is too large!\n");
      printf("Value of przecinek.frameLimit was changed from %i to %i,\n", frameLimit, PRZECINEK_FRAME_MAX);
      fflush(stdout);
    }

    frameLimit=PRZECINEK_FRAME_MAX;
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

  // Create global [hdc]
  hdc=GetDC(NULL);

  // Create global [graphics]
  GdipCreateFromHDC(hdc, &graphics);

  // Create [stringFormat]
  GdipCreateStringFormat(0, 0, &stringFormat);
  GdipStringFormatGetGenericTypographic(&stringFormat);

  setup=true;

  return;
}

/****************************************************************
 * |\_____/| pEndup()
 * | .     |
 * |     . | In:
 * \ = , = / Out:
 *
 * This function cleans up debug variables before the end of
 * the program. It should be used when all windows are closed.
 ****************************************************************/
void pEndup(){
  // End GDI+ session
  GdiplusShutdown(gdiToken);

  // Destroy debug [graphics], [hdc] and [stringFormat]
  DeleteObject(graphics);
  ReleaseDC(NULL, hdc);
  GdipDeleteStringFormat(stringFormat);

  setup=false;

  return;
}

/****************************************************************
 * |\_____/| pClear()
 * | .     |
 * |     . | In:
 * \ = , = / Out:
 *
 * This function clears debug console.
 * It uses `cls` or `clear` depending on the OS.
 ****************************************************************/
void pClear(){
  // Clear console
  system("cls");
  fflush(stdout);

  return;
}

/****************************************************************
 * |\_____/| pKey()
 * | .     |
 * |     . | In: wchar_t* [key]
 * \ = , = / Out: us_int
 *
 * This function converts given [key] value into it's id.
 * Returned [key] id depends on current OS.
 ****************************************************************/
unsigned short int pKey(wchar_t *key){
  if(wcscmp(key, L"LMOUSE")==0 || wcscmp(key, L"LMouse")==0 || wcscmp(key, L"lmouse")==0){ return VK_LBUTTON; }
  if(wcscmp(key, L"MMOUSE")==0 || wcscmp(key, L"MMouse")==0 || wcscmp(key, L"mmouse")==0){ return VK_MBUTTON; }
  if(wcscmp(key, L"RMOUSE")==0 || wcscmp(key, L"RMouse")==0 || wcscmp(key, L"rmouse")==0){ return VK_RBUTTON; }
  if(wcscmp(key, L"BACK")==0 || wcscmp(key, L"Back")==0 || wcscmp(key, L"back")==0){ return VK_XBUTTON1; }
  if(wcscmp(key, L"FORWARD")==0 || wcscmp(key, L"Forward")==0 || wcscmp(key, L"forward")==0){ return VK_XBUTTON2; }

  if(wcscmp(key, L"ESC")==0 || wcscmp(key, L"esc")==0 || wcscmp(key, L"Esc")==0){
    return MapVirtualKey(VK_ESCAPE, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"TAB")==0 || wcscmp(key, L"tab")==0 || wcscmp(key, L"Tab")==0){
    return MapVirtualKey(VK_TAB, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"CAPS")==0 || wcscmp(key, L"caps")==0 || wcscmp(key, L"Caps")==0){
    return MapVirtualKey(VK_CAPITAL, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"LSHIFT")==0 || wcscmp(key, L"lshift")==0 || wcscmp(key, L"LShift")==0){
    return MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"LCTRL")==0 || wcscmp(key, L"lctrl")==0 || wcscmp(key, L"LCtrl")==0){
    return MapVirtualKey(VK_LCONTROL, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"LMOD")==0 || wcscmp(key, L"lmod")==0 || wcscmp(key, L"LMod")==0){
    return MapVirtualKey(VK_LWIN, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"LALT")==0 || wcscmp(key, L"lalt")==0 || wcscmp(key, L"LAlt")==0){
    return MapVirtualKey(VK_LMENU, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"SPACE")==0 || wcscmp(key, L"space")==0 || wcscmp(key, L"Space")==0){
    return MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC); }

  else if(wcscmp(key, L"F1")==0 || wcscmp(key, L"f1")==0){ return MapVirtualKey(VK_F1, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F2")==0 || wcscmp(key, L"f2")==0){ return MapVirtualKey(VK_F2, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F3")==0 || wcscmp(key, L"f3")==0){ return MapVirtualKey(VK_F3, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F4")==0 || wcscmp(key, L"f4")==0){ return MapVirtualKey(VK_F4, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F5")==0 || wcscmp(key, L"f5")==0){ return MapVirtualKey(VK_F5, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F6")==0 || wcscmp(key, L"f6")==0){ return MapVirtualKey(VK_F6, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F7")==0 || wcscmp(key, L"f7")==0){ return MapVirtualKey(VK_F7, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F8")==0 || wcscmp(key, L"f8")==0){ return MapVirtualKey(VK_F8, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F9")==0 || wcscmp(key, L"f9")==0){ return MapVirtualKey(VK_F9, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F10")==0 || wcscmp(key, L"f10")==0){ return MapVirtualKey(VK_F10, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F11")==0 || wcscmp(key, L"f11")==0){ return MapVirtualKey(VK_F11, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F12")==0 || wcscmp(key, L"f12")==0){ return MapVirtualKey(VK_F12, MAPVK_VK_TO_VSC); }

  else if(wcscmp(key, L"RALT")==0 || wcscmp(key, L"ralt")==0 || wcscmp(key, L"RAlt")==0){
    return MapVirtualKey(VK_RMENU, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"RWIN")==0 || wcscmp(key, L"rwin")==0 || wcscmp(key, L"RWin")==0){
    return MapVirtualKey(VK_RWIN, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"MENU")==0 || wcscmp(key, L"menu")==0 || wcscmp(key, L"Menu")==0){
    return MapVirtualKey(VK_APPS, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"RCTRL")==0 || wcscmp(key, L"rctrl")==0 || wcscmp(key, L"RCtrl")==0){
    return MapVirtualKey(VK_RCONTROL, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"RSHIFT")==0 || wcscmp(key, L"rshift")==0 || wcscmp(key, L"RShift")==0){
    return MapVirtualKey(VK_RSHIFT, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"ENTER")==0 || wcscmp(key, L"enter")==0 || wcscmp(key, L"Enter")==0){
    return MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"BACKSPACE")==0 || wcscmp(key, L"backspace")==0 || wcscmp(key, L"Backspace")==0){
    return MapVirtualKey(VK_BACK, MAPVK_VK_TO_VSC); }

  else if(wcscmp(key, L"LARROW")==0 || wcscmp(key, L"larrow")==0 || wcscmp(key, L"LArrow")==0){
    return MapVirtualKey(VK_LEFT, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"DARROW")==0 || wcscmp(key, L"darrow")==0 || wcscmp(key, L"DArrow")==0){
    return MapVirtualKey(VK_DOWN, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"RARROW")==0 || wcscmp(key, L"rarrow")==0 || wcscmp(key, L"RArrow")==0){
    return MapVirtualKey(VK_RIGHT, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"UARROW")==0 || wcscmp(key, L"uarrow")==0 || wcscmp(key, L"UArrow")==0){
    return MapVirtualKey(VK_UP, MAPVK_VK_TO_VSC); }

  else if(wcscmp(key, L"PRINTSCRN")==0 || wcscmp(key, L"printscrn")==0 || wcscmp(key, L"PrintScrn")==0){
    return MapVirtualKey(VK_SNAPSHOT, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"SCROLLLOCK")==0 || wcscmp(key, L"scrolllock")==0 || wcscmp(key, L"ScrollLock")==0){
    return MapVirtualKey(VK_SCROLL, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"PAUSEBREAK")==0 || wcscmp(key, L"pausebreak")==0 || wcscmp(key, L"PauseBreak")==0){
    return MapVirtualKey(VK_PAUSE, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"INS")==0 || wcscmp(key, L"ins")==0 || wcscmp(key, L"Ins")==0){
    return MapVirtualKey(VK_INSERT, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"HOME")==0 || wcscmp(key, L"home")==0 || wcscmp(key, L"Home")==0){
    return MapVirtualKey(VK_HOME, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"PAGEU")==0 || wcscmp(key, L"pageu")==0 || wcscmp(key, L"PageU")==0){
    return MapVirtualKey(VK_PRIOR, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"DEL")==0 || wcscmp(key, L"del")==0 || wcscmp(key, L"Del")==0){
    return MapVirtualKey(VK_DELETE, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"END")==0 || wcscmp(key, L"end")==0 || wcscmp(key, L"End")==0){
    return MapVirtualKey(VK_END, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"PAGED")==0 || wcscmp(key, L"paged")==0 || wcscmp(key, L"PageD")==0){
    return MapVirtualKey(VK_NEXT, MAPVK_VK_TO_VSC); }

  else if(wcscmp(key, L"Q")==0 || wcscmp(key, L"q")==0){ return MapVirtualKey(VK_Q, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"W")==0 || wcscmp(key, L"w")==0){ return MapVirtualKey(VK_W, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"E")==0 || wcscmp(key, L"e")==0){ return MapVirtualKey(VK_E, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"R")==0 || wcscmp(key, L"r")==0){ return MapVirtualKey(VK_R, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"T")==0 || wcscmp(key, L"t")==0){ return MapVirtualKey(VK_T, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"Y")==0 || wcscmp(key, L"y")==0){ return MapVirtualKey(VK_Y, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"U")==0 || wcscmp(key, L"u")==0){ return MapVirtualKey(VK_U, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"I")==0 || wcscmp(key, L"i")==0){ return MapVirtualKey(VK_I, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"O")==0 || wcscmp(key, L"o")==0){ return MapVirtualKey(VK_O, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"P")==0 || wcscmp(key, L"p")==0){ return MapVirtualKey(VK_P, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"A")==0 || wcscmp(key, L"a")==0){ return MapVirtualKey(VK_A, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"S")==0 || wcscmp(key, L"s")==0){ return MapVirtualKey(VK_S, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"D")==0 || wcscmp(key, L"d")==0){ return MapVirtualKey(VK_D, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"F")==0 || wcscmp(key, L"f")==0){ return MapVirtualKey(VK_F, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"G")==0 || wcscmp(key, L"g")==0){ return MapVirtualKey(VK_G, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"H")==0 || wcscmp(key, L"h")==0){ return MapVirtualKey(VK_H, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"J")==0 || wcscmp(key, L"j")==0){ return MapVirtualKey(VK_J, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"K")==0 || wcscmp(key, L"k")==0){ return MapVirtualKey(VK_K, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"L")==0 || wcscmp(key, L"l")==0){ return MapVirtualKey(VK_L, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"Z")==0 || wcscmp(key, L"z")==0){ return MapVirtualKey(VK_Z, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"X")==0 || wcscmp(key, L"x")==0){ return MapVirtualKey(VK_X, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"C")==0 || wcscmp(key, L"c")==0){ return MapVirtualKey(VK_C, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"V")==0 || wcscmp(key, L"v")==0){ return MapVirtualKey(VK_V, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"B")==0 || wcscmp(key, L"b")==0){ return MapVirtualKey(VK_B, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"N")==0 || wcscmp(key, L"n")==0){ return MapVirtualKey(VK_N, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"M")==0 || wcscmp(key, L"m")==0){ return MapVirtualKey(VK_M, MAPVK_VK_TO_VSC); }

  else if(wcscmp(key, L"1")==0){ return MapVirtualKey(VK_1, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"2")==0){ return MapVirtualKey(VK_2, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"3")==0){ return MapVirtualKey(VK_3, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"4")==0){ return MapVirtualKey(VK_4, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"5")==0){ return MapVirtualKey(VK_5, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"6")==0){ return MapVirtualKey(VK_6, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"7")==0){ return MapVirtualKey(VK_7, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"8")==0){ return MapVirtualKey(VK_8, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"9")==0){ return MapVirtualKey(VK_9, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"0")==0){ return MapVirtualKey(VK_0, MAPVK_VK_TO_VSC); }

  else if(wcscmp(key, L"`")==0){ return MapVirtualKey(VK_OEM_3, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L",")==0){ return MapVirtualKey(VK_OEM_COMMA, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L".")==0){ return MapVirtualKey(VK_OEM_PERIOD, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"/")==0){ return MapVirtualKey(VK_OEM_2, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L";")==0){ return MapVirtualKey(VK_OEM_SEMICOLON, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"'")==0){ return MapVirtualKey(VK_OEM_QUOTE, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"\\")==0){ return MapVirtualKey(VK_OEM_5, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"[")==0){ return MapVirtualKey(VK_OEM_4, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"]")==0){ return MapVirtualKey(VK_OEM_6, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"-")==0){ return MapVirtualKey(VK_OEM_MINUS, MAPVK_VK_TO_VSC); }
  else if(wcscmp(key, L"=")==0){ return MapVirtualKey(VK_OEM_PLUS, MAPVK_VK_TO_VSC); }

  return 0;
}

/****************************************************************
 * |\_____/| pWindowCreate()
 * | .     |
 * |     . | In: us_int [width], [height], bool [resizable]
 * \ = , = / Out: pWindow
 *
 * This function creates Przecinek window.
 * It sets [ID] for local [window].
 * It checks if all given parameters are valid.
 * It fills all necessary values for [window] and [build].
 * It creates [window] [buffer] for rendering purposes.
 * It sets [window] [title] to default value.
 * It setups [build] objects for later use.
 * It also saves time when [window] was created,
 * to later calculate frame count.
 ****************************************************************/
pWindow pWindowCreate(unsigned short int width, unsigned short int height, bool resizable){
  // Manage console
  if(GetConsoleWindow()==NULL && przecinek.debug==true){
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
  }
  else if(GetConsoleWindow()!=NULL && przecinek.debug==false){ FreeConsole(); }

  // Create local [window]
  pWindow window;

  // Change [windowCount]
  windowCount+=1;

  for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
    if(build[current].exist==false){
      // Set [window] [ID] and reset [window]
      window.ID=current+1;
      pDebugWindowReset(&window);

      // Change [windowCount]
      windowCount+=1;
      break;
    }
    else if(current==WINDOW_MAX-1){
      if(przecinek.debug==true){
        printf("[pWindowCreate() Error]\n");
        printf("Too many windows were created!\n");
        printf("Current window limit is equal to %i.\n", WINDOW_MAX);
        printf("Try to destroy unused windows or change Przecinek window limit,\n");
        fflush(stdout);
      }

      // Return [window]
      window.ID=0;

      return window;
    }
  }

  // Check if Przecinek is initialized
  if(setup==false){
    printf("[pWindowCreate() Error]\n");
    printf("Could not create window!\n");
    printf("Przecinek is not initialized.\n");
    printf("Try to run pSetup() first,\n");
    fflush(stdout);

    // Reset and return [window]
    pDebugWindowReset(&window);
    return window;
  }

  // Check [width] value
  if(width<WINDOW_WIDTH_MIN){
    if(przecinek.debug==true){
      printf("[pWindowCreate() Warning]\n");
      printf("Value of window.width is too small!\n");
      printf("Value of window.width was changed from %i to %i,\n", width, WINDOW_WIDTH_MIN);
      fflush(stdout);
    }

    // Change [width] value
    width=WINDOW_WIDTH_MIN;
  }
  else if(width>WINDOW_WIDTH_MAX){
    if(przecinek.debug==true){
      printf("[pWindowCreate() Warning]\n");
      printf("Value of window.width is too large!\n");
      printf("Value of window.width was changed from %i to %i,\n", width, WINDOW_WIDTH_MAX);
      fflush(stdout);
    }

    // Change [width] value
    width=WINDOW_WIDTH_MAX;
  }

  // Check [height] value
  if(height<WINDOW_HEIGHT_MIN){
    if(przecinek.debug==true){
      printf("[pWindowCreate() Warning]\n");
      printf("Value of window.height is too small!\n");
      printf("Value of window.height was changed from %i to %i,\n", height, WINDOW_HEIGHT_MIN);
      fflush(stdout);
    }

    // Change [height] value
    height=WINDOW_HEIGHT_MIN;
  }
  else if(height>WINDOW_HEIGHT_MAX){
    if(przecinek.debug==true){
      printf("[pWindowCreate() Warning]\n");
      printf("Value of window.height is too large!\n");
      printf("Value of window.height was changed from %i to %i,\n", height, WINDOW_HEIGHT_MAX);
      fflush(stdout);
    }

    // Change [height] value
    height=WINDOW_HEIGHT_MAX;
  }

  // Update [window] values
  window.x=WINDOW_X_DEF;
  window.y=WINDOW_Y_DEF;

  window.width=width;
  window.height=height;

  if(resizable==true){
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

  window.resizable=resizable;
  windowCreateID=window.ID;

  // Update [build] values
  build[window.ID-1].exist=true;

  build[window.ID-1].x=window.x;
  build[window.ID-1].y=window.y;

  build[window.ID-1].width=width;
  build[window.ID-1].height=height;

  build[window.ID-1].widthMin=window.widthMin;
  build[window.ID-1].heightMin=window.heightMin;
  build[window.ID-1].widthMax=window.widthMax;
  build[window.ID-1].heightMax=window.heightMax;

  build[window.ID-1].resizable=resizable;

  // Setup [hInstance] and [build] [class]
  hInstance=GetModuleHandle(NULL);
  swprintf(build[window.ID-1].class, sizeof(build[window.ID-1].class), L"pClass%i", window.ID);

  // Create [wClass]
  wClass.lpfnWndProc=pDebugWindowProc;
  wClass.hInstance=hInstance;

  // Check if [class] already exists
  if(GetClassInfoW(hInstance, build[window.ID-1].class, &wClass)){
    swprintf(build[window.ID-1].class, sizeof(build[window.ID-1].class), L"pWin%i", window.ID+WINDOW_MAX);
  }

  // Setup [wClass]
  wClass.lpszClassName=build[window.ID-1].class;
  wClass.hCursor=LoadCursor(NULL, IDC_ARROW);
  if(!RegisterClassW(&wClass)){
    if(przecinek.debug==true){
      printf("[pWindowCreate() Error]\n");
      printf("WinAPI library class is unavailable!\n");
      printf("Try to close other Przecinek instance or recompile Przecinek,\n");
      fflush(stdout);
    }

    // Reset and return [window]
    pDebugWindowReset(&window);
    return window;
  }

  // Set [build] [style]
  if(resizable==true){ build[window.ID-1].style=WS_OVERLAPPEDWINDOW; }
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

  // Set [title] values
  mbstowcs(window.title, WINDOW_TITLE_DEF, WINDOW_TITLE_CHAR);
  mbstowcs(build[window.ID-1].title, WINDOW_TITLE_DEF, WINDOW_TITLE_CHAR);

  // Create [hwnd] for [build]
  build[window.ID-1].hwnd=CreateWindowExW(
    0, build[window.ID-1].class, window.title, build[window.ID-1].style,
    CW_USEDEFAULT, CW_USEDEFAULT, rectangle.right-rectangle.left, rectangle.bottom-rectangle.top,
    NULL, NULL, hInstance, NULL
  );
  if(build[window.ID-1].hwnd==NULL){
    if(przecinek.debug==true){
      printf("[pWindowCreate() Error]\n");
      printf("WinAPI library hwnd doesn't work!\n");
      printf("Try to reinstall WIN package or recompile Przecinek,\n");
      fflush(stdout);
    }

    // Reset and return [window]
    pDebugWindowReset(&window);
    return window;
  }

  // Calculate [window] offset
  GetWindowRect(build[window.ID-1].hwnd, &rectangle);
  GetClientRect(build[window.ID-1].hwnd, &fix);

  // Set [build] [widthFix] and [heightFix] values
  build[window.ID-1].widthFix=(rectangle.right-rectangle.left)-(fix.right-fix.left);
  build[window.ID-1].heightFix=(rectangle.bottom-rectangle.top)-(fix.bottom-fix.top);

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

  // Get [build] [window] [hdc]
  build[window.ID-1].hdc=GetDC(build[window.ID-1].hwnd);

  // Setup [pixel]
  pixel=(PIXELFORMATDESCRIPTOR){
    sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  SetPixelFormat(build[window.ID-1].hdc, ChoosePixelFormat(build[window.ID-1].hdc, &pixel), &pixel);

  // Create [build] [buffer]
  build[window.ID-1].buffer=wglCreateContext(build[window.ID-1].hdc);
  wglMakeCurrent(build[window.ID-1].hdc, build[window.ID-1].buffer);

  // Setup [build] [buffer]
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, height, 0, (-1), 1);
  glMatrixMode(GL_MODELVIEW);

  // Return local [window]
  return window;
}

/****************************************************************
 * |\_____/| pWindowDrawObject()
 * | .     | In: pWindow* [window], pObject* [object],
 * |     . |     pColor* [color], pImage* [image]
 * \ = , = / Out:
 *
 * This function draws [object] on [window].
 * It checks if [color] and [image] values are valid.
 * It checks for any changes in [object] values.
 * Then it does all the rendering stuff.
 ****************************************************************/
void pWindowDrawObject(pWindow *window, pObject *object, pColor *color, pImage *image){
  if(window==NULL){
    if(przecinek.debug==true){
      printf("[pWindowDrawObject() Error]\n");
      printf("Given window is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }
  if(object==NULL){
    if(przecinek.debug==true){
      printf("[pWindowDrawObject() Error]\n");
      printf("Given object is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }

  if(window->ID!=0){
    if(object->ID!=0 && (image==NULL || (image!=NULL && image->ID!=0))){
      if(color!=NULL){
        // Check [color] values
        if(color->red>255){
          if(przecinek.debug==true){
            printf("[pWindowDrawObject() Warning]\n");
            printf("Value of color.red is too large!\n");
            printf("Value of color.red was changed from %i to 255,\n", color->red);
            fflush(stdout);
          }

          // Correct [color] [red] value
          color->red=255;
        }
        if(color->green>255){
          if(przecinek.debug==true){
            printf("[pWindowDrawObject() Warning]\n");
            printf("Value of color.green is too large!\n");
            printf("Value of color.green was changed from %i to 255,\n", color->green);
            fflush(stdout);
          }

          // Correct [color] [green] value
          color->green=255;
        }
        if(color->blue>255){
          if(przecinek.debug==true){
            printf("[pWindowDrawObject() Warning]\n");
            printf("Value of color.blue is too large!\n");
            printf("Value of color.blue was changed from %i to 255,\n", color->blue);
            fflush(stdout);
          }

          // Correct [color] [blue] value
          color->blue=255;
        }
        if(color->alpha>255){
          if(przecinek.debug==true){
            printf("[pWindowDrawObject() Warning]\n");
            printf("Value of color.alpha is too large!\n");
            printf("Value of color.alpha was changed from %i to 255,\n", color->alpha);
            fflush(stdout);
          }

          // Correct [color] [alpha] value
          color->alpha=255;
        }
      }

      if(image!=NULL){
        if(wcscmp(image->directory, texture[image->ID-1].directory)!=0){
          // Update [texture] [directory] values
          wcscpy(texture[image->ID-1].directory, image->directory);

          // Check [directory] value
          if((wcslen(image->directory)>=4 &&
              (wcscmp(image->directory+wcslen(image->directory)-4, L".png")==0 ||
               wcscmp(image->directory+wcslen(image->directory)-4, L".jpg")==0)) ||
              (wcslen(image->directory)>=5 &&
              wcscmp(image->directory+wcslen(image->directory)-5, L".jpeg")==0)){

            // Check if [texture] [directory] exists
            if(GdipCreateBitmapFromFile(texture[image->ID-1].directory, &imageBuffer)!=Ok){
              if(przecinek.debug==true){
                printf("[pWindowDrawObject() Error]\n");
                printf("GDI+ library could not load texture!\n");
                printf(
                  "Check if you gave the correct value of texture.directory and if your file isn't corruped,\n"
                );

                printf("[pWindowDrawObject() Error]\n");
                printf("Could not draw image!\n");
                printf("It wasn't initialized properly.\n");
                printf("Try to recreate it using pImageCreate(),\n");
                fflush(stdout);
              }

              // Reset [image]
              pDebugImageReset(image);
              image->ID=0;

              return;
            }
          }
          else{
            if(przecinek.debug==true){
              printf("[pWindowDrawObject() Error]\n");
              printf("Value of image.directory doesn't include .png/.jpg/.jpeg extenstion!\n");
              printf("Check if you gave the correct value,\n");

              printf("[pWindowDrawObject() Error]\n");
              printf("Could not draw image!\n");
              printf("It wasn't initialized properly.\n");
              printf("Try to recreate it using pImageCreate(),\n");
              fflush(stdout);
            }

            // Reset [image]
            pDebugImageReset(image);
            image->ID=0;

            return;
          }

          // Load [texture] [source]
          pDebugImageSetup(image);
        }
      }

      // Update [figure] [vertice]
      if(object->vertice!=figure[object->ID-1].vertice){
        // Check [object] [vertice] value
        if(object->vertice<OBJECT_VERTICE_MIN){
          if(przecinek.debug==true){
            printf("[pWindowDrawObject() Warning]\n");
            printf("Value of object.vertice is too small!\n");
            printf(
              "Value of object.vertice was changed from %i to %i,\n",
              object->vertice, OBJECT_VERTICE_MIN
            );
            fflush(stdout);
          }

          // Change [vertice] value
          object->vertice=OBJECT_VERTICE_MIN;
        }
        else if(object->vertice>OBJECT_VERTICE_MAX){
          if(przecinek.debug==true){
            printf("[pWindowDrawObject() Warning]\n");
            printf("Value of object.vertice is too large!\n");
            printf(
              "Value of object.vertice was changed from %i to %i,\n",
              object->vertice, OBJECT_VERTICE_MAX
            );
            fflush(stdout);
          }

          // Change [vertice] value
          object->vertice=OBJECT_VERTICE_MAX;
        }

        // Update [figure] [vertice]
        figure[object->ID-1].vertice=object->vertice;

        figure[object->ID-1].change=true;
      }

      if(object->width!=figure[object->ID-1].width){
        // Update [figure] [width]
        figure[object->ID-1].width=object->width;

        figure[object->ID-1].change=true;
      }

      if(object->height!=figure[object->ID-1].height){
        // Update [figure] [height]
        figure[object->ID-1].height=object->height;

        figure[object->ID-1].change=true;
      }

      if(object->rotation!=figure[object->ID-1].rotation){
        // Check [object] [rotation] value
        if(object->rotation>360){
          if(przecinek.debug==true){
            printf("[pWindowDrawObject() Warning]\n");
            printf("Value of object.rotation is too large!\n");
            printf(
              "Value of object.rotation was changed from %i to 360,\n",
              object->rotation
            );
            fflush(stdout);
          }

          // Change [object] [rotation] value
          object->rotation=360;
        }

        // Update [figure] [rotation]
        figure[object->ID-1].rotation=object->rotation;

        figure[object->ID-1].change=true;
      }

      if(object->x!=figure[object->ID-1].x){
        // Calculate [figure] [point] [x] position
        if(figure[object->ID-1].change==false){
          figure[object->ID-1].position=object->x-figure[object->ID-1].x;

          for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
            figure[object->ID-1].point[current].X+=figure[object->ID-1].position;
          }
        }

        // Update [figure] [x]
        figure[object->ID-1].x=object->x;
      }

      if(object->y!=figure[object->ID-1].y){
        // Calculate [figure] [point] [y] position
        if(figure[object->ID-1].change==false){
          figure[object->ID-1].position=object->y-figure[object->ID-1].y;

          for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
            figure[object->ID-1].point[current].Y+=figure[object->ID-1].position;
          }
        }

        // Update [figure] [y]
        figure[object->ID-1].y=object->y;
      }

      // Update [figure] [point]
      if(figure[object->ID-1].change==true){
        if(figure[object->ID-1].vertice==4){ figure[object->ID-1].rotationFix=45; }
        else if(figure[object->ID-1].vertice%2!=0){ figure[object->ID-1].rotationFix=270; }
        else{ figure[object->ID-1].rotationFix=0; }

        // Calculate all [figure] values
        pDebugObjectCalculate(object);

        figure[object->ID-1].change=false;
      }

      // Set current [build] [buffer]
      wglMakeCurrent(build[window->ID-1].hdc, build[window->ID-1].buffer);

      // Setup [build] [buffer]
      if(color==NULL){
        if(image==NULL){
          glColor4f(
            (float)COLOR_DEFAULT_FOREGROUND.red/255, (float)COLOR_DEFAULT_FOREGROUND.green/255,
            (float)COLOR_DEFAULT_FOREGROUND.blue/255, (float)COLOR_DEFAULT_FOREGROUND.alpha/255
          );
        }
        else{ glColor4f((float)1, (float)1, (float)1, (float)1); }
      }
      else{
        glColor4f(
          (float)color->red/255, (float)color->green/255, (float)color->blue/255, (float)color->alpha/255
        );
      }

      if(image!=NULL){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[image->ID-1].source);
      }

      // Draw on [build] [buffer]
      glBegin(GL_POLYGON);

      for(unsigned short int current=0; current<object->vertice; current+=1){
        glTexCoord2f(
          figure[object->ID-1].sourceX[current],
          figure[object->ID-1].sourceY[current]
        );
        glVertex2i(figure[object->ID-1].point[current].X, figure[object->ID-1].point[current].Y);
      }

      glEnd();

      glDisable(GL_TEXTURE_2D);
    }
    else if(przecinek.debug==true){
      if(object->ID==0){
        printf("[pWindowDrawObject() Error]\n");
        printf("Could not draw object!\n");
        printf("It wasn't initialized properly.\n");
        printf("Try to recreate it using pObjectCreate(),\n");
      }
      if(image->ID==0){
        printf("[pWindowDrawObject() Error]\n");
        printf("Could not draw image!\n");
        printf("It wasn't initialized properly.\n");
        printf("Try to recreate it using pImageCreate(),\n");
      }
      fflush(stdout);
    }
  }
  else if(przecinek.debug==true){
    printf("[pWindowDrawObject() Error]\n");
    printf("Could not draw object on window!\n");
    printf("Window wasn't initialized properly.\n");
    printf("Try to recreate it using pWindowCreate(),\n");
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pWindowDrawText()
 * | .     | In: pWindow* [window], pFont* [font],
 * |     . |     pText* [text], pColor* [color];
 * \ = , = / Out:
 *
 * This function draws [text] in [font] style on [window].
 * It checks if [color] values are valid.
 * It checks for any changes in [font] values.
 * Then it does all the rendering stuff.
 ****************************************************************/
void pWindowDrawText(pWindow *window, pFont *font, pText *text, pColor *color){
  if(window==NULL){
    if(przecinek.debug==true){
      printf("[pWindowDrawText() Error]\n");
      printf("Given window is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }
  if(font==NULL){
    if(przecinek.debug==true){
      printf("[pWindowDrawText() Error]\n");
      printf("Given font is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }
  if(text==NULL){
    if(przecinek.debug==true){
      printf("[pWindowDrawText() Error]\n");
      printf("Given text is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }

  if(window->ID!=0){
    if(font->ID!=0 && text->ID!=0){
      if(color!=NULL){
        // Check [color] values
        if(color->red>255){
          if(przecinek.debug==true){
            printf("[pWindowDrawText() Warning]\n");
            printf("Value of color.red is too large!\n");
            printf("Value of color.red was changed from %i to 255,\n", color->red);
            fflush(stdout);
          }

          // Correct [color] [red] value
          color->red=255;
        }
        if(color->green>255){
          if(przecinek.debug==true){
            printf("[pWindowDrawText() Warning]\n");
            printf("Value of color.green is too large!\n");
            printf("Value of color.green was changed from %i to 255,\n", color->green);
            fflush(stdout);
          }

          // Correct [color] [green] value
          color->green=255;
        }
        if(color->blue>255){
          if(przecinek.debug==true){
            printf("[pWindowDrawText() Warning]\n");
            printf("Value of color.blue is too large!\n");
            printf("Value of color.blue was changed from %i to 255,\n", color->blue);
            fflush(stdout);
          }

          // Correct [color] [blue] value
          color->blue=255;
        }
        if(color->alpha>255){
          if(przecinek.debug==true){
            printf("[pWindowDrawText() Warning]\n");
            printf("Value of color.alpha is too large!\n");
            printf("Value of color.alpha was changed from %i to 255,\n", color->alpha);
            fflush(stdout);
          }

          // Correct [color] [alpha] value
          color->alpha=255;
        }
      }

      if(wcscmp(text->value, code[text->ID-1].value)!=0){
        // Update [code] [value]
        wcscpy(code[text->ID-1].value, text->value);

        code[text->ID-1].change=true;
      }

      if(font->size!=view[font->ID-1].size){
        // Check [font] [size] value
        if(font->size<FONT_SIZE_MIN){
          if(przecinek.debug==true){
            printf("[pWindowDrawText() Warning]\n");
            printf("Value of font.size is too small!\n");
            printf("Value of font.size was changed from %i to %i,\n", font->size, FONT_SIZE_MIN);
            fflush(stdout);
          }

          // Change [font] [size] value
          font->size=FONT_SIZE_MIN;
        }
        else if(font->size>FONT_SIZE_MAX){
          if(przecinek.debug==true){
            printf("[pWindowDrawText() Warning]\n");
            printf("Value of font.size is too large!\n");
            printf("Value of font.size was changed from %i to %i,\n", font->size, FONT_SIZE_MAX);
            fflush(stdout);
          }

          // Change [font] [size] value
          font->size=FONT_SIZE_MAX;
        }

        // Refresh [view] values
        view[font->ID-1].size=font->size;
        view[font->ID-1].change=true;
      }

      if(wcscmp(font->directory, view[font->ID-1].directory)!=0){
        // Update [view] [directory] value
        wcscpy(view[font->ID-1].directory, font->directory);

        view[font->ID-1].change=true;
      }

      if(view[font->ID-1].lineSpacing!=font->lineSpacing ||
          view[font->ID-1].spaceSpacing!=font->spaceSpacing ||
          view[font->ID-1].letterSpacing!=font->letterSpacing){

        // Update [view] [lineSpacing] and [letterSpacing] values
        view[font->ID-1].letterSpacing=font->letterSpacing;
        view[font->ID-1].spaceSpacing=font->spaceSpacing;
        view[font->ID-1].lineSpacing=font->lineSpacing;

        view[font->ID-1].change=true;
      }

      if(code[text->ID-1].x[font->ID-1]!=text->x || code[text->ID-1].y[font->ID-1]!=text->y){
        // Update [code] [sourcePosition] values
        if(view[font->ID-1].change==false){
          for(unsigned short int current=0; current<wcslen(code[text->ID-1].value); current+=1){
            code[text->ID-1].sourcePosition[font->ID-1][current].x-=code[text->ID-1].x[font->ID-1]-text->x;
            code[text->ID-1].sourcePosition[font->ID-1][current].y-=code[text->ID-1].y[font->ID-1]-text->y;
          }
        }

        // Update [code] [x] and [y] values
        code[text->ID-1].x[font->ID-1]=text->x;
        code[text->ID-1].y[font->ID-1]=text->y;
      }

      if(view[font->ID-1].change==true){
        // Reset [view] objects
        GdipDeleteFontFamily(view[font->ID-1].fontFamily);
        GdipDeleteFont(view[font->ID-1].base);
        GdipDeletePrivateFontCollection(&view[font->ID-1].collection);

        // Check [view] [directory] value
        if(wcslen(view[font->ID-1].directory)>=4 &&
            (wcscmp(view[font->ID-1].directory+wcslen(view[font->ID-1].directory)-4, L".ttf")==0 ||
            wcscmp(view[font->ID-1].directory+wcslen(view[font->ID-1].directory)-4, L".otf")==0)){

          // Setup [view] [collection] and check if [directory] exists
          GdipNewPrivateFontCollection(&view[font->ID-1].collection);
          if(GdipPrivateAddFontFile(view[font->ID-1].collection, view[font->ID-1].directory)!=Ok){
            if(przecinek.debug==true){
              printf("[pFontCreate() Error]\n");
              printf("Font located in font.directory doesn't exist!\n");
              printf("Check if you gave the correct value,\n");

              printf("[pWindowDrawText() Error]\n");
              printf("Could not draw text!\n");
              printf("Given font wasn't initialized properly.\n");
              printf("Try to recreate it using pFontCreate(),\n");
              fflush(stdout);
            }

            // Reset [font]
            pDebugFontReset(font);
            font->ID=0;

            return;
          }
        }
        else{
          if(przecinek.debug==true){
            printf("[pFontCreate() Error]\n");
            printf("Value of font.directory doesn't include .ttf/.otf extenstion!\n");
            printf("Check if you gave the correct value,\n");

            printf("[pWindowDrawText() Error]\n");
            printf("Could not draw text!\n");
            printf("Given font wasn't initialized properly.\n");
            printf("Try to recreate it using pFontCreate(),\n");
            fflush(stdout);
          }

          // Reset [font]
          pDebugFontReset(font);
          font->ID=0;

          return;
        }

        // Get [familyCount] from [view] [collection]
        GdipGetFontCollectionFamilyCount(view[font->ID-1].collection, &familyCount);

        // Load all [family] names
        family=(GpFontFamily**)malloc(familyCount*sizeof(GpFontFamily*));
        GdipGetFontCollectionFamilyList(view[font->ID-1].collection, familyCount, family, &familyCount);

        // Check for [view] [collection] name
        for(unsigned short int current=0; current<familyCount; current+=1){
          memset(familyName, 0, sizeof(familyName));
          GdipGetFamilyName(family[current], familyName, 0);

          if(GdipCreateFontFamilyFromName(
            familyName, view[font->ID-1].collection, &view[font->ID-1].fontFamily)==Ok){

            break;
          }
          else if(current==familyCount-1){
            if(przecinek.debug==true){
              printf("[pFontCreate() Error]\n");
              printf("GDI+ library could not load font!\n");
              printf("Your font file might be corrupted,\n");

              printf("[pWindowDrawText() Error]\n");
              printf("Could not draw text!\n");
              printf("Given font wasn't initialized properly.\n");
              printf("Try to recreate it using pFontCreate(),\n");
              fflush(stdout);
            }

            // Reset [font]
            pDebugFontReset(font);
            font->ID=0;

            free(family);

            return;
          }
        }
        free(family);

        // Initialize [view] [base]
        GdipCreateFont(
          view[font->ID-1].fontFamily, view[font->ID-1].size*1.33,
          FontStyleRegular, UnitPixel, &view[font->ID-1].base
        );
      }

      if(view[font->ID-1].change==true || code[text->ID-1].change==true ||
          (code[text->ID-1].sourceSize[font->ID-1][0].width==0 &&
          code[text->ID-1].sourceSize[font->ID-1][0].height==0)){

        // Create [code] [source]
        pDebugTextSetup(text, font);

        view[font->ID-1].change=false;
        code[text->ID-1].change=false;
      }

      // Set current [build] [buffer]
      wglMakeCurrent(build[window->ID-1].hdc, build[window->ID-1].buffer);

      // Setup [build] [buffer]
      glEnable(GL_TEXTURE_2D);

      if(color!=NULL){
        glColor4f(
          (float)color->red/255, (float)color->green/255, (float)color->blue/255, (float)color->alpha/255
	      );
	    }
      else{
        glColor4f(
          (float)COLOR_DEFAULT_FOREGROUND.red/255, (float)COLOR_DEFAULT_FOREGROUND.green/255,
          (float)COLOR_DEFAULT_FOREGROUND.blue/255, (float)COLOR_DEFAULT_FOREGROUND.alpha/255
        );
	    }

      for(unsigned short int current=0; current<wcslen(text->value); current+=1){
        // Bind [code] [source]
        glBindTexture(GL_TEXTURE_2D, code[text->ID-1].source[font->ID-1][current]);

        // Draw on [build] [buffer]
        glBegin(GL_QUADS);

        glTexCoord2f((float)0, (float)0);
        glVertex2i(
          code[text->ID-1].sourcePosition[font->ID-1][current].x,
          code[text->ID-1].sourcePosition[font->ID-1][current].y
        );

        glTexCoord2f((float)0, (float)1);
        glVertex2i(
          code[text->ID-1].sourcePosition[font->ID-1][current].x,
          code[text->ID-1].sourcePosition[font->ID-1][current].y+
          code[text->ID-1].sourceSize[font->ID-1][current].height
        );

        glTexCoord2f((float)1, (float)1);
        glVertex2i(
          code[text->ID-1].sourcePosition[font->ID-1][current].x+
          code[text->ID-1].sourceSize[font->ID-1][current].width,
          code[text->ID-1].sourcePosition[font->ID-1][current].y+
          code[text->ID-1].sourceSize[font->ID-1][current].height
        );

        glTexCoord2f((float)1, (float)0);
        glVertex2i(
          code[text->ID-1].sourcePosition[font->ID-1][current].x+
          code[text->ID-1].sourceSize[font->ID-1][current].width,
          code[text->ID-1].sourcePosition[font->ID-1][current].y
        );

        glEnd();
      }

      glDisable(GL_TEXTURE_2D);
    }
    else if(przecinek.debug==true){
      if(font->ID==0){
        printf("[pWindowDrawText() Error]\n");
        printf("Could not draw text!\n");
        printf("Given font wasn't initialized properly.\n");
        printf("Try to recreate it using pFontCreate(),\n");
      }
      if(text->ID==0){
        printf("[pWindowDrawText() Error]\n");
        printf("Could not draw text!\n");
        printf("It wasn't initialized properly.\n");
        printf("Try to recreate it using pTextCreate(),\n");
      }
      fflush(stdout);
    }
  }
  else if(przecinek.debug==true){
    printf("[pWindowDrawText() Error]\n");
    printf("Could not draw text on window!\n");
    printf("Window wasn't initialized properly.\n");
    printf("Try to recreate it using pWindowCreate(),\n");
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pWindowClear()
 * | .     | In: pWindow* [window], s_int [x], [y], us_int [width],
 * |     . |     [height], pColor* [color], pImage* [image]
 * \ = , = / Out:
 *
 * This function clears [window] with given [color] with/or [image].
 * Cleared area depends on given position and size values.
 ****************************************************************/
void pWindowClear(
  pWindow *window, short int x, short int y, unsigned short int width,
  unsigned short int height, pColor *color, pImage *image
){

  if(window==NULL){
    if(przecinek.debug==true){
      printf("[pWindowClear() Error]\n");
      printf("Given window is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }

  if(window->ID!=0){
    if(image==NULL || (image!=NULL && image->ID!=0)){
      if(color!=NULL){
        // Check [color] values
        if(color->red>255){
          if(przecinek.debug==true){
            printf("[pWindowClear() Warning]\n");
            printf("Value of color.red is too large!\n");
            printf("Value of color.red was changed from %i to 255,\n", color->red);
            fflush(stdout);
          }

          // Correct [color] [red] value
          color->red=255;
        }
        if(color->green>255){
          if(przecinek.debug==true){
            printf("[pWindowClear() Warning]\n");
            printf("Value of color.green is too large!\n");
            printf("Value of color.green was changed from %i to 255,\n", color->green);
            fflush(stdout);
          }

          // Correct [color] [green] value
          color->green=255;
        }
        if(color->blue>255){
          if(przecinek.debug==true){
            printf("[pWindowClear() Warning]\n");
            printf("Value of color.blue is too large!\n");
            printf("Value of color.blue was changed from %i to 255,\n", color->blue);
            fflush(stdout);
          }

          // Correct [color] [blue] value
          color->blue=255;
        }
        if(color->alpha>255){
          if(przecinek.debug==true){
            printf("[pWindowClear() Warning]\n");
            printf("Value of color.alpha is too large!\n");
            printf("Value of color.alpha was changed from %i to 255,\n", color->alpha);
            fflush(stdout);
          }

          // Correct [color] [alpha] value
          color->alpha=255;
        }
      }

      if(image!=NULL){
        if(wcscmp(image->directory, texture[image->ID-1].directory)!=0){
          // Update [texture] [directory] values
          wcscpy(texture[image->ID-1].directory, image->directory);

          // Check [directory] value
          if((wcslen(image->directory)>=4 &&
              (wcscmp(image->directory+wcslen(image->directory)-4, L".png")==0 ||
               wcscmp(image->directory+wcslen(image->directory)-4, L".jpg")==0)) ||
              (wcslen(image->directory)>=5 &&
              wcscmp(image->directory+wcslen(image->directory)-5, L".jpeg")==0)){

            // Check if [texture] [directory] exists
            if(GdipCreateBitmapFromFile(texture[image->ID-1].directory, &imageBuffer)!=Ok){
              if(przecinek.debug==true){
                printf("[pWindowDrawObject() Error]\n");
                printf("GDI+ library could not load texture!\n");
                printf(
                  "Check if you gave the correct value of texture.directory and if your file isn't corruped,\n"
                );

                printf("[pWindowDrawObject() Error]\n");
                printf("Could not draw image!\n");
                printf("It wasn't initialized properly.\n");
                printf("Try to recreate it using pImageCreate(),\n");
                fflush(stdout);
              }

              // Reset [image]
              pDebugImageReset(image);
              image->ID=0;

              return;
            }
          }
          else{
            if(przecinek.debug==true){
              printf("[pWindowDrawObject() Error]\n");
              printf("Value of image.directory doesn't include .png/.jpg/.jpeg extenstion!\n");
              printf("Check if you gave the correct value,\n");

              printf("[pWindowDrawObject() Error]\n");
              printf("Could not draw image!\n");
              printf("It wasn't initialized properly.\n");
              printf("Try to recreate it using pImageCreate(),\n");
              fflush(stdout);
            }

            // Reset [image]
            pDebugImageReset(image);
            image->ID=0;

            return;
          }

          // Load [texture] [source]
          pDebugImageSetup(image);
        }
      }

      // Set current [build] [buffer]
      wglMakeCurrent(build[window->ID-1].hdc, build[window->ID-1].buffer);

      // Setup [build] [buffer]
      if(color==NULL){
        if(image==NULL){
          glColor4f(
            (float)COLOR_DEFAULT_BACKGROUND.red/255, (float)COLOR_DEFAULT_BACKGROUND.green/255,
            (float)COLOR_DEFAULT_BACKGROUND.blue/255, (float)COLOR_DEFAULT_BACKGROUND.alpha/255
          );
        }
        else{ glColor4f((float)1, (float)1, (float)1, (float)1); }
      }
      else{
        glColor4f(
          (float)color->red/255, (float)color->green/255, (float)color->blue/255, (float)color->alpha/255
        );
      }

      if(image!=NULL){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[image->ID-1].source);
      }

      // Draw on [build] [buffer]
      glBegin(GL_QUADS);

      glTexCoord2f((float)0, (float)0);
      glVertex2i(x, y);

      glTexCoord2f((float)0, (float)1);
      glVertex2i(x, y+height);

      glTexCoord2f((float)1, (float)1);
      glVertex2i(x+width, y+height);

      glTexCoord2f((float)1, (float)0);
      glVertex2i(x+width, y);

      glEnd();

      glDisable(GL_TEXTURE_2D);
    }
    else if(przecinek.debug==true){
      printf("[pWindowClear() Error]\n");
      printf("Could not draw image!\n");
      printf("It wasn't initialized properly.\n");
      printf("Try to recreate it using pImageCreate(),\n");
    }
  }
  else if(przecinek.debug==true){
    printf("[pWindowClear() Error]\n");
    printf("Could not clear window!\n");
    printf("Window wasn't initialized properly.\n");
    printf("Try to recreate it using pWindowCreate(),\n");
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pWindowHandle()
 * | .     |
 * |     . | In: pWindow* [window]
 * \ = , = / Out:
 *
 * This function handles every global action.
 * It checks for any [window] messages. It switches buffers.
 * It updates [key] values. It updates mouse position,
 * display size and window count for [przecinek]. It updates
 * many [window] values. It checks if [window] is fullscreen,
 * if it changed its size or position, focus or title, etc.
 * It also updates frame count.
 ****************************************************************/
void pWindowHandle(pWindow *window){
  if(window==NULL){
    if(przecinek.debug==true){
      printf("[pWindowHandle() Error]\n");
      printf("Given window is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }

  if(window->ID!=0){
    // Manage console
    if(GetConsoleWindow()!=NULL && przecinek.debug==false){ FreeConsole(); }

    // Update [windowMainID]
    if(build[windowMainID].exist==false){
      for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
        if(build[current].exist==true){
          windowMainID=current;
          break;
        }
      }
    }

    // Set current [build] [buffer]
    wglMakeCurrent(build[window->ID-1].hdc, build[window->ID-1].buffer);

    if((window->ID-1)==windowMainID){
      // Change [przecinek] [key] values from `1` to `2`
      for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
        if(input[button]!=0){ przecinek.key[input[button]]=2; }
        else{ break; }
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
      przecinek.windowCount=windowCount;

      // Calculate [window] offset
      GetWindowRect(build[window->ID-1].hwnd, &rectangle);
      GetClientRect(build[window->ID-1].hwnd, &fix);

      // Update [build] [widthFix] and [heightFix] values
      if(build[window->ID-1].fullScreen==false){
        build[window->ID-1].widthFix=(rectangle.right-rectangle.left)-(fix.right-fix.left);
        build[window->ID-1].heightFix=(rectangle.bottom-rectangle.top)-(fix.bottom-fix.top);
      }

      // Update [window] [resizable]
      window->resizable=build[window->ID-1].resizable;

      // Update [przecinek] [keyCaps]
      przecinek.keyCaps=GetKeyState(VK_CAPITAL)&0x0001;
    }

    // Manage pending [message]
    while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    // Manage close [message]
    if(build[window->ID-1].DESTROY==true){
      // Reset [window]
      pDebugWindowReset(window);
      window->ID=0;

      return;
    }

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

      // Update [build] [buffer]
      glViewport(0, 0, build[window->ID-1].width, build[window->ID-1].height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0, build[window->ID-1].width, build[window->ID-1].height, 0, (-1), 1);

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
      if(window->x<0){ build[window->ID-1].x+=USHRT_MAX; }
      if(window->y<0){ build[window->ID-1].y+=USHRT_MAX; }

      // Set [build] [style]
      if(build[window->ID-1].resizable==true){ build[window->ID-1].style=WS_OVERLAPPEDWINDOW | WS_VISIBLE; }
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

      // Update [build] [buffer]
      glViewport(0, 0, build[window->ID-1].width, build[window->ID-1].height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0, build[window->ID-1].width, build[window->ID-1].height, 0, (-1), 1);

      build[window->ID-1].fullScreen=false;
    }

    if(window->fullScreen==false){
      // Manage position change [message]
      if(window->x!=build[window->ID-1].x || window->y!=build[window->ID-1].y){
        // Update [build] position parameters
        build[window->ID-1].x=window->x;
        build[window->ID-1].y=window->y;

        // Change [przecinek] [key] values to `0`
        for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
          if(input[button]!=0){
            przecinek.key[input[button]]=0;
            input[button]=0;
          }
          else{ break; }
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

        // Change [przecinek] [key] values to `0`
        for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
          if(input[button]!=0){
            przecinek.key[input[button]]=0;
            input[button]=0;
          }
          else{ break; }
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
            printf("[pWindowHandle() Warning]\n");
            printf("Value of window.width is too small!\n");
            printf("Value of window.width was changed from %i to %i,\n", window->width, WINDOW_WIDTH_MIN);
            fflush(stdout);
          }

          // Change [window] [width] value
          window->width=WINDOW_WIDTH_MIN;
        }
        else if(window->width>WINDOW_WIDTH_MAX){
          if(przecinek.debug==true){
            printf("[pWindowHandle() Warning]\n");
            printf("Value of window.width is too large!\n");
            printf("Value of window.width was changed from %i to %i,\n", window->width, WINDOW_WIDTH_MAX);
            fflush(stdout);
          }

          // Change [window] [width] value
          window->width=WINDOW_WIDTH_MAX;
        }

        // Check [window] [height] value
        if(window->height<WINDOW_HEIGHT_MIN){
          if(przecinek.debug==true){
            printf("[pWindowHandle() Warning]\n");
            printf("Value of window.height is too small!\n");
            printf("Value of window.height was changed from %i to %i,\n", window->height, WINDOW_HEIGHT_MIN);
            fflush(stdout);
          }

          // Change [window] [height] value
          window->height=WINDOW_HEIGHT_MIN;
        }
        else if(window->height>WINDOW_HEIGHT_MAX){
          if(przecinek.debug==true){
            printf("[pWindowHandle() Warning]\n");
            printf("Value of window.height is too large!\n");
            printf("Value of window.height was changed from %i to %i,\n", window->height, WINDOW_HEIGHT_MAX);
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

        // Change [przecinek] [key] values to `0`
        for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
          if(input[button]!=0){
            przecinek.key[input[button]]=0;
            input[button]=0;
          }
          else{ break; }
        }

        // Update [build] [buffer]
        glViewport(0, 0, build[window->ID-1].width, build[window->ID-1].height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, build[window->ID-1].width, build[window->ID-1].height, 0, (-1), 1);

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

        // Change [przecinek] [key] values to `0`
        for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
          if(input[button]!=0){
            przecinek.key[input[button]]=0;
            input[button]=0;
          }
          else{ break; }
        }

        // Update [build] [buffer]
        glViewport(0, 0, build[window->ID-1].width, build[window->ID-1].height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, build[window->ID-1].width, build[window->ID-1].height, 0, (-1), 1);

        // Reset [build] [message] values
        build[window->ID-1].SIZE.width=0;
        build[window->ID-1].SIZE.height=0;
      }
    }

    // Manage focus in [message]
    if(build[window->ID-1].FOCUSIN==true){
      build[window->ID-1].focus=true;

      for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
        if(current==window->ID-1){ continue; }

        if(build[current].exist==true){
          if(build[current].focus==true){ build[current].focus=false; }
        }
        else{ break; }
      }

      // Reset [build] [message] values
      build[window->ID-1].FOCUSIN=false;
    }

    // Manage focus out [message]
    if(build[window->ID-1].FOCUSOUT==true){
      build[window->ID-1].focus=false;

      // Reset [build] [message] values
      build[window->ID-1].FOCUSOUT=false;
    }

    // Update [window] [focus]
    window->focus=build[window->ID-1].focus;

    if(wcscmp(window->title, build[window->ID-1].title)!=0){
      // Update [build] [title] value
      wcscpy(build[window->ID-1].title, window->title);

      // Change [window] [title]
      SetWindowTextW(build[window->ID-1].hwnd, window->title);
    }

    if(window->resizable==true){
      // Update [build] size limit values
      build[window->ID-1].widthMin=window->widthMin;
      build[window->ID-1].heightMin=window->heightMin;
      build[window->ID-1].widthMax=window->widthMax;
      build[window->ID-1].heightMax=window->heightMax;

      // Check [window] [widthMin] and [widthMax] values
      if(build[window->ID-1].widthMin>build[window->ID-1].widthMax){
        if(przecinek.debug==true){
          printf("[pWindowHandle() Warning]\n");
          printf("Value of window.widthMin is larger than window.widthMax!\n");
          printf(
            "Value of window.widthMin was changed from %i to %i,\n",
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
          printf("[pWindowHandle() Warning]\n");
          printf("Value of window.heightMin is larger than window.heightMax!\n");
          printf(
            "Value of window.heightMin was changed from %i to %i,\n",
            window->heightMin, window->heightMax
          );
          fflush(stdout);
        }

        // Change [window] and [build] [heightMin] value
        window->heightMin=window->heightMax;
        build[window->ID-1].heightMin=build[window->ID-1].heightMax;
      }

      if(build[window->ID-1].widthMin>build[window->ID-1].width ||
          build[window->ID-1].widthMax<build[window->ID-1].width ||
          build[window->ID-1].heightMin>build[window->ID-1].height ||
          build[window->ID-1].heightMax<build[window->ID-1].height){

        // Correct [window] [width] value
        if(build[window->ID-1].widthMin>build[window->ID-1].width){
          build[window->ID-1].width=build[window->ID-1].widthMin;
          window->width=build[window->ID-1].widthMin;
        }
        else if(build[window->ID-1].widthMax<build[window->ID-1].width){
          build[window->ID-1].width=build[window->ID-1].widthMax;
          window->width=build[window->ID-1].widthMax;
        }

        // Correct [window] [height] value
        if(build[window->ID-1].heightMin>build[window->ID-1].height){
          build[window->ID-1].height=build[window->ID-1].heightMin;
          window->height=build[window->ID-1].heightMin;
        }
        else if(build[window->ID-1].heightMax<build[window->ID-1].height){
          build[window->ID-1].height=build[window->ID-1].heightMax;
          window->height=build[window->ID-1].heightMax;
        }

        // Resize [window]
        SetWindowPos(
          build[window->ID-1].hwnd, NULL, 0, 0,
          window->width, window->height, SWP_NOZORDER | SWP_NOMOVE
        );

        // Change [przecinek] [key] values to `0`
        for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
          if(input[button]!=0){
            przecinek.key[input[button]]=0;
            input[button]=0;
          }
          else{ break; }
        }

        // Update [build] [buffer]
        glViewport(0, 0, build[window->ID-1].width, build[window->ID-1].height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, build[window->ID-1].width, build[window->ID-1].height, 0, (-1), 1);

        // Reset [build] [message] values
        build[window->ID-1].SIZE.width=0;
        build[window->ID-1].SIZE.height=0;
      }
    }

    // Switch [window] buffers
    SwapBuffers(build[window->ID-1].hdc);

    // Refresh [window]
    InvalidateRect(build[window->ID-1].hwnd, NULL, TRUE);

    // Check [przecinek] [frameLimit] value
    if(przecinek.frameLimit<PRZECINEK_FRAME_MIN){
      if(przecinek.debug==true){
        printf("[pWindowHandle() Warning]\n");
        printf("Value of przecinek.frameLimit is too small!\n");
        printf(
          "Value of przecinek.frameLimit was changed from %i to %i,\n",
          przecinek.frameLimit, PRZECINEK_FRAME_MIN
        );
        fflush(stdout);
      }

      przecinek.frameLimit=PRZECINEK_FRAME_MIN;
    }
    else if(przecinek.frameLimit>PRZECINEK_FRAME_MAX){
      if(przecinek.debug==true){
        printf("[pWindowHandle() Warning]\n");
        printf("Value of przecinek.frameLimit is too large!\n");
        printf(
          "Value of przecinek.frameLimit was changed from %i to %i,\n",
          przecinek.frameLimit, PRZECINEK_FRAME_MAX
        );
        fflush(stdout);
      }

      przecinek.frameLimit=PRZECINEK_FRAME_MAX;
    }

    // Set [currentFrameLimit] first value
    if((window->ID-1)==windowMainID && window->frameCount==0){ currentFrameLimit=przecinek.frameLimit; }

    // Update [frameCount] and sleep
    build[window->ID-1].frameCount+=1;
    Sleep((1000/windowCount)/currentFrameLimit);

    // Calculate [frameOverHead] value
    if(build[window->ID-1].frameCount>=przecinek.frameLimit &&
        GetTickCount()-build[window->ID-1].frameStart<1000){

      frameOverHead+=1;
    }
    else if(GetTickCount()-build[window->ID-1].frameStart>=1000 &&
        build[window->ID-1].frameCount<przecinek.frameLimit-3){

      frameOverHead=build[window->ID-1].frameCount-przecinek.frameLimit;
    }

    if(GetTickCount()-build[window->ID-1].frameStart>=1000){
      // Set [window] [frameCount] and reset loop
      window->frameCount=build[window->ID-1].frameCount;

      build[window->ID-1].frameCount=0;
      build[window->ID-1].frameStart=GetTickCount();

      if((window->ID-1)==windowMainID){
        // Correct [currentFrameLimit] value
        currentFrameLimit-=frameOverHead;
        if(currentFrameLimit>=przecinek.frameLimit*3){ currentFrameLimit=przecinek.frameLimit*3; }

        // Reset [frameOverHead] value
        frameOverHead=0;
      }
    }
  }
  else if(przecinek.debug==true){
    printf("[pWindowHandle() Error]\n");
    printf("Could not handle window!\n");
    printf("Window wasn't initialized properly.\n");
    printf("Try to recreate it using pWindowCreate(),\n");
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pWindowDestroy()
 * | .     |
 * |     . | In: pWindow* [window]
 * \ = , = / Out:
 *
 * This function destroys given [window].
 * It sends destroy signal, which activates after next
 * [window] handle function usage.
 * It also resets [window] `ID` to `0`.
 ****************************************************************/
void pWindowDestroy(pWindow* window){
  if(window==NULL){
    if(przecinek.debug==true){
      printf("[pWindowDestroy() Error]\n");
      printf("Given window is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }

  if(window->ID!=0){
    // Send kill event
    PostMessage(build[window->ID-1].hwnd, WM_CLOSE, 0, 0);
  }
  else if(przecinek.debug==true){
    printf("[pWindowDestroy() Warning]\n");
    printf("Could not destroy window!\n");
    printf("It's already closed,\n");
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pObjectCreate()
 * | .     |
 * |     . | In: us_int [width], [height]
 * \ = , = / Out: pObject
 *
 * This function creates [object].
 * It fills all [object] variables.
 * Created [object] depends on [vertice] count.
 ****************************************************************/
pObject pObjectCreate(unsigned short int vertice, unsigned short int width, unsigned short int height){
  // Create local [object]
  pObject object;

  for(unsigned short int current=0; current<FONT_MAX; current+=1){
    if(figure[current].exist==false){
      // Set [object] [ID] and reset it
      object.ID=current+1;
      pDebugObjectReset(&object);

      break;
    }
    else if(current==OBJECT_MAX-1){
      if(przecinek.debug==true){
        printf("[pObjectCreate() Error]\n");
        printf("Too many objects were created!\n");
        printf("Current object limit is equal to %i.\n", OBJECT_MAX);
        printf("Try to destroy unused objects or change Przecinek object limit,\n");
        fflush(stdout);
      }

      // Return [object]
      object.ID=0;

      return object;
    }
  }

  // Check if Przecinek is initialized
  if(setup==false){
    printf("[pObjectCreate() Error]\n");
    printf("Could not create object!\n");
    printf("Przecinek is not initialized.\n");
    printf("Try to run pSetup() first,\n");
    fflush(stdout);

    // Reset and return [object]
    pDebugObjectReset(&object);
    object.ID=0;

    return object;
  }

  // Check [vertice] value
  if(vertice<OBJECT_VERTICE_MIN){
    if(przecinek.debug==true){
      printf("[pObjectCreate() Warning]\n");
      printf("Value of object.vertice is too small!\n");
      printf("Value of object.vertice was changed from %i to %i,\n", vertice, OBJECT_VERTICE_MIN);
      fflush(stdout);
    }

    // Change [vertice] value
    vertice=OBJECT_VERTICE_MIN;
  }
  else if(vertice>OBJECT_VERTICE_MAX){
    if(przecinek.debug==true){
      printf("[pObjectCreate() Warning]\n");
      printf("Value of object.vertice is too large!\n");
      printf("Value of object.vertice was changed from %i to %i,\n", vertice, OBJECT_VERTICE_MAX);
      fflush(stdout);
    }

    // Change [vertice] value
    vertice=OBJECT_VERTICE_MAX;
  }

  // Set [object] values
  object.width=width;
  object.height=height;
  object.vertice=vertice;
  object.rotation=0;

  // Set [figure] values
  figure[object.ID-1].exist=true;

  figure[object.ID-1].width=width;
  figure[object.ID-1].height=height;
  figure[object.ID-1].vertice=vertice;

  // Set [figure] [rotationFix] value
  if(vertice==4){ figure[object.ID-1].rotationFix=45; }
  else if(vertice%2!=0){ figure[object.ID-1].rotationFix=270; }
  else{ figure[object.ID-1].rotationFix=0; }

  // Calculate all [figure] values
  pDebugObjectCalculate(&object);

  // Return local [object]
  return object;
}

/****************************************************************
 * |\_____/| pObjectCollision()
 * | .     |
 * |     . | In: pObject* [object1], [object2]
 * \ = , = / Out: bool
 *
 * This function checks if two [object] collides.
 * It simulates triangular collisions between several points.
 * Then it returns value based on earlier calculations.
 ****************************************************************/
bool pObjectCollision(pObject *object1, pObject *object2){
  if(object1==NULL || object2==NULL){
    if(przecinek.debug==true){
      printf("[pObjectCollision() Error]\n");
      printf("Given object is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    // Could not check collsion, return `false`
    return false;
  }

  // Simple collision check on two squares
  if(figure[object1->ID-1].vertice==4 && figure[object2->ID-1].vertice==4 &&
      figure[object1->ID-1].rotation==0 && figure[object2->ID-1].rotation==0){

    // Return this junk of code
    return(
      object1->x<object2->x+object2->width &&
      object1->x+object1->width>object2->x &&
      object1->y<object2->y+object2->height &&
      object1->y+object1->height>object2->y
    );
  }

  // Complex collision based on vertices of both figures
  else if((object1->x<object2->x+object2->width) &&
      (object1->x+object1->width>object2->x) &&
      (object1->y<object2->y+object2->height) &&
      (object1->y+object1->height>object2->y)==true){

    // Reset some variables
    distanceMinX=SHRT_MAX;
    distanceMinY=SHRT_MAX;
    distanceMaxX=SHRT_MIN;
    distanceMaxY=SHRT_MIN;
    distance=1000000;

    // Calculate [object1] [centerA]
    centerA.x=figure[object1->ID-1].x+(figure[object1->ID-1].width/2);
    centerA.y=figure[object1->ID-1].y+(figure[object1->ID-1].height/2);

    // Calculate [object2] [centerB]
    centerB.x=figure[object2->ID-1].x+(figure[object2->ID-1].width/2);
    centerB.y=figure[object2->ID-1].y+(figure[object2->ID-1].height/2);

    for(unsigned short int current=0; current<figure[object1->ID-1].vertice; current+=1){
      // Calculate [distanceMinX] and [distanceMinY] values
      if(distanceMinX>figure[object1->ID-1].point[current].X){
        distanceMinX=figure[object1->ID-1].point[current].X;
      }
      if(distanceMinY>figure[object1->ID-1].point[current].Y){
        distanceMinY=figure[object1->ID-1].point[current].Y;
      }

      // Calculate [distanceMaxX] and [distanceMaxY] values
      if(distanceMaxX<figure[object1->ID-1].point[current].X){
        distanceMaxX=figure[object1->ID-1].point[current].X;
      }
      if(distanceMinY<figure[object1->ID-1].point[current].Y){
        distanceMaxY=figure[object1->ID-1].point[current].Y;
      }

      // Calculate [ratio] value
      ratio=sqrt(pow((centerB.x-figure[object1->ID-1].point[current].X), 2)+
        pow((centerB.y-figure[object1->ID-1].point[current].Y), 2));

      // Update [distance] value
      if(ratio<distance){
        distance=ratio;
        pointA[0]=current;
      }
    }

    // Calculate [ratio] value
    ratio=(figure[object1->ID-1].vertice/15)+1;

    // Calculate [pointA] `1` and `2`
    pointA[1]=pointA[0]-ratio;
    pointA[2]=pointA[0]+ratio;

    if(pointA[1]<0){ pointA[1]+=figure[object1->ID-1].vertice; }
    if(pointA[1]>figure[object1->ID-1].vertice-1){ pointA[1]-=figure[object1->ID-1].vertice; }

    if(pointA[2]<0){ pointA[2]+=figure[object1->ID-1].vertice; }
    if(pointA[2]>figure[object1->ID-1].vertice-1){ pointA[2]-=figure[object1->ID-1].vertice; }

    // Calculate [pointA] `3` and `4`
    if(figure[object1->ID-1].vertice>=10){
      pointA[3]=pointA[0]-(2*ratio);
      pointA[4]=pointA[0]+(2*ratio);

      if(pointA[3]<0){ pointA[3]+=figure[object1->ID-1].vertice; }
      if(pointA[3]>figure[object1->ID-1].vertice-1){ pointA[3]-=figure[object1->ID-1].vertice; }

      if(pointA[4]<0){ pointA[4]+=figure[object1->ID-1].vertice; }
      if(pointA[4]>figure[object1->ID-1].vertice-1){ pointA[4]-=figure[object1->ID-1].vertice; }
    }

    // Reset some variables
    distanceMinX=SHRT_MAX;
    distanceMinY=SHRT_MAX;
    distanceMaxX=SHRT_MIN;
    distanceMaxY=SHRT_MIN;
    distance=1000000;

    for(unsigned short int current=0; current<figure[object2->ID-1].vertice; current+=1){
      // Calculate [distanceMinX] and [distanceMinY] values
      if(distanceMinX>figure[object1->ID-1].point[current].X){
        distanceMinX=figure[object1->ID-1].point[current].X;
      }
      if(distanceMinY>figure[object1->ID-1].point[current].Y){
        distanceMinY=figure[object1->ID-1].point[current].Y;
      }

      // Calculate [distanceMaxX] and [distanceMaxY] values
      if(distanceMaxX<figure[object1->ID-1].point[current].X){
        distanceMaxX=figure[object1->ID-1].point[current].X;
      }
      if(distanceMinY<figure[object1->ID-1].point[current].Y){
        distanceMaxY=figure[object1->ID-1].point[current].Y;
      }

      // Calculate [ratio] value
      ratio=sqrt(pow((centerA.x-figure[object2->ID-1].point[current].X), 2)+
        pow((centerA.y-figure[object2->ID-1].point[current].Y), 2));

      // Update [distance] value
      if(ratio<distance){
        distance=ratio;
        pointB[0]=current;
      }
    }

    // Calculate [ratio] value
    ratio=(figure[object2->ID-1].vertice/15)+1;

    // Calculate [pointB] `1` and `2`
    pointB[1]=pointB[0]-ratio;
    pointB[2]=pointB[0]+ratio;

    if(pointB[1]<0){ pointB[1]+=figure[object2->ID-1].vertice; }
    if(pointB[1]>object2->vertice-1){ pointB[1]-=figure[object2->ID-1].vertice; }

    if(pointB[2]<0){ pointB[2]+=figure[object2->ID-1].vertice; }
    if(pointB[2]>figure[object2->ID-1].vertice-1){ pointB[2]-=figure[object2->ID-1].vertice; }

    // Calculate [pointB] `3` and `4`
    if(figure[object2->ID-1].vertice>=10){
      pointB[3]=pointB[0]-(2*ratio);
      pointB[4]=pointB[0]+(2*ratio);

      if(pointB[3]<0){ pointB[3]+=figure[object2->ID-1].vertice; }
      if(pointB[3]>figure[object2->ID-1].vertice-1){ pointB[3]-=figure[object2->ID-1].vertice; }

      if(pointB[4]<0){ pointB[4]+=figure[object2->ID-1].vertice; }
      if(pointB[4]>figure[object2->ID-1].vertice-1){ pointB[4]-=figure[object2->ID-1].vertice; }
    }

    // Check for collision X.X
    if(
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].X, figure[object1->ID-1].point[pointA[0]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[1]].X, figure[object1->ID-1].point[pointA[1]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].X, figure[object2->ID-1].point[pointB[0]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[1]].X, figure[object2->ID-1].point[pointB[1]].Y },
  centerB)==false &&
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].X, figure[object1->ID-1].point[pointA[0]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[1]].X, figure[object1->ID-1].point[pointA[1]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].X, figure[object2->ID-1].point[pointB[0]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[2]].X, figure[object2->ID-1].point[pointB[2]].Y },
  centerB)==false &&
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].X, figure[object1->ID-1].point[pointA[0]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[2]].X, figure[object1->ID-1].point[pointA[2]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].X, figure[object2->ID-1].point[pointB[0]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[1]].X, figure[object2->ID-1].point[pointB[1]].Y },
  centerB)==false &&
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].X, figure[object1->ID-1].point[pointA[0]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[2]].X, figure[object1->ID-1].point[pointA[2]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].X, figure[object2->ID-1].point[pointB[0]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[2]].X, figure[object2->ID-1].point[pointB[2]].Y },
  centerB)==false
    ){

      if(figure[object2->ID-1].vertice>=10){
        if(
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].X, figure[object1->ID-1].point[pointA[0]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[1]].X, figure[object1->ID-1].point[pointA[1]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[1]].X, figure[object2->ID-1].point[pointB[1]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[3]].X, figure[object2->ID-1].point[pointB[3]].Y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].X, figure[object1->ID-1].point[pointA[0]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[1]].X, figure[object1->ID-1].point[pointA[1]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[2]].X, figure[object2->ID-1].point[pointB[2]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[4]].X, figure[object2->ID-1].point[pointB[4]].Y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].X, figure[object1->ID-1].point[pointA[0]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[2]].X, figure[object1->ID-1].point[pointA[2]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[1]].X, figure[object2->ID-1].point[pointB[1]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[3]].X, figure[object2->ID-1].point[pointB[3]].Y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].X, figure[object1->ID-1].point[pointA[0]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[2]].X, figure[object1->ID-1].point[pointA[2]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[2]].X, figure[object2->ID-1].point[pointB[2]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[4]].X, figure[object2->ID-1].point[pointB[4]].Y },
  centerB)==true
        ){

          // Collision found, return `true`
          return true;
        }
      }
      if(figure[object1->ID-1].vertice>=10){
        if(
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[1]].X, figure[object1->ID-1].point[pointA[1]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[3]].X, figure[object1->ID-1].point[pointA[3]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].X, figure[object2->ID-1].point[pointB[0]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[1]].X, figure[object2->ID-1].point[pointB[1]].Y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[1]].X, figure[object1->ID-1].point[pointA[1]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[3]].X, figure[object1->ID-1].point[pointA[3]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].X, figure[object2->ID-1].point[pointB[0]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[2]].X, figure[object2->ID-1].point[pointB[2]].Y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[2]].X, figure[object1->ID-1].point[pointA[2]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[4]].X, figure[object1->ID-1].point[pointA[4]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].X, figure[object2->ID-1].point[pointB[0]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[1]].X, figure[object2->ID-1].point[pointB[1]].Y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[2]].X, figure[object1->ID-1].point[pointA[2]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[4]].X, figure[object1->ID-1].point[pointA[4]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].X, figure[object2->ID-1].point[pointB[0]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[2]].X, figure[object2->ID-1].point[pointB[2]].Y },
  centerB)==true
        ){

          // Collision found, return `true`
          return true;
        }
      }
      if(figure[object1->ID-1].vertice>=10 && figure[object2->ID-1].vertice>=10){
        if(
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[1]].X, figure[object1->ID-1].point[pointA[1]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[3]].X, figure[object1->ID-1].point[pointA[3]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[1]].X, figure[object2->ID-1].point[pointB[1]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[3]].X, figure[object2->ID-1].point[pointB[3]].Y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[1]].X, figure[object1->ID-1].point[pointA[1]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[3]].X, figure[object1->ID-1].point[pointA[3]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[2]].X, figure[object2->ID-1].point[pointB[2]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[4]].X, figure[object2->ID-1].point[pointB[4]].Y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[2]].X, figure[object1->ID-1].point[pointA[2]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[4]].X, figure[object1->ID-1].point[pointA[4]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[1]].X, figure[object2->ID-1].point[pointB[1]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[3]].X, figure[object2->ID-1].point[pointB[3]].Y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[2]].X, figure[object1->ID-1].point[pointA[2]].Y },
  (pPosition){ figure[object1->ID-1].point[pointA[4]].X, figure[object1->ID-1].point[pointA[4]].Y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[2]].X, figure[object2->ID-1].point[pointB[2]].Y },
  (pPosition){ figure[object2->ID-1].point[pointB[4]].X, figure[object2->ID-1].point[pointB[4]].Y },
  centerB)==true
        ){

          // Collision found, return `true`
          return true;
        }
      }
    }
    else{
      // Collision found, return `true`
      return true;
    }
  }

  // There is no collision, return `false`
  return false;
}

/****************************************************************
 * |\_____/| pObjectDestroy()
 * | .     |
 * |     . | In: pObject* [object]
 * \ = , = / Out:
 *
 * This function destroys given [object].
 * It also resets [object] `ID` to `0`.
 ****************************************************************/
void pObjectDestroy(pObject *object){
  if(object==NULL){
    if(przecinek.debug==true){
      printf("[pObjectDestroy() Error]\n");
      printf("Given object is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }

  if(object->ID!=0){
    // Fully reset [object]
    pDebugObjectReset(object);
    object->ID=0;
  }
  else if(przecinek.debug==true){
    printf("[pObjectDestroy() Warning]\n");
    printf("Could not destroy object!\n");
    printf("It's already closed,\n");
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pFontCreate()
 * | .     |
 * |     . | In: wchar_t* [directory], us_int [size]
 * \ = , = / Out: pFont
 *
 * This function creates [font] object. It sets [ID]
 * for local [font]. It checks if [size] value is valid.
 * It checks if [directory] exists.
 * It loads [font] and saves it to memory.
 ****************************************************************/
pFont pFontCreate(wchar_t *directory, unsigned short int size){
  // Create local [font]
  pFont font;

  for(unsigned short int current=0; current<FONT_MAX; current+=1){
    if(view[current].exist==false){
      // Set [font] [ID] and reset it
      font.ID=current+1;
      pDebugFontReset(&font);

      break;
    }
    else if(current==FONT_MAX-1){
      if(przecinek.debug==true){
        printf("[pFontCreate() Error]\n");
        printf("Too many fonts were created!\n");
        printf("Current font limit is equal to %i.\n", FONT_MAX);
        printf("Try to destroy unused fonts or change Przecinek font limit,\n");
        fflush(stdout);
      }

      // Return [font]
      font.ID=0;

      return font;
    }
  }

  // Check if Przecinek is initialized
  if(setup==false){
    printf("[pFontCreate() Error]\n");
    printf("Could not create font!\n");
    printf("Przecinek is not initialized.\n");
    printf("Try to run pSetup() first,\n");
    fflush(stdout);

    // Reset and return [font]
    pDebugFontReset(&font);
    font.ID=0;

    return font;
  }

  // Check [size] value
  if(size<FONT_SIZE_MIN){
    if(przecinek.debug==true){
      printf("[pFontCreate() Warning]\n");
      printf("Value of font.size is too small!\n");
      printf("Value of font.size was changed from %i to %i,\n", size, FONT_SIZE_MIN);
      fflush(stdout);
    }

    // Change [size] value
    size=FONT_SIZE_MIN;
  }
  else if(size>FONT_SIZE_MAX){
    if(przecinek.debug==true){
      printf("[pFontCreate() Warning]\n");
      printf("Value of font.size is too large!\n");
      printf("Value of font.size was changed from %i to %i,\n", size, FONT_SIZE_MAX);
      fflush(stdout);
    }

    // Change [size] value
    size=FONT_SIZE_MAX;
  }

  // Set [font] values
  font.size=size;
  wcscpy(font.directory, directory);

  // Set [view] values
  view[font.ID-1].exist=true;

  view[font.ID-1].size=size;
  wcscpy(view[font.ID-1].directory, directory);

  // Check [directory] value
  if(wcslen(directory)>=4 &&
      (wcscmp(directory+wcslen(directory)-4, L".ttf")==0 ||
       wcscmp(directory+wcslen(directory)-4, L".otf")==0)){

    // Setup [view] [collection] and check if [directory] exists
    GdipNewPrivateFontCollection(&view[font.ID-1].collection);
    if(GdipPrivateAddFontFile(view[font.ID-1].collection, directory)!=Ok){
      if(przecinek.debug==true){
        printf("[pFontCreate() Error]\n");
        printf("Font located in font.directory doesn't exist!\n");
        printf("Check if you gave the correct value,\n");
        fflush(stdout);
      }

      // Reset and return [font]
      pDebugFontReset(&font);
      font.ID=0;

      return font;
    }
  }
  else{
    if(przecinek.debug==true){
      printf("[pFontCreate() Error]\n");
      printf("Value of font.directory doesn't include .ttf/.otf extenstion!\n");
      printf("Check if you gave the correct value,\n");
      fflush(stdout);
    }

    // Reset and return [font]
    pDebugFontReset(&font);
    font.ID=0;

    return font;
  }

  // Get [familyCount] from [view] [collection]
  GdipGetFontCollectionFamilyCount(view[font.ID-1].collection, &familyCount);

  // Load all [family] names
  family=(GpFontFamily**)malloc(familyCount*sizeof(GpFontFamily*));
  GdipGetFontCollectionFamilyList(view[font.ID-1].collection, familyCount, family, &familyCount);

  // Check for [view] [collection] name
  for(unsigned short int current=0; current<familyCount; current+=1){
    memset(familyName, 0, sizeof(familyName));
    GdipGetFamilyName(family[current], familyName, 0);

    if(GdipCreateFontFamilyFromName(
      familyName, view[font.ID-1].collection, &view[font.ID-1].fontFamily)==Ok){

      break;
    }
    else if(current==familyCount-1){
      if(przecinek.debug==true){
        printf("[pFontCreate() Error]\n");
        printf("GDI+ library could not load font!\n");
        printf("Your font file might be corrupted,\n");
        fflush(stdout);
      }

      // Reset and return [font]
      pDebugFontReset(&font);
      font.ID=0;

      free(family);

      return font;
    }
  }
  free(family);

  // Initialize [view] [base]
  GdipCreateFont(
    view[font.ID-1].fontFamily, size*1.33, FontStyleRegular, UnitPixel, &view[font.ID-1].base
  );

  // Return local [font]
  return font;
}

/****************************************************************
 * |\_____/| pFontDestroy()
 * | .     |
 * |     . | In: pFont* [font]
 * \ = , = / Out:
 *
 * This function destroys given [font].
 * It also resets [font] `ID` to `0`.
 ****************************************************************/
void pFontDestroy(pFont *font){
  if(font==NULL){
    if(przecinek.debug==true){
      printf("[pFontDestroy() Error]\n");
      printf("Given font is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }

  if(font->ID!=0){
    // Fully reset [font]
    pDebugFontReset(font);
    font->ID=0;
  }
  else if(przecinek.debug==true){
    printf("[pFontDestroy() Warning]\n");
    printf("Could not destroy font!\n");
    printf("It's already closed,\n");
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pTextCreate()
 * | .     |
 * |     . | In: wchar_t* [value]
 * \ = , = / Out: pText
 *
 * This function creates [text] object. It sets [ID]
 * for local [text]. It fills all [text] variables.
 ****************************************************************/
pText pTextCreate(wchar_t *value){
  // Create local [text]
  pText text;

  for(unsigned short int current=0; current<TEXT_MAX; current+=1){
    if(code[current].exist==false){
      // Set [text] [ID] and reset it
      text.ID=current+1;
      pDebugTextReset(&text);

      break;
    }
    else if(current==TEXT_MAX-1){
      if(przecinek.debug==true){
        printf("[pTextCreate() Error]\n");
        printf("Too many texts were created!\n");
        printf("Current text limit is equal to %i.\n", TEXT_MAX);
        printf("Try to destroy unused texts or change Przecinek text limit,\n");
        fflush(stdout);
      }

      // Return [text]
      text.ID=0;

      return text;
    }
  }

  // Check if Przecinek is initialized
  if(setup==false){
    printf("[pTextCreate() Error]\n");
    printf("Could not create text!\n");
    printf("Przecinek is not initialized.\n");
    printf("Try to run pSetup() first,\n");
    fflush(stdout);

    // Reset and return [text]
    pDebugTextReset(&text);
    text.ID=0;

    return text;
  }

  // Set [text] value
  wcscpy(text.value, value);

  // Set [code] value
  code[text.ID-1].exist=true;

  wcscpy(code[text.ID-1].value, value);

  // Return local [text]
  return text;
}

/****************************************************************
 * |\_____/| pTextDestroy()
 * | .     |
 * |     . | In: pText* [text]
 * \ = , = / Out:
 *
 * This function destroys given [text].
 * It also resets [text] `ID` to `0`.
 ****************************************************************/
void pTextDestroy(pText *text){
  if(text==NULL){
    if(przecinek.debug==true){
      printf("[pTextDestroy() Error]\n");
      printf("Given text is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }

  if(text->ID!=0){
    // Fully reset [text]
    pDebugTextReset(text);
    text->ID=0;
  }
  else if(przecinek.debug==true){
    printf("[pTextDestroy() Warning]\n");
    printf("Could not destroy text!\n");
    printf("It's already closed,\n");
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pImageCreate()
 * | .     |
 * |     . | In: wchar_t* [directory]
 * \ = , = / Out: pImage
 *
 * This function creates [image] object. It sets [ID]
 * for local [image]. It fills all [image] variables.
 ****************************************************************/
pImage pImageCreate(wchar_t *directory){
  // Create local [image]
  pImage image;

  for(unsigned short int current=0; current<IMAGE_MAX; current+=1){
    if(texture[current].exist==false){
      // Set [image] [ID] and reset it
      image.ID=current+1;
      pDebugImageReset(&image);

      break;
    }
    else if(current==IMAGE_MAX-1){
      if(przecinek.debug==true){
        printf("[pImageCreate() Error]\n");
        printf("Too many images were created!\n");
        printf("Current image limit is equal to %i.\n", IMAGE_MAX);
        printf("Try to destroy unused images or change Przecinek image limit,\n");
        fflush(stdout);
      }

      // Return [image]
      image.ID=0;

      return image;
    }
  }

  // Check if Przecinek is initialized
  if(setup==false){
    printf("[pImageCreate() Error]\n");
    printf("Could not create image!\n");
    printf("Przecinek is not initialized.\n");
    printf("Try to run pSetup() first,\n");
    fflush(stdout);

    // Reset and return [image]
    pDebugImageReset(&image);
    image.ID=0;

    return image;
  }

  // Set [image] values
  wcscpy(image.directory, directory);

  // Set [texture] values
  texture[image.ID-1].exist=true;

  wcscpy(texture[image.ID-1].directory, directory);

  // Check [directory] value
  if((wcslen(directory)>=4 &&
      (wcscmp(directory+wcslen(directory)-4, L".png")==0 ||
       wcscmp(directory+wcslen(directory)-4, L".jpg")==0)) ||
      (wcslen(directory)>=5 &&
      wcscmp(directory+wcslen(directory)-5, L".jpeg")==0)){

    // Check if [texture] [directory] exists
    if(GdipCreateBitmapFromFile(directory, &imageBuffer)!=Ok){
      if(przecinek.debug==true){
        printf("[pImageCreate() Error]\n");
        printf("GDI+ library could not load texture!\n");
        printf("Check if you gave the correct value of texture.directory and if your file isn't corruped,\n");
        fflush(stdout);
      }

      // Reset and return [image]
      pDebugImageReset(&image);
      image.ID=0;

      return image;
    }
  }
  else{
    if(przecinek.debug==true){
      printf("[pImageCreate() Error]\n");
      printf("Value of image.directory doesn't include .png/.jpg/.jpeg extenstion!\n");
      printf("Check if you gave the correct value,\n");
      fflush(stdout);
    }

    // Reset and return [image]
    pDebugImageReset(&image);
    image.ID=0;

    return image;
  }

  // Load [texture] [source]
  pDebugImageSetup(&image);

  // Return local [image]
  return image;
}

/****************************************************************
 * |\_____/| pImageDestroy()
 * | .     |
 * |     . | In: pImage* [image]
 * \ = , = / Out:
 *
 * This function destroys given [image].
 * It also resets [image] `ID` to `0`.
 ****************************************************************/
void pImageDestroy(pImage *image){
  if(image==NULL){
    if(przecinek.debug==true){
      printf("[pImageDestroy() Error]\n");
      printf("Given image is NULL!\n");
      printf("Make sure to use your brain once in a while,\n");
      fflush(stdout);
    }

    return;
  }

  if(image->ID!=0){
    // Fully reset [image]
    pDebugImageReset(image);
    image->ID=0;
  }
  else if(przecinek.debug==true){
    printf("[pImageDestroy() Warning]\n");
    printf("Could not destroy image!\n");
    printf("It's already closed,\n");
    fflush(stdout);
  }

  return;
}
