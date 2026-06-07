cmake_minimum_required(VERSION 3.10)

project(LecteurAudio C)

set(CMAKE_C_STANDARD 11)

add_executable(LecteurAudio
    src/main.c
    src/audio.c
    src/renderer.c
    src/call_OS.c
)

if(WIN32)

    target_include_directories(LecteurAudio PRIVATE
        libs/SDL2/include
        libs/SDL2_image/include
        libs/SDL2_mixer/include
        libs/libsndfile/include
    )

    target_link_directories(LecteurAudio PRIVATE
        libs/SDL2/lib
        libs/SDL2_image/lib
        libs/SDL2_mixer/lib
        libs/libsndfile/lib
    )

    target_link_libraries(LecteurAudio
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        sndfile
    )

elseif(UNIX)

    find_package(PkgConfig REQUIRED)

    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
    pkg_check_modules(SNDFILE REQUIRED sndfile)

    target_include_directories(LecteurAudio PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SNDFILE_INCLUDE_DIRS}
    )

    target_link_libraries(LecteurAudio
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SNDFILE_LIBRARIES}
    )

endif()
