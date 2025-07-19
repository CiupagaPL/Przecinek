/*Orginal {,} Made By Ciupaga*/
/*https://github.com/CiupagaPL/Przecinek*/
/*NOTE: You can remove top comment from this file :-PP*/

#include<iostream>
#include<cstdlib>
using namespace std;

#include"src/window.h"

int main(void){
  pDebug(true);

  pWindow window=pWindowCreate(1280,720,1);
  pWindowSetBackground(&window,255,255,255);
  pWindowSetTitle(&window,"c");
  pWindowSetPosition(&window,300,300);

  while(window.on){
    pWindowHandle(&window);
    printf("%i %i",window.cursor.x,window.cursor.y);
  }

  return 0;
}
