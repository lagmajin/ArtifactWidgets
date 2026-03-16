cmake_minimum_required(VERSION 3.30)
project(ArtifactWidgets)

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
    # Exclude third_party/wobjectimpl.h (C++20 modules incompatible)
    if(file_path MATCHES "\\.bak$" OR
       file_path MATCHES "include/third_party" OR
       file_path MATCHES "third_party/wobject" OR
       file_path MATCHES "src/Common/WidgetCounter\\.cpp" OR
       file_path MATCHES "src/Knob/KnobEditorWidget/KnobCheckBox\\.cppm")
        continue()
    endif()

    # 以前の 4096 バイト制限では、大量の include の後に export module がある場合に
    # 検出に失敗することがあったため、制限を 32KB に拡大します。
    file(READ "${file_path}" raw_content LIMIT 32768)
    
    # コメントを除去して誤判定を防ぐ
    string(REGEX REPLACE "//[^\n]*" "" content "${raw_content}")
    string(REGEX REPLACE "/\\*.*\\*/" "" content "${content}")
    
    set(is_mod_interface_or_partition FALSE)
    set(found_mod_name "")

    # 1. インターフェース判定: export module M;
    if(content MATCHES "export[ \t\r\n]+module[ \t\r\n]+([a-zA-Z0-9\\._:]+)[ \t\r\n]*;")
        set(is_mod_interface_or_partition TRUE)
        set(found_mod_name "${CMAKE_MATCH_1}")
    # 2. パーティション判定: module M:P; (コロンを含む)
    elseif(content MATCHES "module[ \t\r\n]+([a-zA-Z0-9\\._]+:[a-zA-Z0-9\\._:]+)[ \t\r\n]*;")
        set(is_mod_interface_or_partition TRUE)
        set(found_mod_name "${CMAKE_MATCH_1}")
    endif()

    if(is_mod_interface_or_partition)
        list(FIND _PROVIDED_MODULES "${found_mod_name}" _mod_idx)
        if(_mod_idx EQUAL -1)
            list(APPEND WIDGETS_MODULES "${file_path}")
            list(APPEND _PROVIDED_MODULES "${found_mod_name}")
        else()
            list(APPEND WIDGETS_IMPL "${file_path}")
            message(WARNING "Duplicate module name detected: ${found_mod_name} in ${file_path}")
        endif()
    else()
        list(APPEND WIDGETS_IMPL "${file_path}")
    endif()
endforeach()

# ---------------------------------------------------------------------------
# ターゲット定義
# ---------------------------------------------------------------------------
add_library(ArtifactWidgets SHARED)

set_target_properties(ArtifactWidgets PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    CXX_MODULE_STD ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)

# MSVC specific C++23 modules settings
if(ARTIFACT_FRONTEND_MSVC)
    target_compile_options(ArtifactWidgets PRIVATE /WX-)
    # Enable C++23 standard (latest)
    target_compile_options(ArtifactWidgets PRIVATE /std:c++latest)
    # Enable modules support
    target_compile_options(ArtifactWidgets PRIVATE /experimental:module)
    # Enable full C++23 library support
    target_compile_options(ArtifactWidgets PRIVATE /Zc:__cplusplus /permissive-)
endif()

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
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/verdigris/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/../libs/DiligentEngine"
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

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC EnTT::EnTT)

find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC magic_enum::magic_enum)

find_package(Boost REQUIRED COMPONENTS iostreams)
target_link_libraries(ArtifactWidgets PUBLIC Boost::iostreams)

if(WIN32)
    target_link_libraries(ArtifactWidgets PRIVATE dwmapi shell32 user32)
endif()
