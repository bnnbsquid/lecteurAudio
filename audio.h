#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#define MAX_MUS_CHARGER 1024

void decharger_audio(Mix_Music **audio);
double duree_mus(char *chemin_audio);
int charger_audio(Mix_Music **audio, char *chemin_audio, double *duree);
int charger_audios(char *chemin_audio, char audios_charger[MAX_MUS_CHARGER][MAX_PATH]);
int lancer_recommencer_audio(Mix_Music **audio);
void pause();
void reprendre();

#endif // AUDIO_H_INCLUDED
