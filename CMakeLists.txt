cmake_minimum_required(VERSION 3.9)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk_server VERSION ${PROJECT_VESRION})

set(CMAKE_CXX_STANDARD 14)

set(BOOST_ROOT /usr/include/boost)
set(Boost_USE_STATIC_LIBS ON)

find_package(Threads)
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_library(async SHARED async.cpp async.h)
add_library(server SHARED server.cpp server.h)

add_executable(bulk_server main.cpp)

target_link_libraries(server
    async
    ${Boost_LIBRARIES} 
    ${Boost_SYSTEM_LIBRARY}
)

target_link_libraries(bulk_server 
    server 
    ${CMAKE_THREAD_LIBS_INIT} 
)

set_target_properties(async PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(server PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
    target_compile_options(async PRIVATE
        /W4
    )
    target_compile_options(server PRIVATE
        /W4
    )
    # if(WITH_BOOST_TEST)
    #     target_compile_options(test_version PRIVATE
    #         /W4
    #     )
    # endif()
else ()
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    # if(WITH_BOOST_TEST)
    #     target_compile_options(test_version PRIVATE
    #         -Wall -Wextra -pedantic -Werror
    #     )
    # endif()
endif()

# install(TARGETS async RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

