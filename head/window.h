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
#include <stdbool.h>

#ifndef WINDOW_H
#define WINDOW_H

#ifdef __cplusplus
extern "C"{
  #endif

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

  /* |\____/| [pSize], [pPosition], [pColor] Structure
   * |  o o |
   */
  typedef struct{ unsigned int width, height; } pSize;
  typedef struct{ int x, y; } pPosition;
  typedef struct{ unsigned int r, g, b; } pColor;

  /* |\____/| [pPrzecinek] Structure and Variable
   * |  o o |
   */
  typedef struct{
    bool debug;
    unsigned int windowCount, frameLimit;

    pSize display;
    pPosition cursor;
  } pPrzecinek;

  extern pPrzecinek przecinek;

  /* |\____/| [pWindow] Structure
   * |  o o |
   */
  typedef struct{
    unsigned int ID;
    bool active;

    int x, y;
    unsigned int width, height;
    unsigned int widthMin, heightMin, widthMax, heightMax;

    bool resize;
    char title[TITLE_MAX];
    unsigned int border;
    bool fullScreen;
  } pWindow;

  /* |\____/| [pBuildX11], [pBuildWin] Structures
   * |  o o |
   */
  //pBuildX11, pBuildWin

  /* |\____/| [pEvent] Structure
   * |  o o |
   */
  typedef struct{
    bool focus;
    unsigned int frameCount;

    unsigned int key[KEY_MAX];
    bool keyCaps;
  } pEvent;

  /* |\____/| [pObject] Structure
   * |  o o |
   */
  typedef struct{
    int x, y;
    unsigned int width, height;

    pColor color;
  } pObject;

  /* |\_____/| pSetup() Function
   * |       | Used for initialization of the library
   * | o   o | [debug] (true/false), [frameLimit] (1:MAX)
   * \ = , = / Returns nothing
   */
  void pSetup(bool debug, unsigned int frameLimit);

  /* |\_____/| pClear() Function
   * |       | Used for clearing console
   * | o   o | None
   * \ = , = / Returns nothing
   */
  void pClear();

  /* |\_____/| pWindowReset() Function
   * |       | Used for resetting window
   * | o   o | [window]
   * \ = , = / Returns nothing
   */
  //void pWindowReset(pWindow *window);

  /* |\_____/| pWindowProc() Function
   * |       | Used for managing WIN windows
   * | o   o | [hwnd], [uMessage], [wParameter], [lParameter]
   * \ = , = / Returns `0`
   */
  //LRESULT CALLBACK pWindowProc(HWND hwnd, UINT uMessage, WPARAM wParameter, LPARAM lParameter);

  /* |\_____/| pWindowCreate() Function
   * |       | Used for creating windows
   * | o   o | [width], [height] (MIN:MAX), [resize] (true/false)
   * \ = , = / Returns [window]
   */
  pWindow pWindowCreate(unsigned int width, unsigned int height, bool resize);

  /* |\_____/| pWindowDrawObject() Function
   * |       | Used for drawing objects on window
   * | o   o | [window], [object]
   * \ = , = / Returns nothing
   */
  void pWindowDrawObject(pWindow *window, pObject *object);

  /* |\_____/| WindowClear() Function
   * |       | Used for clearing window
   * | o   o | [window]
   * \ = , = / Returns nothing
   */
  void pWindowClear(pWindow *window);

  /* |\_____/| WindowClose() Function
   * |       | Used for closing window
   * | o   o | [window]
   * \ = , = / Returns nothing
   */
  void pWindowClose(pWindow *window);

  /* |\_____/| pEventCreate() Function
   * |       | Used for creating events
   * | o   o | None
   * \ = , = / Returns [event]
   */
  pEvent pEventCreate();

  /* |\_____/| pEventHandle() Function
   * |       | Used for handling window events
   * | o   o | [window] [event]
   * \ = , = / Returns nothing
   */
  void pEventHandle(pWindow *window, pEvent *event);

  /* |\_____/| pObjectCreate() Function
   * |       | Used for creating objects
   * | o   o | [width], [height] (0:8bit)
   * \ = , = / Returns [object]
   */
  pObject pObjectCreate(unsigned int width, unsigned int height);

  /* |\_____/| pCollision() Function
   * |       | Used for checking collisions between objects
   * | o   o | [object1], [object2]
   * \ = , = / Returns (true/false)
   */
  bool pCollision(pObject object1, pObject object2);

  /* |\_____/| pKey() Function
   * |       | Used for converting key names into codes
   * | o   o | [key]
   * \ = , = / Returns (0:255)
   */
  unsigned int pKey(const char *key);

  #ifdef __cplusplus
}
#endif
#endif
