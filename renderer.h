#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

SDL_Texture* loadImage(const char *chemin, SDL_Renderer **rendu);
void afficherSpriteSheet(SDL_Texture *text, int w, int h, int frame, SDL_Renderer **rendu, SDL_Rect emplacement);

#endif // RENDERER_H_INCLUDED
