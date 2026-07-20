#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

SDL_Texture* loadImage(const char *chemin, SDL_Renderer **rendu);
void afficherSpriteSheet(SDL_Texture *text, int w, int h, int frame, SDL_Renderer **rendu, SDL_Rect emplacement);
void afficherSpriteSheetEx(SDL_Texture *text, int w, int h, int frame, SDL_Renderer **rendu, SDL_Rect emplacement, const double angle, SDL_Point *center, const SDL_RendererFlip flip);
void MAJBarreAndPoint(SDL_Rect *rectPoint, SDL_Rect *rectBarreAudio, const double pos, const double duree, const bool etatMusic, int taille_barre_audio);
void renduBoucle(SDL_Renderer **rendu,
                 SDL_Rect rectCroix, SDL_Rect rectBarreAudioFix, SDL_Rect rectBarreAudio, SDL_Rect rectBarreAudioLarge, SDL_Rect rectSautAvant, SDL_Rect rectSautApres, SDL_Rect rectPlay, SDL_Rect rectCharger, SDL_Rect rectPoint,
                 SDL_Texture *quit, SDL_Texture *playButton, SDL_Texture *chargerButton, SDL_Texture *point,
                 bool sourisOnQuit, bool btnLancer);

#endif // RENDERER_H_INCLUDED
