/* Include all libraries */
#include "main.h"
#include "func.h"
#include "obj.h"

/* Create general-project variables */
//int Scene = 1;

/* Program control function */
int main(void) {
    /* Create window */
    sfRenderWindow *Window = createWindow(Window, 1280, 720, "Group Project", 60, false);

    /* Create general-project objects */
    sfFont *Font = createFont(Window, Font, "../fonts/arial.ttf");
    sfText *Version_Text = createText(Version_Text, Font, "Build 2", 24, 0, 690);

    /* Create rest objects */
    sfText *Cube_Text = createText(Cube_Text, Font, "      ^- Cube <3", 84, 120, 324);
    sfTexture *Cube_Texture = createTexture(Window, Cube_Texture, "../sprites/Static/Cube.png");
    sfRectangleShape *Cube_Object = createObject(Cube_Object, Cube_Texture, 128, 128, 200, 200);
    sfTexture *TestAnimated_Texture = createTexture(Window, TestAnimated_Texture, "../sprites/Animated/LoadingIcon/1.png");
    sfRectangleShape *TestAnimated_Object = createObject(TestAnimated_Object, TestAnimated_Texture, 64, 64, 25, 25);
    sfTexture *Player_Texture = createTexture(Window, Player_Texture, "../sprites/Animated/LoadingIcon/2.png");
    sfRectangleShape *Player_Object = createObject(Player_Object, Player_Texture, 64, 64, 900, 400);

    /* Update function */
    while(sfRenderWindow_isOpen(Window)) {
        /* Create and manage events */
        createEvent(Window);
        //createTestAnimated(Window, TestAnimated_Texture, TestAnimated_Sprite, 30);
        
        /* Clear window with color background */
        sfRenderWindow_clear(Window, sfBlue);

        /* Draw general project objects */
        sfRenderWindow_drawText(Window, Version_Text, NULL);

        /* Draw rest objects */
        sfRenderWindow_drawRectangleShape(Window, Cube_Object, NULL);
        sfRenderWindow_drawText(Window, Cube_Text, NULL);
        sfRenderWindow_drawRectangleShape(Window, TestAnimated_Object, NULL);
        sfRenderWindow_drawRectangleShape(Window, Player_Object, NULL);

        moveObject_withKeyboard(Player_Object, 5);

        /* Display objects on window */
        sfRenderWindow_display(Window);
    }

    /* Clean resources by destroying all objects */
    sfRectangleShape_destroy(Cube_Object);
    sfTexture_destroy(Cube_Texture);
    sfRectangleShape_destroy(TestAnimated_Object);
    sfTexture_destroy(TestAnimated_Texture);
    sfRectangleShape_destroy(Player_Object);
    sfTexture_destroy(Player_Texture);

    sfFont_destroy(Font);
    sfText_destroy(Version_Text);
    sfRenderWindow_destroy(Window);

    /* Return default value */
    return 0;
}
