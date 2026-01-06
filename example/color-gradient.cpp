#include <iostream>

#include "przecinek.h"

// Store colors
float red=0.00, green=0.00;

int main(void){
  // Initialize Przecinek, disable console, set frame limit to `60`
  pSetup(false, 60);

  // Create unresizable [window] and [event]
  pWindow window=pWindowCreate(1280, 720, false);
  pEvent event=pEventCreate();

  // Set [window] position
  window.x=300;
  window.y=300;

  // Set [window] [title] to `{,} Color Gradient`
  wcscpy(window.title, L"{,} Color Gradient");

  // Create [background] object
  pObject background=pObjectCreate(1280, 720);

  while(window.active){
    // Calculate [red] and [green] values based on cursor position
    if(przecinek.cursor.x>window.x && przecinek.cursor.x<window.x+window.width &&
        przecinek.cursor.y>window.y && przecinek.cursor.y<window.y+window.height){
      red=(float)255/window.width*(przecinek.cursor.x-window.x);
      green=(float)255/window.height*(przecinek.cursor.y-window.y);
    }

    // Update [background] colors
    background.color.red=(int)red;
    background.color.green=(int)green;

    // Draw [background]
    pWindowDrawObject(&window, &background);

    // Handle [window] [event]
    pEventHandle(&window, &event);
  }

  return 0;
}
