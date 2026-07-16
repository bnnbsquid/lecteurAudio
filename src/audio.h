<<<<<<< HEAD
<<<<<<<< HEAD:src/audio.h
#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <stdbool.h>

=======
#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
#define MAX_MUS_CHARGER 1024

#ifndef MAX_PATH
    #define MAX_PATH 4096
#endif

<<<<<<< HEAD
========
#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#define MAX_MUS_CHARGER 1024

>>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.):audio.h
void decharger_audio(Mix_Music **audio);
double duree_mus(char *chemin_audio);
int charger_audio(Mix_Music **audio, char *chemin_audio, double *duree);
int charger_audios(char *chemin_audio, char audios_charger[MAX_MUS_CHARGER][MAX_PATH]);
<<<<<<<< HEAD:src/audio.h
int lancer_recommencer_audio(Mix_Music **audio, bool *lancer, bool *pause);
void pauseMus();
void reprendre();

#endif // AUDIO_H_INCLUDED
========
int lancer_recommencer_audio(Mix_Music **audio);
void pause();
void reprendre();

#endif // AUDIO_H_INCLUDED
>>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.):audio.h
=======
void decharger_audio(Mix_Music **audio);
double duree_mus(char *chemin_audio);
int charger_audio(Mix_Music **audio, char *chemin_audio, double *duree);
int charger_audios(const char *chemin_audio, char audios_charger[MAX_MUS_CHARGER][MAX_PATH]);
int lancer_recommencer_audio(Mix_Music **audio);
void pause();
void reprendre();

#endif // AUDIO_H_INCLUDED
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
