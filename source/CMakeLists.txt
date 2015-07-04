
# Include directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Libraries
set(IDE_FOLDER "")
add_subdirectory(qlearning)

# Applications
set(IDE_FOLDER "Applications")
add_subdirectory(qcli)

