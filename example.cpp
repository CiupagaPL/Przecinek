/* |\____/| Example {,} Script
 * |  o o |
 */
#include <iostream>
#include <string.h>
using namespace std;

#include "head/window.h"

int main(void){
  pDebug(true);

  /*Create and Setup [window]*/
  pWindow window=pWindowCreate(100, 100, 2);
  
  window.width=200;
  window.height=200;

  strncpy(window.title, "debug", sizeof(window.title)-1);
  window.title[sizeof(window.title)-1]='\0';

  window.x=400;
  window.y=400;
  window.frameLimit=60;

  /*Create [event] for [window]*/
  pEvent event=pEventCreate();

  /*Create and Setup [object1]*/
  pObject object1=pObjectCreate(50, 50);
  object1.x=25;
  object1.y=25;

  object1.color.r=255;
  object1.color.g=255;
  object1.color.b=0;

  /*Create and Setup [object2]*/
  pObject object2=pObjectCreate(50, 50);
  object2.x=50;
  object2.y=50;

  object2.color.r=255;
  object2.color.g=0;
  object2.color.b=0;

  while(window.active){
    /*Move [object2]*/
    if(event.key[pKeyConvert("w")]>=1){ object2.y-=3; }
    if(event.key[pKeyConvert("a")]>=1){ object2.x-=3; }
    if(event.key[pKeyConvert("s")]>=1){ object2.y+=3; }
    if(event.key[pKeyConvert("d")]>=1){ object2.x+=3; }

    /*<3*/
    if(event.key[pKeyConvert("c")]==1){
      printf("c!! :-3\n");
      fflush(stdout);
    }

    /*Clear [window] and Draw Objects*/
    pWindowClear(&window);
    pWindowDraw(&window, &object2);
    pWindowDraw(&window, &object1);

    /*Handle [event] for [window]*/
    pEventHandle(&window, &event);
  }

  return 0;
}
