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

/*************************
 * Structures and defines
 *************************/

/* |\____/| Define Default Values
 * |  o o |
 */
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
#define FRAME_MAX 640

#define FONT_MAX 64
#define FONT_SIZE_MAX 1024
#define FONT_NAME_MAX 256
#define TEXT_MAX 8192

/* |\____/| [pSize], [pPosition], [pColor] Structure
 * |  o o |
 */
typedef struct{ unsigned short int width, height; } pSize;
typedef struct{ int x, y; } pPosition;
typedef struct{ unsigned short int red, green, blue, alpha; } pColor;

/* |\____/| [pPrzecinek] Structure
 * |  o o |
 */
typedef struct{
  bool debug;
  unsigned short int windowCount, frameLimit;

  pSize display;
  pPosition cursor;
} pPrzecinek;

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
  wchar_t title[TITLE_MAX];
  unsigned short int border;
  bool fullScreen;
} pWindow;

/* |\____/| [pBuildX11] Structure
 * |  o o |
 */
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

/* |\____/| [pEvent] Structure
 * |  o o |
 */
typedef struct{
  bool focus;
  unsigned short int frameCount;

  unsigned short int key[KEY_MAX];
  bool keyCaps;
} pEvent;

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
  wchar_t name[FONT_NAME_MAX];
  wchar_t directory[FONT_NAME_MAX];

  pColor color;
} pFont;

/* |\____/| [pFontX11] Structure
 * |  o o |
 */
typedef struct{
  unsigned short int size;
  char name[FONT_NAME_MAX];
  char directory[FONT_NAME_MAX];

  wchar_t nameWide[FONT_NAME_MAX];
  wchar_t directoryWide[FONT_NAME_MAX];
  char value[FONT_NAME_MAX+42];
  XftFont *base;

  unsigned int x, y;

  FcPattern *pattern;
  FcObjectSet *objectSet;
  FcFontSet *fontSet;

  bool change;
} pFontX11;

/* |\____/| [pText] Structure
 * |  o o |
 */
typedef struct{
  int x, y;

  wchar_t value[TEXT_MAX];
} pText;

/************
 * Variables
 ************/

// FreeType
FT_Library freeType;
FT_Face face;

// Przecinek
pPrzecinek przecinek;

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

/************
 * Functions
 ************/

/* |\____/| Reset functions
 * |  o o |
 */
void pWindowReset(pWindow *window);
void pFontReset(pFont *font);

/* |\_____/| pSetup() Function
 * |       | Used for initialization of the library
 * | o   o | [debug] (true/false), [frameLimit] (1:MAX)
 * \ = , = / Returns nothing
 */
void pSetup(bool debug, unsigned short int frameLimit){
  // Initialize FreeType
  FT_Init_FreeType(&freeType);

  // Initialize locale
  setlocale(LC_CTYPE, "");

  // Update [przecinek] [debug] value
  przecinek.debug=debug;

  // Update [przecinek] [frameLimit] value
  if(frameLimit==0){
    if(przecinek.debug==true){
printf(
  "[pWarning, G001] \"Frame limit value is too low\" (changing from: 0 to: 1),\n"
);
      fflush(stdout);
    }

    frameLimit=1;
  }
  else if(frameLimit>FRAME_MAX){
    if(przecinek.debug==true){
printf(
  "[pWarning, G002] \"Frame limit value is too big\" (changing from: %i to: %i),\n",
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
  "[pError, W201] \"Could not open X11 display\",\n"
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

  return;
}

/* |\_____/| pClear() Function
 * |       | Used for clearing console
 * | o   o | None
 * \ = , = / Returns nothing
 */
void pClear(){
  // Clear console
  system("clear");

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

/* |\_____/| pWindowCreate() Function
 * |       | Used for creating windows
 * | o   o | [width], [height] (MIN:MAX), [resize] (true/false)
 * \ = , = / Returns [window]
 */
pWindow pWindowCreate(unsigned short int width, unsigned short int height, bool resize){
  // Create local [window]
  pWindow window;

  // Change [winCount]
  winCount+=1;

  for(unsigned short int current=0; current<WINDOW_MAX; current+=1){
    if(build[current].width==0 && build[current].height==0){
      // Set [window] [ID] and reset [window]
      window.ID=current+1;
      pWindowReset(&window);

      // Change [winCount]
      winCount+=1;
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

      window.ID=0;
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
  "[pError, W201] \"Could not open X11 display\",\n"
);
      fflush(stdout);
    }

    // Reset and return [window]
    pWindowReset(&window);

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
  "[pError, W202] \"Could not create X11 window\",\n"
);
      fflush(stdout);
    }

    // Reset [window] and close [build] [display]
    pWindowReset(&window);
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

/* |\_____/| pWindowDrawObject() Function
 * |       | Used for drawing objects on window
 * | o   o | [window], [object]
 * \ = , = / Returns nothing
 */
void pWindowDrawObject(pWindow *window, pObject *object){
  if(window->active==true){
    // Check [object] [color] values
    if(object->color.red>255){
      if(przecinek.debug==true){
printf(
  "[pWarning, O001] \"Object red color value is too big\" (changing from: %i to 255),\n",
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
  "[pWarning, O002] \"Object green color value is too big\" (changing from: %i to 255),\n",
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
  "[pWarning, O003] \"Object blue color value is too big\" (changing from: %i to 255),\n",
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
  "[pWarning, O004] \"Object alpha color value is too big\" (changing from: %i to 100),\n",
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
  "[pError, W301] \"Could not draw object\" (window is closed),\n"
);
    fflush(stdout);
  }

  return;
}

//temp
void pWindowDrawText(pWindow *window, pFont *font, pText *text){
  if(window->active==true){
    if(font->ID!=0){
      // Check [font] [color] values
      if(font->color.red>255){
        if(przecinek.debug==true){
printf(
  "[pWarning, F003] \"Font red color value is too big\" (changing from: %i to 255),\n",
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
  "[pWarning, F004] \"Font green color value is too big\" (changing from: %i to 255),\n",
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
  "[pWarning, F005] \"Font blue color value is too big\" (changing from: %i to 255),\n",
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
  "[pWarning, F006] \"Font alpha color value is too big\" (changing from: %i to 100),\n",
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
        if(font->size==0){
          if(przecinek.debug==true){
printf(
  "[pWarning, F001] \"Font size value is too low\" (changing from: 0 to 1),\n"
);
            fflush(stdout);
          }

          // Change [size] value
          font->size=1;
        }
        if(font->size>FONT_SIZE_MAX){
          if(przecinek.debug==true){
printf(
  "[pWarning, F002] \"Font size value is too big\" (changing from: %i to %i),\n",
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
  "[pError, F002] \"Could not load font\",\n"
);
printf(
  "[pError, W303] \"Could not draw text\" (font is closed),\n"
);
            fflush(stdout);
          }

          // Reset [font]
          pFontReset(font);
          font->ID=0;

          return;
        }

        // Load [face] and check if font loads properly
        if(FT_New_Face(freeType, view[font->ID-1].directory, 0, &face)){
          if(przecinek.debug==true){
printf(
  "[pError, F201] \"Could not create X11 font\",\n"
);
printf(
  "[pError, W303] \"Could not draw text\" (font is closed),\n"
);
            fflush(stdout);
          }

          // Reset [font]
          pFontReset(font);
          font->ID=0;

          return;
        }

        view[font->ID-1].change=false;
      }

      // Set [view] [x] and [y] values
      view[font->ID-1].x=text->x;
      view[font->ID-1].y=text->y;

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
                &build[window->ID-1].color, view[font->ID-1].x + col,
                view[font->ID-1].y - face->glyph->bitmap_top + row, 1, 1
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
  "[pError, W303] \"Could not draw text\" (font is closed),\n"
);
      fflush(stdout);
    }
  }
  else if(przecinek.debug==true){
printf(
  "[pError, W302] \"Could not draw text\" (window is closed),\n"
);
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
  "[pError, W303] \"Could not clear screen\" (window is closed),\n"
);
    fflush(stdout);
  }

  return;
}

/* |\_____/| WindowClose() Function
 * |       | Used for closing window
 * | o   o | [window]
 * \ = , = / Returns nothing
 */
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
  "[pWarning, W101] \"Window is already closed\",\n"
);
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

  for(unsigned short int current=0; current<KEY_MAX; current+=1){
    event.key[current]=0;
  }
  event.keyCaps=false;

  // Return local [event]
  return event;
}

/* |\_____/| pEventHandle() Function
 * |       | Used for handling window events
 * | o   o | [window], [event]
 * \ = , = / Returns nothing
 */
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
        pWindowReset(window);

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
  "[pWarning, W005] \"Window x value is too low\" (changing from: %i to %i),\n",
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
  "[pWarning, W006] \"Window x value is too big\" (changing from: %i to %i),\n",
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
  "[pWarning, W007] \"Window y value is too low\" (changing from: %i to %i),\n",
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
  "[pWarning, W008] \"Window y value is too big\" (changing from: %i to %i),\n",
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
  "[pWarning, W009] \"Window widthMin value is too low\" (changing from: %i to %i),\n",
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
  "[pWarning, W010] \"Window widthMin value is too big\" (changing from: %i to %i),\n",
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
  "[pWarning, W011] \"Window heightMin value is too low\" (changing from: %i to %i),\n",
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
  "[pWarning, W012] \"Window heightMin value is too big\" (changing from: %i to %i),\n",
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
  "[pWarning, W013] \"Window widthMax value is too low\" (changing from: %i to %i),\n",
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
  "[pWarning, W014] \"Window widthMax value is too big\" (changing from: %i to %i),\n",
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
  "[pWarning, W015] \"Window heightMax value is too low\" (changing from: %i to %i),\n",
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
  "[pWarning, W016] \"Window heightMax value is too big\" (changing from: %i to %i),\n",
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
  "[pWarning, W017] \"Window widthMin value is bigger than widthMax\" (changing from: %i to %i),\n",
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
  "[pWarning, W018] \"Window heightMin value is bigger than heightMax\" (changing from: %i to %i),\n",
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
  "[pError, G001] \"Could not handle event\" (window is closed),\n"
);
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

  object.color.red=0;
  object.color.green=0;
  object.color.blue=0;
  object.color.alpha=100;

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
  return(
    object1.x<object2.x+object2.width &&
    object1.x+object1.width>object2.x &&
    object1.y<object2.y+object2.height &&
    object1.y+object1.height>object2.y
  );
}

//temp
void pFontReset(pFont *font){
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

  view[font->ID-1].pattern=NULL;
  view[font->ID-1].objectSet=NULL;
  view[font->ID-1].fontSet=NULL;

  view[font->ID-1].change=false;

  return;
}

//temp
pFont pFontCreate(wchar_t *name, wchar_t *directory, unsigned short int size){
  // Create local [font]
  pFont font;

  for(unsigned short int current=0; current<FONT_MAX; current+=1){
    if(view[current].size==0){
      // Set [font] [ID] and reset [font]
      font.ID=current+1;
      pFontReset(&font);

      break;
    }
    else if(current==FONT_MAX-1){
      if(przecinek.debug==true){
printf(
  "[pError, F001] \"Too many fonts were created\" (limit: %i),\n",
  FONT_MAX
);
        fflush(stdout);
      }

      // Reset and return [font]
      pFontReset(&font);
      font.ID=0;

      return font;
    }
  }

  // Check [size] value
  if(size==0){
    if(przecinek.debug==true){
printf(
  "[pWarning, F001] \"Font size value is too low\" (changing from: 0 to 1),\n"
);
      fflush(stdout);
    }

    // Change [size] value
    size=1;
  }
  if(size>FONT_SIZE_MAX){
    if(przecinek.debug==true){
printf(
  "[pWarning, F002] \"Font size value is too big\" (changing from: %i to %i),\n",
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
  "[pError, F002] \"Could not load font\",\n"
);
      fflush(stdout);
    }

    // Reset and return [font]
    pFontReset(&font);
    font.ID=0;

    return font;
  }

  // Load [face] and check if font loads properly
  FTError=FT_New_Face(freeType, view[font.ID-1].directory, 0, &face);
  if(FTError==true){
    if(przecinek.debug==true){
printf(
  "[pError, F201] \"Could not create X11 font\",\n"
);
      fflush(stdout);
    }

    // Reset and return [font]
    pFontReset(&font);
    font.ID=0;

    return font;
  }

  // Set [face] size
  FT_Set_Pixel_Sizes(face, 0, font.size);

  // Return local [font]
  return font;
}

//temp
void pFontClose(pFont *font){
  // Fully reset [font]
  pFontReset(font);
  font->ID=0;

  return;
}

//temp
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

/* |\_____/| pKey() Function
 * |       | Used for converting key names into codes
 * | o   o | [key]
 * \ = , = / Returns (0:256)
 */
unsigned short int pKey(char *key){
  if(strcmp(key, "LMOUSE")==0 || strcmp(key, "LMouse")==0 ||
    strcmp(key, "lmouse")==0){ return 1; }
  if(strcmp(key, "MMOUSE")==0 || strcmp(key, "MMouse")==0 ||
    strcmp(key, "mmouse")==0){ return 2; }
  if(strcmp(key, "RMOUSE")==0 || strcmp(key, "RMouse")==0 ||
    strcmp(key, "rmouse")==0){ return 3; }
  if(strcmp(key, "BACK")==0 || strcmp(key, "Back")==0 ||
    strcmp(key, "back")==0){ return 4; }
  if(strcmp(key, "FORWARD")==0 || strcmp(key, "Forward")==0 ||
    strcmp(key, "forward")==0){ return 5; }

  if(strcmp(key, "ESC")==0 || strcmp(key, "esc")==0 || strcmp(key, "Esc")==0){ return 9; }
  if(strcmp(key, "TAB")==0 || strcmp(key, "tab")==0 || strcmp(key, "Tab")==0){ return 23; }
  if(strcmp(key, "CAPS")==0 || strcmp(key, "caps")==0 || strcmp(key, "Caps")==0){ return 66; }
  if(strcmp(key, "LSHIFT")==0 || strcmp(key, "lshift")==0 || strcmp(key, "LShift")==0){ return 50; }
  if(strcmp(key, "LCTRL")==0 || strcmp(key, "lctrl")==0 || strcmp(key, "LCtrl")==0){ return 37; }
  if(strcmp(key, "LMOD")==0 || strcmp(key, "lmod")==0 || strcmp(key, "LMod")==0){ return 133; }
  if(strcmp(key, "LALT")==0 || strcmp(key, "lalt")==0 || strcmp(key, "LAlt")==0){ return 64; }
  if(strcmp(key, "SPACE")==0 || strcmp(key, "space")==0 || strcmp(key, "Space")==0){ return 65; }

  if(strcmp(key, "F1")==0 || strcmp(key, "f1")==0){ return 67; }
  if(strcmp(key, "F2")==0 || strcmp(key, "f2")==0){ return 68; }
  if(strcmp(key, "F3")==0 || strcmp(key, "f3")==0){ return 69; }
  if(strcmp(key, "F4")==0 || strcmp(key, "f4")==0){ return 70; }
  if(strcmp(key, "F5")==0 || strcmp(key, "f5")==0){ return 71; }
  if(strcmp(key, "F6")==0 || strcmp(key, "f6")==0){ return 72; }
  if(strcmp(key, "F7")==0 || strcmp(key, "f7")==0){ return 73; }
  if(strcmp(key, "F8")==0 || strcmp(key, "f8")==0){ return 74; }
  if(strcmp(key, "F9")==0 || strcmp(key, "f9")==0){ return 75; }
  if(strcmp(key, "F10")==0 || strcmp(key, "f10")==0){ return 76; }
  if(strcmp(key, "F11")==0 || strcmp(key, "f11")==0){ return 95; }
  if(strcmp(key, "F12")==0 || strcmp(key, "f12")==0){ return 96; }

  if(strcmp(key, "RALT")==0 || strcmp(key, "ralt")==0 || strcmp(key, "RAlt")==0){ return 108; }
  if(strcmp(key, "RWIN")==0 || strcmp(key, "rwin")==0 || strcmp(key, "RWin")==0){ return 134; }
  if(strcmp(key, "MENU")==0 || strcmp(key, "menu")==0 || strcmp(key, "Menu")==0){ return 135; }
  if(strcmp(key, "RCTRL")==0 || strcmp(key, "rctrl")==0 || strcmp(key, "RCtrl")==0){ return 105; }
  if(strcmp(key, "RSHIFT")==0 || strcmp(key, "rshift")==0 || strcmp(key, "RShift")==0){ return 62; }
  if(strcmp(key, "ENTER")==0 || strcmp(key, "enter")==0 || strcmp(key, "Enter")==0){ return 36; }
  if(strcmp(key, "BACKSPACE")==0 || strcmp(key, "backspace")==0 ||
    strcmp(key, "Backspace")==0){ return 22; }

  if(strcmp(key, "LARROW")==0 || strcmp(key, "larrow")==0 || strcmp(key, "LArrow")==0){ return 113; }
  if(strcmp(key, "DARROW")==0 || strcmp(key, "darrow")==0 || strcmp(key, "DArrow")==0){ return 116; }
  if(strcmp(key, "RARROW")==0 || strcmp(key, "rarrow")==0 || strcmp(key, "RArrow")==0){ return 114; }
  if(strcmp(key, "UARROW")==0 || strcmp(key, "uarrow")==0 || strcmp(key, "UArrow")==0){ return 111; }

  if(strcmp(key, "PRINTSCRN")==0 || strcmp(key, "printscrn")==0 ||
    strcmp(key, "PrintScrn")==0){ return 107; }
  if(strcmp(key, "SCROLLLOCK")==0 || strcmp(key, "scrolllock")==0 ||
    strcmp(key, "ScrollLock")==0){ return 78; }
  if(strcmp(key, "PAUSEBREAK")==0 || strcmp(key, "pausebreak")==0 ||
    strcmp(key, "PauseBreak")==0){ return 127; }
  if(strcmp(key, "INS")==0 || strcmp(key, "ins")==0 || strcmp(key, "Ins")==0){ return 118; }
  if(strcmp(key, "HOME")==0 || strcmp(key, "home")==0 || strcmp(key, "Home")==0){ return 110; }
  if(strcmp(key, "PAGEU")==0 || strcmp(key, "pageu")==0 || strcmp(key, "PageU")==0){ return 112; }
  if(strcmp(key, "DEL")==0 || strcmp(key, "del")==0 || strcmp(key, "Del")==0){ return 119; }
  if(strcmp(key, "END")==0 || strcmp(key, "end")==0 || strcmp(key, "End")==0){ return 115; }
  if(strcmp(key, "PAGED")==0 || strcmp(key, "paged")==0 || strcmp(key, "PageD")==0){ return 117; }

  if(strcmp(key, "Q")==0 || strcmp(key, "q")==0){ return 24; }
  if(strcmp(key, "W")==0 || strcmp(key, "w")==0){ return 25; }
  if(strcmp(key, "E")==0 || strcmp(key, "e")==0){ return 26; }
  if(strcmp(key, "R")==0 || strcmp(key, "r")==0){ return 27; }
  if(strcmp(key, "T")==0 || strcmp(key, "t")==0){ return 28; }
  if(strcmp(key, "Y")==0 || strcmp(key, "y")==0){ return 29; }
  if(strcmp(key, "U")==0 || strcmp(key, "u")==0){ return 30; }
  if(strcmp(key, "I")==0 || strcmp(key, "i")==0){ return 31; }
  if(strcmp(key, "O")==0 || strcmp(key, "o")==0){ return 32; }
  if(strcmp(key, "P")==0 || strcmp(key, "p")==0){ return 33; }
  if(strcmp(key, "A")==0 || strcmp(key, "a")==0){ return 38; }
  if(strcmp(key, "S")==0 || strcmp(key, "s")==0){ return 39; }
  if(strcmp(key, "D")==0 || strcmp(key, "d")==0){ return 40; }
  if(strcmp(key, "F")==0 || strcmp(key, "f")==0){ return 41; }
  if(strcmp(key, "G")==0 || strcmp(key, "g")==0){ return 42; }
  if(strcmp(key, "H")==0 || strcmp(key, "h")==0){ return 43; }
  if(strcmp(key, "J")==0 || strcmp(key, "j")==0){ return 44; }
  if(strcmp(key, "K")==0 || strcmp(key, "k")==0){ return 45; }
  if(strcmp(key, "L")==0 || strcmp(key, "l")==0){ return 46; }
  if(strcmp(key, "Z")==0 || strcmp(key, "z")==0){ return 52; }
  if(strcmp(key, "X")==0 || strcmp(key, "x")==0){ return 53; }
  if(strcmp(key, "C")==0 || strcmp(key, "c")==0){ return 54; }
  if(strcmp(key, "V")==0 || strcmp(key, "v")==0){ return 55; }
  if(strcmp(key, "B")==0 || strcmp(key, "b")==0){ return 56; }
  if(strcmp(key, "N")==0 || strcmp(key, "n")==0){ return 57; }
  if(strcmp(key, "M")==0 || strcmp(key, "m")==0){ return 58; }

  if(strcmp(key, "1")==0){ return 10; }
  if(strcmp(key, "2")==0){ return 11; }
  if(strcmp(key, "3")==0){ return 12; }
  if(strcmp(key, "4")==0){ return 13; }
  if(strcmp(key, "5")==0){ return 14; }
  if(strcmp(key, "6")==0){ return 15; }
  if(strcmp(key, "7")==0){ return 16; }
  if(strcmp(key, "8")==0){ return 17; }
  if(strcmp(key, "9")==0){ return 18; }
  if(strcmp(key, "0")==0){ return 19; }

  if(strcmp(key, "`")==0){ return 49; }
  if(strcmp(key, ",")==0){ return 59; }
  if(strcmp(key, ".")==0){ return 60; }
  if(strcmp(key, "/")==0){ return 61; }
  if(strcmp(key, ";")==0){ return 47; }
  if(strcmp(key, "'")==0){ return 48; }
  if(strcmp(key, "\\")==0){ return 51; }
  if(strcmp(key, "[")==0){ return 34; }
  if(strcmp(key, "]")==0){ return 35; }
  if(strcmp(key, "-")==0){ return 20; }
  if(strcmp(key, "=")==0){ return 21; }

  return 0;
}
