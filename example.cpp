/*Orginal {,} Made By Ciupaga*/
/*https://github.com/CiupagaPL/Przecinek*/
/*NOTE: You can remove top comment from this file :-PP*/

#include<iostream>
#include<math.h>
using namespace std;

#include"head/window.h"

float tempRed=0.00,tempGreen=0.00;
unsigned int red=0,green=0;

int main(void){
  przecinek.debug=true;

  pWindow window=pWindowCreate(1280,720,0);
  pWindowSetBackground(&window,255,255,255);
  pWindowSetTitle(&window,"{,} Color Gradient");
  pWindowSetPosition(&window,300,300);

  while(window.on){
    pWindowHandle(&window);
    if(!window.on){ break; }

    if(przecinek.cursor.x>window.x&&przecinek.cursor.x<window.x+window.width&&
        przecinek.cursor.y>window.y&&przecinek.cursor.y<window.y+window.height){
      tempRed=(float)255/window.width*(przecinek.cursor.x-window.x);
      tempGreen=(float)255/window.height*(przecinek.cursor.y-window.y);
    }

    red=(int)round(tempRed);
    green=(int)round(tempGreen);
    if(window.red!=red||window.green!=green){ pWindowSetBackground(&window,red,green,0); }
  }

  return 0;
}
