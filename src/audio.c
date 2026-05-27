#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <sndfile.h>
#include <windows.h>

#include "audio.h"

#define MAX_MUS_CHARGER 1024

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


void pause(){
    Mix_PauseMusic();
}

void reprendre(){
    Mix_ResumeMusic();
}
