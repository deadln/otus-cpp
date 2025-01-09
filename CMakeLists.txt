cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VESRION})

# option(WITH_BOOST_TEST "Whether to build Boost test" ON)

# configure_file(version.h.in version.h)

find_package(Boost COMPONENTS program_options filesystem unit_test_framework regex REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(bayan main.cpp)
add_library(crc32 crc32.cpp)
add_library(md5 md5.cpp)

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# target_include_directories(ipfilter
#     PRIVATE "${CMAKE_BINARY_DIR}"
# )

target_link_libraries(bayan ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    crc32
    md5
    # bulk_buffer
)

# if(WITH_BOOST_TEST)
#     find_package(Boost COMPONENTS unit_test_framework REQUIRED)
# #    add_executable(test_version test_version.cpp)

#     set_target_properties(test_version PROPERTIES
#         CXX_STANDARD 14
#         CXX_STANDARD_REQUIRED ON
#     )

#     set_target_properties(test_version PROPERTIES
#         COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
#         INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
#     )

#     target_link_libraries(test_version
#         ${Boost_LIBRARIES}
#         ipfilter
#     )
# endif()

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    # target_compile_options(bulk_buffer PRIVATE
    #     /W4
    # )
    # if(WITH_BOOST_TEST)
    #     target_compile_options(test_version PRIVATE
    #         /W4
    #     )
    # endif()
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic # -Werror
    )
    # target_compile_options(bulk_buffer PRIVATE
    #     -Wall -Wextra -pedantic -Werror
    # )
    # if(WITH_BOOST_TEST)
    #     target_compile_options(test_version PRIVATE
    #         -Wall -Wextra -pedantic -Werror
    #     )
    # endif()
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

# if(WITH_BOOST_TEST)
#     enable_testing()
#     add_test(test_version test_version)
# endif()
