CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0)

PROJECT(02_add)

FILE(GLOB PROJECT_SOURCES "src/*.cc")

# Add the source code to the project's executable。
ADD_EXECUTABLE(02_add ${PROJECT_SOURCES})
# Set properties: output path
SET_TARGET_PROPERTIES(02_add    PROPERTIES 
                                RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin
                                RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin
                                RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin
                                RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin)

TARGET_LINK_LIBRARIES(02_add PRIVATE OpenCL::OpenCL)