#include "call_OS.h" // générer par IA

#ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>

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

#else

#include "call_OS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    return 1;
}

#endif
