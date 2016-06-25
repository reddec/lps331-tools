cmake_minimum_required(VERSION 2.8)
project(lps331 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall -Wextra")
set(HEADERS lps331.h)
set(SOURCE_FILES lps331.c)
set(SOURCE_CLI_FILES main.c ${HEADERS})
add_executable(lps331 ${SOURCE_CLI_FILES})
add_library(liblps331 SHARED ${SOURCE_FILES})
add_library(liblps331_static STATIC ${SOURCE_FILES})
target_link_libraries(lps331 liblps331_static)
set_target_properties(liblps331_static PROPERTIES OUTPUT_NAME lps331)
set_target_properties(liblps331 PROPERTIES OUTPUT_NAME lps331)

install(TARGETS lps331 liblps331 liblps331_static
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES ${HEADERS} DESTINATION usr/include)

include(CPack)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "LPS331 utilities")
SET(CPACK_PACKAGE_VENDOR "Baryshnikov Alexander <dev@baryshnikov.net>")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "2")