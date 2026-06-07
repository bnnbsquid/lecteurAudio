#include "call_OS.h"

#ifdef _WIN32

// code Win32 actuel

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    return 1;
}

#endif
