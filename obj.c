/* Przecinek made by Ciupaga; Simple program-maker engine created using CSFML.
 * GPL 3.0 (C) 2024 Ciupaga */

/* Include all libraries */
#include "main.h"
#include "func.h"

/* Create main menu scene */
int startScene(sfRenderWindow *Window, sfFont *Font, int Scene) {
    /* Create menu objects */
    sfText *Start_Text = createText(Start_Text, Font, "Press Enter to Start!", 24, 69, 69, sfBlack);
    sfTexture *Title_Texture = createTexture(Window, Title_Texture, "Sprites/Logo/Logo.png");
    sfRectangleShape *Title_Object = createTextureObject(Title_Object, Title_Texture, 1200, 300, 25, 50);

    /* Draw menu objects */
    sfRenderWindow_drawText(Window, Start_Text, NULL);
    sfRenderWindow_drawRectangleShape(Window, Title_Object, NULL);

    /* Destroy objects to clean memory */
    sfText_destroy(Start_Text);
    sfTexture_destroy(Title_Texture);
    sfRectangleShape_destroy(Title_Object);

    /* Start next scene if any key was pressed */
    if(sfKeyboard_isKeyPressed(sfKeyEnter)/* || sfMouse_isButtonPressed(sfmouse)*/){
        Scene = 2;
    }

    return Scene;
}

/* Create test scene */
void testScene(sfRenderWindow *Window, sfFont *Font) {
    
}
