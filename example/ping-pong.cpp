/* |\____/| Example {,} Ping Pong Script
 * |  o o |
 */
#include <iostream>
#include <string.h>
#include <cmath>

#include "przecinek.h"

int main(void){
  // Disable console, set frame limit to `60`
  pSetup(false, 60);

  // Create unresizable [window] and [event]
  pWindow window=pWindowCreate(800, 600, false);
  pEvent event=pEventCreate();

  // Set [window] [title] to `debug`
  strncpy(window.title, "debug", sizeof(window.title)-1);
  window.title[sizeof(window.title)-1]='\0';

  // Create and setup [racket1] object
  pObject racket1=pObjectCreate(15, 200);
  racket1.x=0;
  racket1.y=190;
  racket1.color.r=0;
  racket1.color.g=0;
  racket1.color.b=0;

  // Create and setup [racket2] object
  pObject racket2=pObjectCreate(15, 200);
  racket2.x=785;
  racket2.y=190;
  racket2.color.r=0;
  racket2.color.g=0;
  racket2.color.b=0;

  // Create and setup [middle1] object
  pObject middle1=pObjectCreate(10, 600);
  middle1.x=385;
  middle1.y=0;
  middle1.color.r=255;
  middle1.color.g=125;
  middle1.color.b=125;

  // Create and setup [middle2] object
  pObject middle2=pObjectCreate(10, 600);
  middle2.x=395;
  middle2.y=0;
  middle2.color.r=125;
  middle2.color.g=255;
  middle2.color.b=125;

  // Create and setup [middle3] object
  pObject middle3=pObjectCreate(10, 600);
  middle3.x=405;
  middle3.y=0;
  middle3.color.r=125;
  middle3.color.g=125;
  middle3.color.b=255;

  // Create and setup [ball] object
  pObject ball=pObjectCreate(20, 20);
  ball.x=390;
  ball.y=290;
  ball.color.r=0;
  ball.color.g=0;
  ball.color.b=0;

  // Create some more variables
  bool start=false;
  short int timer=0, angle=90, direction=10;

  while(window.active){
    // Close [window] if `ESC` is being pressed
    if(event.key[pKey("ESC")]==1){
      pWindowClose(&window);
      break;
    }

    // Start the game if `SPACE` is being pressed
    if(event.key[pKey("SPACE")]==1){
      start=true;
      timer=0;
    }

    if(start==false){
      // Manage animation
      timer++;
      if(timer>40){ timer=0; }
    }
    else{
      // Move [racket1] and [racket2] up if `W` or `UARROW` is being hold
      if(event.key[pKey("W")]>=1 || event.key[pKey("UARROW")]>=1){
        if(racket1.y>0 && racket2.y>0){
          racket1.y-=7;
          racket2.y-=7;
        }
      }
      // Move [racket1] and [racket2] down if `S` or `DARROW` is being hold
      if(event.key[pKey("S")]>=1 || event.key[pKey("DARROW")]>=1){
        if(racket1.y<400 && racket2.y<400){
          racket1.y+=7;
          racket2.y+=7;
        }
      }

      // Change [ball] position
      ball.x+=direction;
      if(przecinek.cursor.y-window.y<0){ ball.y=0; }
      else if(przecinek.cursor.y-window.y>=580){ ball.y=580; }
      else{ ball.y=przecinek.cursor.y-window.y; }

      // Check for collisions
      if(pObjectCollision(racket1, ball)==true){ direction=10; }
      else if(pObjectCollision(racket2, ball)==true){ direction=-10; }

      // Reset game
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
    pWindowClear(&window);

    // Draw objects
    pWindowDrawObject(&window, &middle1);
    pWindowDrawObject(&window, &middle2);
    pWindowDrawObject(&window, &middle3);

    pWindowDrawObject(&window, &racket1);
    pWindowDrawObject(&window, &racket2);

    if(timer<20){ pWindowDrawObject(&window, &ball); }

    // Handle [window] [event]
    pEventHandle(&window, &event);
  }

  return 0;
}
