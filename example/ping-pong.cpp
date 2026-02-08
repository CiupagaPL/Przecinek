#include <iostream>
#include <cmath>

#include "przecinek.h"

int main(void){
  // Initialize Przecinek, disable console, set frame limit to `60`
  pSetup(false, 60);

  // Create unresizable [window]
  pWindow window=pWindowCreate(800, 600, false);

  // Set [window] [title] to `{,} Ping Pong`
  wcscpy(window.title, L"{,} Ping Pong");

  // Create colors
  pColor background={ 255, 255, 255, 100 };
  pColor foreground={ 50, 50, 60, 100 };
  pColor red={ 255, 125, 125, 100 };
  pColor green={ 125, 255, 125, 100 };
  pColor blue={ 125, 125, 255, 100 };

  // Create and setup [racket1] object
  pObject racket1=pObjectCreate(4, 15, 200);
  racket1.x=0;
  racket1.y=190;

  // Create and setup [racket2] object
  pObject racket2=pObjectCreate(4, 15, 200);
  racket2.x=785;
  racket2.y=190;

  // Create and setup [middle1] object
  pObject middle1=pObjectCreate(4, 10, 600);
  middle1.x=385;
  middle1.y=0;

  // Create and setup [middle2] object
  pObject middle2=pObjectCreate(4, 10, 600);
  middle2.x=395;
  middle2.y=0;

  // Create and setup [middle3] object
  pObject middle3=pObjectCreate(4, 10, 600);
  middle3.x=405;
  middle3.y=0;

  // Create and setup [ball] object
  pObject ball=pObjectCreate(12, 20, 20);
  ball.x=390;
  ball.y=290;

  // Create some more variables
  bool start=false;
  short int timer=0, angle=90, direction=10;

  while(window.ID!=0){
    // Close [window] if `ESC` is being pressed
    if(przecinek.key[pKey(L"ESC")]==1){
      pWindowDestroy(&window);
      break;
    }

    // Start the game if `SPACE` is being pressed
    if(przecinek.key[pKey(L"SPACE")]==1){
      start=true;
      timer=0;
    }

    if(start==false){
      // Manage animation
      timer+=1;
      if(timer>40){ timer=0; }
    }
    else{
      // Move [racket1] and [racket2] up if `W` or `UARROW` is being hold
      if(przecinek.key[pKey(L"W")]>=1 || przecinek.key[pKey(L"UARROW")]>=1){
        if(racket1.y>0){ racket1.y-=7; }
        if(racket2.y<400){ racket2.y+=7; }
      }
      // Move [racket1] and [racket2] down if `S` or `DARROW` is being hold
      if(przecinek.key[pKey(L"S")]>=1 || przecinek.key[pKey(L"DARROW")]>=1){
        if(racket1.y<400){ racket1.y+=7; }
        if(racket2.y>0){ racket2.y-=7; }
      }

      // Change [ball] position
      ball.x+=direction;
      if(przecinek.cursor.y-window.y<0){ ball.y=0; }
      else if(przecinek.cursor.y-window.y>=580){ ball.y=580; }
      else{ ball.y=przecinek.cursor.y-window.y; }

      // Check for collisions
      if(pObjectCollision(&racket1, &ball)==true){ direction=10; }
      else if(pObjectCollision(&racket2, &ball)==true){ direction=-10; }

      // Reset variables
      else if(ball.x+ball.width>800 || ball.x<0){
        start=false;
        direction=10;
        angle=90;

        ball.x=390;
        ball.y=290;

        racket1.x=0;
        racket1.y=190;
        racket2.x=785;
        racket2.y=190;
      }
    }

    // Clear [window]
    pWindowClear(&window, 0, 0, window.width, window.height, &background);

    // Draw objects on [window]
    pWindowDrawObject(&window, &middle1, &red);
    pWindowDrawObject(&window, &middle2, &green);
    pWindowDrawObject(&window, &middle3, &blue);

    pWindowDrawObject(&window, &racket1, &foreground);
    pWindowDrawObject(&window, &racket2, &foreground);

    if(timer<20){ pWindowDrawObject(&window, &ball, &foreground); }

    // Handle [window]
    pWindowHandle(&window);
  }

  // Close all objects
  pObjectDestroy(&middle1);
  pObjectDestroy(&middle2);
  pObjectDestroy(&middle3);
  pObjectDestroy(&racket1);
  pObjectDestroy(&racket2);
  pObjectDestroy(&ball);

  pEndup();
  return 0;
}
