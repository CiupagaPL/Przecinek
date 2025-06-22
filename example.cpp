/*Orginal {,} Made By Ciupaga*/
/*https://github.com/CiupagaPL/Przecinek*/
/*NOTE: You can remove top comment from this file :-PP*/

#include<iostream>
#include<cstdlib>
using namespace std;

#include"src/window.h"

int r=0,g=0,b=0,rPrev=0,gPrev=0,bPrev=0;
string temp,rString,gString,bString;

int main(void){
  pWindowCreate(1280,720,1,false);
  pWindowSetBackground(255,255,255);

  while(pWindowOn()){
    pEventHandle();
    if(!pWindowOn()){ break; }

    if(pEventKeyPress("r")){ pWindowSetBackground(255,0,0); }
    if(pEventKeyPress("g")){ pWindowSetBackground(0,255,0); }
    if(pEventKeyPress("b")){ pWindowSetBackground(0,0,255); }

    r=pWindowBackgroundRed();
    g=pWindowBackgroundGreen();
    b=pWindowBackgroundBlue();

    if(pEventKeyHold(",")){
      r=rand()%256;
      g=rand()%256;
      b=rand()%256;
      pWindowSetBackground(r,g,b);
    }

    if(rPrev!=r||gPrev!=g||bPrev!=b){
      rString=to_string(r),gString=to_string(g),bString=to_string(b);
      temp="{,} rgb("+rString+","+gString+","+bString+")";
      const char *title=temp.c_str();

      if(title!=pWindowTitle()){ pWindowSetTitle(title); }

      rPrev=r;
      gPrev=g;
      bPrev=b;
    }
  }

  pWindowCreate(300,200,2,false);
  pWindowSetTitle("Boo!");

  while(pWindowOn()){
    pEventHandle();
    if(!pWindowOn()){ break; }
  }

  return 0;
}
