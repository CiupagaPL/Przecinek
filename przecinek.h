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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#ifndef PRZECINEK_H
#define PRZECINEK_H

#ifdef __cplusplus
extern "C"{
  #endif

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

  #define TITLE_DEF "{,}"
  #define TITLE_MAX 256

  #define KEY_MAX 256
  #define FRAME_MIN 10
  #define FRAME_MAX 640

  #define SHAPE_MAX 512
  #define SHAPE_VERTICE_MIN 3
  #define SHAPE_VERTICE_MAX 300
  #define SHAPE_ROTATION_MAX 360
  #define SHAPE_TRIANGLE 1000000

  #define IMAGE_MAX 512

  #define OBJECT_WIDTH_MIN 4
  #define OBJECT_HEIGHT_MIN 4
  #define OBJECT_WIDTH_MAX 7680
  #define OBJECT_HEIGHT_MAX 4320

  #define FONT_MAX 128
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
   *  |      |  structure and
   *  |______|  variable
   * (--------)
   ********************************/
  typedef struct{
    bool debug;
    unsigned short int windowCount, frameLimit;

    pSize display;
    pPosition cursor;
  } pPrzecinek;

  extern pPrzecinek przecinek;

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
   *  ,______,  Define [pShape]
   *  |      |  structure
   *  |______|
   * (--------)
   ********************************/
  typedef struct{
    unsigned short int ID;

    int x, y;
    unsigned short int width, height;
    unsigned short int vertice, rotation;

    pColor color;
  } pShape;

  /********************************
   *  ,______,  Define [pFont]
   *  |      |  structure
   *  |______|
   * (--------)
   ********************************/
  typedef struct{
    unsigned short int ID;

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
    unsigned short int size;

    wchar_t value[TEXT_MAX];

    pColor color;
  } pText;

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
  void pSetup(bool debug, unsigned short int frameLimit);

  /****************************************************************
   * |\_____/| pClear()
   * | .     |
   * |     . | In:
   * \ = , = / Out:
   *
   * This function clears debug console.
   * It uses `cls` or `clear` depending on the OS.
   ****************************************************************/
  void pClear();

  /****************************************************************
   * |\_____/| pWindowCreate()
   * | .     |
   * |     . | In: us_int [width], [height], bool [resize]
   * \ = , = / Out: pWindow
   *
   * This function creates Przecinek window.
   * It sets [ID] for local [window].
   * It checks if all given parameters are valid.
   * It fills all necessary values for [window] and [build].
   * It sets [window] [title] to default value.
   * It setups [window] buffer for later use.
   * It also saves time when [window] was created,
   * to later calculate frame count.
   ****************************************************************/
  pWindow pWindowCreate(unsigned short int width, unsigned short int height, bool resize);

  /****************************************************************
   * |\_____/| pWindowDrawShape()
   * | .     |
   * |     . | In: pWindow* [window], pShape* [shape]
   * \ = , = / Out:
   *
   * This function draws [shape] on [window] buffer.
   * It checks if [shape] [color] values are valid.
   * Then it does all the rendering stuff.
   ****************************************************************/
  void pWindowDrawShape(pWindow *window, pShape *shape);

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
  void pWindowDrawText(pWindow *window, pFont *font, pText *text);

  /****************************************************************
   * |\_____/| pWindowClear()
   * | .     |
   * |     . | In: pWindow* [window]
   * \ = , = / Out:
   *
   * This function fills [window] buffer with white color.
   * There isn't too much to say about it c.c
   ****************************************************************/
  void pWindowClear(pWindow *window);

  /****************************************************************
   * |\_____/| pWindowClose()
   * | .     |
   * |     . | In: pWindow* [window]
   * \ = , = / Out:
   *
   * This function closes given [window].
   * It sends signal, which's supossed to destroy [window].
   ****************************************************************/
  void pWindowClose(pWindow *window);

  /****************************************************************
   * |\_____/| pEventCreate()
   * | .     |
   * |     . | In:
   * \ = , = / Out: pEvent
   *
   * This function creates [event] object.
   * It fills all [event] variables.
   ****************************************************************/
  pEvent pEventCreate();

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
  void pEventHandle(pWindow *window, pEvent *event);

  /****************************************************************
   * |\_____/| pShapeCreate()
   * | .     |
   * |     . | In: us_int [vertice], [width], [height]
   * \ = , = / Out: pShape
   *
   * This function creates [shape].
   * It fills all [shape] variables.
   * Created [shape] depends on [vertice] count.
   ****************************************************************/
  pShape pShapeCreate(unsigned short int vertice, unsigned short int width, unsigned short int height);

  /****************************************************************
   * |\_____/| pShapeCollision()
   * | .     |
   * |     . | In: pShape* [shape1], [shape2]
   * \ = , = / Out: bool
   *
   * This function checks if two [shape] objects collides.
   * It simulates triangular collisions between several points.
   * Then it returns value based on earlier calculations.
   ****************************************************************/
  bool pShapeCollision(pShape *shape1, pShape *shape2);

  /****************************************************************
   * |\_____/| pFontCreate()
   * | .     |
   * |     . | In: wchar_t* [name], [directory], us_int [size]
   * \ = , = / Out: pFont
   *
   * This function creates [font] object. It sets [ID]
   * for local [font]. It checks if [size] value is valid.
   * It checks if [directory] exists.
   * It loads [font] and saves it to memory.
   ****************************************************************/
  pFont pFontCreate(wchar_t *name, wchar_t *directory, unsigned short int size);

  /****************************************************************
   * |\_____/| pFontClose()
   * | .     |
   * |     . | In: pFont* [font]
   * \ = , = / Out:
   *
   * This function resets given [font].
   * It deletes loaded [font] data from the memory.
   ****************************************************************/
  void pFontClose(pFont *font);

  /****************************************************************
   * |\_____/| pTextCreate()
   * | .     |
   * |     . | In: wchar_t* [value]
   * \ = , = / Out: pText
   *
   * This function creates [text] object.
   * It fills all [text] variables.
   ****************************************************************/
  pText pTextCreate(wchar_t *value);

  /****************************************************************
   * |\_____/| pKey()
   * | .     |
   * |     . | In: wchar_t* [key]
   * \ = , = / Out: us_int
   *
   * This function converts given [key] value into it's id.
   * Returned [key] id depends on current OS.
   ****************************************************************/
  unsigned short int pKey(wchar_t *key);

  #ifdef __cplusplus
}
#endif
#endif
