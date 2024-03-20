/* Przecinek made by Ciupaga; Simple program-maker engine created using CSFML.
 * GPL 3.0 (C) 2024 Ciupaga */

/* Include all libraries */
#include "main.h"

/* Load functions from c file */
sfRenderWindow *createWindow(sfRenderWindow *Window, unsigned int WindowX, unsigned int WindowY, const char *tempWindow_title, unsigned int Window_Fps, bool isWindow_resizable);
void createEvent(sfRenderWindow *Window);
void limitWindow(sfRenderWindow *Window);
sfFont *createFont(sfRenderWindow *Window, sfFont *Font, const char *tempFont_Dir);
sfText *createText(sfText *Text, sfFont *Font, const char *tempText_Value, unsigned int Text_Size, float Text_PositionX, float Text_PositionY, sfColor Color);
sfSoundBuffer *createSoundBuffer(sfRenderWindow *Window, sfSoundBuffer *SoundBuffer, const char *tempSoundDir);
sfSound *createSound(sfSound *Sound, sfSoundBuffer *SoundBuffer);
sfMusic *createMusic(sfRenderWindow *Window, sfMusic *Music, const char *tempMusic_Dir);
sfTexture *createTexture(sfRenderWindow *Window, sfTexture *Texture, const char *tempTexture_Dir);
sfRectangleShape *createEmptyObject(sfRectangleShape *Object, float Object_Width, float Object_Height, float Object_PositionX, float Object_PositionY, sfColor Color);
sfRectangleShape *createTextureObject(sfRectangleShape *Object, sfTexture *Texture, float Object_Width, float Object_Height, float Object_PositionX, float Object_PositionY);
//sfRectangleShape *createAnimatedObject(sfRectangleShape *Object, int Object_TextureCount, sfTexture *Texture[Object_TextureCount], int Object_Fps);
void moveObject_Keyboard(sfRectangleShape *Object, unsigned int moveBy);
bool detectCollision(sfRectangleShape *First_Object, sfRectangleShape *Second_Object);
