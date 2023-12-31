cmake_minimum_required(VERSION 3.14)
project(ffmpeg-cpp C CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/glfw)
add_subdirectory(lib/FFmpeg)

if(APPLE)
    list(APPEND EXTRA_LIBS
        "-framework OpenGL"
    )

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/MacOSXBundleInfo.plist.in
        ${CMAKE_CURRENT_BINARY_DIR}/assets/MacOSXBundleInfo.plist
    )

    set_target_properties(video-app PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/assets/MacOSXBundleInfo.plist
    )

elseif(WIN32)
    list(APPEND EXTRA_LIBS
        "-lglu32 -lopengl32"
    )
    set(CMAKE_EXE_LINKER_FLAGS "-std=gnu99 -static -static-libgcc -static-libstdc++ -mwindows")
else()
    list(APPEND EXTRA_LIBS
        "-lGL -lX11"
    )
endif()

list(APPEND SOURCES
    src/main.cpp
    src/load_frame.cpp
    )
add_executable(ffmpeg-cpp  ${SOURCES})
target_link_libraries(ffmpeg-cpp FFmpeg glfw ${EXTRA_LIBS})
