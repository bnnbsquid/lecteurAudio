#ifndef CALL_OS_H
#define CALL_OS_H

#ifdef _WIN32
    #include <windows.h>
    #include <commdlg.h>

int ouvrirBoiteFichier(char *chemin, DWORD taille);

#else
#include <string.h>

int ouvrirBoiteFichier(char *chemin, size_t taille);

#endif

#endif
