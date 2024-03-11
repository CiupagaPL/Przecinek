/* Przecinek made by Ciupaga; Simple program-maker engine created using CSFML.
 * GPL 3.0 (C) 2024 Ciupaga */

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

/* Fix window resolution */
void limitWindow(sfRenderWindow *Window) {
    sfVector2u Window_Size = sfRenderWindow_getSize(Window);

    if(Window_Size.y > Window_Size.x) {
        while(Window_Size.y > Window_Size.x) {
            Window_Size.y = Window_Size.y + 1;
        }
    }
     if(Window_Size.x < 640) {
        while(Window_Size.x < 640) {
            Window_Size.x = Window_Size.x + 1;
        }
    }
    if(Window_Size.y < 360) {
        while(Window_Size.y < 360) {
            Window_Size.y = Window_Size.y + 1;
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
sfText *createText(sfText *Text, sfFont *Font, const char *tempText_Value, unsigned int Text_Size, float Text_PositionX, float Text_PositionY) {
    Text = sfText_create();
    sfText_setString(Text, tempText_Value);
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
    sfSound_create();
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

/* Create animated object */
/*sfRectangleShape *createAnimatedObject(sfRectangleShape *Object, int Object_TextureCount, sfTexture *Texture[Object_TextureCount], int Object_Fps) {
    Object = sfRectangleShape_create();
    int Object_TextureCurrentCount = 0, Object_FpsTimer = 0;
    sfRectangleShape_setTexture(Object, Texture[0], sfTrue);

    while(Object_FpsTimer != Object_Fps) {
        Object_TextureCurrentCount = Object_TextureCurrentCount + 1;
        if(Object_TextureCount < Object_TextureCurrentCount) {
            Object_TextureCurrentCount = 0;
        }
        sfRectangleShape_setTexture(Object, Texture[Object_TextureCurrentCount], sfTrue);
        Object_FpsTimer = 0;
    }

    return Object;
}*/

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
