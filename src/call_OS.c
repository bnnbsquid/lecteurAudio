<<<<<<< HEAD
#include "call_OS.h" // gÃ©nÃ©rer par IA
=======
#ifdef _WIN32
    #include <windows.h>
    #include <commdlg.h>
#endif
#include <stdio.h>
#include "call_OS.h"
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)

#ifdef _WIN32

int ouvrirBoiteFichier(char *chemin, DWORD taille){
    OPENFILENAME ofn;

    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(chemin, taille);

<<<<<<< HEAD
    chemin[0] = '\0';   // trÃ¨s important

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetForegroundWindow();   // Ã©vite ouverture derriÃ¨re
=======
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetForegroundWindow();
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
    ofn.lpstrFile = chemin;
    ofn.nMaxFile = taille;

    ofn.lpstrFilter =
<<<<<<< HEAD
        "Audio (*.mp3;*.ogg;*.flac;*.wav;*.aiff;*.aif)\0*.mp3;*.ogg;*.flac;*.wav;*.aiff;*.aif\0"
        "Tous les fichiers (*.*)\0*.*\0";

    ofn.lpstrInitialDir = "C:\\";   // <-- stabilise la boÃ®te
    ofn.lpstrTitle = "Selectionne un fichier audio";
=======
        "Tous les fichiers (*.*)\0*.*\0"
        "Fichiers Audio (*.mp3;*.ogg;*.wav;*.flac)\0*.mp3;*.ogg;*.wav;*.flac\0";

    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = "Selectionne un ou plusieurs fichiers";
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)

    ofn.Flags =
        OFN_EXPLORER |
        OFN_PATHMUSTEXIST |
        OFN_FILEMUSTEXIST |
        OFN_NOCHANGEDIR |
        OFN_ALLOWMULTISELECT;

<<<<<<< HEAD
    if (GetOpenFileName(&ofn))
        return 1;

    // pour savoir si c'est une vraie erreur :
=======
    if (GetOpenFileName(&ofn)) {
        // --- CONVERSION POUR LE MULTISELECT WINDOWS ---
        // On vérifie si plusieurs fichiers ont été sélectionnés
        // Si c'est le cas, il y a un '\0' après le dossier, puis le premier fichier.
        char *ptr = chemin;

        // On avance jusqu'à la fin du premier bloc (le dossier)
        ptr += strlen(ptr) + 1;

        if (*ptr != '\0') {
            // Plusieurs fichiers ont été sélectionnés !
            // On va reconstruire la chaîne au format : Dossier\Fichier1|Dossier\Fichier2|
            char dossier[MAX_PATH];
            strcpy(dossier, chemin);

            char temporaire[16384] = "";

            while (*ptr != '\0') {
                strcat(temporaire, dossier);
                strcat(temporaire, "\\");
                strcat(temporaire, ptr);
                strcat(temporaire, "|");

                // Passe au fichier suivant
                ptr += strlen(ptr) + 1;
            }

            // On retire le dernier '|' inutile
            size_t len = strlen(temporaire);
            if (len > 0) temporaire[len - 1] = '\0';

            // On recopie le tout dans le buffer principal
            strncpy(chemin, temporaire, taille - 1);
            chemin[taille - 1] = '\0';
        }
        return 1;
    }

>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
    DWORD err = CommDlgExtendedError();
    if(err != 0)
        printf("Erreur Win32: %lu\n", err);

    return 0;
}

#else

<<<<<<< HEAD
#include "call_OS.h"
=======
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
int ouvrirBoiteFichier(char *chemin, size_t taille)
{
    const char *cmd =
    "kdialog "
    "--title \"Selectionne un fichier audio\" "
    "--getopenfilename . "
    "\"Audio (*.mp3 *.ogg *.flac *.wav *.aiff *.aif)\" "
    "--multiple "
    "--separate-output "
    "2>/dev/null";

    FILE *fp = popen(cmd, "r");
    if (!fp)
        return 0;

    chemin[0] = '\0';

    size_t pos = 0;
    char ligne[4096];

    while (fgets(ligne, sizeof(ligne), fp))
    {
        size_t len = strlen(ligne);

        if (pos + len + 1 >= taille)
            break;

        memcpy(chemin + pos, ligne, len);
        pos += len;
    }

    chemin[pos] = '\0';

    pclose(fp);

    if (pos == 0)
        return 0;
=======
int ouvrirBoiteFichier(char *chemin, unsigned int taille)
{
    FILE *fp;

    chemin[0] = '\0';

    fp = popen(
        "zenity --file-selection "
        "--multiple "
        "--separator='|' "
        "--title='Selectionne un fichier audio' "
        "--file-filter='*.mp3 *.ogg *.flac *.wav *.aiff *.aif'",
        "r"
    );

    if (!fp)
        return 0;

    if (!fgets(chemin, taille, fp))
    {
        pclose(fp);
        return 0;
    }

    pclose(fp);

    chemin[strcspn(chemin, "\n")] = 0;


    #ifdef __linux__
    for (char *p = chemin; *p; p++)
    {
        if (*p == '|')
            *p = '\0';
    }
    #endif
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)

    return 1;
}

#endif
