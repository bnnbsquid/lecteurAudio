#ifndef CALL_OS_H
<<<<<<< HEAD
#define CALL_OS_H

#ifdef _WIN32
    #include <windows.h>
    #include <commdlg.h>

int ouvrirBoiteFichier(char *chemin, DWORD taille);

#else
#include <string.h>

int ouvrirBoiteFichier(char *chemin, size_t taille);

=======
#define CALL_OS_H

#ifdef _WIN32
    #include <windows.h>
    #include <commdlg.h>

int ouvrirBoiteFichier(char *chemin, DWORD taille);

#else
int ouvrirBoiteFichier(char *chemin, unsigned int taille);

>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
#endif

#endif
