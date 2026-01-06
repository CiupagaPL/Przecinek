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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xrender.h>

#include <ft2build.h>
#include FT_FREETYPE_H

/********************************
 *  ,______,  Define default
 *  |      |  values
 *  |______|
 * (--------)
 ********************************/
#define WINDOW_MAX 16
#define WINDOW_X_DEF 128
#define WINDOW_Y_DEF 128
#define WINDOW_WIDTH_MIN 256
#define WINDOW_HEIGHT_MIN 256
#define WINDOW_WIDTH_MAX 7680
#define WINDOW_HEIGHT_MAX 4320
#define WINDOW_POS_MAX 32768-WINDOW_WIDTH_MAX
#define WINDOW_POS_CHANGE 65536

#define TITLE_DEF "{,}"
#define TITLE_MAX 256

#define KEY_MAX 256
#define FRAME_MIN 10
#define FRAME_MAX 640

#define FONT_MAX 64
#define FONT_SIZE_MIN 4
#define FONT_SIZE_MAX 512
#define FONT_NAME_MAX 256
#define TEXT_MAX 8192

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

  bool resize;
  char title[TITLE_MAX];
  wchar_t titleWide[TITLE_MAX];
  unsigned short int border;
  bool fullScreen;

  Display *display;
  int screen;
  Window base;
  Pixmap buffer;
  bool limitChange;
  bool sizeRefresh, screenRefresh;

  Atom delete, state;
  XSizeHints sizeHint;
  XSetWindowAttributes attributeBase;
  XWindowAttributes attributeBorder;

  GC graphics;
  XRenderColor color;
  Colormap colorMap;
  XRenderPictFormat *format;
  Picture picture;

  XftDraw *textDraw;
  XftColor textColor;

  XKeyboardState keyboardState;

  struct timeval frameStart, frameEnd;
  unsigned short int frameCount;
  double frameMax;
} pBuildX11;

/********************************
 *  ,______,  Define [pFontX11]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  unsigned short int size;
  char name[FONT_NAME_MAX];
  char directory[FONT_NAME_MAX];

  wchar_t nameWide[FONT_NAME_MAX];
  wchar_t directoryWide[FONT_NAME_MAX];
  char value[FONT_NAME_MAX+42];
  XftFont *base;

  int x, y;
  int yFix;

  FcPattern *pattern;
  FcObjectSet *objectSet;
  FcFontSet *fontSet;

  bool change;
} pFontX11;

/********************************
 *  ,______,  Define [pWindow]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  unsigned short int ID;
  bool active;

  int x, y;
  unsigned short int width, height;
  unsigned short int widthMin, heightMin, widthMax, heightMax;

  bool resize;
  wchar_t title[TITLE_MAX];
  unsigned short int border;
  bool fullScreen;
} pWindow;

/********************************
 *  ,______,  Define [pEvent]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  bool focus;
  unsigned short int frameCount;

  unsigned short int key[KEY_MAX];
  bool keyCaps;
} pEvent;

/********************************
 *  ,______,  Define [pObject]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  int x, y;
  unsigned short int width, height;

  pColor color;
} pObject;

/********************************
 *  ,______,  Define [pFont]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  unsigned int ID;

  unsigned short int size;
  wchar_t name[FONT_NAME_MAX];
  wchar_t directory[FONT_NAME_MAX];

  pColor color;
} pFont;

/********************************
 *  ,______,  Define [pText]
 *  |      |  structure
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  int x, y;

  wchar_t value[TEXT_MAX];
} pText;

// FreeType
FT_Library freeType;
FT_Face face;

// Przecinek
pPrzecinek przecinek={ true, 0, 0 };
bool setup=false;

// Window
unsigned short int activeWinID=0;
unsigned short int winCount=0;
pBuildX11 build[WINDOW_MAX];
XEvent currentEvent, currentReport, currentAction;
Display *display;
int screen;
Window root;
unsigned int mask;

// Cursor
pPosition cursorMain, cursorLocal;

// Event
pEvent change;

// Font
FT_Error FTError;
pFontX11 view[FONT_MAX];
struct stat status;

/****************************************************************
 * |\_____/| pDebugWindowReset() [DEBUG]
 * | .     |
 * |     . | In: pWindow* [window]
 * \ = , = / Out:
 *
 * This function resets [window] and [build] values.
 * Cleared variables depend on [window] [ID].
 * It also changes [winCount] by `-1`.
 ****************************************************************/
void pDebugWindowReset(pWindow *window){
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
  memset(window->title, 0, sizeof(window->title));
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

  build[window->ID-1].resize=false;
  memset(build[window->ID-1].title, 0, sizeof(build[window->ID-1].title));
  memset(build[window->ID-1].titleWide, 0, sizeof(build[window->ID-1].titleWide));
  build[window->ID-1].border=0;
  build[window->ID-1].fullScreen=false;

  build[window->ID-1].display=NULL;
  build[window->ID-1].screen=0;
  build[window->ID-1].base=0;
  build[window->ID-1].buffer=0;
  build[window->ID-1].limitChange=false;
  build[window->ID-1].sizeRefresh=false;
  build[window->ID-1].screenRefresh=false;

  build[window->ID-1].delete=None;
  build[window->ID-1].state=None;

  build[window->ID-1].graphics=NULL;
  build[window->ID-1].colorMap=0;

  build[window->ID-1].textDraw=NULL;
  build[window->ID-1].format=NULL;
  build[window->ID-1].picture=0;

  build[window->ID-1].frameStart.tv_sec=0;
  build[window->ID-1].frameStart.tv_usec=0;
  build[window->ID-1].frameEnd.tv_sec=0;
  build[window->ID-1].frameEnd.tv_usec=0;

  build[window->ID-1].frameCount=0;
  build[window->ID-1].frameMax=0;

  // Change [winCount]
  winCount--;

  return;
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
  memset(font->name, 0, sizeof(font->name));
  memset(font->directory, 0, sizeof(font->directory));

  font->color.red=0;
  font->color.green=0;
  font->color.blue=0;
  font->color.alpha=100;

  // Reset [view] values
  view[font->ID-1].size=0;
  memset(view[font->ID-1].name, 0, sizeof(view[font->ID-1].name));
  memset(view[font->ID-1].directory, 0, sizeof(view[font->ID-1].directory));

  memset(view[font->ID-1].nameWide, 0, sizeof(view[font->ID-1].name));
  memset(view[font->ID-1].directoryWide, 0, sizeof(view[font->ID-1].directory));
  memset(view[font->ID-1].value, 0, sizeof(view[font->ID-1].value));
  view[font->ID-1].base=NULL;

  view[font->ID-1].x=0;
  view[font->ID-1].y=0;
  view[font->ID-1].yFix=0;

  view[font->ID-1].pattern=NULL;
  view[font->ID-1].objectSet=NULL;
  view[font->ID-1].fontSet=NULL;

  view[font->ID-1].change=false;

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
 * It also sets [setup] to `true`.
 ****************************************************************/
void pSetup(bool debug, unsigned short int frameLimit){
  // Initialize FreeType
  FT_Init_FreeType(&freeType);

  // Initialize locale
  setlocale(LC_CTYPE, "");

  // Update [przecinek] [debug] value
  przecinek.debug=debug;

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
 * |\_____/| pWindowCreate()
 * | .     |
 * |     . | In: us_int [width], [height], bool [resize]
 * \ = , = / Out: pWindow
 *
 * This function creates Przecinek window.
 * It adds `1` to [winCount]. It sets [ID] for local [window].
 * It checks if all given parameters are valid.
 * It fills all necessary values for [window] and [build].
 * It sets [window] [title] to default value.
 * It setups [window] buffer for later use.
 * It also saves time when [window] was created,
 * to later calculate frame count.
 ****************************************************************/
pWindow pWindowCreate(unsigned short int width, unsigned short int height, bool resize){
  // Create local [window]
  pWindow window;

  // Change [winCount]
  winCount+=1;

  for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
    if(build[current].width==0 && build[current].height==0){
      // Set [window] [ID] and reset [window]
      window.ID=current+1;
      pDebugWindowReset(&window);

      // Change [winCount]
      winCount+=1;
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
  if(width>WINDOW_WIDTH_MAX){
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
  if(height>WINDOW_HEIGHT_MAX){
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
    XCloseDisplay(build[window.ID-1].display);

    window.ID=0;
    return window;
  }

  // Set [title] value
  XStoreName(build[window.ID-1].display, build[window.ID-1].base, TITLE_DEF);
  mbstowcs(window.title, TITLE_DEF, TITLE_MAX);
  strcpy(build[window.ID-1].title, TITLE_DEF);
  mbstowcs(build[window.ID-1].titleWide, TITLE_DEF, TITLE_MAX);

  // Create [build] [delete] and [state]
  build[window.ID-1].delete=XInternAtom(build[window.ID-1].display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(
    build[window.ID-1].display, build[window.ID-1].base, &build[window.ID-1].delete, 1
  );
  build[window.ID-1].state=XInternAtom(build[window.ID-1].display, "_NET_WM_STATE", False);

  // Get [build] [attributeBorder] values
  XGetWindowAttributes(
    build[window.ID-1].display, build[window.ID-1].base, &build[window.ID-1].attributeBorder
  );

  // Calculate [window] and [build] [border] values
  window.border=
    width-build[window.ID-1].attributeBorder.width+build[window.ID-1].attributeBorder.border_width;
  build[window.ID-1].border=window.border;

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

  // Create [build] [graphics]
  build[window.ID-1].graphics=XCreateGC(
    build[window.ID-1].display, build[window.ID-1].base, 0, NULL
  );
  XSetForeground(
    build[window.ID-1].display, build[window.ID-1].graphics,
    WhitePixel(build[window.ID-1].display, build[window.ID-1].screen
  ));

  // Setup [build] [colorMap]
  build[window.ID-1].colorMap=DefaultColormap(build[window.ID-1].display, build[window.ID-1].screen);

  // Set timer start value
  gettimeofday(&build[window.ID-1].frameStart, NULL);

  // Create [build] [buffer]
  build[window.ID-1].buffer=XCreatePixmap(
    build[window.ID-1].display, build[window.ID-1].base,
    width, height,
    DefaultDepth(build[window.ID-1].display, build[window.ID-1].screen)
  );

  // Return local [window]
  return window;
}

/****************************************************************
 * |\_____/| pWindowDrawObject()
 * | .     |
 * |     . | In: pWindow* [window], pObject* [object]
 * \ = , = / Out:
 *
 * This function draws [object] on [window] buffer.
 * It checks if [object] [color] values are valid.
 * Then it does all the rendering stuff.
 ****************************************************************/
void pWindowDrawObject(pWindow *window, pObject *object){
  if(window->active==true){
    // Check [object] [color] values
    if(object->color.red>255){
      if(przecinek.debug==true){
printf(
  "[pWO01] \"Object red color value is too big\" (changing from: %i to 255),\n",
  object->color.red
);
        fflush(stdout);
      }

      // Correct [object] [color] [red] value
      object->color.red=255;
    }
    if(object->color.green>255){
      if(przecinek.debug==true){
printf(
  "[pWO02] \"Object green color value is too big\" (changing from: %i to 255),\n",
  object->color.green
);
        fflush(stdout);
      }

      // Correct [object] [color] [green] value
      object->color.green=255;
    }
    if(object->color.blue>255){
      if(przecinek.debug==true){
printf(
  "[pWO03] \"Object blue color value is too big\" (changing from: %i to 255),\n",
  object->color.blue
);
        fflush(stdout);
      }

      // Correct [object] [color] [blue] value
      object->color.blue=255;
    }
    if(object->color.alpha>100){
      if(przecinek.debug==true){
printf(
  "[pWO04] \"Object alpha color value is too big\" (changing from: %i to 100),\n",
  object->color.alpha
);
        fflush(stdout);
      }

      // Correct [object] [color] [alpha] value
      object->color.alpha=100;
    }

    // Set [build] [color] values
    build[window->ID-1].color.red=(unsigned char)object->color.red*256;
    build[window->ID-1].color.green=(unsigned char)object->color.green*256;
    build[window->ID-1].color.blue=(unsigned char)object->color.blue*256;
    build[window->ID-1].color.alpha=(unsigned char)object->color.alpha*655;

    // Setup [build] [format] and [picture]
    build[window->ID-1].format=XRenderFindVisualFormat(
      build[window->ID-1].display, DefaultVisual(build[window->ID-1].display, build[window->ID-1].screen)
    );
    build[window->ID-1].picture=XRenderCreatePicture(
      build[window->ID-1].display, build[window->ID-1].buffer, build[window->ID-1].format, 0, NULL
    );

    // Draw on [build] [buffer]
    XRenderFillRectangle(
      build[window->ID-1].display, PictOpOver, build[window->ID-1].picture,
      &build[window->ID-1].color, object->x, object->y, object->width, object->height
    );
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
 * | .     |
 * |     . | In: pWindow* [window], pFont* [font], pText* [text]
 * \ = , = / Out:
 *
 * This function draws [text] in [font] style on [window] buffer.
 * It checks if [font] [color] values are valid.
 * It checks for any changes in [font] values.
 * Then it does all the rendering stuff.
 ****************************************************************/
void pWindowDrawText(pWindow *window, pFont *font, pText *text){
  if(window->active==true){
    if(font->ID!=0){
      // Check [font] [color] values
      if(font->color.red>255){
        if(przecinek.debug==true){
printf(
  "[pWF03] \"Font red color value is too big\" (changing from: %i to 255),\n",
  font->color.red
);
          fflush(stdout);
        }

        // Correct [font] [color] [red] value
        font->color.red=255;
      }
      if(font->color.green>255){
        if(przecinek.debug==true){
printf(
  "[pWF04] \"Font green color value is too big\" (changing from: %i to 255),\n",
  font->color.green
);
          fflush(stdout);
        }

        // Correct [font] [color] [green] value
        font->color.green=255;
      }
      if(font->color.blue>255){
        if(przecinek.debug==true){
printf(
  "[pWF05] \"Font blue color value is too big\" (changing from: %i to 255),\n",
  font->color.blue
);
          fflush(stdout);
        }

        // Correct [font] [color] [blue] value
        font->color.blue=255;
      }
      if(font->color.alpha>100){
        if(przecinek.debug==true){
printf(
  "[pWF06] \"Font alpha color value is too big\" (changing from: %i to 100),\n",
  font->color.alpha
);
          fflush(stdout);
        }

        // Correct [font] [color] [alpha] value
        font->color.alpha=100;
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

          // Change [size] value
          font->size=FONT_SIZE_MIN;
        }
        if(font->size>FONT_SIZE_MAX){
          if(przecinek.debug==true){
printf(
  "[pWF02] \"Font size value is too big\" (changing from: %i to %i),\n",
  font->size, FONT_SIZE_MAX
);
            fflush(stdout);
          }

          // Change [size] value
          font->size=FONT_SIZE_MAX;
        }

        // Update [face] size
        FT_Set_Pixel_Sizes(face, 0, font->size);

        // Refresh [view] values
        view[font->ID-1].size=font->size;
        view[font->ID-1].change=true;
      }

      // Update [view] [name] value
      if(wcscmp(font->name, view[font->ID-1].nameWide)!=0){
        wcscpy(view[font->ID-1].nameWide, font->name);

        view[font->ID-1].change=true;
      }

      // Update [view] [directory] value
      if(wcscmp(font->directory, view[font->ID-1].directoryWide)!=0){
        wcscpy(view[font->ID-1].directoryWide, font->directory);

        view[font->ID-1].change=true;
      }

      // Update [view] [base]
      if(view[font->ID-1].change==true){
        // Check if [directory] exists
        if(stat(view[font->ID-1].directory, &status)!=0){
          if(przecinek.debug==true){
printf(
  "[pEF02] \"Could not load font\",\n"
);
printf(
  "[pEG03] \"Could not draw text\" (font is closed),\n"
);
            fflush(stdout);
          }

          // Reset [font]
          pDebugFontReset(font);
          font->ID=0;

          return;
        }

        // Load [face] and check if font loads properly
        if(FT_New_Face(freeType, view[font->ID-1].directory, 0, &face)){
          if(przecinek.debug==true){
printf(
  "[pEFx1] \"Could not create X11 font\",\n"
);
printf(
  "[pEG03] \"Could not draw text\" (font is closed),\n"
);
            fflush(stdout);
          }

          // Reset [font]
          pDebugFontReset(font);
          font->ID=0;

          return;
        }

        view[font->ID-1].change=false;
      }

      // Update [view] [yFix] value
      view[font->ID-1].yFix=(int)((float)((view[font->ID-1].size*64)/72));

      // Set [view] [x] and [y] values
      view[font->ID-1].x=text->x;
      view[font->ID-1].y=text->y+view[font->ID-1].yFix;

      // Set [build] [color] values
      build[window->ID-1].color.red=(unsigned char)font->color.red*256;
      build[window->ID-1].color.green=(unsigned char)font->color.green*256;
      build[window->ID-1].color.blue=(unsigned char)font->color.blue*256;
      build[window->ID-1].color.alpha=(unsigned char)font->color.alpha*655;

      // Setup [build] [format] and [picture]
      build[window->ID-1].format=XRenderFindVisualFormat(
        build[window->ID-1].display, DefaultVisual(build[window->ID-1].display, build[window->ID-1].screen)
      );
      build[window->ID-1].picture=XRenderCreatePicture(
        build[window->ID-1].display, build[window->ID-1].buffer, build[window->ID-1].format, 0, NULL
      );

      for(wchar_t *current=text->value; *current; current+=1){
        // Load [current] character
        if(FT_Load_Char(face, *current, FT_LOAD_RENDER)){ continue; }

        // Fill [text] pixel by pixel
        for(unsigned int row=0; row<face->glyph->bitmap.rows; row+=1){
          for(unsigned int col=0; col<face->glyph->bitmap.width; col+=1){
            if(face->glyph->bitmap.buffer[row*face->glyph->bitmap.pitch+col]){
              // Draw on [build] [buffer]
              XRenderFillRectangle(
                build[window->ID-1].display, PictOpOver, build[window->ID-1].picture,
                &build[window->ID-1].color, view[font->ID-1].x+col,
                (view[font->ID-1].y-face->glyph->bitmap_top)+row, 1, 1
              );
            }
          }
        }

        // Move current position
        view[font->ID-1].x+=face->glyph->advance.x>>6;
      }
    }
    else if(przecinek.debug==true){
printf(
  "[pEG03] \"Could not draw text\" (font is closed),\n"
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
 * | .     |
 * |     . | In: pWindow* [window]
 * \ = , = / Out:
 *
 * This function fills [window] buffer with white color.
 * There isn't too much to say about it c.c
 ****************************************************************/
void pWindowClear(pWindow* window){
  if(window->active==true){
    // Clear [build] [buffer]
    XSetForeground(
      build[window->ID-1].display, build[window->ID-1].graphics,
      WhitePixel(build[window->ID-1].display, build[window->ID-1].screen)
    );
    XFillRectangle(
      build[window->ID-1].display, build[window->ID-1].buffer,
      build[window->ID-1].graphics, 0, 0, window->width, window->height
    );
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
 * |\_____/| pWindowClose()
 * | .     |
 * |     . | In: pWindow* [window]
 * \ = , = / Out:
 *
 * This function closes given [window].
 * It sends signal, which's supossed to destroy [window].
 ****************************************************************/
void pWindowClose(pWindow *window){
  if(window->active==true){
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
 * |\_____/| pEventCreate()
 * | .     |
 * |     . | In:
 * \ = , = / Out: pEvent
 *
 * This function creates [event] object.
 * It fills all [event] variables.
 ****************************************************************/
pEvent pEventCreate(){
  // Create local [event]
  pEvent event;

  // Set [event] values
  event.focus=false;
  event.frameCount=0;

  for(unsigned short int current=0; current<KEY_MAX; current+=1){
    event.key[current]=0;
  }
  event.keyCaps=false;

  // Return local [event]
  return event;
}

/****************************************************************
 * |\_____/| pEventHandle()
 * | .     |
 * |     . | In: pWindow* [window], pEvent* [event]
 * \ = , = / Out:
 *
 * This function handles every global action.
 * It checks for any [window] messages. It switches buffers.
 * It updates [key] values. It updates mouse position,
 * display size and window count for [event]. It updates
 * many [window] values. It checks if [window] is fullscreen,
 * if it changed its size or position, focus or title, etc.
 * It also updates frame count.
 ****************************************************************/
void pEventHandle(pWindow *window, pEvent *event){
  if(window->active==true){
    // Change [event] [key] values from `1` to `2`
    for(unsigned short int current=0; current<KEY_MAX; current+=1){
      if(event->key[current]==1){ event->key[current]=2; }
    }

    while(XPending(build[window->ID-1].display)){
      // Manage pending [currentEvent]
      XNextEvent(build[window->ID-1].display, &currentEvent);

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
      przecinek.windowCount=winCount;

      // Update [build] [attributeBorder] value
      XGetWindowAttributes(
        build[window->ID-1].display, build[window->ID-1].base, &build[window->ID-1].attributeBorder
      );

      // Update [build] [border]
      build[window->ID-1].border=
        build[window->ID-1].width-build[window->ID-1].attributeBorder.width+
        build[window->ID-1].attributeBorder.border_width;

      // Update [window] [resize] and [border]
      window->resize=build[window->ID-1].resize;
      window->border=build[window->ID-1].border;

      // Update [event] [keyCaps]
      XGetKeyboardControl(build[window->ID-1].display, &build[window->ID-1].keyboardState);
      event->keyCaps=(build[window->ID-1].keyboardState.led_mask&(1<<1)!=0);

      // Manage close [currentEvent]
      if(currentEvent.type==ClientMessage &&
          (Atom)currentEvent.xclient.data.l[0]==build[window->ID-1].delete){
        // Clean [buffer] and [graphics]
        XFreePixmap(build[window->ID-1].display, build[window->ID-1].buffer);
        XFreeGC(build[window->ID-1].display, build[window->ID-1].graphics);

        // Destroy [base] and close [display]
        XDestroyWindow(build[window->ID-1].display, build[window->ID-1].base);
        XCloseDisplay(build[window->ID-1].display);

        // Reset [window]
        pDebugWindowReset(window);

        // Set [change] value and reset [event]
        change=pEventCreate();
        event=&change;

        window->ID=0;
        return;
      }
      else{ window->active=true; }

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

        // Resize [window] and update [buffer]
        XResizeWindow(build[window->ID-1].display, build[window->ID-1].base,
          window->width, window->height
        );
        build[window->ID-1].buffer=XCreatePixmap(
          build[window->ID-1].display, build[window->ID-1].base,
          window->width, window->height,
          DefaultDepth(build[window->ID-1].display, build[window->ID-1].screen)
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

        // Resize [window] and update [buffer]
        XResizeWindow(
          build[window->ID-1].display, build[window->ID-1].base, window->width, window->height
        );
        build[window->ID-1].buffer=XCreatePixmap(
          build[window->ID-1].display, build[window->ID-1].base,
          window->width, window->height,
          DefaultDepth(build[window->ID-1].display, build[window->ID-1].screen)
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
          if(window->x<-WINDOW_POS_MAX){
            if(przecinek.debug==true){
printf(
  "[pWB05] \"Window x value is too low\" (changing from: %i to %i),\n",
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
  "[pWB06] \"Window x value is too big\" (changing from: %i to %i),\n",
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
  "[pWB07] \"Window y value is too low\" (changing from: %i to %i),\n",
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

          // Change [event] [key] values to `0`
          for(unsigned short int current=0; current<KEY_MAX; current+=1){
            event->key[current]=0;
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

          // Change [event] [key] values to `0`
          for(unsigned short int current=0; current<KEY_MAX; current+=1){
            event->key[current]=0;
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
          if(window->width>WINDOW_WIDTH_MAX){
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
          if(window->height>WINDOW_HEIGHT_MAX){
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

          // Change [event] [key] values to `0`
          for(unsigned short int current=0; current<KEY_MAX; current+=1){
            event->key[current]=0;
          }

          // Resize [window] and update [buffer]
          XResizeWindow(
            build[window->ID-1].display, build[window->ID-1].base,
            window->width, window->height
          );
          build[window->ID-1].buffer=XCreatePixmap(
            build[window->ID-1].display, build[window->ID-1].base,
            window->width, window->height,
            DefaultDepth(build[window->ID-1].display, build[window->ID-1].screen)
          );

          build[window->ID-1].sizeRefresh=true;
        }
        else if(currentEvent.type==ConfigureNotify && window->width!=currentEvent.xconfigure.width){
          // Update [window] and [build] size parameters
          window->width=currentEvent.xconfigure.width;
          window->height=currentEvent.xconfigure.height;
          build[window->ID-1].width=currentEvent.xconfigure.width;
          build[window->ID-1].height=currentEvent.xconfigure.height;

          // Change [event] [key] values to `0`
          for(unsigned short int current=0; current<KEY_MAX; current+=1){
            event->key[current]=0;
          }

          // Update [buffer]
          build[window->ID-1].buffer=XCreatePixmap(
            build[window->ID-1].display, build[window->ID-1].base,
            window->width, window->height,
            DefaultDepth(build[window->ID-1].display, build[window->ID-1].screen)
          );

          // Clear [window]
          XSetForeground(
            build[window->ID-1].display, build[window->ID-1].graphics,
            WhitePixel(build[window->ID-1].display, build[window->ID-1].screen)
          );
          XFillRectangle(
            build[window->ID-1].display, build[window->ID-1].base,
            build[window->ID-1].graphics, 0, 0, window->width, window->height
          );

          build[window->ID-1].sizeRefresh=true;
        }
      }
      else{ build[window->ID-1].sizeRefresh=false; }

      // Manage key press [currentEvent]
      if(currentEvent.type==KeyPress){
        if(event->key[currentEvent.xkey.keycode]==0){ event->key[currentEvent.xkey.keycode]=1; }
        else{ event->key[currentEvent.xkey.keycode]=2; }
      }
      if(currentEvent.type==ButtonPress){
        if(currentEvent.xbutton.button==8){ currentEvent.xbutton.button=4; }
        if(currentEvent.xbutton.button==9){ currentEvent.xbutton.button=5; }

        if(event->key[currentEvent.xbutton.button]==0){ event->key[currentEvent.xbutton.button]=1; }
        else{ event->key[currentEvent.xbutton.button]=2; }
      }

      // Manage key release [currentEvent]
      if(currentEvent.type==KeyRelease){
        if(XEventsQueued(build[window->ID-1].display, QueuedAfterReading)){
          XPeekEvent(build[window->ID-1].display, &currentReport);

          if(currentReport.type==KeyPress &&
              currentReport.xkey.time==currentEvent.xkey.time &&
              currentReport.xkey.keycode==currentEvent.xkey.keycode){
            // Fully released key
            event->key[currentEvent.xkey.keycode]=2;

            XNextEvent(build[window->ID-1].display, &currentEvent);
          }

          // System released key
          else{ event->key[currentEvent.xkey.keycode]=0; }
        }
        else{ event->key[currentEvent.xkey.keycode]=0; }
      }
      if(currentEvent.type==ButtonRelease){
        if(currentEvent.xbutton.button==8){ currentEvent.xbutton.button=4; }
        if(currentEvent.xbutton.button==9){ currentEvent.xbutton.button=5; }

        if(XEventsQueued(build[window->ID-1].display, QueuedAfterReading)){
          XPeekEvent(build[window->ID-1].display, &currentReport);

          if(currentReport.type==ButtonPress &&
              currentReport.xbutton.time==currentEvent.xbutton.time &&
              currentReport.xbutton.button==currentEvent.xbutton.button){
            // Fully released key
            event->key[currentEvent.xbutton.button]=2;

            XNextEvent(build[window->ID-1].display, &currentEvent);
          }

          // System released key
          else{ event->key[currentEvent.xbutton.button]=0; }
        }
        else{ event->key[currentEvent.xbutton.button]=0; }
      }

      // Manage focus in [currentEvent]
      if(currentEvent.type==FocusIn){
        activeWinID=window->ID;
        event->focus=true;
      }

      // Manage focus out [currentEvent]
      if(currentEvent.type==FocusOut && activeWinID==window->ID){
        activeWinID=0;
        event->focus=false;

        // Change [event] [key] values to `0`
        for(unsigned short int current=0; current<KEY_MAX; current+=1){
          event->key[current]=0;
        }
      }

      // Update [window] [title] value
      if(wcscmp(window->title, build[window->ID-1].titleWide)!=0){
        // Update [build] [title] value
        wcscpy(build[window->ID-1].titleWide, window->title);
        wcstombs(build[window->ID-1].title, window->title, TITLE_MAX);

        // Change [window] [title]
        XStoreName(build[window->ID-1].display, build[window->ID-1].base, build[window->ID-1].title);
      }

      if(window->resize==true){
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

          // Change [event] [key] values to `0`
          for(unsigned short int current=0; current<KEY_MAX; current+=1){
            event->key[current]=0;
          }

          // Resize [window] and update [buffer]
          XResizeWindow(
            build[window->ID-1].display, build[window->ID-1].base, window->width, window->height
          );
          build[window->ID-1].buffer=XCreatePixmap(
            build[window->ID-1].display, build[window->ID-1].base,
            window->width, window->height,
            DefaultDepth(build[window->ID-1].display, build[window->ID-1].screen)
          );
        }

        // Correct [window] and [build] [height] value
        if(build[window->ID-1].heightMin==build[window->ID-1].heightMax &&
            build[window->ID-1].height!=build[window->ID-1].heightMin){
          window->height=build[window->ID-1].heightMin;
          build[window->ID-1].height=build[window->ID-1].heightMin;

          // Change [event] [key] values to `0`
          for(unsigned short int current=0; current<KEY_MAX; current+=1){
            event->key[current]=0;
          }

          // Resize [window] and update [buffer]
          XResizeWindow(
            build[window->ID-1].display, build[window->ID-1].base, window->height, window->height
          );
          build[window->ID-1].buffer=XCreatePixmap(
            build[window->ID-1].display, build[window->ID-1].base,
            window->height, window->height,
            DefaultDepth(build[window->ID-1].display, build[window->ID-1].screen)
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

    // Switch [window] [buffer]
    if(currentEvent.type!=ConfigureNotify){
      XCopyArea(
        build[window->ID-1].display, build[window->ID-1].buffer, build[window->ID-1].base,
        DefaultGC(build[window->ID-1].display, build[window->ID-1].screen),
        0, 0, window->width, window->height, 0, 0
      );
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
    usleep((1000000/winCount)/przecinek.frameLimit);

    // Set and calculate current time
    gettimeofday(&build[window->ID-1].frameEnd, NULL);
    build[window->ID-1].frameMax=
      (double)((build[window->ID-1].frameEnd.tv_sec-build[window->ID-1].frameStart.tv_sec)+
      (build[window->ID-1].frameEnd.tv_usec-build[window->ID-1].frameStart.tv_usec)/1000000.0);

    if(build[window->ID-1].frameMax>=1.0){
      // Set [event] [frameCount] and reset loop
      event->frameCount=build[window->ID-1].frameCount;

      build[window->ID-1].frameCount=0;
      build[window->ID-1].frameMax=0;
      build[window->ID-1].frameStart=build[window->ID-1].frameEnd;
    }
  }
  else if(przecinek.debug==true){
printf(
  "[pEG02] \"Could not handle event\" (window is closed),\n"
);
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
 ****************************************************************/
pObject pObjectCreate(unsigned short int width, unsigned short int height){
  // Create local [object]
  pObject object;

  // Set [object] values
  object.x=0;
  object.y=0;

  object.width=width;
  object.height=height;

  object.color.red=0;
  object.color.green=0;
  object.color.blue=0;
  object.color.alpha=100;

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
 * Its abilities will be extended in the future.
 ****************************************************************/
bool pObjectCollision(pObject *object1, pObject *object2){
  // Return collision
  return(
    object1->x<object2->x+object2->width &&
    object1->x+object1->width>object2->x &&
    object1->y<object2->y+object2->height &&
    object1->y+object1->height>object2->y
  );
}

/****************************************************************
 * |\_____/| pFontCreate()
 * | .     |
 * |     . | In: wchar_t* [name], [directory], us_int [size]
 * \ = , = / Out: pFont
 *
 * This function creates [font] object. It checks
 * if [size] value is valid. It checks if [directory] exists.
 * It loads [font] and save it to memory.
 ****************************************************************/
pFont pFontCreate(wchar_t *name, wchar_t *directory, unsigned short int size){
  // Create local [font]
  pFont font;

  for(unsigned short int current=0; current<FONT_MAX; current+=1){
    if(view[current].size==0){
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
  if(size>FONT_SIZE_MAX){
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
  wcscpy(font.name, name);
  wcscpy(font.directory, directory);
  font.size=size;

  // Set [view] values
  view[font.ID-1].size=size;
  wcscpy(view[font.ID-1].nameWide, name);
  wcscpy(view[font.ID-1].directoryWide, directory);
  wcstombs(view[font.ID-1].name, view[font.ID-1].nameWide, FONT_NAME_MAX);
  wcstombs(view[font.ID-1].directory, view[font.ID-1].directoryWide, FONT_NAME_MAX);

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

  // Load [face] and check if font loads properly
  FTError=FT_New_Face(freeType, view[font.ID-1].directory, 0, &face);
  if(FTError==true){
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

  // Set [face] size
  FT_Set_Pixel_Sizes(face, 0, font.size);

  // Return local [font]
  return font;
}

/****************************************************************
 * |\_____/| pFontClose()
 * | .     |
 * |     . | In: pFont* [font]
 * \ = , = / Out:
 *
 * This function resets given [font].
 * It deletes loaded [font] data from the memory.
 ****************************************************************/
void pFontClose(pFont *font){
  // Fully reset [font]
  pDebugFontReset(font);
  font->ID=0;

  return;
}

/****************************************************************
 * |\_____/| pTextCreate()
 * | .     |
 * |     . | In: wchar_t* [value]
 * \ = , = / Out: pText
 *
 * This function creates [text] object.
 * It fills all [text] variables.
 ****************************************************************/
pText pTextCreate(wchar_t *value){
  // Create local [text]
  pText text;

  // Set [text] values
  text.x=0;
  text.y=0;

  wcscpy(text.value, value);

  // Return local [text]
  return text;
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
