#include "call_OS.h"

#ifdef _WIN32

// ton code Windows

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ouvrirBoiteFichier(char *chemin, unsigned int taille)
{
    FILE *fp;
    char buffer[16384];

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

    if (!fgets(buffer, sizeof(buffer), fp))
    {
        pclose(fp);
        return 0;
    }

    pclose(fp);

    buffer[strcspn(buffer, "\n")] = '\0';

    size_t pos = 0;

    char *token = strtok(buffer, "|");

    while (token)
    {
        size_t len = strlen(token);

        if (pos + len + 2 >= taille)
            break;

        memcpy(chemin + pos, token, len);
        pos += len;

        chemin[pos++] = '\0';

        token = strtok(NULL, "|");
    }

    chemin[pos] = '\0'; /* double NULL final */

    return 1;
}

#endif
