/* Przecinek made by Ciupaga; Simple program-maker engine created using CSFML.
 * GPL 3.0 (C) 2024 Ciupaga */

/* Include all libraries */
#include "main.h"
#include "func.h"

/* Create main menu scene */
void mainMenu(sfRenderWindow *Window, sfFont *Font) {
    /* Create menu objects */
    sfText *Text = createText(Text, Font, "test", 24, 69, 69);

    /* Draw menu objects */
    sfRenderWindow_drawText(Window, Text, NULL);

    /* Destroy objects to clean memory */
    sfText_destroy(Text);
}

/* Create test scene */
void testScene(sfRenderWindow *Window, sfFont *Font) {
    
}
