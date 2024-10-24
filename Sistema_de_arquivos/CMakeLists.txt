cmake_minimum_required(VERSION 3.16)

project(sistema_de_arquivos VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# Se não for necessário, você pode remover esta linha que define arquivos de tradução
set(TS_FILES sistema_de_arquivos_en_US.ts)

# Aqui estão os arquivos do projeto, incluindo os que foram adicionados recentemente
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    filesystemnode.h
    file.h
    file.cpp
    directory.h
    directory.cpp
    ${TS_FILES}  # Remova esta linha se não for usar arquivos de tradução
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(sistema_de_arquivos
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    # Apenas se usar arquivos de tradução
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

else()
    if(ANDROID)
        add_library(sistema_de_arquivos SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(sistema_de_arquivos
            ${PROJECT_SOURCES}
        )
    endif()

    # Apenas se usar arquivos de tradução
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Vincular o executável ao módulo Qt::Widgets
target_link_libraries(sistema_de_arquivos PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Configurações específicas para macOS e iOS
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.sistema_de_arquivos)
endif()

# Definir propriedades adicionais para macOS ou Windows
set_target_properties(sistema_de_arquivos PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Configurações de instalação
include(GNUInstallDirs)
install(TARGETS sistema_de_arquivos
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finaliza a configuração do executável para Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(sistema_de_arquivos)
endif()
