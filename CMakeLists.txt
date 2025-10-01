cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(docx2txt LANGUAGES CXX C)

# pugixml
set(BUILD_SHARED_LIBS OFF)
set(PUGIXML_COMPACT ON)
add_subdirectory(3rdparty/pugixml)

# docx2txt
option(DOCX2TXT_SHARED "Build shared library" OFF)
option(DOCX2TXT_TESTS "Build and run tests" ON)

if(DOCX2TXT_SHARED)
    set(BUILD_SHARED_LIBS ON)

    add_library(docx2txt SHARED)
else()
    add_library(docx2txt STATIC)
endif()

target_sources(docx2txt PRIVATE
        3rdparty/miniz/miniz.c
        include/docx2txt.hpp
        src/docx2txt.cpp
        src/Data.hpp
        src/Data.cpp
        src/Document.hpp
        src/Document.cpp
        src/Reader.hpp
        src/Reader.cpp
)

target_include_directories(docx2txt PUBLIC include 3rdparty/miniz)
target_link_libraries(docx2txt PRIVATE pugixml::static)

# optional tests
if(DOCX2TXT_TESTS)
    include(CTest)

    enable_testing()

    add_executable(docx2txt_test tests/main.cpp)

    target_link_libraries(docx2txt_test PRIVATE docx2txt)
    target_include_directories(docx2txt_test PRIVATE include)

    file(GLOB DOCX_CASES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/*.docx")

    foreach(docx ${DOCX_CASES})
        get_filename_component(base "${docx}" NAME_WE)

        # use same expectation (e.g. test.txt) for different sources (e.g. test.docx, test_libre.docx, test_online.docx)
        string(REGEX REPLACE "_[^_]*$" "" base_trimmed "${base}")

        set(txt "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/${base_trimmed}.txt")

        add_test(NAME docx2txt.${base}
                COMMAND docx2txt_test "${docx}" "${txt}"
        )

        message("Test '${base}' added")
    endforeach()

    add_custom_target(docx2txt_tests
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>
            DEPENDS docx2txt_test
            USES_TERMINAL
    )
endif()