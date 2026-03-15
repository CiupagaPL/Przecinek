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

// Unix only libraries
#include <unistd.h>
#include <sys/time.h>

// X11 and Xft libraries
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>

// GL and GLX libraries
#include <GL/gl.h>
#include <GL/glx.h>

// PNG and JPEG libraries
#include <png.h>
#include <jpeglib.h>

// FreeType2 library
#include <ft2build.h>

/********************************
 *  ,______,  Define [pBuildX11]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  short int x, y;
  short int xBac, yBac;

  unsigned short int width, height;
  unsigned short int widthMin, heightMin, widthMax, heightMax;
  unsigned short int widthBac, heightBac;

  char title[WINDOW_TITLE_CHAR];
  wchar_t titleW[WINDOW_TITLE_CHAR];
  struct timeval frameStart, frameEnd;
  unsigned short int frameCount;

  bool resizable, focus, fullScreen;

  GLXContext buffer;

  Display *display;
  int screen;
  Window base;
  XVisualInfo *visual;
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
  short int x, y;
  unsigned short int width, height;
  unsigned short int vertice, rotation;

  pPosition point[OBJECT_VERTICE_MAX];
  pPosition center;
  float sourceX[OBJECT_VERTICE_MAX], sourceY[OBJECT_VERTICE_MAX];
  unsigned short int rotationFix;

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
  short int letterSpacing, spaceSpacing, lineSpacing;
  char directory[FONT_DIRECTORY_CHAR];
  wchar_t directoryW[FONT_DIRECTORY_CHAR];

  FT_Face face;

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
  wchar_t valueW[TEXT_CHAR];

  short int x[FONT_MAX], y[FONT_MAX];

  GLuint source[FONT_MAX][TEXT_CHAR];
  pSize sourceSize[FONT_MAX][TEXT_CHAR];
  pPosition sourcePosition[FONT_MAX][TEXT_CHAR];

  bool change;

  bool exist;
} pCodeX11;

/********************************
 *  ,______,  Define [pTextureX11]
 *  |      |  structure [DEBUG]
 *  |______|
 * (--------)
 ********************************/
typedef struct{
  char directory[IMAGE_DIRECTORY_CHAR];
  wchar_t directoryW[IMAGE_DIRECTORY_CHAR];

  GLuint source;

  bool exist;
} pTextureX11;

// Przecinek
pPrzecinek przecinek={ true, 0, 0 };
bool setup=false;

FT_Library freeType;

int cursorMainX, cursorMainY, cursorLocalX, cursorLocalY;

// Window
unsigned short int windowMainID=0;
unsigned short int windowCount=0;

pBuildX11 build[WINDOW_MAX];

XEvent currentEvent, currentReport, currentAction;
Display *display;
int screen;
Window root;
unsigned int mask;
GLXContext buffer;
XVisualInfo *visual;

GLXFBConfig *facebookConfig;
int facebookCount;

double frameMax;
unsigned short int currentFrameLimit;
short int frameOverHead;

XKeyboardState keyboardState;

unsigned short int input[PRZECINEK_KEY_MAX];
bool inputChange;

// Object
pFigureX11 figure[OBJECT_MAX];

float widthScale, heightScale, ratio, distance;
int distanceMinX, distanceMinY, distanceMaxX, distanceMaxY;
int edgeX, edgeY, projectionX, projectionY;
pPosition centerA, centerB;
int pointA[5], pointB[5];

// Font
pViewX11 view[FONT_MAX];

struct stat status;

// Text
pCodeX11 code[TEXT_MAX];

short int xFix, yFix;

char *segment, *token;
unsigned short int line;
GLubyte* textData;

// Image
pTextureX11 texture[IMAGE_MAX];

FILE *fileImage;
unsigned int imageWidth, imageHeight;
unsigned short int imageChannel;

png_byte pngDepth, pngColor;
png_structp png;
png_infop pngInfo;
png_byte **pngBuffer;

struct jpeg_decompress_struct jpegInfo;
struct jpeg_error_mgr jpegError;
unsigned char *jpegBuffer;
unsigned char *jpegRow[1];

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

  build[window->ID-1].resizable=false;
  build[window->ID-1].focus=false;
  build[window->ID-1].fullScreen=false;

  if(build[window->ID-1].buffer!=NULL){
    glXMakeCurrent(build[window->ID-1].display, None, NULL);
    glXDestroyContext(build[window->ID-1].display, build[window->ID-1].buffer);

    build[window->ID-1].buffer=NULL;
  }

  if(build[window->ID-1].base!=0){
    XDestroyWindow(build[window->ID-1].display, build[window->ID-1].base);
    build[window->ID-1].base=0;
  }
  build[window->ID-1].screen=0;
  if(build[window->ID-1].display!=NULL){
    XCloseDisplay(build[window->ID-1].display);
    build[window->ID-1].display=NULL;
  }
  build[window->ID-1].visual=NULL;
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
  memset(figure[object->ID-1].sourceX, 0, sizeof(figure[object->ID-1].sourceX));
  memset(figure[object->ID-1].sourceY, 0, sizeof(figure[object->ID-1].sourceY));
  figure[object->ID-1].rotationFix=0;

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
    figure[object->ID-1].point[current].x=50+(sqrt(pow(100, 2)*2)/2)*cos(ratio);

    // Calculate [figure] [point] [y]
    figure[object->ID-1].point[current].y=50+(sqrt(pow(100, 2)*2)/2)*sin(ratio);

    // Check for [distanceMinX] and [distanceMinY] values
    if(figure[object->ID-1].point[current].x<distanceMinX){
      distanceMinX=figure[object->ID-1].point[current].x;
    }
    if(figure[object->ID-1].point[current].y<distanceMinY){
      distanceMinY=figure[object->ID-1].point[current].y;
    }

    // Check for [distanceMaxX] and [distanceMaxY] values
    if(figure[object->ID-1].point[current].x>distanceMaxX){
      distanceMaxX=figure[object->ID-1].point[current].x;
    }
    if(figure[object->ID-1].point[current].y>distanceMaxY){
      distanceMaxY=figure[object->ID-1].point[current].y;
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
    figure[object->ID-1].point[current].x=
      (figure[object->ID-1].width/2)+
      (sqrt(pow((float)figure[object->ID-1].width/widthScale, 2)*2)/2)*
      cos(ratio);

    // Calculate [figure] [point] [y]
    figure[object->ID-1].point[current].y=
      (figure[object->ID-1].height/2)+
      (sqrt(pow((float)figure[object->ID-1].height/heightScale, 2)*2)/2)*
      sin(ratio);

    // Check for [distanceMinX] and [distanceMinY] values
    if(figure[object->ID-1].point[current].x<distanceMinX){
      distanceMinX=figure[object->ID-1].point[current].x;
    }
    if(figure[object->ID-1].point[current].y<distanceMinY){
      distanceMinY=figure[object->ID-1].point[current].y;
    }
  }

  for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
    // Correct [figure] [point] position
    figure[object->ID-1].point[current].x-=distanceMinX-figure[object->ID-1].x;
    figure[object->ID-1].point[current].y-=distanceMinY-figure[object->ID-1].y;

    // Calculate [distanceMaxX] and [distanceMaxY]
    if(figure[object->ID-1].point[current].x>distanceMaxX){
      distanceMaxX=figure[object->ID-1].point[current].x;
    }
    if(figure[object->ID-1].point[current].y>distanceMaxY){
      distanceMaxY=figure[object->ID-1].point[current].y;
    }
  }

  for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
    // Calculate [figure] [sourceX] and [sourceY] values
    figure[object->ID-1].sourceX[current]=
      (float)(distanceMaxX-figure[object->ID-1].point[current].x)/
      (float)(distanceMaxX-figure[object->ID-1].x);
    figure[object->ID-1].sourceY[current]=
      (float)(distanceMaxY-figure[object->ID-1].point[current].y)/
      (float)(distanceMaxY-figure[object->ID-1].y);

    figure[object->ID-1].sourceY[current]=1-figure[object->ID-1].sourceY[current];
    figure[object->ID-1].sourceX[current]=1-figure[object->ID-1].sourceX[current];
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
    // Calculate [edgeX] and [edgeY] values
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
  font->lineSpacing=0;

  // Reset [view] values
  view[font->ID-1].size=0;
  view[font->ID-1].letterSpacing=0;
  view[font->ID-1].lineSpacing=0;
  memset(view[font->ID-1].directory, 0, sizeof(view[font->ID-1].directory));
  memset(view[font->ID-1].directoryW, 0, sizeof(view[font->ID-1].directoryW));

  if(view[font->ID-1].face!=NULL){
    FT_Done_Face(view[font->ID-1].face);
    view[font->ID-1].face=NULL;
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
  memset(code[text->ID-1].valueW, 0, sizeof(code[text->ID-1].valueW));

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

  // Reset [line], [xFix] and calculate [yFix] values
  line=0;
  xFix=0;
  yFix=(view[font->ID-1].size*458*1.33)/512;

  for(unsigned short int current=0; current<wcslen(code[text->ID-1].valueW); current+=1){
    // Add space to new [line]
    if(code[text->ID-1].valueW[current]==L'\n'){
      xFix=0;
      line+=1;

      continue;
    }
    // Add space to space O.O
    if(code[text->ID-1].valueW[current]==L' '){
      xFix-=view[font->ID-1].spaceSpacing*1.33;

      continue;
    }

    // Load [current] char from [code] [valueW]
    if(FT_Load_Char(view[text->ID-1].face, code[text->ID-1].valueW[current], FT_LOAD_RENDER) ||
        view[font->ID-1].face->glyph->bitmap.width==0 ||
        view[font->ID-1].face->glyph->bitmap.rows==0){

      continue;
    }

    // Setup [textData]
    textData=(GLubyte*)malloc(
      view[font->ID-1].face->glyph->bitmap.width*view[font->ID-1].face->glyph->bitmap.rows*4
    );
    for(unsigned int current=0;
        current<view[font->ID-1].face->glyph->bitmap.width*view[font->ID-1].face->glyph->bitmap.rows;
        current+=1){

      textData[current*4]=view[font->ID-1].face->glyph->bitmap.buffer[current];
      textData[(current*4)+1]=view[font->ID-1].face->glyph->bitmap.buffer[current];
      textData[(current*4)+2]=view[font->ID-1].face->glyph->bitmap.buffer[current];
      textData[(current*4)+3]=view[font->ID-1].face->glyph->bitmap.buffer[current];
    }

    // Setup [code] [source]
    glBindTexture(GL_TEXTURE_2D, code[text->ID-1].source[font->ID-1][current]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Initialize [code] [source]
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, view[font->ID-1].face->glyph->bitmap.width,
      view[font->ID-1].face->glyph->bitmap.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, textData
    );

    // Set [code] [sourceSize] values
    code[text->ID-1].sourceSize[font->ID-1][current].width=view[font->ID-1].face->glyph->metrics.width>>6;
    code[text->ID-1].sourceSize[font->ID-1][current].height=view[font->ID-1].face->glyph->metrics.height>>6;

    // Set [code] [sourcePosition] values
    code[text->ID-1].sourcePosition[font->ID-1][current].x=text->x-xFix;
    code[text->ID-1].sourcePosition[font->ID-1][current].y=
      text->y-view[font->ID-1].face->glyph->bitmap_top+yFix+
      ((view[font->ID-1].face->size->metrics.height>>6)*line)+(view[font->ID-1].lineSpacing*1.33*line);

    // Change [xFix] value
    xFix-=code[text->ID-1].sourceSize[font->ID-1][current].width+(view[font->ID-1].letterSpacing*1.33);

    // Clear [textData]
    free(textData);
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
  memset(texture[image->ID-1].directoryW, 0, sizeof(texture[image->ID-1].directoryW));

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
  // Reset [pngBuffer] and [jpegBuffer]
  /*if(pngBuffer!=NULL){
    for(unsigned long int current=0; current<INT_MAX; current+=1){
      if(pngBuffer[current]==NULL){ break; }
      free(pngBuffer[current]);
    }
    free(pngBuffer);
  }*/
  /*if(jpegBuffer!=NULL){
    free(jpegBuffer);
    free(jpegRow[0]);
  }*/

  // Reset [texture] [source]
  glDeleteTextures(1, &texture[image->ID-1].source);
  glGenTextures(1, &texture[image->ID-1].source);

  if(wcscmp(texture[image->ID-1].directoryW+wcslen(texture[image->ID-1].directoryW)-4, L".png")==0){
    // Recreate [png] and [pngInfo]
    png_destroy_read_struct(&png, &pngInfo, NULL);
    png=png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    pngInfo=png_create_info_struct(png);
    setjmp(png_jmpbuf(png));

    // Link [fileImage] to [png]
    png_init_io(png, fileImage);
    png_read_info(png, pngInfo);

    // Check [pngDepth] and [pngColor] values
    pngDepth=png_get_bit_depth(png, pngInfo);
    pngColor=png_get_color_type(png, pngInfo);

    if(pngColor==PNG_COLOR_TYPE_PALETTE){ png_set_palette_to_rgb(png); }

    if(pngColor==PNG_COLOR_TYPE_GRAY && pngDepth<8){ png_set_expand_gray_1_2_4_to_8(png); }
    if(pngDepth==16){ png_set_strip_16(png); }

    // Update [pngInfo]
    png_read_update_info(png, pngInfo);

    // Set [imageWidth] and [imageHeight] values
    imageWidth=png_get_image_width(png, pngInfo);
    imageHeight=png_get_image_height(png, pngInfo);

    if(imageWidth%2==0){ imageWidth+=4; }
    else{ imageWidth+=5; }

    // Setup [pngBuffer]
    pngBuffer=(png_byte **)malloc(sizeof(png_byte *)*imageHeight);
    for(unsigned long int current=0; current<imageHeight; current+=1){
      pngBuffer[current]=(png_byte *)malloc(png_get_rowbytes(png, pngInfo));
    }

    // Load data from [png] into [pngBuffer]
    for(unsigned long int current=0; current<imageHeight; current+=1){
      png_read_row(png, pngBuffer[current], NULL);
    }

    png_read_end(png, pngInfo);

    // Setup [texture] [source]
    glGenTextures(1, &texture[image->ID-1].source);
    glBindTexture(GL_TEXTURE_2D, texture[image->ID-1].source);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngBuffer[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else{
    // Recreate [jpegInfo]
    jpeg_destroy_decompress(&jpegInfo);
    jpeg_create_decompress(&jpegInfo);

    // Setup [jpegInfo]
    jpegInfo.err=jpeg_std_error(&jpegError);
    jpeg_create_decompress(&jpegInfo);
    jpeg_stdio_src(&jpegInfo, fileImage);
    jpeg_read_header(&jpegInfo, TRUE);
    jpeg_start_decompress(&jpegInfo);

    // Set [imageWidth], [imageHeight] and [imageChannel] values
    imageWidth=jpegInfo.output_width;
    imageHeight=jpegInfo.output_height;
    imageChannel=jpegInfo.output_components;

    // Setup [jpegBuffer]
    jpegBuffer=malloc(imageWidth*imageHeight*imageChannel);

    // Load data from [jpeg] into [jpegBuffer]
    while(jpegInfo.output_scanline<imageHeight){
      jpegRow[0]=jpegBuffer+jpegInfo.output_scanline*imageWidth*imageChannel;
      jpeg_read_scanlines(&jpegInfo, jpegRow, 1);
    }

    jpeg_finish_decompress(&jpegInfo);

    // Setup [texture] [source]
    glGenTextures(1, &texture[image->ID-1].source);
    glBindTexture(GL_TEXTURE_2D, texture[image->ID-1].source);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, jpegBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  // Clean up
  if(fileImage!=NULL){ fclose(fileImage); }

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
  if(FT_Init_FreeType(&freeType)){
    if(przecinek.debug==true){
      printf("[pSetup() Error]\n");
      printf("FreeType2 library could not be initialized!\n");
      printf("Try to reinstall FT2 package or recompile Przecinek,\n");
      fflush(stdout);
    }

    return;
  }

  // Initialize libPNG
  png=png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  pngInfo=png_create_info_struct(png);
  if(setjmp(png_jmpbuf(png))){
    if(przecinek.debug==true){
      printf("[pSetup() Error]\n");
      printf("libPNG library could not be initialized!\n");
      printf("Try to reinstall libpng package or recompile Przecinek,\n");
      fflush(stdout);
    }

    return;
  }

  // Initialize libJPEG
  jpeg_create_decompress(&jpegInfo);

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

  // Create global [display]
  display=XOpenDisplay(NULL);
  if(display==NULL){
    if(przecinek.debug==true){
      printf("[pSetup() Error]\n");
      printf("X11 library display is unavailable!\n");
      printf("Try to reinstall X11 package or recompile Przecinek,\n");
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

  // Get [facebookConfig] value and create [visual]
  facebookConfig=glXGetFBConfigs(display, screen, &facebookCount);
  visual=glXGetVisualFromFBConfig(display, facebookConfig[0]);

  // Create global [buffer]
  buffer=glXCreateContext(display, visual, NULL, GL_TRUE);

  // Update [przecinek] [cursor] values
  if(XQueryPointer(
      display, root, &root, &root, &cursorMainX, &cursorMainY,
      &cursorLocalX, &cursorLocalY, &mask)){

    przecinek.cursor.x=cursorMainX;
    przecinek.cursor.y=cursorMainY;
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
  FT_Done_FreeType(freeType);

  // Reset [pngBuffer] and [jpegBuffer]
  /*if(pngBuffer){
    for(unsigned long int current=0; current<imageHeight; current+=1){
      free(pngBuffer[current]);
    }
    free(pngBuffer);
  }
  if(jpegBuffer){ free(jpegBuffer); }*/

  // Destroy debug [buffer], [display] and [root]
  glXMakeCurrent(display, None, NULL);
  glXDestroyContext(display, buffer);
  XCloseDisplay(display);
  XDestroyWindow(display, root);

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
  system("clear");
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
 * It creates [window] [buffer] for rendering purposes.
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
      printf("[pWindowCreate() Error]\n");
      printf("X11 library display is unavailable!\n");
      printf("Try to reinstall X11 package or recompile Przecinek,\n");
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

  // Setup [build] [visual]
  facebookConfig=glXGetFBConfigs(build[window.ID-1].display, build[window.ID-1].screen, &facebookCount);
  build[window.ID-1].visual=glXGetVisualFromFBConfig(build[window.ID-1].display, facebookConfig[0]);

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
      printf("[pWindowCreate() Error]\n");
      printf("X11 library window is unavailable!\n");
      printf("Try to reinstall X11 package or recompile Przecinek,\n");
      fflush(stdout);
    }

    // Reset [window] and close [build] [display]
    pDebugWindowReset(&window);

    window.ID=0;
    return window;
  }

  // Set [title] value
  XStoreName(build[window.ID-1].display, build[window.ID-1].base, WINDOW_TITLE_DEF);
  mbstowcs(window.title, WINDOW_TITLE_DEF, WINDOW_TITLE_CHAR);
  strcpy(build[window.ID-1].title, WINDOW_TITLE_DEF);
  mbstowcs(build[window.ID-1].titleW, WINDOW_TITLE_DEF, WINDOW_TITLE_CHAR);

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

  // Create [build] [buffer]
  build[window.ID-1].buffer=glXCreateContext(
    build[window.ID-1].display, build[window.ID-1].visual, buffer, GL_TRUE
  );
  glXMakeCurrent(build[window.ID-1].display, build[window.ID-1].base, build[window.ID-1].buffer);

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
        if(wcscmp(image->directory, texture[image->ID-1].directoryW)!=0){
          // Update [texture] [directory] and [directoryW] values
          wcscpy(texture[image->ID-1].directoryW, image->directory);
          wcstombs(texture[image->ID-1].directory, image->directory, IMAGE_DIRECTORY_CHAR);

          // Check [directory] value
          if((wcslen(image->directory)>=4 &&
              (wcscmp(image->directory+wcslen(image->directory)-4, L".png")==0 ||
               wcscmp(image->directory+wcslen(image->directory)-4, L".jpg")==0)) ||
              (wcslen(image->directory)>=5 &&
              wcscmp(image->directory+wcslen(image->directory)-5, L".jpeg")==0)){

            // Check if [texture] [directory] exists
            if(stat(texture[image->ID-1].directory, &status)!=0){
              if(przecinek.debug==true){
                printf("[pWindowDrawObject() Error]\n");
                printf("Image located in image.directory doesn't exist!\n");
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

          // Load [texture] [directory] to [fileImage]
          fileImage=fopen(texture[image->ID-1].directory, "rb");
          if(fileImage==false){
            if(przecinek.debug==true){
              printf("[pWindowDrawObject() Error]\n");
              printf("libPNG/libJPEG library could not load texture!\n");
              printf("Your image file might be corrupted,\n");

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

          // Change [object] [vertice] value
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

          // Change [object] [vertice] value
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

      if(object->x!=figure[object->ID-1].x || object->y!=figure[object->ID-1].y){
        // Calculate [figure] [point] position values
        if(figure[object->ID-1].change==false){
          for(unsigned short int current=0; current<figure[object->ID-1].vertice; current+=1){
            figure[object->ID-1].point[current].x+=object->x-figure[object->ID-1].x;
            figure[object->ID-1].point[current].y+=object->y-figure[object->ID-1].y;
          }
        }

        // Update [figure] [x] and [y] values
        figure[object->ID-1].x=object->x;
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
      glXMakeCurrent(build[window->ID-1].display, build[window->ID-1].base, build[window->ID-1].buffer);

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
        glVertex2i(figure[object->ID-1].point[current].x, figure[object->ID-1].point[current].y);
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

      if(wcscmp(text->value, code[text->ID-1].valueW)!=0){
        // Update [code] [value]
        wcscpy(code[text->ID-1].valueW, text->value);

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

      if(wcscmp(font->directory, view[font->ID-1].directoryW)!=0){
        // Update [view] [directoryW] and [directory] values
        wcscpy(view[font->ID-1].directoryW, font->directory);
        wcstombs(view[font->ID-1].directory, font->directory, sizeof(view[font->ID-1].directory));

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
          for(unsigned short int current=0; current<wcslen(code[text->ID-1].valueW); current+=1){
            code[text->ID-1].sourcePosition[font->ID-1][current].x-=code[text->ID-1].x[font->ID-1]-text->x;
            code[text->ID-1].sourcePosition[font->ID-1][current].y-=code[text->ID-1].y[font->ID-1]-text->y;
          }
        }

        // Update [code] [x] and [y] values
        code[text->ID-1].x[font->ID-1]=text->x;
        code[text->ID-1].y[font->ID-1]=text->y;
      }

      if(view[font->ID-1].change==true){
        // Check [view] [directory] value
        if(wcslen(view[font->ID-1].directoryW)>=4 &&
            (wcscmp(view[font->ID-1].directoryW+wcslen(view[font->ID-1].directoryW)-4, L".ttf")==0 ||
             wcscmp(view[font->ID-1].directoryW+wcslen(view[font->ID-1].directoryW)-4, L".otf")==0)){

          // Check if [view] [directory] exists
          if(stat(view[font->ID-1].directory, &status)!=0){
            if(przecinek.debug==true){
              printf("[pWindowDrawText() Error]\n");
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
            printf("[pWindowDrawText() Error]\n");
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

        // Reset [view] [face]
        FT_Done_Face(view[font->ID-1].face);

        // Load [view] [face] and check if [font] loads properly
        if(FT_New_Face(freeType, view[font->ID-1].directory, 0, &view[font->ID-1].face)){
          if(przecinek.debug==true){
            printf("[pWindowDrawText() Error]\n");
            printf("FreeType2 library could not load font!\n");
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

          return;
        }

        FT_Set_Pixel_Sizes(view[font->ID-1].face, 0, view[font->ID-1].size*1.33);
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
      glXMakeCurrent(build[window->ID-1].display, build[window->ID-1].base, build[window->ID-1].buffer);

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
        if(wcscmp(image->directory, texture[image->ID-1].directoryW)!=0){
          // Update [texture] [directory] and [directoryW] values
          wcscpy(texture[image->ID-1].directoryW, image->directory);
          wcstombs(texture[image->ID-1].directory, image->directory, IMAGE_DIRECTORY_CHAR);

          // Check [directory] value
          if((wcslen(image->directory)>=4 &&
              (wcscmp(image->directory+wcslen(image->directory)-4, L".png")==0 ||
               wcscmp(image->directory+wcslen(image->directory)-4, L".jpg")==0)) ||
              (wcslen(image->directory)>=5 &&
              wcscmp(image->directory+wcslen(image->directory)-5, L".jpeg")==0)){

            // Check if [texture] [directory] exists
            if(stat(texture[image->ID-1].directory, &status)!=0){
              if(przecinek.debug==true){
                printf("[pWindowDrawObject() Error]\n");
                printf("Image located in image.directory doesn't exist!\n");
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

          // Load [texture] [directory] to [fileImage]
          fileImage=fopen(texture[image->ID-1].directory, "rb");
          if(fileImage==false){
            if(przecinek.debug==true){
              printf("[pWindowDrawObject() Error]\n");
              printf("libPNG/libJPEG library could not load texture!\n");
              printf("Your image file might be corrupted,\n");

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
      glXMakeCurrent(build[window->ID-1].display, build[window->ID-1].base, build[window->ID-1].buffer);

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
    glXMakeCurrent(build[window->ID-1].display, build[window->ID-1].base, build[window->ID-1].buffer);

    if((window->ID-1)==windowMainID){
      // Change [przecinek] [key] values from `1` to `2`
      for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
        if(input[button]!=0){ przecinek.key[input[button]]=2; }
        else{ break; }
      }

      // Update [przecinek] [display] values
      przecinek.display.width=DisplayWidth(build[window->ID-1].display, build[window->ID-1].screen);
      przecinek.display.height=DisplayHeight(build[window->ID-1].display, build[window->ID-1].screen);

      // Update [przecinek] [cursor] values
      root=DefaultRootWindow(build[window->ID-1].display);

      if(XQueryPointer(
          build[window->ID-1].display, root, &root, &root,
          &cursorMainX, &cursorMainY, &cursorLocalX, &cursorLocalY, &mask)){
        przecinek.cursor.x=cursorMainX;
        przecinek.cursor.y=cursorMainY;
      }
      else{
        przecinek.cursor.x=0;
        przecinek.cursor.y=0;
      }

      // Update [przecinek] [windowCount]
      przecinek.windowCount=windowCount;

      // Update [przecinek] [keyCaps]
      XGetKeyboardControl(build[window->ID-1].display, &keyboardState);
      przecinek.keyCaps=(keyboardState.led_mask&(1<<1)!=0);
    }

    // Update [window] [resizable]
    window->resizable=build[window->ID-1].resizable;

    if(wcscmp(window->title, build[window->ID-1].titleW)!=0){
      // Update [build] [title] value
      wcscpy(build[window->ID-1].titleW, window->title);
      wcstombs(build[window->ID-1].title, window->title, WINDOW_TITLE_CHAR);

      // Change [window] [title]
      XStoreName(build[window->ID-1].display, build[window->ID-1].base, build[window->ID-1].title);
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

      // Manage key press [currentEvent]
      if(currentEvent.type==KeyPress){
        if(przecinek.key[currentEvent.xkey.keycode]==0){
          przecinek.key[currentEvent.xkey.keycode]=1;

          for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
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

          for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
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
            for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
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
            for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
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
          for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
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
            for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
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
            for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
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
          for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
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

        // Update [build] [buffer]
        glViewport(0, 0, build[window->ID-1].width, build[window->ID-1].height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, build[window->ID-1].width, build[window->ID-1].height, 0, (-1), 1);

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

        // Update [build] [buffer]
        glViewport(0, 0, build[window->ID-1].width, build[window->ID-1].height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, build[window->ID-1].width, build[window->ID-1].height, 0, (-1), 1);

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
          XMoveWindow(build[window->ID-1].display, build[window->ID-1].base, window->x, window->y);
        }
        else if(currentEvent.type==ConfigureNotify &&
            (window->x!=currentEvent.xconfigure.x || window->y!=currentEvent.xconfigure.y)){

          // Update [build] and [window] position parameters
          window->x=currentEvent.xconfigure.x;
          window->y=currentEvent.xconfigure.y;
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
        }

        // Manage size change [currentEvent]
        if(build[window->ID-1].width!=window->width || build[window->ID-1].height!=window->height){
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

          // Change [przecinek] [key] values to `0`
          for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
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

          // Update [build] [buffer]
          glViewport(0, 0, build[window->ID-1].width, build[window->ID-1].height);
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          glOrtho(0, build[window->ID-1].width, build[window->ID-1].height, 0, (-1), 1);

          build[window->ID-1].sizeRefresh=true;
        }
        else if(currentEvent.type==ConfigureNotify &&
            (window->width!=currentEvent.xconfigure.width || window->height!=currentEvent.xconfigure.height)){

          // Update [window] and [build] size parameters
          window->width=currentEvent.xconfigure.width;
          window->height=currentEvent.xconfigure.height;
          build[window->ID-1].width=currentEvent.xconfigure.width;
          build[window->ID-1].height=currentEvent.xconfigure.height;

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
        for(unsigned short int button=0; button<PRZECINEK_KEY_MAX; button+=1){
          if(input[button]!=0){
            przecinek.key[input[button]]=0;
            input[button]=0;
          }
          else{ break; }
        }
      }

      if(window->resizable==true){
        if(build[window->ID-1].widthMin!=window->widthMin){
          // Update [build] [widthMin] values
          build[window->ID-1].limitChange=true;
          build[window->ID-1].widthMin=window->widthMin;

          if(window->widthMin<WINDOW_WIDTH_MIN){ build[window->ID-1].sizeHint.min_width=WINDOW_WIDTH_MIN; }
          else{ build[window->ID-1].sizeHint.min_width=window->widthMin; }
        }

        if(build[window->ID-1].heightMin!=window->heightMin){
          // Update [build] [heightMin] values
          build[window->ID-1].limitChange=true;
          build[window->ID-1].heightMin=window->heightMin;

          if(window->heightMin<WINDOW_HEIGHT_MIN){ build[window->ID-1].sizeHint.min_height=WINDOW_HEIGHT_MIN; }
          else{ build[window->ID-1].sizeHint.min_height=window->heightMin; }
        }

        if(build[window->ID-1].widthMax!=window->widthMax){
          // Update [build] [widthMax] values
          build[window->ID-1].limitChange=true;
          build[window->ID-1].widthMax=window->widthMax;

          if(window->widthMax>WINDOW_WIDTH_MAX){ build[window->ID-1].sizeHint.max_width=WINDOW_WIDTH_MAX; }
          else{ build[window->ID-1].sizeHint.max_width=window->widthMax; }
        }

        if(build[window->ID-1].heightMax!=window->heightMax){
          // Update [build] [heightMax] values
          build[window->ID-1].limitChange=true;
          build[window->ID-1].heightMax=window->heightMax;

          if(window->heightMax>WINDOW_HEIGHT_MAX){ build[window->ID-1].sizeHint.max_height=WINDOW_HEIGHT_MAX; }
          else{ build[window->ID-1].sizeHint.max_height=window->heightMax; }
        }

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

          if(window->widthMin<WINDOW_WIDTH_MIN){ build[window->ID-1].sizeHint.min_width=WINDOW_WIDTH_MIN; }
          else{ build[window->ID-1].sizeHint.min_width=window->widthMin; }
          if(window->widthMax>WINDOW_WIDTH_MAX){ build[window->ID-1].sizeHint.max_width=WINDOW_WIDTH_MAX; }
          else{ build[window->ID-1].sizeHint.max_width=window->widthMax; }
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

          if(window->heightMin<WINDOW_HEIGHT_MIN){ build[window->ID-1].sizeHint.min_height=WINDOW_HEIGHT_MIN; }
          else{ build[window->ID-1].sizeHint.min_height=window->heightMin; }
          if(window->heightMax>WINDOW_HEIGHT_MAX){ build[window->ID-1].sizeHint.max_height=WINDOW_HEIGHT_MAX; }
          else{ build[window->ID-1].sizeHint.max_height=window->heightMax; }
        }

        if(build[window->ID-1].width>build[window->ID-1].widthMax ||
            build[window->ID-1].width<build[window->ID-1].widthMin ||
            build[window->ID-1].height>build[window->ID-1].heightMax ||
            build[window->ID-1].height<build[window->ID-1].heightMin){

          // Check [build] [width] and [height] values
          if(build[window->ID-1].width>build[window->ID-1].widthMax){
            window->width=build[window->ID-1].widthMax;
          }
          else if(build[window->ID-1].width<build[window->ID-1].widthMin){
            window->width=build[window->ID-1].widthMin;
          }

          if(build[window->ID-1].height>build[window->ID-1].heightMax){
            window->height=build[window->ID-1].heightMax;
          }
          else if(build[window->ID-1].height<build[window->ID-1].heightMin){
            window->height=build[window->ID-1].heightMin;
          }

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

          // Resize [window]
          XResizeWindow(
            build[window->ID-1].display, build[window->ID-1].base, window->width, window->height
          );

          // Update [build] [buffer]
          glViewport(0, 0, build[window->ID-1].width, build[window->ID-1].height);
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          glOrtho(0, build[window->ID-1].width, build[window->ID-1].height, 0, (-1), 1);
        }
      }
    }

    // Switch [window] buffers
    if(currentEvent.type!=ConfigureNotify){
      glXSwapBuffers(build[window->ID-1].display, build[window->ID-1].base);
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

    // Update [build] [frameCount], sleep and calculate [frameMax]
    build[window->ID-1].frameCount+=1;
    usleep((1000000/windowCount)/currentFrameLimit);

    // Set and calculate current time
    gettimeofday(&build[window->ID-1].frameEnd, NULL);
    frameMax=(double)(
      (build[window->ID-1].frameEnd.tv_sec-build[window->ID-1].frameStart.tv_sec)+
      (build[window->ID-1].frameEnd.tv_usec-build[window->ID-1].frameStart.tv_usec)/1000000.0
    );

    // Calculate [frameOverHead] value
    if(build[window->ID-1].frameCount>=przecinek.frameLimit && frameMax<1.0){
      frameOverHead+=1;
    }
    else if(frameMax>=1.0 && build[window->ID-1].frameCount<przecinek.frameLimit-3){
      frameOverHead=build[window->ID-1].frameCount-przecinek.frameLimit;
    }

    if(frameMax>=1.0){
      // Set [window] [frameCount] and reset loop
      window->frameCount=build[window->ID-1].frameCount;

      build[window->ID-1].frameCount=0;
      build[window->ID-1].frameStart=build[window->ID-1].frameEnd;

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
void pWindowDestroy(pWindow *window){
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
      if(distanceMinX>figure[object1->ID-1].point[current].x){
        distanceMinX=figure[object1->ID-1].point[current].x;
      }
      if(distanceMinY>figure[object1->ID-1].point[current].y){
        distanceMinY=figure[object1->ID-1].point[current].y;
      }

      // Calculate [distanceMaxX] and [distanceMaxY] values
      if(distanceMaxX<figure[object1->ID-1].point[current].x){
        distanceMaxX=figure[object1->ID-1].point[current].x;
      }
      if(distanceMinY<figure[object1->ID-1].point[current].y){
        distanceMaxY=figure[object1->ID-1].point[current].y;
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
    distanceMinX=SHRT_MAX;
    distanceMinY=SHRT_MAX;
    distanceMaxX=SHRT_MIN;
    distanceMaxY=SHRT_MIN;
    distance=1000000;

    for(unsigned short int current=0; current<figure[object2->ID-1].vertice; current+=1){
      // Calculate [distanceMinX] and [distanceMinY] values
      if(distanceMinX>figure[object1->ID-1].point[current].x){
        distanceMinX=figure[object1->ID-1].point[current].x;
      }
      if(distanceMinY>figure[object1->ID-1].point[current].y){
        distanceMinY=figure[object1->ID-1].point[current].y;
      }

      // Calculate [distanceMaxX] and [distanceMaxY] values
      if(distanceMaxX<figure[object1->ID-1].point[current].x){
        distanceMaxX=figure[object1->ID-1].point[current].x;
      }
      if(distanceMinY<figure[object1->ID-1].point[current].y){
        distanceMaxY=figure[object1->ID-1].point[current].y;
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
  wcscpy(view[font.ID-1].directoryW, directory);
  wcstombs(view[font.ID-1].directory, directory, FONT_DIRECTORY_CHAR);

  // Check [directory] value
  if(wcslen(directory)>=4 &&
      (wcscmp(directory+wcslen(directory)-4, L".ttf")==0 || wcscmp(directory+wcslen(directory)-4, L".otf")==0)){

    // Check if [view] [directory] exists
    if(stat(view[font.ID-1].directory, &status)!=0){
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

  // Load [view] [face] and check if [font] loads properly
  if(FT_New_Face(freeType, view[font.ID-1].directory, 0, &view[font.ID-1].face)){
    if(przecinek.debug==true){
      printf("[pFontCreate() Error]\n");
      printf("FreeType2 library could not load font!\n");
      printf("Your font file might be corrupted,\n");
      fflush(stdout);
    }

    // Reset and return [font]
    pDebugFontReset(&font);
    font.ID=0;

    return font;
  }

  FT_Set_Pixel_Sizes(view[font.ID-1].face, 0, view[font.ID-1].size*1.33);

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

  // Set [code] values
  code[text.ID-1].exist=true;

  wcscpy(code[text.ID-1].valueW, value);

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

  wcscpy(texture[image.ID-1].directoryW, directory);
  wcstombs(texture[image.ID-1].directory, directory, IMAGE_DIRECTORY_CHAR);

  // Check [directory] value
  if((wcslen(directory)>=4 &&
      (wcscmp(directory+wcslen(directory)-4, L".png")==0 ||
       wcscmp(directory+wcslen(directory)-4, L".jpg")==0)) ||
      (wcslen(directory)>=5 &&
      wcscmp(directory+wcslen(directory)-5, L".jpeg")==0)){

    // Check if [texture] [directory] exists
    if(stat(texture[image.ID-1].directory, &status)!=0){
      if(przecinek.debug==true){
        printf("[pImageCreate() Error]\n");
        printf("Image located in image.directory doesn't exist!\n");
        printf("Check if you gave the correct value,\n");
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

  // Load [texture] [directory] to [fileImage]
  fileImage=fopen(texture[image.ID-1].directory, "rb");
  if(fileImage==false){
    if(przecinek.debug==true){
      printf("[pImageCreate() Error]\n");
      printf("libPNG/libJPEG library could not load texture!\n");
      printf("Your image file might be corrupted,\n");
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
