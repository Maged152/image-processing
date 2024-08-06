include(FetchContent)

####################################### Fetch libpng #######################################
FetchContent_Declare(
  libpng
  GIT_REPOSITORY https://github.com/glennrp/libpng.git
  GIT_TAG v1.6.43 
)

# only the static lib
set(PNG_TESTS OFF CACHE BOOL "Disable libpng tests" FORCE)
set(PNG_TOOLS OFF CACHE BOOL "Disable libpng executables" FORCE)
set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_STATIC ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_ALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(libpng)