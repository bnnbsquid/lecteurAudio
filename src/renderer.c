<<<<<<< HEAD
#include <stdbool.h>
=======
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "renderer.h"

SDL_Texture* loadImage(const char *chemin, SDL_Renderer **rendu){
    SDL_Surface *img = IMG_Load(chemin);
    if(img == NULL){
        printf("IMG_Load erreur : %s\n", IMG_GetError());
        printf("chemin : %s\n", chemin);
        system("pause");
        exit(1);
    }
    SDL_Texture *sprite = SDL_CreateTextureFromSurface(*rendu, img);
    if(sprite == NULL) exit(2);
    SDL_FreeSurface(img);
    return sprite;
}

void afficherSpriteSheet(SDL_Texture *text, int w, int h, int frame, SDL_Renderer **rendu, SDL_Rect emplacement){
    SDL_Rect src = {w * frame, 0, w, h};
    SDL_RenderCopy(*rendu, text, &src, &emplacement);
}

void afficherSpriteSheetEx(SDL_Texture *text, int w, int h, int frame, SDL_Renderer **rendu, SDL_Rect emplacement, const double angle, SDL_Point *center, const SDL_RendererFlip flip){
    SDL_Rect src = {w * frame, 0, w, h};
    SDL_RenderCopyEx(*rendu, text, &src, &emplacement, angle, center, flip);
}

<<<<<<< HEAD
void MAJBarreAndPoint(SDL_Rect *rectPoint, SDL_Rect *rectBarreAudio, const double pos, const double duree, const bool etatMusic, int taille_barre_audio){ // argument pos = Mix_GetMusicPosition(audio);
    rectPoint->x = rectBarreAudio->w + rectBarreAudio->x - rectPoint->w / 2;

    if(pos > 0.0f && pos < duree && etatMusic != false)
        rectBarreAudio->w = pos * (taille_barre_audio / duree);
    else if(pos > duree && etatMusic != false)
        rectBarreAudio->w = duree * (taille_barre_audio / duree);
    else
        rectBarreAudio->w = 0;
}

void renduBoucle(SDL_Renderer **rendu,
                 SDL_Rect rectCroix, SDL_Rect rectBarreAudioFix, SDL_Rect rectBarreAudio, SDL_Rect rectBarreAudioLarge, SDL_Rect rectSautAvant, SDL_Rect rectSautApres, SDL_Rect rectPlay, SDL_Rect rectCharger, SDL_Rect rectPoint,
                 SDL_Texture *quit, SDL_Texture *playButton, SDL_Texture *chargerButton, SDL_Texture *point,
                 bool sourisOnQuit, bool btnLancer){
    afficherSpriteSheet(quit, 39, 41, sourisOnQuit, rendu, rectCroix);

    SDL_SetRenderDrawColor(*rendu, 200, 200, 200, 100);
    SDL_RenderFillRect(*rendu, &rectBarreAudioFix);

    SDL_SetRenderDrawColor(*rendu, 0, 0, 0, 255);
    SDL_RenderFillRect(*rendu, &rectBarreAudio);

    SDL_SetRenderDrawColor(*rendu, 255, 255, 255, 255);

    afficherSpriteSheet(playButton, 50, 50, 1, rendu, rectSautAvant);
    afficherSpriteSheet(playButton, 50, 50, 0, rendu, rectSautApres);
    afficherSpriteSheet(playButton, 50, 50, btnLancer + 2, rendu, rectPlay);
    afficherSpriteSheet(chargerButton, 136, 20, 0, rendu, rectCharger);
    afficherSpriteSheetEx(point, 69, 69, 0, rendu, rectPoint, 45.0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(*rendu);
    SDL_RenderClear(*rendu);
}
=======
void MAJBarreAndPoint(SDL_Rect *rectPoint, SDL_Rect *rectBarreAudio, const double pos, const double duree, const int etatMusic){ // argument pos = Mix_GetMusicPosition(audio);
    rectPoint->x = rectBarreAudio->w + rectBarreAudio->x - rectPoint->w / 2;

    if(pos > 0.0f && pos < duree && etatMusic != 0)
        rectBarreAudio->w = pos * (1839 / duree);
    else if(pos > duree && etatMusic != 0)
        rectBarreAudio->w = duree * (1839 / duree);
    else
        rectBarreAudio->w = 0;
}
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
