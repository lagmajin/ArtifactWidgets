cmake_minimum_required(VERSION 3.30)
project(ArtifactWidgets)

# ---------------------------------------------------------------------------
# ソース収集とモジュール分類
# ---------------------------------------------------------------------------
file(GLOB_RECURSE POTENTIAL_SOURCES 
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
       file_path MATCHES "third_party/wobject")
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

# Gradual .cppm -> .cpp implementation-unit migration pilots. Keep the
# renamed implementation explicit so this CMakeLists change triggers a fresh
# configure and the source cannot disappear behind a stale file(GLOB) result.
set(_widget_cpp_implementation_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Image/BasicImageViewWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Audio/AudioBusWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Button/FloatColorPickerButton.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Code/CodeEditor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Code/SyntaxHighlighter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Render/RenderQueueManagerWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AbstractWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/AbstractDialog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/AppProgress_Win.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/DragSpinBox.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/EditableLabel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/EnhancedSlider.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Color/ColorWheel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Color/HistgramWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Color/ParadeScopeWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Color/VectorScopeWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Color/WaveformScopeWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Dialog/FloatColorPicker.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Dialog/InterpretFootageDialog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Dialog/KeyboardOverlayDialog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Effect/WidgetGlowFrame.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Time/TimeCodeEditor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/AlignmentComboBox.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/ClickableLabel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/DialogPlacement.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/ToggleGlyphButton.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Viewer/ContentViewer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Dock/Pane.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Time/TimeCodeLabel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Color/ColorViewLabel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/CollapsibleSection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/WidgetCounter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Knob/KnobEditorWidget/KnobCheckBox.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/WinJumpList.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/WinTaskbarProgress.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Common/CommandAction.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Console/ConsoleWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Knob/KnobSlider.cpp")
foreach(_widget_cpp_impl IN LISTS _widget_cpp_implementation_sources)
    list(FIND WIDGETS_IMPL "${_widget_cpp_impl}" _widget_cpp_impl_idx)
    if(_widget_cpp_impl_idx EQUAL -1)
        list(APPEND WIDGETS_IMPL "${_widget_cpp_impl}")
    endif()
endforeach()

# ---------------------------------------------------------------------------
# ターゲット定義
# ---------------------------------------------------------------------------
add_library(ArtifactWidgets SHARED)

if(ARTIFACT_ENABLE_LAYER_TEST)
    set(_artifact_widgets_runtime_dir "${CMAKE_BINARY_DIR}/bin/LayerTests/$<CONFIG>")
else()
    set(_artifact_widgets_runtime_dir "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
endif()
set_target_properties(ArtifactWidgets PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${_artifact_widgets_runtime_dir}"
    LIBRARY_OUTPUT_DIRECTORY "${_artifact_widgets_runtime_dir}"
    ARCHIVE_OUTPUT_DIRECTORY "${_artifact_widgets_runtime_dir}"
)

if(TARGET ArtifactStdModule)
    add_dependencies(ArtifactWidgets ArtifactStdModule)
    if(MSVC AND _artifact_std_ifc)
        target_compile_options(ArtifactWidgets PRIVATE
            "/reference;std=${_artifact_std_ifc}"
        )
    endif()
elseif(TARGET __CMAKE::CXX23)
    set_property(TARGET ArtifactWidgets PROPERTY CXX_MODULE_STD ON)
else()
    set_property(TARGET ArtifactWidgets PROPERTY CXX_MODULE_STD OFF)
endif()

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

if(MSVC)
    # MSBuild otherwise treats every .cppm as a module interface. Keep the
    # exported providers in WIDGETS_MODULES and compile paired implementation
    # units as C++ while CMake supplies their module dependency ordering.
    foreach(_widget_impl IN LISTS WIDGETS_IMPL)
        if(_widget_impl MATCHES "[.]cppm$")
            file(STRINGS "${_widget_impl}" _widget_export_module
                REGEX "^[ \t]*export[ \t]+module[ \t]+")
            if(NOT _widget_export_module)
                set_property(SOURCE "${_widget_impl}" APPEND PROPERTY
                    VS_SETTINGS "CompileAs=CompileAsCpp")
            endif()
        endif()
    endforeach()
endif()

if(ARTIFACT_COMPILER_CLANG_GNU)
    # The upstream GNU-style Clang driver treats .cppm as a module interface
    # unless the implementation units are explicitly compiled as C++ source.
    # Keep P1689 scanning enabled so imports and module partitions still get
    # ordered against the public interfaces in WIDGETS_MODULES.
    foreach(_widget_impl IN LISTS WIDGETS_IMPL)
        if(_widget_impl MATCHES "[.]cppm$")
            file(STRINGS "${_widget_impl}" _widget_export_module
                REGEX "^[ \t]*export[ \t]+module[ \t]+")
            if(NOT _widget_export_module)
                set_source_files_properties("${_widget_impl}" PROPERTIES
                    CXX_SCAN_FOR_MODULES ON)
                set_property(SOURCE "${_widget_impl}" APPEND PROPERTY
                    COMPILE_OPTIONS -x c++)
            endif()
        endif()
    endforeach()
endif()

if(APPLE)
    target_sources(ArtifactWidgets PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Mac/QuickLook.mm"
    )
endif()

target_compile_definitions(ArtifactWidgets
    PRIVATE WIDGET_LIBRARY_DLL_EXPORT
    INTERFACE WIDGET_LIBRARY_DLL_IMPORT
)

target_include_directories(ArtifactWidgets
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/verdigris/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party"
        "${CMAKE_CURRENT_SOURCE_DIR}/../libs/DiligentEngine"
)

# ---------------------------------------------------------------------------
# 依存関係
# ---------------------------------------------------------------------------
target_link_libraries(ArtifactWidgets PUBLIC ArtifactCore ArtifactCoreAudio ArtifactCoreMedia ArtifactCoreUI ArtifactCoreFile)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Concurrent Multimedia)
target_link_libraries(ArtifactWidgets PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::Concurrent Qt6::Multimedia)

find_package(OpenCV REQUIRED)
target_link_libraries(ArtifactWidgets PUBLIC ${OpenCV_LIBS})

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

find_package(FFMPEG REQUIRED)
if(FFMPEG_FOUND)
    target_include_directories(ArtifactWidgets PUBLIC ${FFMPEG_INCLUDE_DIRS})
    if(FFMPEG_LIBRARY_DIRS)
        target_link_directories(ArtifactWidgets PUBLIC ${FFMPEG_LIBRARY_DIRS})
    endif()
    target_link_libraries(ArtifactWidgets PUBLIC ${FFMPEG_LIBRARIES})
else()
    target_link_libraries(ArtifactWidgets PUBLIC
        "${VCPKG_INSTALLED}/lib/avcodec.lib"
        "${VCPKG_INSTALLED}/lib/avformat.lib"
        "${VCPKG_INSTALLED}/lib/avutil.lib"
        "${VCPKG_INSTALLED}/lib/swscale.lib"
        "${VCPKG_INSTALLED}/lib/swresample.lib"
    )
    target_include_directories(ArtifactWidgets PUBLIC "${VCPKG_INSTALLED}/include")
endif()

artifact_enable_delay_load(ArtifactWidgets
    DEBUG_DLLS
        opencv_core4d.dll
        opencv_calib3d4d.dll
        opencv_dnn4d.dll
        opencv_features2d4d.dll
        opencv_flann4d.dll
        opencv_gapi4d.dll
        opencv_highgui4d.dll
        opencv_imgcodecs4d.dll
        opencv_imgproc4d.dll
        opencv_ml4d.dll
        opencv_objdetect4d.dll
        opencv_photo4d.dll
        opencv_stitching4d.dll
        opencv_video4d.dll
        opencv_videoio4d.dll
        tbb12_debug.dll
        tbbmalloc_debug.dll
        tbbbind_2_5_debug.dll
        avcodec-62.dll
        avformat-62.dll
        avutil-60.dll
        swscale-9.dll
        swresample-6.dll
    RELEASE_DLLS
        opencv_core4.dll
        opencv_calib3d4.dll
        opencv_dnn4.dll
        opencv_features2d4.dll
        opencv_flann4.dll
        opencv_gapi4.dll
        opencv_highgui4.dll
        opencv_imgcodecs4.dll
        opencv_imgproc4.dll
        opencv_ml4.dll
        opencv_objdetect4.dll
        opencv_photo4.dll
        opencv_stitching4.dll
        opencv_video4.dll
        opencv_videoio4.dll
        tbb12.dll
        tbbmalloc.dll
        tbbbind_2_5.dll
        avcodec-62.dll
        avformat-62.dll
        avutil-60.dll
        swscale-9.dll
        swresample-6.dll
)

if(WIN32)
    target_link_libraries(ArtifactWidgets PRIVATE dwmapi shell32 user32)
endif()

if(DEFINED ARTIFACT_USE_EXPLICIT_STD_IFC)
    set_property(TARGET ArtifactWidgets PROPERTY CXX_MODULE_STD OFF)
endif()
