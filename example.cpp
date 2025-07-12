/*Orginal {,} Made By Ciupaga*/
/*https://github.com/CiupagaPL/Przecinek*/
/*NOTE: You can remove top comment from this file :-PP*/

#include<iostream>
#include<cstdlib>
using namespace std;

#include"src/window.h"

int redPrev=0,greenPrev=0,bluePrev=0;
string temp,redString,greenString,blueString;

int main(void){
  pDebug(false);

  pWindow window=pWindowCreate(1280,720,1);
  pWindowSetBackground(&window,255,255,255);

  while(window.on){
    pWindowHandle(&window);
    if(!window.on){ break; }

    if(pKeyPress("r")){ pWindowSetBackground(&window,255,0,0); }
    if(pKeyPress("g")){ pWindowSetBackground(&window,0,255,0); }
    if(pKeyPress("b")){ pWindowSetBackground(&window,0,0,255); }

    if(pKeyHold(",")){
      printf("rgb(%i,%i,%i)\n",rand()%256,rand()%256,rand()%256);
    } if(pKeyPress(",")){
      window.red=rand()%256;
      window.green=rand()%256;
      window.blue=rand()%256;
      pWindowSetBackground(&window,window.red,window.green,window.blue);
    }

    if(redPrev!=window.red||greenPrev!=window.green||bluePrev!=window.blue){
      redString=to_string(window.red),greenString=to_string(window.green),blueString=to_string(window.blue);
      temp="{,} rgb("+redString+","+greenString+","+blueString+")";
      const char *title=temp.c_str();

      if(title!=window.title){ pWindowSetTitle(&window,title); }

      redPrev=window.red;
      greenPrev=window.green;
      bluePrev=window.blue;
    }
  } if(!window.on){
    pWindow boo=pWindowCreate(300,200,2);
    pWindowSetTitle(&boo,"Boo!");
    pWindowSetPosition(&boo,300,200);

    while(boo.on){
      pWindowHandle(&boo);
      if(!boo.on){ break; }
    }
  }

  return 0;
}
