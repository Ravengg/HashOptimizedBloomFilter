add_executable(${PROJECT_NAME}_cli
        Cli.cc)
set_target_properties(${PROJECT_NAME}_cli PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "./"
        OUTPUT_NAME "${PROJECT_NAME}"
        )
target_link_libraries(${PROJECT_NAME}_cli
        -Wl,-Bstatic ${HOBF_LIBRARIES}
        -Wl,-Bdynamic -lstdc++)

target_include_directories(${PROJECT_NAME}_cli BEFORE
        PRIVATE ${PROJECT_SOURCE_DIR}/src
        )
