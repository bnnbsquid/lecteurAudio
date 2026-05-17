Lorsque j'utilisais la sdl et que j'ai découvert sdl_mixer et bien je me suis dis qu'en C on pouvais faire des applis très rapide et je me suis lancer le défi de faire une appli similaire au le lecteur multimédia de windows mais en plus rapide (et spécialiser dans l'audio uniquement).

Ce projet me sert essenciellement a progresser et peut etre espérer un jour avoir une petite renomé (un rêve) un rêve réalisable si j'y consacre du temps.


Comment compiler avec code:blocks :

1. télécharger d'abord le repo et ouvrer le fichier lecteur_audio.cbp à l'aide de code:blocks.
2. dans le menu "project -> build options ... -> search directories" modifier dans compiler les chemins de destinations pour : SDL2, SDL2_mixer, SDL2_image, sndfile pour qu'il pointe sur le include puis dans linker pointer sur lib. Vous trouverez les librairies sur les repo de sdl et sndfile.
3. apuiez sur le logo compiler ou Ctrl-F9 pour créé l'éxécutable.
