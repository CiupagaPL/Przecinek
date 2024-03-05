/* Include all libraries */
#include "main.h"

/* Load functions from c file */
sfRenderWindow *createWindow(sfRenderWindow *Window, unsigned int WindowX, unsigned int WindowY, const char *tempWindow_title, unsigned int Window_Fps, bool isWindow_resizable);
void createEvent(sfRenderWindow *Window);
sfFont *createFont(sfRenderWindow *Window, sfFont *Font, const char *tempFontDir);
sfText *createText(sfText *Text, sfFont *Font, const char *tempTextDir, unsigned int Text_Size, float Text_PositionX, float Text_PositionY);
sfSoundBuffer *createSoundBuffer(sfRenderWindow *Window, sfSoundBuffer *SoundBuffer, const char *tempSoundDir);
sfSound *createSound(sfSound *Sound, sfSoundBuffer *SoundBuffer);
sfMusic *createMusic(sfRenderWindow *Window, sfMusic *Music, const char *tempMusicDir);
sfTexture *createTexture(sfRenderWindow *Window, sfTexture *Texture, const char *tempTextureDir);
sfRectangleShape *createObject(sfRectangleShape *Object, sfTexture *Texture, float Object_Width, float Object_Height, float Object_PositionX, float Object_PositionY);
