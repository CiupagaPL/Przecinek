/* Przecinek made by Ciupaga; Simple program-maker engine created using CSFML.
 * GPL 3.0 (C) 2024 Ciupaga */

/* Include all libraries */
#include "main.h"
#include "func.h"
#include "obj.h"

/* Create general-project variables */
unsigned int Scene = 1, WindowX = 1280, WindowY = 720;

/* Program control function */
int main(void) {
    /* Create window */
    sfRenderWindow *Window = createWindow(Window, WindowX, WindowY, "Przecinek", 60, false);

    /* Create general-project objects */
    sfFont *Font = createFont(Window, Font, "Fonts/FiraMono.ttf");
    sfText *Version_Text = createText(Version_Text, Font, "Build 3 [Unfinished]", 24, 0, 690);

    /* Update function */
    while(sfRenderWindow_isOpen(Window)) {
        /* Create and manage events */
        createEvent(Window);

        /* Fix window resolution */
        limitWindow(Window);

        /* Clear window with color background */
        sfRenderWindow_clear(Window, sfBlue);

        /* Draw general project objects */
        sfRenderWindow_drawText(Window, Version_Text, NULL);

        // Main menu
        if(Scene == 1) {
            mainMenu(Window, Font);
        }

        /* Display objects on window */
        sfRenderWindow_display(Window);
    }

    /* Clean resources */
    sfFont_destroy(Font);
    sfText_destroy(Version_Text);
    sfRenderWindow_destroy(Window);

    /* Return default value */
    return 0;
}
