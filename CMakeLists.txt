cmake_minimum_required(VERSION 3.5)

project(genshin_relics LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS  "-municode")

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

# 设置链接库目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 添加include路径及源文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(./ SrcFiles)
aux_source_directory(./src SrcFiles)

# 添加ts(翻译)文件并设置刷新和生成qm文件
file(GLOB TS_FILES ${PROJECT_SOURCE_DIR}/*.ts)
qt5_create_translation(QM_FILES ${PROJECT_SOURCE_DIR} ${TS_FILES} OPTIONS -no-obsolete)
add_custom_target(OUTPUT ALL DEPENDS ${QM_FILES})

# 链接指定库文件
#link_libraries(qwtd)

# 生成可执行文件并设置输出目录
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/Bin)
# 请把ui文件名复制于下,此处不加ts文件
add_executable(${PROJECT_NAME} ${SrcFiles}
    #${TS_FILES}
    genshin_relics.ui
    genshin_relics.qrc
    logo.rc
  )

# 复制qm文件到项目目录,需编译两次
add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD # qrc文件处理仍是先于此,故需两次编译才能更新qrc里面的qm资源
    COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${PROJECT_SOURCE_DIR}
  )

#target_compile_definitions(${PROJECT_NAME} PRIVATE GLOBAL_STATIC_LIBRARY)

#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-municode")
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
