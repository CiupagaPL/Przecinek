/* Include all libraries */
#include "main.h"

/* Create window */
sfRenderWindow *createWindow(sfRenderWindow *Window, unsigned int WindowX, unsigned int WindowY, const char *tempWindow_title, unsigned int Window_Fps, bool isWindow_resizable) {
    sfVideoMode Window_mode = {WindowX, WindowY, 32};

    if(!isWindow_resizable) {
        Window = sfRenderWindow_create(Window_mode, tempWindow_title, sfTitlebar | sfClose, NULL);
    }
    if(isWindow_resizable) {
        Window = sfRenderWindow_create(Window_mode, tempWindow_title, sfResize | sfClose, NULL);
    }
    sfRenderWindow_setFramerateLimit(Window, Window_Fps);
    
    if(!Window) {
        sfRenderWindow_close(Window);
        printf("Error 01; Couldn't load window!\n");
    }

    return Window;
}

/* Create event */
void createEvent(sfRenderWindow *Window) {
    sfEvent Event;
    while(sfRenderWindow_pollEvent(Window, &Event)) {
        if(Event.type == sfEvtClosed || sfKeyboard_isKeyPressed(sfKeyEscape)) {
            sfRenderWindow_close(Window);
        }
    }
}

/* Create font */
sfFont *createFont(sfRenderWindow *Window, sfFont *Font, const char *tempFontDir) {
    Font = sfFont_createFromFile(tempFontDir);
    if(!Font) {
        sfRenderWindow_close(Window);
        printf("Error 02; Couldn't load font!\n");
    }

    return Font;
}

/* Create text */
sfText *createText(sfText *Text, sfFont *Font, const char *tempTextDir, unsigned int Text_Size, float Text_PositionX, float Text_PositionY) {
    Text = sfText_create();
    sfText_setString(Text, tempTextDir);
    sfText_setCharacterSize(Text, Text_Size);

    sfText_setFont(Text, Font);

    sfVector2f Text_Position;
    Text_Position.x = Text_PositionX;
    Text_Position.y = Text_PositionY;
    sfText_setPosition(Text, Text_Position);

    return Text;
}

/* Create sound buffer */
sfSoundBuffer *createSoundBuffer(sfRenderWindow *Window, sfSoundBuffer *SoundBuffer, const char *tempSoundDir) {
    SoundBuffer = sfSoundBuffer_createFromFile(tempSoundDir);
    if(!SoundBuffer) {
        sfRenderWindow_close(Window);
        printf("Error 03; Couldn't load sound!\n");
    }

    return SoundBuffer;
}

/* Create sound */
sfSound *createSound(sfSound *Sound, sfSoundBuffer *SoundBuffer) {
    sfSound_setBuffer(Sound, SoundBuffer);

    return Sound;
}

/* Create music */
sfMusic *createMusic(sfRenderWindow *Window, sfMusic *Music, const char *tempMusicDir) {
    sfMusic_createFromFile(tempMusicDir);
    if(!Music) {
        sfRenderWindow_close(Window);
        printf("Error 04; Couldn't load music!\n");
    }

    return Music;
}

/* Create texture */
sfTexture *createTexture(sfRenderWindow *Window, sfTexture *Texture, const char *tempTextureDir) {
    Texture = sfTexture_createFromFile(tempTextureDir, NULL);
    if(!Texture) {
        sfRenderWindow_close(Window);
        printf("Error 05; Couldn't load texture!\n");
    }

    return Texture;
}

/* Create object */
sfRectangleShape *createObject(sfRectangleShape *Object, sfTexture *Texture, float Object_Width, float Object_Height, float Object_PositionX, float Object_PositionY) {
    Object = sfRectangleShape_create();
    
    sfRectangleShape_setTexture(Object, Texture, sfTrue);

    sfVector2f Object_Size = {Object_Width, Object_Height};
    sfRectangleShape_setSize(Object, Object_Size);

    sfVector2f Object_Position = {Object_PositionX, Object_PositionY};
    sfRectangleShape_setPosition(Object, Object_Position);
    
    return Object;
}
