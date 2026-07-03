include(FetchContent)

####################################### Fetch ThreadPool #######################################
FetchContent_Declare(
  thread_pool
  GIT_REPOSITORY https://github.com/Maged152/ThreadPool.git
  GIT_TAG v1.1.0 
)

set(ThreadPool_BUILD_EXAMPLES OFF CACHE BOOL "Disable examples" FORCE)
set(ThreadPool_BUILD_Doc OFF CACHE BOOL "Disable doc" FORCE)

FetchContent_MakeAvailable(thread_pool)

####################################### Fetch PixelImage #######################################
FetchContent_Declare(
  pixel_image
  GIT_REPOSITORY https://github.com/Maged152/PixelImage.git
  GIT_TAG 4dec8c926aa35a324ad7c4b31b3eb35713efe42c 

)

set(PixelImage_BUILD_EXAMPLES OFF CACHE BOOL "Disable examples" FORCE)

FetchContent_MakeAvailable(pixel_image)