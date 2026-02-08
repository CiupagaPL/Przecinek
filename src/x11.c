/****************************************************************
 *         |\_____/|  Orginal {,} made by Ciupaga
 *         |       |  Copyright 2025-2026
 *  ___    | >   < |
 * /   \  _\ = , = /  https://github.com/CiupagaPL/Przecinek
 * \__  \/        |   Make sure to read the License and Manual!
 *    \_         /
 *      \ \----\ \
 *      {,{,} {,},}
 ****************************************************************/

// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <locale.h>

// Unix only libraries
#include <unistd.h>
#include <sys/time.h>

// X11 and Xft libraries
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

// FreeType2 library
#include <ft2build.h>

// Cairo libraries
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <cairo/cairo-ft.h>

/********************************
 *  ,______,  Define default
 *  |      |  values
 *  |______|
 * (--------)
 ********************************/
#define WINDOW_MAX 4
#define WINDOW_X_DEF 128
#define WINDOW_Y_DEF 128
#define WINDOW_WIDTH_MIN 256
#define WINDOW_HEIGHT_MIN 256
#define WINDOW_WIDTH_MAX 7680
#define WINDOW_HEIGHT_MAX 4320
#define WINDOW_POS_MAX 32768-WINDOW_WIDTH_MAX
#define WINDOW_POS_CHANGE 65536

#define TITLE_DEF "{,} Window"
#define TITLE_MAX 256

#define KEY_MAX 256
#define FRAME_MIN 10
#define FRAME_MAX 640

#define OBJECT_MAX 512
#define OBJECT_WIDTH_MIN 4
#define OBJECT_HEIGHT_MIN 4
#define OBJECT_WIDTH_MAX 7680
#define OBJECT_HEIGHT_MAX 4320
#define OBJECT_VERTICE_MIN 3
#define OBJECT_VERTICE_MAX 300
#define OBJECT_ROTATION_MAX 360
#define OBJECT_TRIANGLE 1000000

#define FONT_MAX 32
#define FONT_SIZE_MIN 4
#define FONT_SIZE_MAX 512
#define FONT_NAME_MAX 256

#define TEXT_MAX 128
#define TEXT_SIZE_MAX 1024

/********************************
 *  ,______,  Define [pSize],
 *  |      |  [pPosition],
 *  |______|  [pColor] structures
 * (--------)
 ********************************/
typedef struct{ unsigned short int width, height; } pSize;
typedef struct{ int x, y; } pPosition;
typedef struct{ unsigned short int red, green, blue, alpha; } pColor;

/********************************
 *  ,______,  Define [pPrzecinek]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  bool debug;
  unsigned short int windowCount, frameLimit;

  unsigned short int key[KEY_MAX];
  bool keyCaps;

  pSize display;
  pPosition cursor;
} pPrzecinek;

/********************************
 *  ,______,  Define [pBuildX11]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  int x, y;
  int xBac, yBac;

  unsigned short int width, height;
  unsigned short int widthMin, heightMin, widthMax, heightMax;
  unsigned short int widthBac, heightBac;

  char title[TITLE_MAX];
  wchar_t titleW[TITLE_MAX];
  struct timeval frameStart, frameEnd;
  unsigned short int frameCount;
  double frameMax;

  bool resizable, focus, fullScreen;

  cairo_surface_t *surface;
  cairo_t *cairo;

  Display *display;
  int screen;
  Window base;
  bool limitChange, sizeRefresh, screenRefresh;

  Atom delete, state;
  XSizeHints sizeHint;
  XSetWindowAttributes attributeBase;

  bool exist;
} pBuildX11;

/********************************
 *  ,______,  Define [pFigureX11]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  int x, y;
  unsigned short int width, height;
  unsigned short int vertice, rotation;

  pPosition point[OBJECT_VERTICE_MAX];
  pPosition center;
  unsigned short int rotationFix;

  int position;
  bool change;

  bool exist;
} pFigureX11;

/********************************
 *  ,______,  Define [pViewX11]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  unsigned short int size;

  char directory[FONT_NAME_MAX];
  wchar_t directoryW[FONT_NAME_MAX];

  FT_Face face[WINDOW_MAX];
  cairo_font_face_t *cairoFace[WINDOW_MAX];

  bool change;

  bool exist;
} pViewX11;

/********************************
 *  ,______,  Define [pCodeX11]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  char value[TEXT_SIZE_MAX];
  wchar_t valueW[TEXT_SIZE_MAX];

  int xFix, yFix;

  bool exist;
} pCodeX11;

/********************************
 *  ,______,  Define [pWindow]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  unsigned short int ID;

  int x, y;
  unsigned short int width, height;
  unsigned short int widthMin, heightMin, widthMax, heightMax;

  wchar_t title[TITLE_MAX];
  unsigned short int frameCount;

  bool resizable, focus, fullScreen;
} pWindow;

/********************************
 *  ,______,  Define [pObject]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  unsigned short int ID;

  int x, y;
  unsigned short int width, height;
  unsigned short int vertice, rotation;
} pObject;

/********************************
 *  ,______,  Define [pFont]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  unsigned short int ID;

  unsigned short int size;
  wchar_t directory[FONT_NAME_MAX];
} pFont;

/********************************
 *  ,______,  Define [pText]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  unsigned short int ID;

  int x, y;

  wchar_t value[TEXT_SIZE_MAX];
} pText;

// Przecinek
pPrzecinek przecinek={ true, 0, 0 };
bool setup=false;

FT_Library freeType;

pPosition cursorMain, cursorLocal;

// Window
unsigned short int windowMainID=0;
unsigned short int windowCount=0;

pBuildX11 build[WINDOW_MAX];

XEvent currentEvent, currentReport, currentAction;
Display *display;
int screen;
Window root;
unsigned int mask;

XKeyboardState keyboardState;

unsigned short int input[KEY_MAX];
bool inputChange;

// Object
pFigureX11 figure[OBJECT_MAX];

float widthScale, heightScale, ratio, distance;
pPosition distanceMin, distanceMax;
pPosition edge, projection;
pPosition centerA, centerB;
int pointA[5], pointB[5];

// Font
pViewX11 view[FONT_MAX];

cairo_font_extents_t fontExtents;

struct stat status;

// Text
pCodeX11 code[TEXT_MAX];

char *segment;
char *token;

/****************************************************************
 * |\_____/| pDebugColorCheck() [DEBUG]
 * | .     |
 * |     . | In: pColor* [color]
 * \ = , = / Out:
 *
 * This function checks if all [color] values are correct.
 * Maximal values are based on rgba(255, 255, 255, 100).
 ****************************************************************/
void pDebugColorCheck(pColor *color){
  // Check [color] values
  if(color->red>255){
    if(przecinek.debug==true){
printf(
  "[pWG04] \"Color red value is too big\" (changing from: %i to 255),\n",
  color->red
);
      fflush(stdout);
    }

    // Correct [color] [red] value
    color->red=255;
  }
  if(color->green>255){
    if(przecinek.debug==true){
printf(
  "[pWG05] \"Color green value is too big\" (changing from: %i to 255),\n",
  color->green
);
      fflush(stdout);
    }

    // Correct [color] [green] value
    color->green=255;
  }
  if(color->blue>255){
    if(przecinek.debug==true){
printf(
  "[pWG06] \"Color blue value is too big\" (changing from: %i to 255),\n",
  color->blue
);
      fflush(stdout);
    }

    // Correct [color] [blue] value
    color->blue=255;
  }
  if(color->alpha>100){
    if(przecinek.debug==true){
printf(
  "[pWG07] \"Color alpha value is too big\" (changing from: %i to 100),\n",
  color->alpha
);
      fflush(stdout);
    }

    // Correct [color] [alpha] value
    color->alpha=100;
  }

  return;
}

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

  memset(build[window->ID-1].title, 0, sizeof(build[window->ID-1].title));
  memset(build[window->ID-1].titleW, 0, sizeof(build[window->ID-1].titleW));

  build[window->ID-1].frameStart.tv_sec=0;
  build[window->ID-1].frameStart.tv_usec=0;
  build[window->ID-1].frameEnd.tv_sec=0;
  build[window->ID-1].frameEnd.tv_usec=0;
  build[window->ID-1].frameCount=0;
  build[window->ID-1].frameMax=0;

  build[window->ID-1].resizable=false;
  build[window->ID-1].focus=false;
  build[window->ID-1].fullScreen=false;

  cairo_surface_destroy(build[window->ID-1].surface);

  if(build[window->ID-1].base!=0){
    XDestroyWindow(build[window->ID-1].display, build[window->ID-1].base);
    build[window->ID-1].base=0;
  }
  build[window->ID-1].screen=0;
  if(build[window->ID-1].display!=NULL){
    XCloseDisplay(build[window->ID-1].display);
    build[window->ID-1].display=NULL;
  }
  build[window->ID-1].limitChange=false;
  build[window->ID-1].sizeRefresh=false;
  build[window->ID-1].screenRefresh=false;

  build[window->ID-1].delete=None;
  build[window->ID-1].state=None;

  build[window->ID-1].exist=false;

  // Change [windowCount]
  windowCount-=1;

  return;
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
  figure[object->ID-1].center.x=0;
  figure[object->ID-1].center.y=0;
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
  distanceMin.x=OBJECT_WIDTH_MAX;
  distanceMin.y=OBJECT_HEIGHT_MAX;
  distanceMax.x=(-OBJECT_WIDTH_MAX);
  distanceMax.y=(-OBJECT_HEIGHT_MAX);

  for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
    // Calculate base [ratio]
    ratio=((current*360/figure[object->ID-1].vertice)+
      figure[object->ID-1].rotation+figure[object->ID-1].rotationFix)*(M_PI/180);

    // Calculate [figure] [point] [x]
    figure[object->ID-1].point[current].x=50+(sqrt(pow(100, 2)*2)/2)*cos(ratio);

    // Calculate [figure] [point] [y]
    figure[object->ID-1].point[current].y=50+(sqrt(pow(100, 2)*2)/2)*sin(ratio);

    // Check for [distanceMin] [x] and [y] values
    if(figure[object->ID-1].point[current].x<distanceMin.x){
      distanceMin.x=figure[object->ID-1].point[current].x;
    }
    if(figure[object->ID-1].point[current].y<distanceMin.y){
      distanceMin.y=figure[object->ID-1].point[current].y;
    }

    // Check for [distanceMax] [x] and [y] values
    if(figure[object->ID-1].point[current].x>distanceMax.x){
      distanceMax.x=figure[object->ID-1].point[current].x;
    }
    if(figure[object->ID-1].point[current].y>distanceMax.y){
      distanceMax.y=figure[object->ID-1].point[current].y;
    }
  }

  // Calculate [widthScale] and [heightScale] values
  widthScale=(float)(distanceMax.x-distanceMin.x)/100;
  heightScale=(float)(distanceMax.y-distanceMin.y)/100;

  // Reset some variables
  distanceMin.x=OBJECT_WIDTH_MAX;
  distanceMin.y=OBJECT_HEIGHT_MAX;

  for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
    // Calculate base ratio
    ratio=((current*360/figure[object->ID-1].vertice)+
      figure[object->ID-1].rotation+figure[object->ID-1].rotationFix)*(M_PI/180);

    // Calculate [figure] [point] [x]
    figure[object->ID-1].point[current].x=
      (figure[object->ID-1].width/2)+
      (sqrt(pow((float)figure[object->ID-1].width/widthScale, 2)*2)/2)*
      cos(ratio);

    // Calculate [figure] [point] [y]
    figure[object->ID-1].point[current].y=
      (figure[object->ID-1].height/2)+
      (sqrt(pow((float)figure[object->ID-1].height/heightScale, 2)*2)/2)*
      sin(ratio);

    // Check for [distanceMin] [x] and [y] values
    if(figure[object->ID-1].point[current].x<distanceMin.x){
      distanceMin.x=figure[object->ID-1].point[current].x;
    }
    if(figure[object->ID-1].point[current].y<distanceMin.y){
      distanceMin.y=figure[object->ID-1].point[current].y;
    }
  }

  for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
    // Correct [figure] [point] position
    figure[object->ID-1].point[current].x-=distanceMin.x-figure[object->ID-1].x;
    figure[object->ID-1].point[current].y-=distanceMin.y-figure[object->ID-1].y;
  }

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
      edge.x=triangleA[(current+1)%3].x-triangleA[current].x;
      edge.y=triangleA[(current+1)%3].y-triangleA[current].y;
    }
    else{
      edge.x=triangleB[(current-2)%3].x-triangleB[current-3].x;
      edge.y=triangleB[(current-2)%3].y-triangleB[current-3].y;
    }

    // Reset some variables
    distanceMin.x=OBJECT_TRIANGLE;
    distanceMin.y=OBJECT_TRIANGLE;
    distanceMax.x=(-OBJECT_TRIANGLE);
    distanceMax.y=(-OBJECT_TRIANGLE);

    for(unsigned short int loop=0; loop<3; loop+=1){
      // Caululate [projection] values
      projection.x = (triangleA[loop].x*(-edge.y))+(triangleA[loop].y*edge.x);
      projection.y = (triangleB[loop].x*(-edge.y))+(triangleB[loop].y*edge.x);

      // Check for collision
      if(projection.x<distanceMin.x){ distanceMin.x=projection.x; }
      if(projection.x>distanceMax.x){ distanceMax.x=projection.x; }
      if(projection.y<distanceMin.y){ distanceMin.y=projection.y; }
      if(projection.y>distanceMax.y){ distanceMax.y=projection.y; }
    }

    if(distanceMax.x<distanceMin.y || distanceMax.y<distanceMin.x){
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

  // Reset [view] values
  view[font->ID-1].size=0;

  memset(view[font->ID-1].directory, 0, sizeof(view[font->ID-1].directory));
  memset(view[font->ID-1].directoryW, 0, sizeof(view[font->ID-1].directoryW));

  for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
    if(view[font->ID-1].face[current]!=NULL){
      FT_Done_Face(view[font->ID-1].face[current]);
      view[font->ID-1].face[current]=NULL;
    }

    if(view[font->ID-1].cairoFace[current]!=NULL){
      cairo_font_face_destroy(view[font->ID-1].cairoFace[current]);
    }
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
  memset(code[text->ID-1].valueW, 0, sizeof(code[text->ID-1].valueW));

  code[text->ID-1].xFix=0;
  code[text->ID-1].yFix=0;

  code[text->ID-1].exist=false;

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

  // Initialize FreeType
  FT_Init_FreeType(&freeType);

  // Update [przecinek] [debug] value
  przecinek.debug=debug;

  // Set [przecinek] [key] and [keyCaps] values
  memset(przecinek.key, 0, sizeof(przecinek.key));
  przecinek.keyCaps=false;

  // Update [przecinek] [frameLimit] value
  if(frameLimit<FRAME_MIN){
    if(przecinek.debug==true){
printf(
  "[pWG01] \"Frame limit value is too low\" (changing from: %i to: %i),\n",
  frameLimit, FRAME_MIN
);
      fflush(stdout);
    }

    frameLimit=FRAME_MIN;
  }
  else if(frameLimit>FRAME_MAX){
    if(przecinek.debug==true){
printf(
  "[pWG02] \"Frame limit value is too big\" (changing from: %i to: %i),\n",
  frameLimit, FRAME_MAX
);
      fflush(stdout);
    }

    frameLimit=FRAME_MAX;
  }
  przecinek.frameLimit=frameLimit;

  // Create global [display]
  display=XOpenDisplay(NULL);
  if(display==NULL){
    if(przecinek.debug==true){
printf(
  "[pEBx1] \"Could not open X11 display\",\n"
);
      fflush(stdout);
    }

    return;
  }

  // Create global [screen]
  screen=DefaultScreen(display);

  // Create global [root]
  root=DefaultRootWindow(display);

  // Update [przecinek] [display] values
  przecinek.display.width=XDisplayWidth(display, screen);
  przecinek.display.height=XDisplayHeight(display, screen);

  // Update [przecinek] [cursor] values
  if(XQueryPointer(
      display, root, &root, &root, &cursorMain.x, &cursorMain.y,
      &cursorLocal.x, &cursorLocal.y, &mask)){

    przecinek.cursor.x=cursorMain.x;
    przecinek.cursor.y=cursorMain.y;
  }
  else{
    przecinek.cursor.x=0;
    przecinek.cursor.y=0;
  }

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
  // End FreeType session
  //FT_Done_FreeType(freeType);

  // Destroy debug [display] and [root]
  XCloseDisplay(display);
  XDestroyWindow(display, root);

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
  system("clear");

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
  if(wcscmp(key, L"LMOUSE")==0 || wcscmp(key, L"LMouse")==0 || wcscmp(key, L"lmouse")==0){ return 1; }
  if(wcscmp(key, L"MMOUSE")==0 || wcscmp(key, L"MMouse")==0 || wcscmp(key, L"mmouse")==0){ return 2; }
  if(wcscmp(key, L"RMOUSE")==0 || wcscmp(key, L"RMouse")==0 || wcscmp(key, L"rmouse")==0){ return 3; }
  if(wcscmp(key, L"BACK")==0 || wcscmp(key, L"Back")==0 || wcscmp(key, L"back")==0){ return 4; }
  if(wcscmp(key, L"FORWARD")==0 || wcscmp(key, L"Forward")==0 || wcscmp(key, L"forward")==0){ return 5; }

  if(wcscmp(key, L"ESC")==0 || wcscmp(key, L"esc")==0 || wcscmp(key, L"Esc")==0){ return 9; }
  if(wcscmp(key, L"TAB")==0 || wcscmp(key, L"tab")==0 || wcscmp(key, L"Tab")==0){ return 23; }
  if(wcscmp(key, L"CAPS")==0 || wcscmp(key, L"caps")==0 || wcscmp(key, L"Caps")==0){ return 66; }
  if(wcscmp(key, L"LSHIFT")==0 || wcscmp(key, L"lshift")==0 || wcscmp(key, L"LShift")==0){ return 50; }
  if(wcscmp(key, L"LCTRL")==0 || wcscmp(key, L"lctrl")==0 || wcscmp(key, L"LCtrl")==0){ return 37; }
  if(wcscmp(key, L"LMOD")==0 || wcscmp(key, L"lmod")==0 || wcscmp(key, L"LMod")==0){ return 133; }
  if(wcscmp(key, L"LALT")==0 || wcscmp(key, L"lalt")==0 || wcscmp(key, L"LAlt")==0){ return 64; }
  if(wcscmp(key, L"SPACE")==0 || wcscmp(key, L"space")==0 || wcscmp(key, L"Space")==0){ return 65; }

  if(wcscmp(key, L"F1")==0 || wcscmp(key, L"f1")==0){ return 67; }
  if(wcscmp(key, L"F2")==0 || wcscmp(key, L"f2")==0){ return 68; }
  if(wcscmp(key, L"F3")==0 || wcscmp(key, L"f3")==0){ return 69; }
  if(wcscmp(key, L"F4")==0 || wcscmp(key, L"f4")==0){ return 70; }
  if(wcscmp(key, L"F5")==0 || wcscmp(key, L"f5")==0){ return 71; }
  if(wcscmp(key, L"F6")==0 || wcscmp(key, L"f6")==0){ return 72; }
  if(wcscmp(key, L"F7")==0 || wcscmp(key, L"f7")==0){ return 73; }
  if(wcscmp(key, L"F8")==0 || wcscmp(key, L"f8")==0){ return 74; }
  if(wcscmp(key, L"F9")==0 || wcscmp(key, L"f9")==0){ return 75; }
  if(wcscmp(key, L"F10")==0 || wcscmp(key, L"f10")==0){ return 76; }
  if(wcscmp(key, L"F11")==0 || wcscmp(key, L"f11")==0){ return 95; }
  if(wcscmp(key, L"F12")==0 || wcscmp(key, L"f12")==0){ return 96; }

  if(wcscmp(key, L"RALT")==0 || wcscmp(key, L"ralt")==0 || wcscmp(key, L"RAlt")==0){ return 108; }
  if(wcscmp(key, L"RWIN")==0 || wcscmp(key, L"rwin")==0 || wcscmp(key, L"RWin")==0){ return 134; }
  if(wcscmp(key, L"MENU")==0 || wcscmp(key, L"menu")==0 || wcscmp(key, L"Menu")==0){ return 135; }
  if(wcscmp(key, L"RCTRL")==0 || wcscmp(key, L"rctrl")==0 || wcscmp(key, L"RCtrl")==0){ return 105; }
  if(wcscmp(key, L"RSHIFT")==0 || wcscmp(key, L"rshift")==0 || wcscmp(key, L"RShift")==0){ return 62; }
  if(wcscmp(key, L"ENTER")==0 || wcscmp(key, L"enter")==0 || wcscmp(key, L"Enter")==0){ return 36; }
  if(wcscmp(key, L"BACKSPACE")==0 || wcscmp(key, L"backspace")==0 || wcscmp(key, L"Backspace")==0){ return 22; }

  if(wcscmp(key, L"LARROW")==0 || wcscmp(key, L"larrow")==0 || wcscmp(key, L"LArrow")==0){ return 113; }
  if(wcscmp(key, L"DARROW")==0 || wcscmp(key, L"darrow")==0 || wcscmp(key, L"DArrow")==0){ return 116; }
  if(wcscmp(key, L"RARROW")==0 || wcscmp(key, L"rarrow")==0 || wcscmp(key, L"RArrow")==0){ return 114; }
  if(wcscmp(key, L"UARROW")==0 || wcscmp(key, L"uarrow")==0 || wcscmp(key, L"UArrow")==0){ return 111; }

  if(wcscmp(key, L"PRINTSCRN")==0 || wcscmp(key, L"printscrn")==0 || wcscmp(key, L"PrintScrn")==0){ return 107; }
  if(wcscmp(key, L"SCROLLLOCK")==0 || wcscmp(key, L"scrolllock")==0 || wcscmp(key, L"ScrollLock")==0){ return 78; }
  if(wcscmp(key, L"PAUSEBREAK")==0 || wcscmp(key, L"pausebreak")==0 || wcscmp(key, L"PauseBreak")==0){ return 127; }
  if(wcscmp(key, L"INS")==0 || wcscmp(key, L"ins")==0 || wcscmp(key, L"Ins")==0){ return 118; }
  if(wcscmp(key, L"HOME")==0 || wcscmp(key, L"home")==0 || wcscmp(key, L"Home")==0){ return 110; }
  if(wcscmp(key, L"PAGEU")==0 || wcscmp(key, L"pageu")==0 || wcscmp(key, L"PageU")==0){ return 112; }
  if(wcscmp(key, L"DEL")==0 || wcscmp(key, L"del")==0 || wcscmp(key, L"Del")==0){ return 119; }
  if(wcscmp(key, L"END")==0 || wcscmp(key, L"end")==0 || wcscmp(key, L"End")==0){ return 115; }
  if(wcscmp(key, L"PAGED")==0 || wcscmp(key, L"paged")==0 || wcscmp(key, L"PageD")==0){ return 117; }

  if(wcscmp(key, L"Q")==0 || wcscmp(key, L"q")==0){ return 24; }
  if(wcscmp(key, L"W")==0 || wcscmp(key, L"w")==0){ return 25; }
  if(wcscmp(key, L"E")==0 || wcscmp(key, L"e")==0){ return 26; }
  if(wcscmp(key, L"R")==0 || wcscmp(key, L"r")==0){ return 27; }
  if(wcscmp(key, L"T")==0 || wcscmp(key, L"t")==0){ return 28; }
  if(wcscmp(key, L"Y")==0 || wcscmp(key, L"y")==0){ return 29; }
  if(wcscmp(key, L"U")==0 || wcscmp(key, L"u")==0){ return 30; }
  if(wcscmp(key, L"I")==0 || wcscmp(key, L"i")==0){ return 31; }
  if(wcscmp(key, L"O")==0 || wcscmp(key, L"o")==0){ return 32; }
  if(wcscmp(key, L"P")==0 || wcscmp(key, L"p")==0){ return 33; }
  if(wcscmp(key, L"A")==0 || wcscmp(key, L"a")==0){ return 38; }
  if(wcscmp(key, L"S")==0 || wcscmp(key, L"s")==0){ return 39; }
  if(wcscmp(key, L"D")==0 || wcscmp(key, L"d")==0){ return 40; }
  if(wcscmp(key, L"F")==0 || wcscmp(key, L"f")==0){ return 41; }
  if(wcscmp(key, L"G")==0 || wcscmp(key, L"g")==0){ return 42; }
  if(wcscmp(key, L"H")==0 || wcscmp(key, L"h")==0){ return 43; }
  if(wcscmp(key, L"J")==0 || wcscmp(key, L"j")==0){ return 44; }
  if(wcscmp(key, L"K")==0 || wcscmp(key, L"k")==0){ return 45; }
  if(wcscmp(key, L"L")==0 || wcscmp(key, L"l")==0){ return 46; }
  if(wcscmp(key, L"Z")==0 || wcscmp(key, L"z")==0){ return 52; }
  if(wcscmp(key, L"X")==0 || wcscmp(key, L"x")==0){ return 53; }
  if(wcscmp(key, L"C")==0 || wcscmp(key, L"c")==0){ return 54; }
  if(wcscmp(key, L"V")==0 || wcscmp(key, L"v")==0){ return 55; }
  if(wcscmp(key, L"B")==0 || wcscmp(key, L"b")==0){ return 56; }
  if(wcscmp(key, L"N")==0 || wcscmp(key, L"n")==0){ return 57; }
  if(wcscmp(key, L"M")==0 || wcscmp(key, L"m")==0){ return 58; }

  if(wcscmp(key, L"1")==0){ return 10; }
  if(wcscmp(key, L"2")==0){ return 11; }
  if(wcscmp(key, L"3")==0){ return 12; }
  if(wcscmp(key, L"4")==0){ return 13; }
  if(wcscmp(key, L"5")==0){ return 14; }
  if(wcscmp(key, L"6")==0){ return 15; }
  if(wcscmp(key, L"7")==0){ return 16; }
  if(wcscmp(key, L"8")==0){ return 17; }
  if(wcscmp(key, L"9")==0){ return 18; }
  if(wcscmp(key, L"0")==0){ return 19; }

  if(wcscmp(key, L"`")==0){ return 49; }
  if(wcscmp(key, L",")==0){ return 59; }
  if(wcscmp(key, L".")==0){ return 60; }
  if(wcscmp(key, L"/")==0){ return 61; }
  if(wcscmp(key, L";")==0){ return 47; }
  if(wcscmp(key, L"'")==0){ return 48; }
  if(wcscmp(key, L"\\")==0){ return 51; }
  if(wcscmp(key, L"[")==0){ return 34; }
  if(wcscmp(key, L"]")==0){ return 35; }
  if(wcscmp(key, L"-")==0){ return 20; }
  if(wcscmp(key, L"=")==0){ return 21; }

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
 * It sets [window] [title] to default value.
 * It setups [build] objects for later use.
 * It also saves time when [window] was created,
 * to later calculate frame count.
 ****************************************************************/
pWindow pWindowCreate(unsigned short int width, unsigned short int height, bool resizable){
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
printf(
  "[pEB01] \"Too many windows were created\" (limit: %i),\n",
  WINDOW_MAX
);
        fflush(stdout);
      }

      // Reset and return [window]
      pDebugWindowReset(&window);

      window.ID=0;
      return window;
    }
  }

  // Check if Przecinek is initialized
  if(setup==false){
printf(
  "[pEG01] \"Could not create window\" (Przecinek is not initialized),\n"
);
    fflush(stdout);

    // Reset and return [window]
    pDebugWindowReset(&window);
    return window;
  }

  // Check [width] value
  if(width<WINDOW_WIDTH_MIN){
    if(przecinek.debug==true){
printf(
  "[pWB01] \"Window width value is too low\" (changing from: %i to %i),\n",
  width, WINDOW_WIDTH_MIN
);
      fflush(stdout);
    }

    // Change [width] value
    width=WINDOW_WIDTH_MIN;
  }
  else if(width>WINDOW_WIDTH_MAX){
    if(przecinek.debug==true){
printf(
  "[pWB02] \"Window width value is too big\" (changing from: %i to %i),\n",
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
  "[pWB03] \"Window height value is too low\" (changing from: %i to %i),\n",
  height, WINDOW_HEIGHT_MIN
);
      fflush(stdout);
    }

    // Change [height] value
    height=WINDOW_HEIGHT_MIN;
  }
  else if(height>WINDOW_HEIGHT_MAX){
    if(przecinek.debug==true){
printf(
  "[pWB04] \"Window height value is too big\" (changing from: %i to %i),\n",
  height, WINDOW_HEIGHT_MAX
);
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

  // Create [diplay] for [build]
  build[window.ID-1].display=XOpenDisplay(NULL);
  if(build[window.ID-1].display==NULL){
    if(przecinek.debug==true){
printf(
  "[pEBx1] \"Could not open X11 display\",\n"
);
      fflush(stdout);
    }

    // Reset and return [window]
    pDebugWindowReset(&window);

    window.ID=0;
    return window;
  }

  // Create [screen] for [build]
  build[window.ID-1].screen=DefaultScreen(build[window.ID-1].display);

  // Setup [build] [attributeBase]
  build[window.ID-1].attributeBase.background_pixel=WhitePixel(
    build[window.ID-1].display, build[window.ID-1].screen
  );

  // Create [base] for [build]
  build[window.ID-1].base=XCreateWindow(
    build[window.ID-1].display,
    RootWindow(build[window.ID-1].display, build[window.ID-1].screen),
    WINDOW_X_DEF, WINDOW_Y_DEF, width, height, 1,
    CopyFromParent, InputOutput, CopyFromParent,
    CWOverrideRedirect | CWBackPixel | CWEventMask,
    &build[window.ID-1].attributeBase
  );
  if(build[window.ID-1].base==0){
    if(przecinek.debug==true){
printf(
  "[pEBx2] \"Could not create X11 window\",\n"
);
      fflush(stdout);
    }

    // Reset [window] and close [build] [display]
    pDebugWindowReset(&window);

    window.ID=0;
    return window;
  }

  // Set [title] value
  XStoreName(build[window.ID-1].display, build[window.ID-1].base, TITLE_DEF);
  mbstowcs(window.title, TITLE_DEF, TITLE_MAX);
  strcpy(build[window.ID-1].title, TITLE_DEF);
  mbstowcs(build[window.ID-1].titleW, TITLE_DEF, TITLE_MAX);

  // Create [build] [delete] and [state]
  build[window.ID-1].delete=XInternAtom(build[window.ID-1].display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(
    build[window.ID-1].display, build[window.ID-1].base, &build[window.ID-1].delete, 1
  );
  build[window.ID-1].state=XInternAtom(build[window.ID-1].display, "_NET_WM_STATE", False);

  // Set [build] [sizeHint] values
  build[window.ID-1].sizeHint.flags=PMinSize | PMaxSize;
  build[window.ID-1].sizeHint.min_width=window.widthMin;
  build[window.ID-1].sizeHint.min_height=window.heightMin;
  build[window.ID-1].sizeHint.max_width=window.widthMax;
  build[window.ID-1].sizeHint.max_height=window.heightMax;

  // Update [build] hints
  XSetWMNormalHints(build[window.ID-1].display, build[window.ID-1].base, &build[window.ID-1].sizeHint);

  // Setup [build] input
  XSelectInput(
    build[window.ID-1].display, build[window.ID-1].base,
    KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
    ExposureMask | StructureNotifyMask | FocusChangeMask | PointerMotionMask
  );

  // Map, move and refresh [build]
  XMapWindow(build[window.ID-1].display, build[window.ID-1].base);
  XMoveWindow(build[window.ID-1].display, build[window.ID-1].base, window.x, window.y);
  XFlush(build[window.ID-1].display);

  // Set timer start value
  gettimeofday(&build[window.ID-1].frameStart, NULL);

  // Create [build] [surface]
  build[window.ID-1].surface=cairo_xlib_surface_create(
    build[window.ID-1].display, build[window.ID-1].base,
    DefaultVisual(build[window.ID-1].display, build[window.ID-1].screen),
    width, height
  );

  // Return local [window]
  return window;
}

/****************************************************************
 * |\_____/| pWindowDrawObject()
 * | .     | In: pWindow* [window], pObject* [object],
 * |     . |     pColor* [color]
 * \ = , = / Out:
 *
 * This function draws [object] on [window].
 * It checks if [color] values are valid.
 * It checks for any changes in [object] values.
 * Then it does all the rendering stuff.
 ****************************************************************/
void pWindowDrawObject(pWindow *window, pObject *object, pColor *color){
  if(window->ID!=0){
    if(object->ID!=0){
      // Check [color] values
      pDebugColorCheck(color);

      // Update [figure] [vertice]
      if(object->vertice!=figure[object->ID-1].vertice){
        // Check [object] [vertice] value
        if(object->vertice<OBJECT_VERTICE_MIN){
          if(przecinek.debug==true){
printf(
  "[pWO01] \"Object vertice value is too low\" (changing from: %i to %i),\n",
  object->vertice, OBJECT_VERTICE_MIN
);
            fflush(stdout);
          }

          // Change [object] [vertice] value
          object->vertice=OBJECT_VERTICE_MIN;
        }
        else if(object->vertice>OBJECT_VERTICE_MAX){
          if(przecinek.debug==true){
printf(
  "[pWO02] \"Object vertice value is too big\" (changing from: %i to %i),\n",
  object->vertice, OBJECT_VERTICE_MIN
);
            fflush(stdout);
          }

          // Change [object] [vertice] value
          object->vertice=OBJECT_VERTICE_MAX;
        }

        // Update [figure] [vertice]
        figure[object->ID-1].vertice=object->vertice;

        figure[object->ID-1].change=true;
      }

      // Update [figure] [width]
      if(object->width!=figure[object->ID-1].width){
        // Check [object] [width] value
        if(object->width<OBJECT_WIDTH_MIN){
          if(przecinek.debug==true){
printf(
  "[pWO03] \"Object width value is too low\" (changing from: %i to %i),\n",
  object->width, OBJECT_WIDTH_MIN
);
            fflush(stdout);
          }

          // Change [object] [width] value
          object->width=OBJECT_WIDTH_MIN;
        }
        else if(object->width>OBJECT_WIDTH_MAX){
          if(przecinek.debug==true){
printf(
  "[pWO04] \"Object width value is too big\" (changing from: %i to %i),\n",
  object->width, OBJECT_WIDTH_MAX
);
            fflush(stdout);
          }

          // Change [object] [width] value
          object->width=OBJECT_WIDTH_MAX;
        }

        // Update [figure] [width]
        figure[object->ID-1].width=object->width;

        figure[object->ID-1].change=true;
      }

      // Update [figure] [height]
      if(object->height!=figure[object->ID-1].height){
        // Check [object] [height] value
        if(object->height<OBJECT_HEIGHT_MIN){
          if(przecinek.debug==true){
printf(
  "[pWO05] \"Object height value is too low\" (changing from: %i to %i),\n",
  object->height, OBJECT_HEIGHT_MIN
);
            fflush(stdout);
          }

          // Change [object] [height] value
          object->height=OBJECT_HEIGHT_MIN;
        }
        else if(object->height>OBJECT_HEIGHT_MAX){
          if(przecinek.debug==true){
printf(
  "[pWO06] \"Object height value is too big\" (changing from: %i to %i),\n",
  object->height, OBJECT_HEIGHT_MAX
);
            fflush(stdout);
          }

          // Change [object] [height] value
          object->height=OBJECT_HEIGHT_MAX;
        }

        // Update [figure] [height]
        figure[object->ID-1].height=object->height;

        figure[object->ID-1].change=true;
      }

      // Update [figure] [rotation]
      if(object->rotation!=figure[object->ID-1].rotation){
        // Check [object] [rotation] value
        if(object->rotation>OBJECT_ROTATION_MAX){
          if(przecinek.debug==true){
printf(
  "[pWO07] \"Object rotation value is too big\" (changing from: %i to %i),\n",
  object->rotation, OBJECT_ROTATION_MAX
);
            fflush(stdout);
          }

          // Change [object] [rotation] value
          object->rotation=OBJECT_ROTATION_MAX;
        }

        // Update [figure] [rotation]
        figure[object->ID-1].rotation=object->rotation;

        figure[object->ID-1].change=true;
      }

      // Update [figure] [x]
      if(object->x!=figure[object->ID-1].x){
        // Check [object] [x] value
        if(object->x<(-WINDOW_POS_MAX)){
          if(przecinek.debug==true){
printf(
  "[pWO08] \"Object x value is too low\" (changing from: %i to %i),\n",
  object->x, (-WINDOW_POS_MAX)
);
            fflush(stdout);
          }

          // Change [object] [x] value
          object->x=(-WINDOW_POS_MAX);
        }
        else if(object->x>WINDOW_POS_MAX){
          if(przecinek.debug==true){
printf(
  "[pWO09] \"Object x value is too big\" (changing from: %i to %i),\n",
  object->x, WINDOW_POS_MAX
);
            fflush(stdout);
          }

          // Change [object] [x] value
          object->x=WINDOW_POS_MAX;
        }

        // Calculate [figure] [point] [x] position
        if(figure[object->ID-1].change==false){
          figure[object->ID-1].position=object->x-figure[object->ID-1].x;

          for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
            figure[object->ID-1].point[current].x+=figure[object->ID-1].position;
          }
        }

        // Update [figure] [x]
        figure[object->ID-1].x=object->x;
      }

      // Update [figure] [y]
      if(object->y!=figure[object->ID-1].y){
        // Check [object] [y] value
        if(object->y<(-WINDOW_POS_MAX)){
          if(przecinek.debug==true){
printf(
  "[pWO10] \"Object y value is too low\" (changing from: %i to %i),\n",
  object->y, (-WINDOW_POS_MAX)
);
            fflush(stdout);
          }

          // Change [object] [y] value
          object->y=(-WINDOW_POS_MAX);
        }
        else if(object->y>WINDOW_POS_MAX){
          if(przecinek.debug==true){
printf(
  "[pWO11] \"Object y value is too big\" (changing from: %i to %i),\n",
  object->y, WINDOW_POS_MAX
);
            fflush(stdout);
          }

          // Change [object] [y] value
          object->y=WINDOW_POS_MAX;
        }

        // Calculate [figure] [point] [y] position
        if(figure[object->ID-1].change==false){
          figure[object->ID-1].position=object->y-figure[object->ID-1].y;

          for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
            figure[object->ID-1].point[current].y+=figure[object->ID-1].position;
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

      // Setup [build] [cairo]
      build[window->ID-1].cairo=cairo_create(build[window->ID-1].surface);

      // Set [build] [cairo] [color] values
      cairo_set_source_rgba(
        build[window->ID-1].cairo,
        (float)color->red/255, (float)color->green/255, (float)color->blue/255, (float)color->alpha/100
      );

      // Draw on [build] [cairo]
      cairo_move_to(
        build[window->ID-1].cairo,
        figure[object->ID-1].point[0].x, figure[object->ID-1].point[0].y
      );

      for(unsigned short int current=1; current<object->vertice; current+=1){
        cairo_line_to(
          build[window->ID-1].cairo,
          figure[object->ID-1].point[current].x, figure[object->ID-1].point[current].y
        );
      }

      cairo_close_path(build[window->ID-1].cairo);
      cairo_fill(build[window->ID-1].cairo);

      // Clean [build] [cairo]
      cairo_destroy(build[window->ID-1].cairo);
    }
    else if(przecinek.debug==true){
printf(
  "[pEG03] \"Could not draw object\" (object is closed),\n"
);
      fflush(stdout);
    }
  }
  else if(przecinek.debug==true){
printf(
  "[pEG02] \"Could not draw object\" (window is closed),\n"
);
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
  if(window->ID!=0){
    if(font->ID!=0){
      // Check [color] values
      pDebugColorCheck(color);

      // Check [text] [x] value
      if(text->x<(-WINDOW_POS_MAX)){
        if(przecinek.debug==true){
printf(
  "[pWT01] \"Text x value is too low\" (changing from: %i to %i),\n",
  text->x, (-WINDOW_POS_MAX)
);
          fflush(stdout);
        }

        // Change [text] [x] value
        text->x=(-WINDOW_POS_MAX);
      }
      else if(text->x>WINDOW_POS_MAX){
        if(przecinek.debug==true){
printf(
  "[pWT02] \"Text x value is too big\" (changing from: %i to %i),\n",
  text->x, WINDOW_POS_MAX
);
          fflush(stdout);
        }

        // Change [text] [x] value
        text->x=WINDOW_POS_MAX;
      }

      // Check [text] [y] value
      if(text->y<(-WINDOW_POS_MAX)){
        if(przecinek.debug==true){
printf(
  "[pWT03] \"Text y value is too low\" (changing from: %i to %i),\n",
  text->y, (-WINDOW_POS_MAX)
);
          fflush(stdout);
        }

        // Change [text] [y] value
        text->y=(-WINDOW_POS_MAX);
      }
      else if(text->y>WINDOW_POS_MAX){
        if(przecinek.debug==true){
printf(
  "[pWT04] \"Text y value is too big\" (changing from: %i to %i),\n",
  text->y, WINDOW_POS_MAX
);
          fflush(stdout);
        }

        // Change [text] [y] value
        text->y=WINDOW_POS_MAX;
      }

      // Update [code] [value]
      if(wcscmp(text->value, code[text->ID-1].valueW)!=0){
        wcscpy(code[text->ID-1].valueW, text->value);
        wcstombs(code[text->ID-1].value, text->value, sizeof(code[text->ID-1].value));
      }

      // Update [view] [size]
      if(font->size!=view[font->ID-1].size){
        // Check [font] [size] value
        if(font->size<FONT_SIZE_MIN){
          if(przecinek.debug==true){
printf(
  "[pWF01] \"Font size value is too low\" (changing from: %i to %i),\n",
  font->size, FONT_SIZE_MIN
);
            fflush(stdout);
          }

          // Change [font] [size] value
          font->size=FONT_SIZE_MIN;
        }
        else if(font->size>FONT_SIZE_MAX){
          if(przecinek.debug==true){
printf(
  "[pWF02] \"Font size value is too big\" (changing from: %i to %i),\n",
  font->size, FONT_SIZE_MAX
);
            fflush(stdout);
          }

          // Change [font] [size] value
          font->size=FONT_SIZE_MAX;
        }

        // Refresh [view] values
        view[font->ID-1].size=font->size;
        view[font->ID-1].change=true;
      }

      // Update [view] [directory] value
      if(wcscmp(font->directory, view[font->ID-1].directoryW)!=0){
        wcscpy(view[font->ID-1].directoryW, font->directory);
        wcstombs(view[font->ID-1].directory, font->directory, sizeof(view[font->ID-1].directory));

        view[font->ID-1].change=true;
      }

      // Update [view] font
      if(view[font->ID-1].change==true){
        // Check if [view] [directory] exists
        if(stat(view[font->ID-1].directory, &status)!=0){
          if(przecinek.debug==true){
printf(
  "[pEF02] \"Could not load font\",\n"
);
printf(
  "[pEG04] \"Could not draw text\" (font is closed),\n"
);
            fflush(stdout);
          }

          // Reset [font]
          pDebugFontReset(font);
          font->ID=0;

          return;
        }

        // Reset [view] [face]
        for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
          FT_Done_Face(view[font->ID-1].face[current]);
        }

        // Load [view] [face] and check if font loads properly
        for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
          if(FT_New_Face(freeType, view[font->ID-1].directory, 0, &view[font->ID-1].face[current])){
            if(przecinek.debug==true){
printf(
  "[pEFx1] \"Could not create X11 font\",\n"
);
printf(
  "[pEG04] \"Could not draw text\" (font is closed),\n"
);
              fflush(stdout);
            }

            // Reset [font]
            pDebugFontReset(font);
            font->ID=0;

            return;
          }
        }

        // Load [view] [cairoFace]
        for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
          view[font->ID-1].cairoFace[current]=cairo_ft_font_face_create_for_ft_face(
            view[font->ID-1].face[current], FT_LOAD_NO_HINTING
          );
          FT_Set_Pixel_Sizes(view[font->ID-1].face[current], 0, view[font->ID-1].size);
        }

        view[font->ID-1].change=false;
      }

      // Update [code] [xFix] and [yFix] value
      code[text->ID-1].xFix=(view[font->ID-1].size*48)/512;
      code[text->ID-1].yFix=(view[font->ID-1].size*352)/512;

      // Setup [build] [cairo]
      build[window->ID-1].cairo=cairo_create(build[window->ID-1].surface);

      // Setup [build] [cairo] font
      cairo_set_font_face(build[window->ID-1].cairo, view[font->ID-1].cairoFace[window->ID-1]);
      cairo_set_font_size(build[window->ID-1].cairo, view[font->ID-1].size);
      cairo_font_extents(build[window->ID-1].cairo, &fontExtents);

      // Set [build] [cairo] [color] values
      cairo_set_source_rgba(
        build[window->ID-1].cairo,
        (float)color->red/255, (float)color->green/255, (float)color->blue/255, (float)color->alpha/100
      );

      // Load [code] [value] [segment]
      segment=strtok_r(code[text->ID-1].value, "\n", &token);
      while(segment!=NULL){
        // Draw on [build] [cairo]
        cairo_move_to(
          build[window->ID-1].cairo,
          text->x-code[text->ID-1].xFix, text->y+code[text->ID-1].yFix
        );
        cairo_show_text(build[window->ID-1].cairo, segment);

        // Update [code] [yFix]
        code[text->ID-1].yFix+=fontExtents.height;

        // Load next [segment]
        segment=strtok_r(NULL, "\n", &token);
      }

      // Update [code] [value]
      wcstombs(code[text->ID-1].value, text->value, sizeof(code[text->ID-1].value));

      // Clean [build] [cairo]
      cairo_destroy(build[window->ID-1].cairo);
    }
    else if(przecinek.debug==true){
printf(
  "[pEG04] \"Could not draw text\" (font is closed),\n"
);
      fflush(stdout);
    }
  }
  else if(przecinek.debug==true){
printf(
  "[pEG02] \"Could not draw text\" (window is closed),\n"
);
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pWindowClear()
 * | .     | In: pWindow* [window], int [x], [y],
 * |     . |     us_int [width], [height], pColor* [color]
 * \ = , = / Out:
 *
 * This function clears [window] with given color.
 * Cleared area depends on given position and size values.
 ****************************************************************/
void pWindowClear(
  pWindow *window, int x, int y,
  unsigned short int width, unsigned short int height, pColor *color
){

  if(window->ID!=0){
    // Check [color] values
    pDebugColorCheck(color);

    // Setup [build] [cairo]
    build[window->ID-1].cairo=cairo_create(build[window->ID-1].surface);

    // Set [build] [cairo] [color] values
    cairo_set_source_rgba(
      build[window->ID-1].cairo,
      (float)color->red/255, (float)color->green/255, (float)color->blue/255, (float)color->alpha/100
    );

    // Draw on [build] [cairo]
    cairo_rectangle(build[window->ID-1].cairo, x, y, width, height);
    cairo_fill(build[window->ID-1].cairo);

    // Clean [build] [cairo]
    cairo_destroy(build[window->ID-1].cairo);
  }
  else if(przecinek.debug==true){
printf(
  "[pEG02] \"Could not clear screen\" (window is closed),\n"
);
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
  if(window->ID!=0){
    // Update [windowMainID]
    if(build[windowMainID].exist==false){
      for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
        if(build[current].exist==true){
          windowMainID=current;
        }
      }
    }

    // Change [przecinek] [key] values from `1` to `2`
    if((window->ID-1)==windowMainID){
      for(unsigned short int button=0; button<KEY_MAX; button+=1){
        if(input[button]!=0){ przecinek.key[input[button]]=2; }
        else{ break; }
      }
    }

    while(XPending(build[window->ID-1].display)){
      // Manage pending [currentEvent]
      XNextEvent(build[window->ID-1].display, &currentEvent);

      // Manage close [currentEvent]
      if(currentEvent.type==ClientMessage &&
          (Atom)currentEvent.xclient.data.l[0]==build[window->ID-1].delete){

        // Reset [window]
        pDebugWindowReset(window);
        window->ID=0;

        return;
      }

      // Update [przecinek] [display] values
      przecinek.display.width=DisplayWidth(build[window->ID-1].display, build[window->ID-1].screen);
      przecinek.display.height=DisplayHeight(build[window->ID-1].display, build[window->ID-1].screen);

      // Update [przecinek] [cursor] values
      root=DefaultRootWindow(build[window->ID-1].display);

      if(XQueryPointer(
          build[window->ID-1].display, root, &root, &root,
          &cursorMain.x, &cursorMain.y, &cursorLocal.x, &cursorLocal.y, &mask)){
        przecinek.cursor.x=cursorMain.x;
        przecinek.cursor.y=cursorMain.y;
      }
      else{
        przecinek.cursor.x=0;
        przecinek.cursor.y=0;
      }

      // Update [przecinek] [windowCount]
      przecinek.windowCount=windowCount;

      // Update [window] [resizable]
      window->resizable=build[window->ID-1].resizable;

      // Update [przecinek] [keyCaps]
      XGetKeyboardControl(build[window->ID-1].display, &keyboardState);
      przecinek.keyCaps=(keyboardState.led_mask&(1<<1)!=0);

      // Manage key press [currentEvent]
      if(currentEvent.type==KeyPress){
        if(przecinek.key[currentEvent.xkey.keycode]==0){
          przecinek.key[currentEvent.xkey.keycode]=1;

          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]==0){
              input[button]=currentEvent.xkey.keycode;
              break;
            }
          }
        }
      }
      if(currentEvent.type==ButtonPress){
        if(currentEvent.xbutton.button==8){ currentEvent.xbutton.button=4; }
        if(currentEvent.xbutton.button==9){ currentEvent.xbutton.button=5; }

        if(przecinek.key[currentEvent.xbutton.button]==0){
          przecinek.key[currentEvent.xbutton.button]=1;

          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]==0){
              input[button]=currentEvent.xbutton.button;
              break;
            }
          }
        }
      }

      // Manage key release [currentEvent]
      if(currentEvent.type==KeyRelease){
        if(XEventsQueued(build[window->ID-1].display, QueuedAfterReading)){
          XPeekEvent(build[window->ID-1].display, &currentReport);

          if(currentReport.type==KeyPress &&
              currentReport.xkey.time==currentEvent.xkey.time &&
              currentReport.xkey.keycode==currentEvent.xkey.keycode){

            // System released [key]
            for(unsigned short int button=0; button<KEY_MAX; button+=1){
              if(input[button]==currentEvent.xkey.keycode){
                przecinek.key[currentEvent.xkey.keycode]=2;
                break;
              }
              else if(input[button]==0){ break; }
            }

            XNextEvent(build[window->ID-1].display, &currentEvent);
          }

          // Fully released [key]
          else{
            przecinek.key[currentEvent.xkey.keycode]=0;

            inputChange=false;
            for(unsigned short int button=0; button<KEY_MAX; button+=1){
              if(input[button]==currentEvent.xkey.keycode || inputChange==true){
                input[button]=input[button+1];
                inputChange=true; 

                if(input[button]==0){ break; }
              }
            }
          }
        }
        // Fully released [key]
        else{
          przecinek.key[currentEvent.xkey.keycode]=0;

          inputChange=false;
          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]==currentEvent.xkey.keycode || inputChange==true){
              input[button]=input[button+1];
              inputChange=true;

              if(input[button]==0){ break; }
            }
          }
        }
      }
      if(currentEvent.type==ButtonRelease){
        if(currentEvent.xbutton.button==8){ currentEvent.xbutton.button=4; }
        if(currentEvent.xbutton.button==9){ currentEvent.xbutton.button=5; }

        if(XEventsQueued(build[window->ID-1].display, QueuedAfterReading)){
          XPeekEvent(build[window->ID-1].display, &currentReport);

          if(currentReport.type==ButtonPress &&
              currentReport.xbutton.time==currentEvent.xbutton.time &&
              currentReport.xbutton.button==currentEvent.xbutton.button){

            // System released [key]
            for(unsigned short int button=0; button<KEY_MAX; button+=1){
              if(input[button]==currentEvent.xbutton.button){
                przecinek.key[currentEvent.xbutton.button]=2;
                break;
              }
              else if(input[button]==0){ break; }
            }

            XNextEvent(build[window->ID-1].display, &currentEvent);
          }

          // Fully released [key]
          else{
            przecinek.key[currentEvent.xbutton.button]=0;

            inputChange=false;
            for(unsigned short int button=0; button<KEY_MAX; button+=1){
              if(input[button]==currentEvent.xbutton.button || inputChange==true){
                input[button]=input[button+1];
                inputChange=true;

                if(input[button]==0){ break; }
              }
            }
          }
        }
        // Fully released [key]
        else{
          przecinek.key[currentEvent.xbutton.button]=0;

          inputChange=false;
          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]==currentEvent.xbutton.button || inputChange==true){
              input[button]=input[button+1];
              inputChange=true;

              if(input[button]==0){ break; }
            }
          }
        }
      }

      // Manage fullscreen change [currentEvent]
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

        // Resize [window]
        XResizeWindow(build[window->ID-1].display, build[window->ID-1].base,
          window->width, window->height
        );

        // Update [build] [surface]
        cairo_surface_destroy(build[window->ID-1].surface);
        build[window->ID-1].surface=cairo_xlib_surface_create(
          build[window->ID-1].display, build[window->ID-1].base,
          DefaultVisual(build[window->ID-1].display, build[window->ID-1].screen),
          window->width, window->height
        );

        // Update [window] and [build] position parameters
        window->x=0;
        window->y=0;
        build[window->ID-1].x=window->x;
        build[window->ID-1].y=window->y;

        // Move [window]
        XMoveWindow(build[window->ID-1].display, build[window->ID-1].base, window->x, window->y);

        build[window->ID-1].fullScreen=true;
        build[window->ID-1].screenRefresh=true;
      }
      else if(window->fullScreen==false && build[window->ID-1].fullScreen==true){
        // Change [window] and [build] size parameters to the backuped ones
        window->width=build[window->ID-1].widthBac;
        window->height=build[window->ID-1].heightBac;
        build[window->ID-1].width=window->width;
        build[window->ID-1].height=window->height;

        // Resize [window]
        XResizeWindow(
          build[window->ID-1].display, build[window->ID-1].base, window->width, window->height
        );

        // Update [build] [surface]
        cairo_surface_destroy(build[window->ID-1].surface);
        build[window->ID-1].surface=cairo_xlib_surface_create(
          build[window->ID-1].display, build[window->ID-1].base,
          DefaultVisual(build[window->ID-1].display, build[window->ID-1].screen),
          window->width, window->height
        );

        // Change [window] and [build] position parameters to the backuped ones
        window->x=build[window->ID-1].xBac;
        window->y=build[window->ID-1].yBac;
        build[window->ID-1].x=window->x;
        build[window->ID-1].y=window->y;

        // Move [window]
        XMoveWindow(build[window->ID-1].display, build[window->ID-1].base, window->x, window->y);

        build[window->ID-1].fullScreen=false;
        build[window->ID-1].screenRefresh=true;
      }
      else{ build[window->ID-1].screenRefresh=false; }

      if(window->fullScreen==false){
        // Manage position change [currentEvent]
        if(build[window->ID-1].x!=window->x || build[window->ID-1].y!=window->y){
          // Check [window] [x] value
          if(window->x<(-WINDOW_POS_MAX)){
            if(przecinek.debug==true){
printf(
  "[pWB05] \"Window x value is too low\" (changing from: %i to %i),\n",
  window->x, (-WINDOW_POS_MAX)
);
              fflush(stdout);
            }

            // Change [window] [x] value
            window->x=(-WINDOW_POS_MAX);
          }
          else if(window->x>WINDOW_POS_MAX){
            if(przecinek.debug==true){
printf(
  "[pWB06] \"Window x value is too big\" (changing from: %i to %i),\n",
  window->x, WINDOW_POS_MAX
);
              fflush(stdout);
            }

            // Change [window] [x] value
            window->x=WINDOW_POS_MAX;
          }

          // Check [window] [y] value
          if(window->y<(-WINDOW_POS_MAX)){
            if(przecinek.debug==true){
printf(
  "[pWB07] \"Window y value is too low\" (changing from: %i to %i),\n",
  window->y, (-WINDOW_POS_MAX)
);
              fflush(stdout);
            }

            // Change [window] [y] value
            window->y=(-WINDOW_POS_MAX);
          }
          else if(window->y>WINDOW_POS_MAX){
            if(przecinek.debug==true){
printf(
  "[pWB08] \"Window y value is too big\" (changing from: %i to %i),\n",
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

          // Change [przecinek] [key] values to `0`
          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]!=0){
              przecinek.key[input[button]]=0;
              input[button]=0;
            }
            else{ break; }
          }

          // Move [window]
          XMoveWindow(build[window->ID-1].display, build[window->ID-1].base, window->x, window->y);
        }
        else if(currentEvent.type==ConfigureNotify && window->x!=currentEvent.xconfigure.x){
          // Update [build] and [window] position parameters
          window->x=currentEvent.xconfigure.x;
          window->y=currentEvent.xconfigure.y;
          build[window->ID-1].x=window->x;
          build[window->ID-1].y=window->y;

          // Change [przecinek] [key] values to `0`
          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]!=0){
              przecinek.key[input[button]]=0;
              input[button]=0;
            }
            else{ break; }
          }
        }

        // Manage size change [currentEvent]
        if(build[window->ID-1].width!=window->width || build[window->ID-1].height!=window->height){
          // Check [window] [width] value
          if(window->width<WINDOW_WIDTH_MIN){
            if(przecinek.debug==true){
printf(
  "[pWB01] \"Window width value is too low\" (changing from: %i to %i),\n",
  window->width, WINDOW_WIDTH_MIN
);
              fflush(stdout);
            }

            // Change [window] [width] value
            window->width=WINDOW_WIDTH_MIN;
          }
          else if(window->width>WINDOW_WIDTH_MAX){
            if(przecinek.debug==true){
printf(
  "[pWB02] \"Window width value is too big\" (changing from: %i to %i),\n",
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
  "[pWB03] \"Window height value is too low\" (changing from: %i to %i),\n",
  window->height, WINDOW_HEIGHT_MIN
);
              fflush(stdout);
            }

            // Change [window] [height] value
            window->height=WINDOW_HEIGHT_MIN;
          }
          else if(window->height>WINDOW_HEIGHT_MAX){
            if(przecinek.debug==true){
printf(
  "[pWB04] \"Window height value is too big\" (changing from: %i to %i),\n",
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

          // Change [przecinek] [key] values to `0`
          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]!=0){
              przecinek.key[input[button]]=0;
              input[button]=0;
            }
            else{ break; }
          }

          // Resize [window]
          XResizeWindow(
            build[window->ID-1].display, build[window->ID-1].base,
            window->width, window->height
          );

          // Update [build] [surface]
          cairo_surface_destroy(build[window->ID-1].surface);
          build[window->ID-1].surface=cairo_xlib_surface_create(
            build[window->ID-1].display, build[window->ID-1].base,
            DefaultVisual(build[window->ID-1].display, build[window->ID-1].screen),
            window->width, window->height
          );

          build[window->ID-1].sizeRefresh=true;
        }
        else if(currentEvent.type==ConfigureNotify && window->width!=currentEvent.xconfigure.width){
          // Update [window] and [build] size parameters
          window->width=currentEvent.xconfigure.width;
          window->height=currentEvent.xconfigure.height;
          build[window->ID-1].width=currentEvent.xconfigure.width;
          build[window->ID-1].height=currentEvent.xconfigure.height;

          // Change [przecinek] [key] values to `0`
          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]!=0){
              przecinek.key[input[button]]=0;
              input[button]=0;
            }
            else{ break; }
          }

          // Update [build] [surface]
          cairo_surface_destroy(build[window->ID-1].surface);
          build[window->ID-1].surface=cairo_xlib_surface_create(
            build[window->ID-1].display, build[window->ID-1].base,
            DefaultVisual(build[window->ID-1].display, build[window->ID-1].screen),
            window->width, window->height
          );

          build[window->ID-1].sizeRefresh=true;
        }
      }
      else{ build[window->ID-1].sizeRefresh=false; }

      // Manage focus in [currentEvent]
      if(currentEvent.type==FocusIn){
        build[window->ID-1].focus=true;

        for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
          if(current==window->ID-1){ continue; }

          if(build[current].exist==true){
            if(build[current].focus==true){ build[current].focus=false; }
          }
          else{ break; }
        }
      }

      // Manage focus out [currentEvent]
      if(currentEvent.type==FocusOut){ build[window->ID-1].focus=false; }

      // Update [window] [focus]
      if(window->focus!=build[window->ID-1].focus){
        window->focus=build[window->ID-1].focus;

        // Change [przecinek] [key] values to `0`
        for(unsigned short int button=0; button<KEY_MAX; button+=1){
          if(input[button]!=0){
            przecinek.key[input[button]]=0;
            input[button]=0;
          }
          else{ break; }
        }
      }

      if(wcscmp(window->title, build[window->ID-1].titleW)!=0){
        // Update [build] [title] value
        wcscpy(build[window->ID-1].titleW, window->title);
        wcstombs(build[window->ID-1].title, window->title, TITLE_MAX);

        // Change [window] [title]
        XStoreName(build[window->ID-1].display, build[window->ID-1].base, build[window->ID-1].title);
      }

      if(window->resizable==true){
        // Check [window] [widthMin] value
        if(build[window->ID-1].widthMin!=window->widthMin){
          if(window->widthMin<WINDOW_WIDTH_MIN){
            if(przecinek.debug==true){
printf(
  "[pWB09] \"Window widthMin value is too low\" (changing from: %i to %i),\n",
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
  "[pWB10] \"Window widthMin value is too big\" (changing from: %i to %i),\n",
  window->widthMin, WINDOW_WIDTH_MAX
);
              fflush(stdout);
            }

            // Change [window] [widthMin] value
            window->widthMin=WINDOW_WIDTH_MAX;
          }

          // Update other [build] values
          build[window->ID-1].limitChange=true;
          build[window->ID-1].widthMin=window->widthMin;
          build[window->ID-1].sizeHint.min_width=window->widthMin;
        }

        // Check [window] [heightMin] value
        if(build[window->ID-1].heightMin!=window->heightMin){
          if(window->heightMin<WINDOW_HEIGHT_MIN){
            if(przecinek.debug==true){
printf(
  "[pWB11] \"Window heightMin value is too low\" (changing from: %i to %i),\n",
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
  "[pWB12] \"Window heightMin value is too big\" (changing from: %i to %i),\n",
  window->heightMin, WINDOW_HEIGHT_MAX
);
              fflush(stdout);
            }

            // Change [window] [heightMin] value
            window->heightMin=WINDOW_HEIGHT_MAX;
          }

          // Update other [build] values
          build[window->ID-1].limitChange=true;
          build[window->ID-1].heightMin=window->heightMin;
          build[window->ID-1].sizeHint.min_height=window->heightMin;
        }

        // Check [window] [widthMax] value
        if(build[window->ID-1].widthMax!=window->widthMax){
          if(window->widthMax<WINDOW_WIDTH_MIN){
            if(przecinek.debug==true){
printf(
  "[pWB13] \"Window widthMax value is too low\" (changing from: %i to %i),\n",
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
  "[pWB14] \"Window widthMax value is too big\" (changing from: %i to %i),\n",
  window->widthMax, WINDOW_WIDTH_MAX
);
              fflush(stdout);
            }

            // Change [window] [widthMax] value
            window->widthMax=WINDOW_WIDTH_MAX;
          }

          // Update other [build] values
          build[window->ID-1].limitChange=true;
          build[window->ID-1].widthMax=window->widthMax;
          build[window->ID-1].sizeHint.max_width=window->widthMax;
        }

        // Check [window] [heightMax] value
        if(build[window->ID-1].heightMax!=window->heightMax){
          if(window->heightMax<WINDOW_HEIGHT_MIN){
            if(przecinek.debug==true){
printf(
  "[pWB15] \"Window heightMax value is too low\" (changing from: %i to %i),\n",
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
  "[pWB16] \"Window heightMax value is too big\" (changing from: %i to %i),\n",
  window->heightMax, WINDOW_HEIGHT_MAX
);
              fflush(stdout);
            }

            // Change [window] [heightMax] value
            window->heightMax=WINDOW_HEIGHT_MAX;
          }

          // Update other [build] values
          build[window->ID-1].limitChange=true;
          build[window->ID-1].heightMax=window->heightMax;
          build[window->ID-1].sizeHint.max_height=window->heightMax;
        }


        // Check [window] [widthMin] and [widthMax] values
        if(build[window->ID-1].widthMin>build[window->ID-1].widthMax){
          if(przecinek.debug==true){
printf(
  "[pWB17] \"Window widthMin value is bigger than widthMax\" (changing from: %i to %i),\n",
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
  "[pWB18] \"Window heightMin value is bigger than heightMax\" (changing from: %i to %i),\n",
  window->heightMin, window->heightMax
);
            fflush(stdout);
          }

          // Change [window] and [build] [heightMin] value
          window->heightMin=window->heightMax;
          build[window->ID-1].heightMin=build[window->ID-1].heightMax;
        }

        // Correct [window] and [build] [width] value
        if(build[window->ID-1].widthMin==build[window->ID-1].widthMax &&
            build[window->ID-1].width!=build[window->ID-1].widthMin){

          window->width=build[window->ID-1].widthMin;
          build[window->ID-1].width=build[window->ID-1].widthMin;

          // Change [przecinek] [key] values to `0`
          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]!=0){
              przecinek.key[input[button]]=0;
              input[button]=0;
            }
            else{ break; }
          }

          // Resize [window]
          XResizeWindow(
            build[window->ID-1].display, build[window->ID-1].base, window->width, window->height
          );

          // Update [build] [surface]
          cairo_surface_destroy(build[window->ID-1].surface);
          build[window->ID-1].surface=cairo_xlib_surface_create(
            build[window->ID-1].display, build[window->ID-1].base,
            DefaultVisual(build[window->ID-1].display, build[window->ID-1].screen),
            window->width, window->height
          );
        }

        // Correct [window] and [build] [height] value
        if(build[window->ID-1].heightMin==build[window->ID-1].heightMax &&
            build[window->ID-1].height!=build[window->ID-1].heightMin){

          window->height=build[window->ID-1].heightMin;
          build[window->ID-1].height=build[window->ID-1].heightMin;

          // Change [przecinek] [key] values to `0`
          for(unsigned short int button=0; button<KEY_MAX; button+=1){
            if(input[button]!=0){
              przecinek.key[input[button]]=0;
              input[button]=0;
            }
            else{ break; }
          }

          // Resize [window]
          XResizeWindow(
            build[window->ID-1].display, build[window->ID-1].base, window->height, window->height
          );

          // Update [build] [surface]
          cairo_surface_destroy(build[window->ID-1].surface);
          build[window->ID-1].surface=cairo_xlib_surface_create(
            build[window->ID-1].display, build[window->ID-1].base,
            DefaultVisual(build[window->ID-1].display, build[window->ID-1].screen),
            window->width, window->height
          );
        }
      }
      else{
        window->widthMin=window->width;
        window->heightMin=window->height;
        window->widthMax=window->width;
        window->heightMax=window->height;
      }
    }

    // Switch [window] buffers
    if(currentEvent.type!=ConfigureNotify){
      cairo_surface_show_page(build[window->ID-1].surface);
    }

    // Refresh [window]
    XFlush(build[window->ID-1].display);

    // Update [window] [sizeHint]
    if(build[window->ID-1].limitChange==true){
      XSetWMNormalHints(
        build[window->ID-1].display, build[window->ID-1].base, &build[window->ID-1].sizeHint
      );

      build[window->ID-1].limitChange=false;
    }

    // Update [frameCount] and sleep
    build[window->ID-1].frameCount+=1;
    usleep((1000000/windowCount)/przecinek.frameLimit);

    // Set and calculate current time
    gettimeofday(&build[window->ID-1].frameEnd, NULL);
    build[window->ID-1].frameMax=
      (double)((build[window->ID-1].frameEnd.tv_sec-build[window->ID-1].frameStart.tv_sec)+
      (build[window->ID-1].frameEnd.tv_usec-build[window->ID-1].frameStart.tv_usec)/1000000.0);

    if(build[window->ID-1].frameMax>=1.0){
      // Set [window] [frameCount] and reset loop
      window->frameCount=build[window->ID-1].frameCount;

      build[window->ID-1].frameCount=0;
      build[window->ID-1].frameMax=0;
      build[window->ID-1].frameStart=build[window->ID-1].frameEnd;
    }
  }
  else if(przecinek.debug==true){
printf(
  "[pEG02] \"Could not handle window\" (window is closed),\n"
);
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
void pWindowDestroy(pWindow *window){
  if(window->ID!=0){
    // Send kill [currentAction]
    currentAction.type=ClientMessage;
    currentAction.xclient.window=build[window->ID-1].base;
    currentAction.xclient.message_type=build[window->ID-1].delete;
    currentAction.xclient.format=32;
    currentAction.xclient.data.l[0]=build[window->ID-1].delete;
    currentAction.xclient.data.l[1]=CurrentTime;

    XSendEvent(build[window->ID-1].display, build[window->ID-1].base, False, NoEventMask, &currentAction);
  }
  else if(przecinek.debug==true){
printf(
  "[pWG03] \"Window is already closed\",\n"
);
    fflush(stdout);
  }

  return;
}

/****************************************************************
 * |\_____/| pObjectCreate()
 * | .     |
 * |     . | In: us_int [vertice], [width], [height]
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
      // Set [object] [ID] and reset [object]
      object.ID=current+1;
      pDebugObjectReset(&object);

      break;
    }
    else if(current==OBJECT_MAX-1){
      if(przecinek.debug==true){
printf(
  "[pEO01] \"Too many objects were created\" (limit: %i),\n",
  OBJECT_MAX
);
        fflush(stdout);
      }

      // Reset and return [object]
      pDebugObjectReset(&object);
      object.ID=0;

      return object;
    }
  }

  // Check if Przecinek is initialized
  if(setup==false){
printf(
  "[pEG01] \"Could not create object\" (Przecinek is not initialized),\n"
);
    fflush(stdout);

    // Reset and return [object]
    pDebugObjectReset(&object);
    object.ID=0;

    return object;
  }

  // Check [vertice] value
  if(vertice<OBJECT_VERTICE_MIN){
    if(przecinek.debug==true){
printf(
  "[pWO01] \"Object vertice value is too low\" (changing from: %i to %i),\n",
  vertice, OBJECT_VERTICE_MIN
);
      fflush(stdout);
    }

    // Change [vertice] value
    vertice=OBJECT_VERTICE_MIN;
  }
  else if(vertice>OBJECT_VERTICE_MAX){
    if(przecinek.debug==true){
printf(
  "[pWO02] \"Object vertice value is too big\" (changing from: %i to %i),\n",
  vertice, OBJECT_VERTICE_MIN
);
      fflush(stdout);
    }

    // Change [vertice] value
    vertice=OBJECT_VERTICE_MAX;
  }

  // Check [width] value
  if(width<OBJECT_WIDTH_MIN){
    if(przecinek.debug==true){
printf(
  "[pWO03] \"Object width value is too low\" (changing from: %i to %i),\n",
  width, OBJECT_WIDTH_MIN
);
      fflush(stdout);
    }

    // Change [width] value
    width=OBJECT_WIDTH_MIN;
  }
  else if(width>OBJECT_WIDTH_MAX){
    if(przecinek.debug==true){
printf(
  "[pWO04] \"Object width value is too big\" (changing from: %i to %i),\n",
  width, OBJECT_WIDTH_MAX
);
      fflush(stdout);
    }

    // Change [width] value
    width=OBJECT_WIDTH_MAX;
  }

  // Check [height] value
  if(height<OBJECT_HEIGHT_MIN){
    if(przecinek.debug==true){
printf(
  "[pWO05] \"Object height value is too low\" (changing from: %i to %i),\n",
  height, OBJECT_HEIGHT_MIN
);
      fflush(stdout);
    }

    // Change [height] value
    height=OBJECT_HEIGHT_MIN;
  }
  else if(height>OBJECT_HEIGHT_MAX){
    if(przecinek.debug==true){
printf(
  "[pWO06] \"Object height value is too big\" (changing from: %i to %i),\n",
  height, OBJECT_HEIGHT_MAX
);
      fflush(stdout);
    }

    // Change [height] value
    height=OBJECT_HEIGHT_MAX;
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
  // Simple collision check on two squares
  if(figure[object1->ID-1].vertice==4 && figure[object2->ID-1].vertice==4 &&
      figure[object1->ID-1].rotation==0 && figure[object2->ID-1].rotation==0){

    return(
      object1->x<object2->x+object2->width &&
      object1->x+object1->width>object2->x &&
      object1->y<object2->y+object2->height &&
      object1->y+object1->height>object2->y
    );
  }

  // Reset some variables
  distanceMin.x=OBJECT_WIDTH_MAX;
  distanceMin.y=OBJECT_HEIGHT_MAX;
  distanceMax.x=(-OBJECT_WIDTH_MAX);
  distanceMax.y=(-OBJECT_HEIGHT_MAX);
  distance=OBJECT_TRIANGLE;

  // Calculate [object1] [centerA]
  centerA.x=figure[object1->ID-1].x+(figure[object1->ID-1].width/2);
  centerA.y=figure[object1->ID-1].y+(figure[object1->ID-1].height/2);

  // Calculate [object2] [centerB]
  centerB.x=figure[object2->ID-1].x+(figure[object2->ID-1].width/2);
  centerB.y=figure[object2->ID-1].y+(figure[object2->ID-1].height/2);

  for(unsigned short int current=0; current<figure[object1->ID-1].vertice; current+=1){
    // Calculate [distanceMin] values
    if(distanceMin.x>figure[object1->ID-1].point[current].x){
      distanceMin.x=figure[object1->ID-1].point[current].x;
    }
    if(distanceMin.y>figure[object1->ID-1].point[current].y){
      distanceMin.y=figure[object1->ID-1].point[current].y;
    }

    // Calculate [distanceMax] values
    if(distanceMax.x<figure[object1->ID-1].point[current].x){
      distanceMax.x=figure[object1->ID-1].point[current].x;
    }
    if(distanceMin.y<figure[object1->ID-1].point[current].y){
      distanceMax.y=figure[object1->ID-1].point[current].y;
    }

    // Calculate [ratio] value
    ratio=sqrt(pow((centerB.x-figure[object1->ID-1].point[current].x), 2)+
      pow((centerB.y-figure[object1->ID-1].point[current].y), 2));

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
  distanceMin.x=OBJECT_WIDTH_MAX;
  distanceMin.y=OBJECT_HEIGHT_MAX;
  distanceMax.x=(-OBJECT_WIDTH_MAX);
  distanceMax.y=(-OBJECT_HEIGHT_MAX);
  distance=OBJECT_TRIANGLE;

  for(unsigned short int current=0; current<figure[object2->ID-1].vertice; current+=1){
    // Calculate [distanceMin] values
    if(distanceMin.x>figure[object2->ID-1].point[current].x){
      distanceMin.x=figure[object2->ID-1].point[current].x;
    }
    if(distanceMin.y>figure[object2->ID-1].point[current].y){
      distanceMin.y=figure[object2->ID-1].point[current].y;
    }

    // Calculate [distanceMax] values
    if(distanceMax.x<figure[object2->ID-1].point[current].x){
      distanceMax.x=figure[object2->ID-1].point[current].x;
    }
    if(distanceMax.y<figure[object2->ID-1].point[current].y){
      distanceMax.y=figure[object2->ID-1].point[current].y;
    }

    // Calculate [ratio] value
    ratio=sqrt(pow((centerA.x-figure[object2->ID-1].point[current].x), 2)+
      pow((centerA.y-figure[object2->ID-1].point[current].y), 2));

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
  if((object1->x<object2->x+object2->width) &&
      (object1->x+object1->width>object2->x) &&
      (object1->y<object2->y+object2->height) &&
      (object1->y+object1->height>object2->y)==true){
    if(
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].x, figure[object1->ID-1].point[pointA[0]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[1]].x, figure[object1->ID-1].point[pointA[1]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].x, figure[object2->ID-1].point[pointB[0]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[1]].x, figure[object2->ID-1].point[pointB[1]].y },
  centerB)==false &&
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].x, figure[object1->ID-1].point[pointA[0]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[1]].x, figure[object1->ID-1].point[pointA[1]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].x, figure[object2->ID-1].point[pointB[0]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[2]].x, figure[object2->ID-1].point[pointB[2]].y },
  centerB)==false &&
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].x, figure[object1->ID-1].point[pointA[0]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[2]].x, figure[object1->ID-1].point[pointA[2]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].x, figure[object2->ID-1].point[pointB[0]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[1]].x, figure[object2->ID-1].point[pointB[1]].y },
  centerB)==false &&
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].x, figure[object1->ID-1].point[pointA[0]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[2]].x, figure[object1->ID-1].point[pointA[2]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].x, figure[object2->ID-1].point[pointB[0]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[2]].x, figure[object2->ID-1].point[pointB[2]].y },
  centerB)==false
    ){

      if(figure[object2->ID-1].vertice>=10){
        if(
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].x, figure[object1->ID-1].point[pointA[0]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[1]].x, figure[object1->ID-1].point[pointA[1]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[1]].x, figure[object2->ID-1].point[pointB[1]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[3]].x, figure[object2->ID-1].point[pointB[3]].y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].x, figure[object1->ID-1].point[pointA[0]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[1]].x, figure[object1->ID-1].point[pointA[1]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[2]].x, figure[object2->ID-1].point[pointB[2]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[4]].x, figure[object2->ID-1].point[pointB[4]].y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].x, figure[object1->ID-1].point[pointA[0]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[2]].x, figure[object1->ID-1].point[pointA[2]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[1]].x, figure[object2->ID-1].point[pointB[1]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[3]].x, figure[object2->ID-1].point[pointB[3]].y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[0]].x, figure[object1->ID-1].point[pointA[0]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[2]].x, figure[object1->ID-1].point[pointA[2]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[2]].x, figure[object2->ID-1].point[pointB[2]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[4]].x, figure[object2->ID-1].point[pointB[4]].y },
  centerB)==true
        ){

          // Collision found, return `true`
          return true;
        }
      }
      if(figure[object1->ID-1].vertice>=10){
        if(
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[1]].x, figure[object1->ID-1].point[pointA[1]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[3]].x, figure[object1->ID-1].point[pointA[3]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].x, figure[object2->ID-1].point[pointB[0]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[1]].x, figure[object2->ID-1].point[pointB[1]].y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[1]].x, figure[object1->ID-1].point[pointA[1]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[3]].x, figure[object1->ID-1].point[pointA[3]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].x, figure[object2->ID-1].point[pointB[0]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[2]].x, figure[object2->ID-1].point[pointB[2]].y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[2]].x, figure[object1->ID-1].point[pointA[2]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[4]].x, figure[object1->ID-1].point[pointA[4]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].x, figure[object2->ID-1].point[pointB[0]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[1]].x, figure[object2->ID-1].point[pointB[1]].y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[2]].x, figure[object1->ID-1].point[pointA[2]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[4]].x, figure[object1->ID-1].point[pointA[4]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[0]].x, figure[object2->ID-1].point[pointB[0]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[2]].x, figure[object2->ID-1].point[pointB[2]].y },
  centerB)==true
        ){

          // Collision found, return `true`
          return true;
        }
      }
      if(figure[object1->ID-1].vertice>=10 && figure[object2->ID-1].vertice>=10){
        if(
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[1]].x, figure[object1->ID-1].point[pointA[1]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[3]].x, figure[object1->ID-1].point[pointA[3]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[1]].x, figure[object2->ID-1].point[pointB[1]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[3]].x, figure[object2->ID-1].point[pointB[3]].y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[1]].x, figure[object1->ID-1].point[pointA[1]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[3]].x, figure[object1->ID-1].point[pointA[3]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[2]].x, figure[object2->ID-1].point[pointB[2]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[4]].x, figure[object2->ID-1].point[pointB[4]].y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[2]].x, figure[object1->ID-1].point[pointA[2]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[4]].x, figure[object1->ID-1].point[pointA[4]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[1]].x, figure[object2->ID-1].point[pointB[1]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[3]].x, figure[object2->ID-1].point[pointB[3]].y },
  centerB)==true ||
pDebugObjectTriangle(
  (pPosition){ figure[object1->ID-1].point[pointA[2]].x, figure[object1->ID-1].point[pointA[2]].y },
  (pPosition){ figure[object1->ID-1].point[pointA[4]].x, figure[object1->ID-1].point[pointA[4]].y },
  centerA,
  (pPosition){ figure[object2->ID-1].point[pointB[2]].x, figure[object2->ID-1].point[pointB[2]].y },
  (pPosition){ figure[object2->ID-1].point[pointB[4]].x, figure[object2->ID-1].point[pointB[4]].y },
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
  if(object->ID!=0){
    // Fully reset [object]
    pDebugObjectReset(object);
    object->ID=0;
  }
  else if(przecinek.debug==true){
printf(
  "[pWG08] \"Object is already closed\",\n"
);
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
      // Set [font] [ID] and reset [font]
      font.ID=current+1;
      pDebugFontReset(&font);

      break;
    }
    else if(current==FONT_MAX-1){
      if(przecinek.debug==true){
printf(
  "[pEF01] \"Too many fonts were created\" (limit: %i),\n",
  FONT_MAX
);
        fflush(stdout);
      }

      // Reset and return [font]
      pDebugFontReset(&font);
      font.ID=0;

      return font;
    }
  }

  // Check if Przecinek is initialized
  if(setup==false){
printf(
  "[pEG01] \"Could not create font\" (Przecinek is not initialized),\n"
);
    fflush(stdout);

    // Reset and return [font]
    pDebugFontReset(&font);
    font.ID=0;

    return font;
  }

  // Check [size] value
  if(size<FONT_SIZE_MIN){
    if(przecinek.debug==true){
printf(
  "[pWF01] \"Font size value is too low\" (changing from: %i to %i),\n",
  size, FONT_SIZE_MIN
);
      fflush(stdout);
    }

    // Change [size] value
    size=FONT_SIZE_MIN;
  }
  else if(size>FONT_SIZE_MAX){
    if(przecinek.debug==true){
printf(
  "[pWF02] \"Font size value is too big\" (changing from: %i to %i),\n",
  size, FONT_SIZE_MAX
);
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
  wcscpy(view[font.ID-1].directoryW, directory);
  wcstombs(view[font.ID-1].directory, directory, FONT_NAME_MAX);

  // Check if [directory] exists
  if(stat(view[font.ID-1].directory, &status)!=0){
    if(przecinek.debug==true){
printf(
  "[pEF02] \"Could not load font\",\n"
);
      fflush(stdout);
    }

    // Reset and return [font]
    pDebugFontReset(&font);
    font.ID=0;

    return font;
  }

  // Load [view] [face] and check if font loads properly
  for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
    if(FT_New_Face(freeType, view[font.ID-1].directory, 0, &view[font.ID-1].face[current])){
      if(przecinek.debug==true){
printf(
  "[pEFx1] \"Could not create X11 font\",\n"
);
        fflush(stdout);
      }

      // Reset and return [font]
      pDebugFontReset(&font);
      font.ID=0;

      return font;
    }
  }

  // Load [view] [cairoFace]
  for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
    view[font.ID-1].cairoFace[current]=cairo_ft_font_face_create_for_ft_face(
      view[font.ID-1].face[current], FT_LOAD_NO_HINTING
    );
    FT_Set_Pixel_Sizes(view[font.ID-1].face[current], 0, view[font.ID-1].size);
  }

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
  if(font->ID!=0){
    // Fully reset [font]
    pDebugFontReset(font);
    font->ID=0;
  }
  else if(przecinek.debug==true){
printf(
  "[pWG09] \"Font is already closed\",\n"
);
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
      // Set [text] [ID] and reset [text]
      text.ID=current+1;
      pDebugTextReset(&text);

      break;
    }
    else if(current==TEXT_MAX-1){
      if(przecinek.debug==true){
printf(
  "[pET01] \"Too many texts were created\" (limit: %i),\n",
  TEXT_MAX
);
        fflush(stdout);
      }

      // Reset and return [text]
      pDebugTextReset(&text);
      text.ID=0;

      return text;
    }
  }

  // Check if Przecinek is initialized
  if(setup==false){
printf(
  "[pEG01] \"Could not create text\" (Przecinek is not initialized),\n"
);
    fflush(stdout);

    // Reset and return [text]
    pDebugTextReset(&text);
    text.ID=0;

    return text;
  }

  // Set [text] value
  wcscpy(text.value, value);

  // Set [code] values
  code[text.ID-1].exist=true;

  wcscpy(code[text.ID-1].valueW, value);
  wcstombs(code[text.ID-1].value, value, sizeof(code[text.ID-1].value));

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
  if(text->ID!=0){
    // Fully reset [text]
    pDebugTextReset(text);
    text->ID=0;
  }
  else if(przecinek.debug==true){
printf(
  "[pWG10] \"Text is already closed\",\n"
);
    fflush(stdout);
  }

  return;
}
