find_path(SDL_INCLUDE_DIR SDL2/SDL.h PATHS external)
find_library(SDL_LIBRARY SDL2 PATHS lib)
find_library(SDL_MAIN_LIBRARY SDL2main PATHS lib)

mark_as_advanced(SDL_INCLUDE_DIR SDL_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL
  REQUIRED_VARS SDL_INCLUDE_DIR SDL_LIBRARY
)