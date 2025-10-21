/* |\____/| Example {,} Color Gradient Script
 * |  o o |
 */
#include <iostream>
#include <string.h>

#include "przecinek.h"

// Store colors
float red=0.00, green=0.00;

int main(void){
  // Disable console, set frame limit to `60`
  pSetup(false, 60);

  // Create unresizable [window] and [event]
  pWindow window=pWindowCreate(1280, 720, false);
  pEvent event=pEventCreate();

  // Set [window] position
  window.x=300;
  window.y=300;

  // Set [window] [title] to `debug`
  strncpy(window.title, "debug", sizeof(window.title)-1);
  window.title[sizeof(window.title)-1]='\0';

  // Create [background] object
  pObject background=pObjectCreate(1280, 720);

  while(window.active){
    // Handle [window] [event]
    pEventHandle(&window, &event);
    if(!window.active){ break; }

    // Calculate [red] and [green] values
    if(przecinek.cursor.x>window.x && przecinek.cursor.x<window.x+window.width &&
        przecinek.cursor.y>window.y && przecinek.cursor.y<window.y+window.height){
      red=(float)255/window.width*(przecinek.cursor.x-window.x);
      green=(float)255/window.height*(przecinek.cursor.y-window.y);
    }

    // Update [background] colors
    background.color.r=(int)red;
    background.color.g=(int)green;

    // Draw [background]
    pWindowDrawObject(&window, &background);
  }

  return 0;
}
