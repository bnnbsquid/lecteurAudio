#include <windows.h>
#include <stdio.h>

#include "call_OS.h"

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
