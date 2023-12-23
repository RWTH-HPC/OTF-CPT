include(CheckCXXSymbolExists)
include(CheckIncludeFileCXX)

check_include_file_cxx(sys/resource.h HAVE_SYS_RESOURCE_H)
check_include_file_cxx(sys/stat.h HAVE_SYS_STAT_H)
check_include_file_cxx(sys/mman.h HAVE_SYS_MMAN_H)
check_include_file_cxx(sysexits.h HAVE_SYSEXITS_H)
check_include_file_cxx(fcntl.h HAVE_FCNTL_H)
check_include_file_cxx(unistd.h HAVE_UNISTD_H)
# For Apples
check_include_file_cxx(malloc/malloc.h HAVE_MALLOC_MALLOC_H)


check_cxx_symbol_exists(strerror_r string.h HAVE_STRERROR_R)
check_cxx_symbol_exists(getpagesize unistd.h HAVE_GETPAGESIZE)
check_cxx_symbol_exists(sysconf unistd.h HAVE_SYSCONF)
check_cxx_symbol_exists(getrusage sys/resource.h HAVE_GETRUSAGE)
check_cxx_symbol_exists(mallinfo2 malloc.h HAVE_MALLINFO2)
check_cxx_symbol_exists(mallinfo malloc.h HAVE_MALLINFO)
# For the Apple users
check_cxx_symbol_exists(malloc_zone_statistics malloc/malloc.h HAVE_MALLOC_ZONE_STATISTICS)
# mallctl seems to be a jemalloc/FreeBSD thing
check_cxx_symbol_exists(mallctl malloc_np.h HAVE_MALLCTL)
check_cxx_symbol_exists(sbrk unistd.h HAVE_SBRK)
check_cxx_symbol_exists(futimens sys/stat.h HAVE_FUTIMENS)
check_cxx_symbol_exists(futimes sys/time.h HAVE_FUTIMES)

set(LLVM_ON_UNIX 1)
set(LLVM_HOST_TRIPLE "x86_64-unknown-linux-gnu")
set(PACKAGE_NAME "${PROJECT_NAME} (standalone)")
set(PACKAGE_VERSION "${PROJECT_VERSION}")

configure_file(
        llvm-config.h.cmake
        ${CMAKE_BINARY_DIR}/include/llvm/Config/llvm-config.h
)
configure_file(
        config.h.cmake
        ${CMAKE_BINARY_DIR}/include/llvm/Config/config.h
)
configure_file(
        abi-breaking.h.cmake
        ${CMAKE_BINARY_DIR}/include/llvm/Config/abi-breaking.h
)

add_library(FileCheckConfig_Standalone INTERFACE)
target_include_directories(FileCheckConfig_Standalone INTERFACE ${CMAKE_BINARY_DIR}/include)
