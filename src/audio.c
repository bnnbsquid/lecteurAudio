#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <sndfile.h>
#ifdef _WIN32
    #include <windows.h>
#endif

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

<<<<<<< HEAD
#ifdef _WIN32 // fonction charger_audios fait par IA

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

#else

#include <string.h>
#include <stdio.h>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#ifndef MAX_MUS_CHARGER
#define MAX_MUS_CHARGER 100
#endif

int charger_audios(char *chemin_audio, char audios_charger[MAX_MUS_CHARGER][MAX_PATH]) {
    // 1. Détection du séparateur (Linux = '\n', Windows = '\0')
    // Kdialog renvoie une liste séparée par des nouvelles lignes si --multiple est utilisé.

    int est_linux = (strchr(chemin_audio, '\n') != NULL);

    char dossier[MAX_PATH];
    char *p;

    // Initialisation du tableau de sortie
    for(int i = 0; i < MAX_MUS_CHARGER; i++){
        audios_charger[i][0] = '\0';
    }

    if (!est_linux) {
        // --- LOGIQUE WINDOWS (Séparateur \0) ---
        p = chemin_audio;

        // Premier élément = dossier
        strcpy(dossier, p);
        p += strlen(p) + 1;

        // S'il n'y a qu'un seul fichier (le caractère suivant est \0 immédiat)
        if (*p == '\0') {
            strcpy(audios_charger[0], chemin_audio);
            return 1;
        }

        // Plusieurs fichiers
        int j = 0;
        for(int i = 0; *p && i < MAX_MUS_CHARGER; i++) {
            // Windows utilise déjà des backslashes, on garde la logique originale
            sprintf(audios_charger[i], "%s\\%s", dossier, p);
            p += strlen(p) + 1;
            j = i + 1; // Compte le nombre de fichiers chargés
        }
        return j;

    } else {
        // --- LOGIQUE LINUX / KDIALOG (Séparateur \n) ---

        // Sous Linux avec Kdialog, le "dossier" n'est pas envoyé en premier élément séparé.
        // Kdialog renvoie les chemins COMPLETS (absolus) pour chaque fichier sélectionné.
        // Ex: /home/user/Musique/titre1.mp3\n/home/user/Musique/titre2.mp3

        // On travaille directement sur le buffer reçu.
        // On remplace les '\n' par des '\0' pour réutiliser une logique de parcours similaire,
        // ou on parse ligne par ligne.

        p = chemin_audio;
        int count = 0;

        // Cas fichier unique (pas de \n trouvé, mais on est dans le bloc linux car on a testé la présence)
        // En réalité, si strchr a trouvé un \n, on est dans le cas multiple.
        // Si on est ici, c'est qu'il y a au moins un \n.
        // Mais pour être robuste si l'appel Linux renvoie un seul fichier sans \n final parfois :

        // On utilise strtok pour découper par ligne (séparateur \n)
        char *token = strtok(p, "\n");

        while(token != NULL && count < MAX_MUS_CHARGER) {
            // Kdialog renvoie des chemins absolus, pas besoin de reconstruire avec un dossier de base.
            // On copie simplement le chemin tel quel.
            strcpy(audios_charger[count], token);

            // Normalisation : s'assurer qu'on n'a pas de backslash résiduel (au cas où)
            // et gérer les espaces si nécessaire (Kdialog gère bien les espaces dans les noms).

            count++;
            token = strtok(NULL, "\n");
        }

        // Si count est 0 mais qu'il y a du contenu (cas rare d'un seul fichier sans \n détecté avant ?)
        // La détection est_linux repose sur strchr. Si un seul fichier, strchr renvoie NULL,
        // on tombe dans le bloc Windows ci-dessus.
        // Donc ici on est sûr d'avoir du multiple ou une liste.

        return (count > 0) ? count : 0;
    }
}

#endif


int lancer_recommencer_audio(Mix_Music **audio, bool *lancer, bool *pause){
=======
int charger_audios(const char *chemin_audio,
                   char audios_charger[MAX_MUS_CHARGER][MAX_PATH])
{
    char buffer[16384];

    strncpy(buffer, chemin_audio, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    for (int i = 0; i < MAX_MUS_CHARGER; i++) {
        audios_charger[i][0] = '\0';
    }

    int nb = 0;
    char *token = strtok(buffer, "|");

    while (token && nb < MAX_MUS_CHARGER)
    {
        strncpy(audios_charger[nb], token, MAX_PATH - 1);
        audios_charger[nb][MAX_PATH - 1] = '\0';
        nb++;

        token = strtok(NULL, "|");
    }

    return nb;
}

int lancer_recommencer_audio(Mix_Music **audio){
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
    if(*audio){
        Mix_HaltMusic();
        if(Mix_PlayMusic(*audio, 1) < 0)
            printf("Erreur lecture : %s\n", Mix_GetError());
    }
    else{
        return 1;
    }
<<<<<<< HEAD
    *lancer = true;
    *pause = false;
=======
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
    printf("titre : %s\nartiste : %s\nalbum : %s\n", Mix_GetMusicTitle(*audio), Mix_GetMusicArtistTag(*audio), Mix_GetMusicAlbumTag(*audio));
    return 0;
}


<<<<<<< HEAD
void pauseMus(){
=======
void pause(){
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
    Mix_PauseMusic();
}

void reprendre(){
    Mix_ResumeMusic();
}
