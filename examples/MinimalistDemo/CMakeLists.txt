set(SFEMOVIE_MINIMALIST_DEMO "sfeMovieMinimalistDemo")

set(SFEMOVIE_MINIMALIST_DEMO_SRC
    main.cpp
)
source_group("Sources" FILES ${SFEMOVIE_MINIMALIST_DEMO_SRC})

add_executable(
    ${SFEMOVIE_MINIMALIST_DEMO}
    ${SFEMOVIE_MINIMALIST_DEMO_SRC}
)

if (SFEMOVIE_BUILD_STATIC)
    set (SFML_STATIC_LIBRARIES TRUE)
    find_package (SFML 3 COMPONENTS Graphics Window System Audio REQUIRED CONFIG)
    message(STATUS "SFML libraries: ${SFML_LIBRARIES}")
    message(STATUS "Additional dependencies: ${SFML_DEPENDENCIES}")

    set_target_properties(${SFEMOVIE_MINIMALIST_DEMO} PROPERTIES
        COMPILE_DEFINITIONS SFML_STATIC
        COMPILE_DEFINITIONS SFEMOVIE_STATIC)
    target_link_libraries(
        ${SFEMOVIE_MINIMALIST_DEMO}
        ${SFEMOVIE_LIB}
        ${FFMPEG_LIBRARIES}
        ${OTHER_LIBRARIES}
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
    )
else()
    target_link_libraries(
        ${SFEMOVIE_MINIMALIST_DEMO}
        ${SFEMOVIE_LIB}
        ${SFML_LIBRARIES}
        ${SFML_DEPENDENCIES}
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
    )
endif()

if (MACOSX)
    set_target_properties(${SFEMOVIE_MINIMALIST_DEMO} PROPERTIES 
                          BUILD_WITH_INSTALL_RPATH 1 
                          INSTALL_RPATH "@executable_path/")
endif()
