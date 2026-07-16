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
