cmake_minimum_required(VERSION 3.28)
project(ArtifactWidgets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# VCPKG path removed for manifest mode

file(GLOB_RECURSE WIDGETS_IMPL CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE WIDGETS_MODULES CONFIGURE_DEPENDS "include/*.ixx" "src/*.ixx" "src/*.cppm")

add_library(ArtifactWidgets SHARED)

target_sources(ArtifactWidgets
    PUBLIC FILE_SET CXX_MODULES FILES ${WIDGETS_MODULES} ${WIDGETS_IMPL}
)

target_compile_definitions(ArtifactWidgets
    PRIVATE WIDGET_LIBRARY_DLL_EXPORT
    INTERFACE WIDGET_LIBRARY_DLL_IMPORT
)

target_include_directories(ArtifactWidgets
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/include/third_party"
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party"
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/verdigris/src"
)

# 依存関係
target_link_libraries(ArtifactWidgets PUBLIC ArtifactCore)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Multimedia)
target_link_libraries(ArtifactWidgets PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::Multimedia)

find_package(qtadvanceddocking-qt6 REQUIRED)
target_link_libraries(ArtifactWidgets PRIVATE ads::qtadvanceddocking-qt6)

find_package(OpenMP REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC OpenMP::OpenMP_CXX)

find_package(TBB REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC TBB::tbb)

find_package(glm CONFIG REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC glm::glm)

find_package(Stb REQUIRED)
target_include_directories(ArtifactWidgets PUBLIC ${Stb_INCLUDE_DIR})

find_package(assimp REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC assimp::assimp)

find_package(Boost REQUIRED COMPONENTS iostreams)
target_link_libraries(ArtifactWidgets PUBLIC Boost::iostreams)

if(WIN32)
    target_link_libraries(ArtifactWidgets PRIVATE dwmapi shell32 user32)
endif()
