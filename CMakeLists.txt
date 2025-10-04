cmake_minimum_required(VERSION 3.12)

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
    add_library(docx2txt STATIC
            src/Type.hpp)
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

    file(GLOB DOCX_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/*.docx")
    file(GLOB ODT_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/*.odt")

    set(TEST_CASES ${DOCX_FILES} ${ODT_FILES})

    foreach(file ${TEST_CASES})
        get_filename_component(base "${file}" NAME_WE)

        # same expectation (e.g. test.txt) for different sources (e.g. test.docx, test.odt, test_libre.docx,...)
        string(REGEX REPLACE "_[^_]*$" "" base_trimmed "${base}")

        set(txt "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/${base_trimmed}.txt")

        get_filename_component(filename "${file}" NAME)
        string(REPLACE "." "_" testname "${filename}")
        set(testname "docx2txt_${testname}")

        add_test(NAME ${testname}
                COMMAND docx2txt_test "${file}" "${txt}"
        )

        message("Test '${testname}' added")
    endforeach()

    add_custom_target(docx2txt_tests
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>
            DEPENDS docx2txt_test
            USES_TERMINAL
    )
endif()