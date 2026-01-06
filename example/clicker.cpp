#include <iostream>
using namespace std;

#include "przecinek.h"

// Variables for storing clicks
unsigned short int clickA=0, clickB=0, clickC=0;

int main(void){
  // Initialize Przecinek, enable console, set frame limit to `24`
  pSetup(true, 24);

  // Create unresizable [window] and [event]
  pWindow window=pWindowCreate(800, 600, false);
  pEvent event=pEventCreate();

  // Set [window] [title] to `{,} Clicker`
  wcscpy(window.title, L"{,} Clicker");

  // Create [font], load it and set color values
  pFont font=pFontCreate(L"Arial", L"add/arial.ttf", 72);
  pColor fontColor={ 255, 255, 255, 50 };
  font.color=fontColor;

  // Create invisible [mouse] object
  pObject mouse=pObjectCreate(4, 4);

  // Create and setup [middle1] object and color
  pObject middle1=pObjectCreate(200, 600);
  middle1.x=100;
  middle1.y=0;
  pColor middle1Color={ 255, 125, 125, 100 };
  middle1.color=middle1Color;

  // Create and setup [middle2] object
  pObject middle2=pObjectCreate(200, 600);
  middle2.x=300;
  middle2.y=0;
  pColor middle2Color={ 125, 255, 125, 100 };
  middle2.color=middle2Color;

  // Create and setup [middle3] object
  pObject middle3=pObjectCreate(200, 600);
  middle3.x=500;
  middle3.y=0;
  pColor middle3Color={ 125, 125, 255, 100 };
  middle3.color=middle3Color;

  // Create [textA]
  pText textA=pTextCreate(L" ");
  textA.x=150;
  textA.y=250;

  // Create [textB]
  pText textB=pTextCreate(L" ");
  textB.x=350;
  textB.y=250;

  // Create [textC]
  pText textC=pTextCreate(L" ");
  textC.x=550;
  textC.y=250;

  // Create [background] object
  pObject background=pObjectCreate(1280, 720);

  while(window.active){
    // Update [mouse] position
    mouse.x=przecinek.cursor.x-window.x;
    mouse.y=przecinek.cursor.y-window.y;

    // Click [event] on [middle1] object
    if(pObjectCollision(&mouse, &middle1)==true){
      if(event.key[pKey(L"lmouse")]==1){
        clickA+=1;
        cout<<"A"<<endl;
        fflush(stdout);
      }
    }

    // Click [event] on [middle2] object
    if(pObjectCollision(&mouse, &middle2)==true){
      if(event.key[pKey(L"lmouse")]==1){
        clickB+=1;
        cout<<"B"<<endl;
        fflush(stdout);
      }
    }

    // Click [event] on [middle3] object
    if(pObjectCollision(&mouse, &middle3)==true){
      if(event.key[pKey(L"lmouse")]==1){
        clickC+=1;
        cout<<"C"<<endl;
        fflush(stdout);
      }
    }

    // Close [window] if `esc` was pressed
    if(event.key[pKey(L"esc")]==1){ pWindowClose(&window); }

    // Update texts [value]
    swprintf(textA.value, sizeof(textA.value)/sizeof(wchar_t), L"%hu", clickA);
    swprintf(textB.value, sizeof(textB.value)/sizeof(wchar_t), L"%hu", clickB);
    swprintf(textC.value, sizeof(textC.value)/sizeof(wchar_t), L"%hu", clickC);

    // Clear [window]
    pWindowClear(&window);

    // Draw objects on [window]
    pWindowDrawObject(&window, &middle1);
    pWindowDrawObject(&window, &middle2);
    pWindowDrawObject(&window, &middle3);

    // Draw text on [window]
    pWindowDrawText(&window, &font, &textA);
    pWindowDrawText(&window, &font, &textB);
    pWindowDrawText(&window, &font, &textC);

    // Handle [window] [event]
    pEventHandle(&window, &event);
  }

  return 0;
}
