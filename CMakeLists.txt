cmake_minimum_required(VERSION 3.28)
project(ArtifactWidgets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# ソース収集とモジュール分類
# ---------------------------------------------------------------------------
file(GLOB_RECURSE POTENTIAL_SOURCES CONFIGURE_DEPENDS 
    "src/*.cpp" "src/*.cppm" "src/*.ixx"
    "include/*.ixx" "include/*.cppm"
)

set(WIDGETS_MODULES "")
set(WIDGETS_IMPL "")
set(_PROVIDED_MODULES "")

foreach(file_path ${POTENTIAL_SOURCES})
    if(file_path MATCHES "\\.bak$")
        continue()
    endif()

    file(READ "${file_path}" content LIMIT 4096)
    
    set(is_mod TRUE)
    set(found_mod_name "")

    if(content MATCHES "export[ \t\r\n]+module[ \t\r\n]+([a-zA-Z0-9\\._:]+)")
        set(found_mod_name "${CMAKE_MATCH_1}")
    elseif(content MATCHES "module[ \t\r\n]+([a-zA-Z0-9\\._]+:[a-zA-Z0-9\\._:]+)")
        set(found_mod_name "${CMAKE_MATCH_1}")
    else()
        set(is_mod FALSE)
    endif()

    if(is_mod)
        list(FIND _PROVIDED_MODULES "${found_mod_name}" _mod_idx)
        if(_mod_idx EQUAL -1)
            list(APPEND WIDGETS_MODULES "${file_path}")
            list(APPEND _PROVIDED_MODULES "${found_mod_name}")
        else()
            list(APPEND WIDGETS_IMPL "${file_path}")
        endif()
    else()
        list(APPEND WIDGETS_IMPL "${file_path}")
    endif()
endforeach()

# ---------------------------------------------------------------------------
# ターゲット定義
# ---------------------------------------------------------------------------
add_library(ArtifactWidgets SHARED)

target_sources(ArtifactWidgets
    PRIVATE ${WIDGETS_IMPL}
    PUBLIC FILE_SET CXX_MODULES FILES ${WIDGETS_MODULES}
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

# ---------------------------------------------------------------------------
# 依存関係
# ---------------------------------------------------------------------------
target_link_libraries(ArtifactWidgets PUBLIC ArtifactCore)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Multimedia)
target_link_libraries(ArtifactWidgets PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::Multimedia)

find_package(OpenCV REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC ${OpenCV_LIBS})

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

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC EnTT::EnTT)

find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC magic_enum::magic_enum)

find_package(Boost REQUIRED COMPONENTS iostreams)
target_link_libraries(ArtifactWidgets PUBLIC Boost::iostreams)

if(WIN32)
    target_link_libraries(ArtifactWidgets PRIVATE dwmapi shell32 user32)
endif()
