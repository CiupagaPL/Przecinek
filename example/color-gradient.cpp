#include <iostream>

#include "przecinek.h"

// Store colors
float red=0.00, green=0.00;

int main(void){
  // Initialize Przecinek, disable console, set frame limit to `60`
  pSetup(false, 60);

  // Create unresizable [window]
  pWindow window=pWindowCreate(1280, 720, false);

  // Set [window] position
  window.x=300;
  window.y=300;

  // Set [window] [title] to `{,} Color Gradient`
  wcscpy(window.title, L"{,} Color Gradient");

  // Create [background] color
  pColor background={ 0, 0, 0, 100 };

  while(window.ID!=0){
    // Calculate [red] and [green] values based on cursor position
    if(przecinek.cursor.x>window.x && przecinek.cursor.x<window.x+window.width &&
        przecinek.cursor.y>window.y && przecinek.cursor.y<window.y+window.height){
      red=(float)255/window.width*(przecinek.cursor.x-window.x);
      green=(float)255/window.height*(przecinek.cursor.y-window.y);
    }

    // Update [background] parameters
    background.red=red;
    background.green=green;

    // Clear [window] with [background] color
    pWindowClear(&window, 0, 0, window.width, window.height, &background);

    // Handle [window]
    pWindowHandle(&window);
  }

  pEndup();
  return 0;
}
