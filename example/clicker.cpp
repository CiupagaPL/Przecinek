#include <iostream>
using namespace std;

#include "przecinek.h"

// Variables for storing clicks
unsigned short int clickA=0, clickB=0, clickC=0;

int main(void){
  // Initialize Przecinek, enable console, set frame limit to `24`
  pSetup(true, 24);

  // Create unresizable [window]
  pWindow window=pWindowCreate(800, 600, false);

  // Set [window] [title] to `{,} Clicker`
  wcscpy(window.title, L"{,} Clicker");

  // Create [font], load it and set color values
  pFont font=pFontCreate(L"add/arial.ttf", 72);
  pColor fontColor={ 255, 255, 255, 50 };

  // Create invisible [mouse] object
  pObject mouse=pObjectCreate(4, 4, 4);
  pColor invisible={0, 0, 0, 0};

  // Create and setup [middle1] object and color
  pObject middle1=pObjectCreate(4, 200, 600);
  middle1.x=100;
  middle1.y=0;
  pColor middle1Color={ 255, 125, 125, 100 };

  // Create and setup [middle2] object
  pObject middle2=pObjectCreate(4, 200, 600);
  middle2.x=300;
  middle2.y=0;
  pColor middle2Color={ 125, 255, 125, 100 };

  // Create and setup [middle3] object
  pObject middle3=pObjectCreate(4, 200, 600);
  middle3.x=500;
  middle3.y=0;
  pColor middle3Color={ 125, 125, 255, 100 };

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

  while(window.ID!=0){
    // Update [mouse] position
    mouse.x=przecinek.cursor.x-window.x;
    mouse.y=przecinek.cursor.y-window.y;

    // Click [przecinek] on [middle1] object
    if(pObjectCollision(&mouse, &middle1)==true){
      if(przecinek.key[pKey(L"lmouse")]==1){
        clickA+=1;

        cout<<"A"<<endl;
        fflush(stdout);
      }
    }

    // Click [przecinek] on [middle2] object
    if(pObjectCollision(&mouse, &middle2)==true){
      if(przecinek.key[pKey(L"lmouse")]==1){
        clickB+=1;

        cout<<"B"<<endl;
        fflush(stdout);
      }
    }

    // Click [przecinek] on [middle3] object
    if(pObjectCollision(&mouse, &middle3)==true){
      if(przecinek.key[pKey(L"lmouse")]==1){
        clickC+=1;

        cout<<"C"<<endl;
        fflush(stdout);
      }
    }

    // Close [window] if `esc` was pressed
    if(przecinek.key[pKey(L"esc")]==1){ pWindowDestroy(&window); }

    // Update texts [value]
    swprintf(textA.value, sizeof(textA.value)/sizeof(wchar_t), L"%hu", clickA);
    swprintf(textB.value, sizeof(textB.value)/sizeof(wchar_t), L"%hu", clickB);
    swprintf(textC.value, sizeof(textC.value)/sizeof(wchar_t), L"%hu", clickC);

    // Draw objects on [window]
    pWindowDrawObject(&window, &middle1, &middle1Color);
    pWindowDrawObject(&window, &middle2, &middle2Color);
    pWindowDrawObject(&window, &middle3, &middle3Color);

    pWindowDrawObject(&window, &mouse, &invisible);

    // Draw text on [window]
    pWindowDrawText(&window, &font, &textA, &fontColor);
    pWindowDrawText(&window, &font, &textB, &fontColor);
    pWindowDrawText(&window, &font, &textC, &fontColor);

    // Handle [window]
    pWindowHandle(&window);
  }

  // Close all objects
  pFontDestroy(&font);

  pObjectDestroy(&mouse);
  pObjectDestroy(&middle1);
  pObjectDestroy(&middle2);
  pObjectDestroy(&middle3);

  pTextDestroy(&textA);
  pTextDestroy(&textB);
  pTextDestroy(&textC);

  pEndup();
  return 0;
}
