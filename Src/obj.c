/* Include all libraries */
#include "main.h"
#include "func.h"

/* Move object with keyboard */ 
void moveObject_Keyboard(sfRectangleShape *Object, unsigned int moveBy) {
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

/* Detect collisions */
bool detectCollision(sfRectangleShape *First_Object, sfRectangleShape *Second_Object) {
    sfFloatRect First_ObjectCollision1 = (const sfFloatRect) sfRectangleShape_getGlobalBounds(First_Object);
    sfFloatRect *First_ObjectCollision2;
    First_ObjectCollision2->left = First_ObjectCollision1.left;
    First_ObjectCollision2->top = First_ObjectCollision1.top;
    First_ObjectCollision2->width = First_ObjectCollision1.width;
    First_ObjectCollision2->height = First_ObjectCollision1.height;
    const sfFloatRect *First_ObjectCollision3 = First_ObjectCollision2;

    sfFloatRect Second_ObjectCollision1 = (const sfFloatRect) sfRectangleShape_getGlobalBounds(First_Object);
    sfFloatRect *Second_ObjectCollision2;
    Second_ObjectCollision2->left = Second_ObjectCollision1.left;
    Second_ObjectCollision2->top = Second_ObjectCollision1.top;
    Second_ObjectCollision2->width = Second_ObjectCollision1.width;
    Second_ObjectCollision2->height = Second_ObjectCollision1.height;
    const sfFloatRect *Second_ObjectCollision3 = Second_ObjectCollision2;

    if(sfFloatRect_intersects(First_ObjectCollision3, Second_ObjectCollision3, NULL)) {
        return true;
    }

    return false;
}
