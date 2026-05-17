#define SDL_MAIN_HANDLED
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <windows.h>
#include <commdlg.h>
#include <sndfile.h>
//#include <curl/curl.h>

#ifdef _WIN32
    #define popen _popen
    #define pclose _pclose
#endif

#define MAX_MUS_CHARGER 1024

int ouvrirBoiteFichier(char *chemin, DWORD taille){
    OPENFILENAME ofn;

    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(chemin, taille);

    chemin[0] = '\0';   // très important

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetForegroundWindow();   // évite ouverture derrière
    ofn.lpstrFile = chemin;
    ofn.nMaxFile = taille;

    ofn.lpstrFilter =
        "Audio (*.mp3;*.ogg;*.flac;*.wav;*.aiff;*.aif)\0*.mp3;*.ogg;*.flac;*.wav;*.aiff;*.aif\0"
        "Tous les fichiers (*.*)\0*.*\0";

    ofn.lpstrInitialDir = "C:\\";   // <-- stabilise la boîte
    ofn.lpstrTitle = "Selectionne un fichier audio";

    ofn.Flags =
        OFN_EXPLORER |
        OFN_PATHMUSTEXIST |
        OFN_FILEMUSTEXIST |
        OFN_NOCHANGEDIR |
        OFN_ALLOWMULTISELECT;

    if (GetOpenFileName(&ofn))
        return 1;

    // pour savoir si c'est une vraie erreur :
    DWORD err = CommDlgExtendedError();
    if(err != 0)
        printf("Erreur Win32: %lu\n", err);

    return 0;
}

int initialisation(SDL_Window **fenetre, SDL_Renderer **rendu) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("Erreur initialisation SDL_image : %s\n", IMG_GetError());
    return 5;
    }

    if (Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 2048) < 0) {
        printf("Erreur SDL_mixer");
        return 1;
    }

    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

    *fenetre = SDL_CreateWindow("lecteur mp3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP);

    *rendu = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetLogicalSize(*rendu, 1920, 1080);
    return 0;
}

void decharger_audio(Mix_Music **audio){
    if(*audio){
        if(Mix_PlayingMusic()){
            Mix_HaltMusic();
        }
        if(audio)
            Mix_FreeMusic(*audio);
        *audio = NULL;
    }
}

double duree_mus(char *chemin_audio){
    SF_INFO info;
    SNDFILE *file = sf_open(chemin_audio, SFM_READ, &info);

    if (!file) return 0.0;

    double duree = (double)info.frames / info.samplerate;

    sf_close(file);
    return duree;
}

int charger_audio(Mix_Music **audio, char *chemin_audio, double *duree){
    FILE *file = fopen(chemin_audio, "rb");
    if (!file) {
        printf("fopen failed\n");
        return 1;
    }

    SDL_RWops *rw = SDL_RWFromFP(file, 1);
    if (!rw) {
        printf("Erreur SDL_RWFromFile: %s\n", SDL_GetError());
        return 1;
    }

    decharger_audio(&audio[0]);
    if(!audio[0]){
        audio[0] = Mix_LoadMUS_RW(rw, 1);
        if(audio[0] == NULL){
            printf("Erreur Mix_LoadMUS_RW : %s\n", SDL_GetError());
            return 1;
        }
        *duree = duree_mus(chemin_audio);
    }
    else{
        return 1;
    }
    return 0;
}

/*double duree_mus(char *chemin_audio){
    FILE *fip;
    char cmd[MAX_PATH + 66];
    char buf[128];
    double duree = 0.0;

    snprintf(cmd, sizeof(cmd),
        "ffprobe -v error -show_entries format=duration "
        "-of default=noprint_wrappers=1:nokey=1 \"%s\"",
        chemin_audio);

    fip = popen(cmd, "r");
    if (!fip) return 0.0;

    if (fgets(buf, sizeof(buf), fip)) {
        printf("DEBUG RAW: %s\n", buf); // <-- IMPORTANT
        duree = strtod(buf, NULL);
    }

    pclose(fip);
    return duree;
}*/

int charger_audios(char *chemin_audio, char audios_charger[MAX_MUS_CHARGER][MAX_PATH]){
    char *p = chemin_audio;

    // premier élément = dossier
    char dossier[MAX_PATH];
    strcpy(dossier, p);

    p += strlen(p) + 1;

    // s'il n'y a qu'un seul fichier
    if (*p == '\0') {

        for(int i = 0; i < MAX_MUS_CHARGER; i++){
            audios_charger[i][0] = '\0';
        }

        strcpy(audios_charger[0], chemin_audio);
        return 1;

    } else {

        for(int i = 0; i < MAX_MUS_CHARGER; i++){
            audios_charger[i][0] = '\0';
        }

        // plusieurs fichiers
        char buf[MAX_PATH];
        int j = 0;
        for(int i = 0; *p; i++) {
            sprintf(buf, "%s\\%s", dossier, p);
            strcpy(audios_charger[i], buf);
            p += strlen(p) + 1;
            j = i;
        }
        return j;
    }
    return 0;
}

int lancer_recommencer_audio(Mix_Music **audio){
    if(*audio){
        Mix_HaltMusic();
        if(Mix_PlayMusic(*audio, 1) < 0)
            printf("Erreur lecture : %s\n", Mix_GetError());
    }
    else{
        return 1;
    }
    printf("titre : %s\nartiste : %s\nalbum : %s\n", Mix_GetMusicTitle(*audio), Mix_GetMusicArtistTag(*audio), Mix_GetMusicAlbumTag(*audio));
    return 0;
}

void mise_a_l_arret (SDL_Window *fenetre, SDL_Renderer *rendu, Mix_Music *audio){
    if(audio){
        if(Mix_PlayingMusic())
            Mix_HaltMusic();
        Mix_FreeMusic(audio);
    }
    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
}

void pause(){
    Mix_PauseMusic();
}

void reprendre(){
    Mix_ResumeMusic();
}

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

SDL_bool clickInRect(SDL_Rect rect, float coef){
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(x * coef > rect.x && x * coef < rect.x + rect.w && y * coef > rect.y && y * coef < rect.y + rect.h)
        return SDL_TRUE;
    return SDL_FALSE;
}

int main(int argc, char *argv[])
{
    SDL_Window *fenetre = NULL;
    SDL_Renderer *rendu = NULL;
    SDL_Event event;

    SDL_Rect rectEcran = {0, 0, 1920, 1080};
    SDL_Rect rectPlay = {935, 1000, 50, 50}, rectSautAvant = {850, 1000, 50, 50}, rectSautApres = {1020, 1000, 50, 50};
    SDL_Rect rectBarreAudio = {40, 900, 0, 5};
    SDL_Rect rectCharger = {5, 15, 136, 20};

    if(initialisation(&fenetre, &rendu)){
        return 1;
    }

    int w = 0, h = 0;
    SDL_GetWindowSize(fenetre, &w, &h);
    float rapport_x = 1920.0 / w;
    float rapport_y = 1080.0 / h;

    SDL_bool boucle = SDL_TRUE, click = SDL_FALSE;

    if(rapport_x != rapport_y){
        printf("votre ecran n est pas compatible avec ce jeu .Preferer une resolution 16 : 9 \n");
        boucle = SDL_FALSE;
    }
    float coefEcran = rapport_x;

    SDL_Texture *playButton = loadImage("D:/tout_pour_programmer/C/programme/actuel/lecteur_audio/bin/Debug/ressources/boutons.png", &rendu);
    SDL_Texture *chargerButton = loadImage("D:/tout_pour_programmer/C/programme/actuel/lecteur_audio/bin/Debug/ressources/charger.png", &rendu);

    Mix_Music *audio = NULL;

    int etatMusic = 0;
    int etatBoutonPlay = 3;
    int musiqueAJouer = 0;
    int nbMusCharg = 0;
    double duree = 0.0;
    char chemin_audios_charger[MAX_MUS_CHARGER][MAX_PATH];

    if(argc > 1){
        nbMusCharg = 1;
        strcpy(chemin_audios_charger[0], argv[1]);
        musiqueAJouer = 0;

        if(!charger_audio(&audio, chemin_audios_charger[0], &duree)) {
            lancer_recommencer_audio(&audio);
            etatMusic = 1;
            etatBoutonPlay = 2;
        }
    }

    while(boucle){
        click = SDL_FALSE;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                boucle = SDL_FALSE;

            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                click = SDL_TRUE;

                if(clickInRect(rectCharger, coefEcran)){
                    char chemin_audio[16384];
                    if(ouvrirBoiteFichier(chemin_audio, 16384) != 0){
                        nbMusCharg = charger_audios(chemin_audio, chemin_audios_charger);
                        musiqueAJouer = 0;
                        etatMusic = 0;
                        etatBoutonPlay = 3;
                        printf("%f\n", Mix_GetMusicPosition(audio));
                        pause();
                    }
                }

                if(clickInRect(rectPlay, coefEcran)){
                    if(etatMusic == 0 && nbMusCharg != 0){
                        if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                            lancer_recommencer_audio(&audio);

                            etatMusic = 1;
                            etatBoutonPlay = 2;
                        }
                    }
                    else if(etatMusic == 1){
                        pause();
                        etatMusic = 2;
                        etatBoutonPlay = 3;
                    }
                    else if(etatMusic == 2){
                        reprendre();
                        etatMusic = 1;
                        etatBoutonPlay = 2;
                    }
                }

                if(clickInRect(rectSautAvant, coefEcran)){
                    if(Mix_GetMusicPosition(audio) >= 10){
                        lancer_recommencer_audio(&audio);
                        etatMusic = 1;
                        etatBoutonPlay = 2;
                    }
                    else{
                        if(musiqueAJouer != 0){
                            musiqueAJouer -= 1;
                            decharger_audio(&audio);
                            if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                                if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                                    lancer_recommencer_audio(&audio);
                                    etatMusic = 1;
                                    etatBoutonPlay = 2;
                                }
                            }
                        }
                        else{
                            lancer_recommencer_audio(&audio);
                            etatMusic = 1;
                            etatBoutonPlay = 2;
                        }
                    }
                }

                if(clickInRect(rectSautApres, coefEcran)){
                    if(musiqueAJouer + 1 < nbMusCharg){
                        musiqueAJouer += 1;
                        decharger_audio(&audio);
                        if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                            if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                                lancer_recommencer_audio(&audio);
                                etatMusic = 1;
                                etatBoutonPlay = 2;
                            }
                        }
                    }
                }
            }

            if(event.key.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_SPACE){
                    if(etatMusic == 0 && nbMusCharg != 0){
                        if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                            lancer_recommencer_audio(&audio);

                            etatMusic = 1;
                            etatBoutonPlay = 2;
                        }
                    }
                    else if(etatMusic == 1){
                        pause();
                        etatMusic = 2;
                        etatBoutonPlay = 3;
                    }
                    else if(etatMusic == 2){
                        reprendre();
                        etatMusic = 1;
                        etatBoutonPlay = 2;
                    }
                }
            }
        }

        /*if(!Mix_PlayingMusic()){
            etatBoutonPlay = 3;
            etatMusic = 0;
        }*/

        double pos = Mix_GetMusicPosition(audio);
        if(pos > 0.0f && pos < duree && etatMusic != 0)
            rectBarreAudio.w = pos * (1839 / duree);
        else if(pos > duree && etatMusic != 0)
            rectBarreAudio.w = duree * (1839 / duree);
        else
            rectBarreAudio.w = 0;

        if(pos + 0.001f >= duree){
            printf("fin\n");
            decharger_audio(&audio);
            musiqueAJouer += 1;
            if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                    lancer_recommencer_audio(&audio);
                    etatMusic = 1;
                    etatBoutonPlay = 2;
                }
            }
            else{
                etatMusic = 0;
                etatBoutonPlay = 3;
                musiqueAJouer -= 1;
            }
        }

        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderFillRect(rendu, &rectBarreAudio);

        SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
        afficherSpriteSheet(playButton, 50, 50, 1, &rendu, rectSautAvant);
        afficherSpriteSheet(playButton, 50, 50, 0, &rendu, rectSautApres);
        afficherSpriteSheet(playButton, 50, 50, etatBoutonPlay, &rendu, rectPlay);
        afficherSpriteSheet(chargerButton, 136, 20, 0, &rendu, rectCharger);
        SDL_RenderPresent(rendu);
        SDL_RenderClear(rendu);
        SDL_Delay(25);
    }

    SDL_DestroyTexture(playButton);
    SDL_DestroyTexture(chargerButton);

    mise_a_l_arret(fenetre, rendu, audio);

    return 0;
}
