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
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

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

/* |\____/| [pBuildX11] Structure and Variables
 * |  o o |
 */
typedef struct{
  int x, y;
  unsigned int width, height, mode;
  char title[128];

  Display *display;
  int screen;
  Window base;
  Pixmap buffer;

  Atom delete, state, fullscreen, hidden, maximized, hint;

  GC graphics;
  XColor color;
  Colormap colorMap;

  XKeyboardState keyboardState;

  struct timeval start, end;
  unsigned int frameCount;
  double frameMax;
} pBuildX11;

unsigned int activeWinID=0;

pBuildX11 build[32];
XEvent currentEvent, currentReport, currentAction;

bool debug=false;
int windowCount=0;

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

/* |\_____/| [pDebug()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pDebug(bool active){ debug=active; }

/* |\_____/| [pWindowReset()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pWindowReset(pWindow *window){
  /* |\____/| Reset [window]
   * |  o o |
   */
  window->active=false;

  window->x=0;
  window->y=0;

  window->width=0;
  window->height=0;
  window->mode=0;
  window->frameLimit=24;

  strncpy(window->title, "{,}", sizeof(window->title)-1);
  window->title[sizeof(window->title)-1]='\0';

  /* |\____/| Reset [build]
   * |  o o |
   */
  build[window->ID-1].x=0;
  build[window->ID-1].y=0;

  build[window->ID-1].width=0;
  build[window->ID-1].height=0;
  build[window->ID-1].mode=0;

  strncpy(build[window->ID-1].title, "{,}", sizeof(build[window->ID-1].title)-1);
  build[window->ID-1].title[sizeof(build[window->ID-1].title)-1]='\0';

  build[window->ID-1].display=NULL;
  build[window->ID-1].screen=0;
  build[window->ID-1].base=0;
  build[window->ID-1].buffer=0;

  build[window->ID-1].delete=None;
  build[window->ID-1].state=None;
  build[window->ID-1].fullscreen=None;
  build[window->ID-1].hidden=None;
  build[window->ID-1].maximized=None;
  build[window->ID-1].hint=None;

  build[window->ID-1].graphics=NULL;
  build[window->ID-1].colorMap=0;

  build[window->ID-1].start.tv_sec=0;
  build[window->ID-1].start.tv_usec=0;
  build[window->ID-1].end.tv_sec=0;
  build[window->ID-1].end.tv_usec=0;

  build[window->ID-1].frameCount=0;
  build[window->ID-1].frameMax=0;

  windowCount--;

  return;
}

/* |\_____/| [pWindowCreate()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
pWindow pWindowCreate(unsigned int width, unsigned int height, unsigned int mode){
  /* |\____/| Setup [window] ID
   * |  o o |
   */
  pWindow window;
  windowCount++;

  for(unsigned int current=1; current<=32; current++){
    if(build[current-1].width==0 && build[current-1].height==0){
      window.ID=current;
      break;
    }
    else if(current==32){
      if(debug==true){
        printf("[Error] Too many Windows active,\n");
        fflush(stdout);
      }

      pWindowReset(&window);
      return window;
    }
  }

  /* |\____/| Setup [build] Display
   * |  o o |
   */
  build[window.ID-1].display=XOpenDisplay(NULL);
  if(build[window.ID-1].display==NULL){
    if(debug==true){
      printf("[Error] Could not open Display,\n");
      fflush(stdout);
    }

    pWindowReset(&window);
    return window;
  }

  /* |\____/| Setup [build] Screen
   * |  o o |
   */
  build[window.ID-1].screen=DefaultScreen(build[window.ID-1].display);

  /* |\____/| Setup [build] Base
   * |  o o |
   */
  build[window.ID-1].base=XCreateSimpleWindow(
    build[window.ID-1].display,
    RootWindow(build[window.ID-1].display, build[window.ID-1].screen),
    0, 0, width, height, 1,
    BlackPixel(build[window.ID-1].display, build[window.ID-1].screen),
    BlackPixel(build[window.ID-1].display, build[window.ID-1].screen)
  );
  if(build[window.ID-1].base==0){
    if(debug==true){
      printf("[Error] Could not create Window,\n");
      fflush(stdout);
    }

    pWindowReset(&window);
    XCloseDisplay(build[window.ID-1].display);
    return window;
  }

  /* |\____/| Change [window] Variables Values
   * |  o o | TODO: Minimal [window] Size
   */
  window.active=true;

  window.x=(DisplayWidth(build[window.ID-1].display, build[window.ID-1].screen)-width)/2;
  window.y=(DisplayHeight(build[window.ID-1].display, build[window.ID-1].screen)-height)/2;

  window.width=width;
  window.height=height;
  window.mode=mode;
  window.frameLimit=24;

  XStoreName(build[window.ID-1].display, build[window.ID-1].base, "{,}");

  /* |\____/| Change [build] Variables Values
   * |  o o |
   */
  build[window.ID-1].x=window.x;
  build[window.ID-1].y=window.y;

  build[window.ID-1].width=width;
  build[window.ID-1].height=height;
  build[window.ID-1].mode=mode;

  /* |\____/| Setup [build] Status Checks
   * |  o o |
   */
  build[window.ID-1].delete=XInternAtom(build[window.ID-1].display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(build[window.ID-1].display, build[window.ID-1].base,
    &build[window.ID-1].delete, 1
  );
  build[window.ID-1].state=XInternAtom(build[window.ID-1].display,"_NET_WM_STATE",False);
  build[window.ID-1].fullscreen=XInternAtom(build[window.ID-1].display,
    "_NET_WM_STATE_FULLSCREEN",False
  );
  build[window.ID-1].hidden=XInternAtom(build[window.ID-1].display,"_NET_WM_STATE_HIDDEN",False);
  build[window.ID-1].maximized=XInternAtom(build[window.ID-1].display,
    "_NET_WM_STATE_MAXIMIZED_VERT",False
  );

  /* |\____/| Setup [build] Hints
   * |  o o |
   */
  build[window.ID-1].hint=XInternAtom(build[window.ID-1].display, "_NET_WM_HINTS", False);
  if(build[window.ID-1].hint!=None){
    long current[5]={ 1, 0, 0, 0, 0 };

    if(mode==0){
      current[1]=1;
      current[3]=1;
      current[2]=1;
    }
    if(mode==1){ current[2]=1; }

    XChangeProperty(build[window.ID-1].display, build[window.ID-1].base,
      build[window.ID-1].hint, build[window.ID-1].hint, 32, PropModeReplace,
      (unsigned char*)current, 5
    );
  }

  /* |\____/| Initialize and Show [build] Window
   * |  o o |
   */
  XSelectInput(build[window.ID-1].display, build[window.ID-1].base,
    ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | FocusChangeMask | PointerMotionMask
  );
  XMapWindow(build[window.ID-1].display, build[window.ID-1].base);
  XMoveWindow(build[window.ID-1].display, build[window.ID-1].base, window.x, window.y);
  XFlush(build[window.ID-1].display);

  build[window.ID-1].graphics=XCreateGC(build[window.ID-1].display,
    build[window.ID-1].base, 0, NULL
  );
  XSetForeground(build[window.ID-1].display, build[window.ID-1].graphics,
    WhitePixel(build[window.ID-1].display, build[window.ID-1].screen
  ));

  /* |\____/| Setup [build] Buffers
   * |  o o |
   */
  build[window.ID-1].buffer=XCreatePixmap(build[window.ID-1].display, build[window.ID-1].base,
    window.width, window.height, DefaultDepth(build[window.ID-1].display, build[window.ID-1].screen)
  );

  gettimeofday(&build[window.ID-1].start, NULL);

  return window;
}

/* |\_____/| [pWindowDraw()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pWindowDraw(pWindow *window, pObject *object){
  if(window->active==true){
    /* |\____/| Change [build] Color
     * |  o o |
     */
    build[window->ID-1].color.red=(unsigned char)object->color.r*256;
    build[window->ID-1].color.green=(unsigned char)object->color.g*256;
    build[window->ID-1].color.blue=(unsigned char)object->color.b*256;
    build[window->ID-1].color.flags=DoRed | DoGreen | DoBlue;

    build[window->ID-1].colorMap=DefaultColormap(build[window->ID-1].display,
      build[window->ID-1].screen
    );
    XAllocColor(build[window->ID-1].display, build[window->ID-1].colorMap,
      &build[window->ID-1].color
    );

    /* |\____/| Draw on [build] Buffer
     * |  o o |
     */
    XSetForeground(build[window->ID-1].display, build[window->ID-1].graphics,
      build[window->ID-1].color.pixel
    );
    XFillRectangle(build[window->ID-1].display, build[window->ID-1].buffer,
      build[window->ID-1].graphics, object->x, object->y, object->width, object->height
    );
  }
  else if(debug==true){
    printf("[Error] Could not Draw Object,\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| [WindowClear()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pWindowClear(pWindow* window){
  if(window->active==true){
    /* |\____/| Clean [build] Buffer
     * |  o o |
     */
    XSetForeground(build[window->ID-1].display, build[window->ID-1].graphics,
      WhitePixel(build[window->ID-1].display, build[window->ID-1].screen)
    );
    XFillRectangle(build[window->ID-1].display, build[window->ID-1].buffer,
      build[window->ID-1].graphics, 0, 0, window->width, window->height
    );
  }
  else if(debug==true){
    printf("[Error] Could not Clear Window,\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| [WindowClose()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pWindowClose(pWindow *window){
  if(window->active){
    /* |\____/| Send [window] Kill Event
     * |  o o |
     */
    currentAction.type=ClientMessage;
    currentAction.xclient.window=build[window->ID-1].base;
    currentAction.xclient.message_type=build[window->ID-1].delete;
    currentAction.xclient.format=32;
    currentAction.xclient.data.l[0]=build[window->ID-1].delete;
    currentAction.xclient.data.l[1]=CurrentTime;

    XSendEvent(build[window->ID-1].display, build[window->ID-1].base, False, NoEventMask, &currentAction);
  }
  else if(debug){
    printf("[Warning] Window is already closed,\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| [pEventReset()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pEventReset(pEvent *event){
  /* |\____/| Reset [event]
   * |  o o |
   */
  event->focus=false;
  event->frameCount=0;

  for(unsigned int current=0; current<256; current++){
    event->key[current]=0;
  }
  event->keyCaps=false;

  event->positionChange=false;
  event->sizeChange=false;
  event->cursorMove=false;

  event->display.width=0;
  event->display.height=0;
  event->cursor.x=0;
  event->cursor.y=0;

  event->windowCount=0;
  event->debug=false;

  return;
}

/* |\_____/| [pEventCreate()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
pEvent pEventCreate(){
  /* |\____/| Create and Set [event] Values
   * |  o o |
   */
  pEvent event;

  event.focus=false;
  event.frameCount=0;

  for(unsigned int current=0; current<256; current++){
    event.key[current]=0;
  }
  event.keyCaps=false;

  event.positionChange=false;
  event.sizeChange=false;
  event.cursorMove=false;

  event.display.width=0;
  event.display.height=0;
  event.cursor.x=0;
  event.cursor.y=0;

  event.windowCount=0;
  event.debug=0;

  return event;
}

/* |\_____/| [pEventHandle()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
void pEventHandle(pWindow *window, pEvent *event){
  if(window->active==true){
    /* |\____/| Change [window] Buffers
     * |  o o |
     */
    XCopyArea(build[window->ID-1].display, build[window->ID-1].buffer, build[window->ID-1].base,
      DefaultGC(build[window->ID-1].display, build[window->ID-1].screen),
      0, 0, window->width, window->height, 0, 0
    );
    XFlush(build[window->ID-1].display);

    /* |\____/| Reset [event] Keys
     * |  o o |
     */
    for(unsigned int current=0; current<256; current++){
      if(event->key[current]==1){ event->key[current]=2; }
    }

    /* |\____/| Update [event] Variables
     * |  o o |
     */
    event->windowCount=windowCount;
    event->debug=debug;

    while(XPending(build[window->ID-1].display)){
      XNextEvent(build[window->ID-1].display, &currentEvent);

      /* |\____/| Check for Close [currentEvent]
       * |  o o |
       */
      if(currentEvent.type==ClientMessage &&
          (Atom)currentEvent.xclient.data.l[0]==build[window->ID-1].delete){
        pWindowReset(window);
        XFreePixmap(build[window->ID-1].display, build[window->ID-1].buffer);
        XFreeGC(build[window->ID-1].display, build[window->ID-1].graphics);
        XDestroyWindow(build[window->ID-1].display, build[window->ID-1].base);
        XCloseDisplay(build[window->ID-1].display);
        pEventReset(event);

        return;
      }
      else{ window->active=true; }

      /* |\____/| Manage [window] position change
       * |  o o |
       */
      if(build[window->ID-1].x!=window->x || build[window->ID-1].y!=window->y){
        build[window->ID-1].x=window->x;
        build[window->ID-1].y=window->y;

        XMoveWindow(build[window->ID-1].display, build[window->ID-1].base, window->x, window->y);

        event->positionChange=true;
      }
      else if(currentEvent.type==ConfigureNotify && window->x!=currentEvent.xconfigure.x){
        window->x=currentEvent.xconfigure.x;
        window->y=currentEvent.xconfigure.y;
        build[window->ID-1].x=currentEvent.xconfigure.x;
        build[window->ID-1].y=currentEvent.xconfigure.y;

        event->positionChange=true;
      }
      else{ event->positionChange=false; }

      /* |\____/| Manage [window] size change
       * |  o o |
       */
      if(build[window->ID-1].width!=window->width || build[window->ID-1].height!=window->height){
        build[window->ID-1].width=window->width;
        build[window->ID-1].height=window->height;

        XResizeWindow(build[window->ID-1].display, build[window->ID-1].base,
          window->width, window->height
        );
        build[window->ID-1].buffer=XCreatePixmap(build[window->ID-1].display,
          build[window->ID-1].base, window->width, window->height,
          DefaultDepth(build[window->ID-1].display, build[window->ID-1].screen)
        );

        event->sizeChange=true;
      }
      else if(currentEvent.type==ConfigureNotify && window->width!=currentEvent.xconfigure.width){
        window->width=currentEvent.xconfigure.width;
        window->height=currentEvent.xconfigure.height;
        build[window->ID-1].width=currentEvent.xconfigure.width;
        build[window->ID-1].height=currentEvent.xconfigure.height;

        build[window->ID-1].buffer=XCreatePixmap(build[window->ID-1].display,
          build[window->ID-1].base, window->width, window->height,
          DefaultDepth(build[window->ID-1].display, build[window->ID-1].screen)
        );

        event->sizeChange=true;
      }
      else{ event->sizeChange=false; }

      /* |\____/| Check for Key Press [currentEvent]
       * |  o o |
       */
      if(currentEvent.type==KeyPress){
        if(event->key[currentEvent.xkey.keycode]==0){ event->key[currentEvent.xkey.keycode]=1; }
        else{ event->key[currentEvent.xkey.keycode]=2; }
      }

      /* |\____/| Check for Key Release [currentEvent]
       * |  o o |
       */
      if(currentEvent.type==KeyRelease){
        if(XEventsQueued(build[window->ID-1].display, QueuedAfterReading)){
          XPeekEvent(build[window->ID-1].display, &currentReport);

          if(currentReport.type==KeyPress &&
              currentReport.xkey.time==currentEvent.xkey.time &&
              currentReport.xkey.keycode==currentEvent.xkey.keycode){
            event->key[currentEvent.xkey.keycode]=2;

            XNextEvent(build[window->ID-1].display, &currentEvent);
          }
          else{ event->key[currentEvent.xkey.keycode]=0; }
        }
        else{ event->key[currentEvent.xkey.keycode]=0; }
      }

      /* |\____/| Check for Cursor Move [currentEvent]
       * |  o o |
       */
      if(currentEvent.type==MotionNotify){
        event->cursor.x=currentEvent.xmotion.x_root;
        event->cursor.y=currentEvent.xmotion.y_root;

        event->cursorMove=true;
      }
      else{ event->cursorMove=false; }

      /* |\____/| Check for Window Focus In [currentEvent]
       * |  o o |
       */
      if(currentEvent.type==FocusIn){
        activeWinID=window->ID;
        event->focus=true;
      }

      /* |\____/| Check for Window Focus Out [currentEvent]
       * |  o o |
       */
      if(currentEvent.type==FocusOut && activeWinID==window->ID){
        activeWinID=0;
        for(unsigned int current=0; current<256; current++){
          event->key[current]=0;
        }

        event->focus=false;
      }

      /* |\____/| Update [event] Display Size
       * |  o o |
       */
      event->display.width=DisplayWidth(build[window->ID-1].display, build[window->ID-1].screen);
      event->display.height=DisplayHeight(build[window->ID-1].display, build[window->ID-1].screen);

      /* |\____/| Update [event] Key Caps Status
       * |  o o |
       */
      XGetKeyboardControl(build[window->ID-1].display, &build[window->ID-1].keyboardState);
      event->keyCaps=(build[window->ID-1].keyboardState.led_mask&(1<<1)!=0);

      /* |\____/| Update [window] Title
       * |  o o |
       */
      if(window->title!=build[window->ID-1].title){
        if(strlen(window->title)>128){
          strncpy(window->title, build[window->ID-1].title, sizeof(window->title)-1);
          window->title[sizeof(window->title)-1]='\0';

          if(debug==true){
            printf("[Error] Window Title is too Long,\n");
            fflush(stdout);
          }
        }
        else{
          strncpy(build[window->ID-1].title, window->title, sizeof(build[window->ID-1].title)-1);
          build[window->ID-1].title[sizeof(build[window->ID-1].title)-1]='\0';

          XStoreName(build[window->ID-1].display, build[window->ID-1].base, window->title);
        }
      }

      /* |\____/| Update [window] Mode
       * |  o o |
       */
      window->mode=build[window->ID-1].mode;
    }

    /* |\____/| Limit and Count [window] Frames
     * |  o o |
     */
    build[window->ID-1].frameCount++;
    usleep((1000000/windowCount)/window->frameLimit);

    gettimeofday(&build[window->ID-1].end, NULL);
    build[window->ID-1].frameMax=(double)((build[window->ID-1].end.tv_sec-build[window->ID-1].start.tv_sec)+
      (build[window->ID-1].end.tv_usec-build[window->ID-1].start.tv_usec)/1000000.0);

    if(build[window->ID-1].frameMax>=1.0){
      build[window->ID-1].start=build[window->ID-1].end;

      event->frameCount=build[window->ID-1].frameCount;
      build[window->ID-1].frameCount=0;
    }
  }
  else if(debug==true){
    printf("[Error] Could not Handle Last Event,\n");
    fflush(stdout);
  }

  return;
}

/* |\_____/| [pObjectCreate()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
pObject pObjectCreate(unsigned int width, unsigned int height){
  /* |\____/| Create and Set [object] Values
   * |  o o |
   */
  pObject object;

  object.x=0;
  object.y=0;

  object.width=width;
  object.height=height;

  object.color.r=0;
  object.color.g=0;
  object.color.b=0;

  return object;
}

/* |\_____/| [pKeyConvert()] Function
 * |       |
 * | o   o |
 * \ = , = /
 */
unsigned int pKeyConvert(const char *key){
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
