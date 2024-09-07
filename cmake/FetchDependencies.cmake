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