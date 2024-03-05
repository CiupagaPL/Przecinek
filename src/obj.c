/* Include all libraries */
#include "main.h"
#include "func.h"

/* Move object with keyboard */ 
void moveObject_withKeyboard(sfRectangleShape *Object, unsigned int moveBy) {
    sfVector2f Object_Position = sfRectangleShape_getPosition(Object);

    if(sfKeyboard_isKeyPressed(sfKeyUp)) {
        Object_Position.y = Object_Position.y - moveBy;
        sfRectangleShape_setPosition(Object, Object_Position);
    }
    if(sfKeyboard_isKeyPressed(sfKeyDown)) {
        Object_Position.y = Object_Position.y + moveBy;
        sfRectangleShape_setPosition(Object, Object_Position);
    }
    if(sfKeyboard_isKeyPressed(sfKeyLeft)) {
        Object_Position.x = Object_Position.x - moveBy;
        sfRectangleShape_setPosition(Object, Object_Position);
    }
    if(sfKeyboard_isKeyPressed(sfKeyRight)) {
        Object_Position.x = Object_Position.x + moveBy;
        sfRectangleShape_setPosition(Object, Object_Position);
    }
}

/* Do something when there is collision between objects */
/*bool detectCollision(sfRectangleShape *First_Object, sfRectangleShape *Second_Object) {
    const sfFloatRect *First_ObjectCollision = sfRectangleShape_getGlobalBounds(First_Object);
    const sfFloatRect *Second_ObjectCollision; //= sfRectangleShape_getGlobalBounds(Second_Object);

    if(sfFloatRect_intersects(First_ObjectCollision, Second_ObjectCollision, NULL)) {
        return true;
    } 

    return false;
}*/

/* Animate sprites */
/*void createTestAnimated(sfRenderWindow *Window, sfTexture *Texture, sfSprite *Sprite, int Texture_Fps) {
    int Texture_timer;
    while(1) {
        Texture_timer = Texture_timer + 1;
    }
    
    if(Texture_timer == Texture_Fps) {
        createTexture(Window, Texture, "../sprites/Animated/LoadingIcon/2.png");
    }
    if(Texture_timer == Texture_Fps * 2) {
        createTexture(Window, Texture, "../sprites/Animated/LoadingIcon/3.png");
    }
    if(Texture_timer == Texture_Fps * 3) {
        createTexture(Window, Texture, "../sprites/Animated/LoadingIcon/4.png");
    }
    if(Texture_timer == Texture_Fps * 4) {
        createTexture(Window, Texture, "../sprites/Animated/LoadingIcon/5.png");
    }
    if(Texture_timer == Texture_Fps * 5) {
        createTexture(Window, Texture, "../sprites/Animated/LoadingIcon/6.png");
    }
    if(Texture_timer == Texture_Fps * 6) {
        createTexture(Window, Texture, "../sprites/Animated/LoadingIcon/7.png");
    }
    if(Texture_timer == Texture_Fps * 7) {
        createTexture(Window, Texture, "../sprites/Animated/LoadingIcon/8.png");
    }
    if(Texture_timer == Texture_Fps * 8) {
        createTexture(Window, Texture, "../sprites/Animated/LoadingIcon/1.png");
        Texture_timer = 0;
    }
}*/
