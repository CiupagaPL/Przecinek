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

  /* |\____/| [pDebug], [pWindow] Functions
   * |  o o |
   */
  void pDebug(bool active);

  pWindow pWindowCreate(unsigned int width, unsigned int height, unsigned int mode);
  void pWindowDraw(pWindow *window, pObject *object);
  void pWindowClear(pWindow *window);
  void pWindowClose(pWindow *window);

  /* |\____/| [pEvent], [pObject], [pKey] Functions
   * |  o o |
   */
  pEvent pEventCreate();
  void pEventHandle(pWindow *window, pEvent *event);

  pObject pObjectCreate(unsigned int width, unsigned int height);

  unsigned int pKeyConvert(const char *key);

  #ifdef __cplusplus
}
#endif
#endif
