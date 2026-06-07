#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

SDL_Texture* loadImage(const char *chemin, SDL_Renderer **rendu);
void afficherSpriteSheet(SDL_Texture *text, int w, int h, int frame, SDL_Renderer **rendu, SDL_Rect emplacement);
void afficherSpriteSheetEx(SDL_Texture *text, int w, int h, int frame, SDL_Renderer **rendu, SDL_Rect emplacement, const double angle, SDL_Point *center, const SDL_RendererFlip flip);
void MAJBarreAndPoint(SDL_Rect *rectPoint, SDL_Rect *rectBarreAudio, const double pos, const double duree, const int etatMusic);

#endif // RENDERER_H_INCLUDED
