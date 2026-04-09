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

// Standard C libraries
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef PRZECINEK_H
#define PRZECINEK_H

#ifdef __cplusplus
extern "C"{
  #endif

  /********************************
   *  ,______,  Define [pSize],
   *  |      |  [pPosition],
   *  |______|  [pColor] structures
   * (--------)
   ********************************/
  typedef struct{ unsigned short int width, height; } pSize;
  typedef struct{ short int x, y; } pPosition;
  typedef struct{ unsigned short int red, green, blue, alpha; } pColor;

  /********************************
   *  ,______,  Define default
   *  |      |  values
   *  |______|
   * (--------)
   ********************************/
  #define PRZECINEK_VERSION L"a"
  #define PRZECINEK_VERSION_MAJOR 5
  #define PRZECINEK_VERSION_MINOR 0
  //#define PRZECINEK_VERSION_PATCH L"a"
  #define PRZECINEK_UNSTABLE 20
  #define PRZECINEK_EXPERIMENTAL false

  #define PRZECINEK_KEY_MAX 256
  #define PRZECINEK_FRAME_MIN 10
  #define PRZECINEK_FRAME_MAX 480

  #define COLOR_DEFAULT_BACKGROUND (pColor){ 255, 255, 255, 255 }
  #define COLOR_DEFAULT_FOREGROUND (pColor){ 50, 50, 60, 255 }

  #define WINDOW_MAX 4
  #define OBJECT_MAX 512
  #define FONT_MAX 16
  #define TEXT_MAX 128
  #define IMAGE_MAX 128
  #define AUDIO_MAX 32

  #define WINDOW_WIDTH_MIN 128
  #define WINDOW_HEIGHT_MIN 128
  #define WINDOW_WIDTH_MAX SHRT_MAX
  #define WINDOW_HEIGHT_MAX SHRT_MAX
  #define WINDOW_X_DEF 256
  #define WINDOW_Y_DEF 256
  #define WINDOW_TITLE_CHAR 256
  #define WINDOW_TITLE_DEF "{,} Window"

  #define OBJECT_WIDTH_MIN 4
  #define OBJECT_HEIGHT_MIN 4

  #define OBJECT_VERTICE_MIN 3
  #define OBJECT_VERTICE_MAX 1024

  #define FONT_SIZE_MIN 4
  #define FONT_SIZE_MAX 1024
  #define FONT_DIRECTORY_CHAR 256

  #define TEXT_CHAR 2048

  #define IMAGE_DIRECTORY_CHAR 256

  #define AUDIO_VOLUME_MAX 200
  #define AUDIO_REFRESH_RATE 200000
  #define AUDIO_SLEEP_RATE 40

  /********************************
   *  ,______,  Define [pPrzecinek]
   *  |      |  structure
   *  |______|
   * (--------)
   ********************************/
  typedef struct{
    bool debug;
    unsigned short int windowCount, frameLimit;

    unsigned short int key[PRZECINEK_KEY_MAX];
    bool keyCaps;

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

    short int x, y;
    unsigned short int width, height;
    unsigned short int widthMin, heightMin, widthMax, heightMax;

    wchar_t title[WINDOW_TITLE_CHAR];
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

    short int x, y;
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
    short int letterSpacing, spaceSpacing, lineSpacing;
    wchar_t directory[FONT_DIRECTORY_CHAR];
  } pFont;

  /********************************
   *  ,______,  Define [pText]
   *  |      |  structure
   *  |______|
   * (--------)
   ********************************/
  typedef struct{
    unsigned short int ID;

    short int x, y;

    wchar_t value[TEXT_CHAR];
  } pText;

  /********************************
   *  ,______,  Define [pImage]
   *  |      |  structure
   *  |______|
   * (--------)
   ********************************/
  typedef struct{
    unsigned short int ID;

    wchar_t directory[IMAGE_DIRECTORY_CHAR];
  } pImage;

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
   * |\_____/| pEndup()
   * | .     |
   * |     . | In:
   * \ = , = / Out:
   *
   * This function cleans up debug variables before the end of
   * the program. It should be used when all windows are closed.
   ****************************************************************/
  void pEndup();

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
   * |\_____/| pKey()
   * | .     |
   * |     . | In: wchar_t* [key]
   * \ = , = / Out: us_int
   *
   * This function converts given [key] value into it's id.
   * Returned [key] id depends on current OS.
   ****************************************************************/
  unsigned short int pKey(wchar_t *key);

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
  pWindow pWindowCreate(unsigned short int width, unsigned short int height, bool resizable);

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
  void pWindowDrawObject(pWindow *window, pObject *object, pColor *color, pImage *image);

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
  void pWindowDrawText(pWindow *window, pFont *font, pText *text, pColor *color);

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
  );

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
  void pWindowDestroy(pWindow *window);

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
  void pWindowHandle(pWindow *window);

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
  pObject pObjectCreate(unsigned short int vertice, unsigned short int width, unsigned short int height);

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
  bool pObjectCollision(pObject *object1, pObject *object2);

  /****************************************************************
   * |\_____/| pObjectDestroy()
   * | .     |
   * |     . | In: pObject* [object]
   * \ = , = / Out:
   *
   * This function destroys given [object].
   * It also resets [object] `ID` to `0`.
   ****************************************************************/
  void pObjectDestroy(pObject *object);

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
  pFont pFontCreate(wchar_t *directory, unsigned short int size);

  /****************************************************************
   * |\_____/| pFontDestroy()
   * | .     |
   * |     . | In: pFont* [font]
   * \ = , = / Out:
   *
   * This function destroys given [font].
   * It also resets [font] `ID` to `0`.
   ****************************************************************/
  void pFontDestroy(pFont *font);

  /****************************************************************
   * |\_____/| pTextCreate()
   * | .     |
   * |     . | In: wchar_t* [value]
   * \ = , = / Out: pText
   *
   * This function creates [text] object. It sets [ID]
   * for local [text]. It fills all [text] variables.
   ****************************************************************/
  pText pTextCreate(wchar_t *value);

  /****************************************************************
   * |\_____/| pTextDestroy()
   * | .     |
   * |     . | In: pText* [text]
   * \ = , = / Out:
   *
   * This function destroys given [text].
   * It also resets [text] `ID` to `0`.
   ****************************************************************/
  void pTextDestroy(pText *text);

  /****************************************************************
   * |\_____/| pImageCreate()
   * | .     |
   * |     . | In: wchar_t* [directory]
   * \ = , = / Out: pImage
   *
   * This function creates [image] object. It sets [ID]
   * for local [image]. It fills all [image] variables.
   ****************************************************************/
  pImage pImageCreate(wchar_t *directory);

  /****************************************************************
   * |\_____/| pImageDestroy()
   * | .     |
   * |     . | In: pImage* [image]
   * \ = , = / Out:
   *
   * This function destroys given [image].
   * It also resets [image] `ID` to `0`.
   ****************************************************************/
  void pImageDestroy(pImage *image);

  /******************************************
   *  ,______,  [pAudio] structure
   *  |      |
   *  |______|  [DEBUG]
   * (--------)
   ******************************************/
  typedef struct{
    uint16_t ID;

    // Volume percentage (`0` -> AUDIO_VOLUME_MAX)
    uint16_t volume;
    // Audio pause state
    bool pause;

    // Audio length (current, max)
    uint32_t frame, frameMax;
  } pAudio;

  /****************************************************************
   * |\_____/| pAudioCreate()
   * | .     |
   * |     . | In: pAudio* [audio], wchar_t* [directory]
   * \ = , = / Out: uint8_t (`0` -> finished succesfully)
   *
   * Parameters:
   * [audio] - which sound object should be initialized. If
   * given sound is already created, then it will be overwritten.
   * [directory] - from where the sound should be loaded.
   *
   * Additional Description:
   * This function choses [ID] for given [audio], then
   * it setups all needed values, loads file from given [directory]
   * and creates sound thread.
   ****************************************************************/
  uint8_t pAudioCreate(pAudio *audio, wchar_t *directory);

  /****************************************************************
   * |\_____/| pAudioDestroy()
   * | .     |
   * |     . | In: pAudio* [audio]
   * \ = , = / Out: uint8_t (`0` -> finished succesfully)
   *
   * Parameters:
   * [audio] - which sound object should be destroyed.
   * Removed debug values will depend on [audio] [ID].
   ****************************************************************/
  uint8_t pAudioDestroy(pAudio *audio);

  #ifdef __cplusplus
}
#endif
#endif
