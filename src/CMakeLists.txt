cmake_minimum_required(VERSION 3.16)

project(UselessIDE VERSION 0.1 LANGUAGES CXX)



#配置通用编译
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
endif()

#设置可执行文件输出目录
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/debug)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/release)
endif()

#获取文件路径分隔符（解决执行命令的时候有些平台会报错）
file(TO_CMAKE_PATH "/" PATH_SEPARATOR)

#设置版本号
#add_definitions(-DVERSION=0,0,1,0)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Svg Test QuickTest)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#遍历所有Cpp文件
file(GLOB_RECURSE CPP_FILES *.cpp *.h *.hpp)

FOREACH(item ${CPP_FILES})
  IF(${item} MATCHES ".*test.*")
    LIST(REMOVE_ITEM CPP_FILES ${item})
  ENDIF(${item} MATCHES ".*test.*")
ENDFOREACH(item)
#message("files: ${CPP_FILES}")

foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND sources_files ${filename})
endforeach(filepath)

file(GLOB_RECURSE HEADER_FILES *.h)
FOREACH(item ${HEADER_FILES})
  IF(${item} MATCHES ".*test.*")
    LIST(REMOVE_ITEM HEADER_FILES ${item})
  ENDIF(${item} MATCHES ".*test.*")
ENDFOREACH(item)
FOREACH(item ${HEADER_FILES})
  IF(${item} MATCHES ".*main.cpp")
    LIST(REMOVE_ITEM HEADER_FILES ${item})
  ENDIF(${item} MATCHES ".*main.cpp")
ENDFOREACH(item)
foreach(filepath ${HEADER_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND header_files ${filename})
endforeach(filepath)

qt_add_library(IDELib STATIC ${sources_files})
target_link_libraries(IDELib
    Qt6::Core
    Qt6::Quick
    Qt6::Svg
    Qt6::Test
    fluentuiplugin
    FramelessHelper::Core
    FramelessHelper::Quick
)
#qt_finalize_target(IDELib)

#遍历所有qml文件
file(GLOB_RECURSE QML_PATHS *.qml)
foreach(filepath ${QML_PATHS})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
#    get_filename_component(abname ${filename} ABSOLUTE)
    list(APPEND qml_files ${filename})
#    message(${abname})
endforeach(filepath)

#遍历所有资源文件
file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp qmldir *.txt)
foreach(filepath ${RES_PATHS})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
#    get_filename_component(abname ${filename} ABSOLUTE)
    list(APPEND resource_files ${filename})
#    message(${abname})
endforeach(filepath)

#添加可执行文件
#    qt_add_executable(UselessIDE
#        ${sources_files}
#    )

# 复制测试文件到输出目录
file(GLOB_RECURSE TEST_QML *.qml)
foreach(filepath ${TEST_QML})
    IF(${filepath} MATCHES ".*tst.*")
        message("xxxxxxxxxxxxx ${filepath}")
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
        file(COPY ${filename} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    ENDIF(${filepath} MATCHES ".*tst.*")
endforeach(filepath)


qt_add_executable(UselessIDE
    ${sources_files}
)
#添加qml模块
qt_add_qml_module(UselessIDE
    URI UselessIDE
    VERSION 1.0
    QML_FILES ${qml_files}
    RESOURCES ${resource_files}
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/UselessIDE
)

#qt_add_qml_module(IDELib
#    URI IDELib
#    VERSION 1.0
#    QML_FILES ${qml_files}
#    RESOURCES ${resource_files}
#    OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#)

#设置属性
set_target_properties(UselessIDE PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.github.Aaron-Gp.UselessIDE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

#链接库
target_link_libraries(UselessIDE PRIVATE
    Qt6::Quick
    Qt6::Svg
    Qt6::Test
    Qt6::QuickTest
    fluentuiplugin
    FramelessHelper::Core
    FramelessHelper::Quick
)

#安装
install(TARGETS UselessIDE
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_subdirectory(test)
