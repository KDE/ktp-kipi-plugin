project (ktp-kipi-plugin)

set (KTP_KIPI_PLUGIN_VERSION "0.4")

set (CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
     ${CMAKE_MODULE_PATH}
)

set(KDE_MIN_VERSION "4.7.0")
set(IS_KTP_INTERNAL_MODULE TRUE)
set(IS_KIPI_PLUGINS_INTERNAL_MODULE TRUE)
find_package(KDE4 REQUIRED)
find_package(TelepathyQt4 0.9.0 REQUIRED)
find_package(KTp REQUIRED)
find_package(Kipi REQUIRED)
find_package(Kexiv2 REQUIRED)
find_package(Kdcraw REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)

# Add the compiler flags
add_definitions (${QT_DEFINITIONS}
                 ${KDE4_DEFINITIONS}
)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${KDE4_INCLUDES}
                    ${TELEPATHY_QT4_INCLUDE_DIR}
                    ${KTP_INCLUDE_DIR}
                    ${KIPI_INCLUDE_DIR}
)

configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)

set(kipiplugins_SRCS
    libkipiplugins/dialogs/kpimagedialog.cpp
    libkipiplugins/tools/kpaboutdata.cpp
    libkipiplugins/tools/kphostsettings.cpp
    libkipiplugins/tools/kpimageinfo.cpp
    libkipiplugins/tools/kpmetadata.cpp
    libkipiplugins/tools/kpmetasettings.cpp
    libkipiplugins/tools/kprawthumbthread.cpp
    libkipiplugins/tools/kpversion.cpp
    libkipiplugins/widgets/kpimageslist.cpp
    libkipiplugins/widgets/kpprogresswidget.cpp
)

# Kipi plugins debug area
set(AREA_CODE_LOADING "51001")
add_definitions(-DAREA_CODE_LOADING=${AREA_CODE_LOADING})
include_directories(libkipiplugins
                    libkipiplugins/tools
                    libkipiplugins/widgets
)

set (kipiplugin_telepathy_SRCS
     kipi-telepathy-plugin.cpp
     kipi-telepathy-dialog.cpp
     kipi-telepathy-widget.cpp
     ${kipiplugins_SRCS}
)

kde4_add_plugin (kipiplugin_telepathy
                 ${kipiplugin_telepathy_SRCS}
)

target_link_libraries (kipiplugin_telepathy
                       ${QT_QTGUI_LIBRARY}
                       ${KDE4_KDECORE_LIBS}
                       ${KDE4_KDEUI_LIBS}
                       ${KDE4_KIO_LIBS}
                       ${KEXIV2_LIBRARIES}
                       ${KDCRAW_LIBRARIES}
                       ${KIPI_LIBRARIES}
                       ${TELEPATHY_QT4_LIBRARIES}
                       ${KTP_LIBRARIES}
                       ${KTP_MODELS_LIBRARIES}
                       ${KTP_WIDGETS_LIBRARIES}
)

install (TARGETS kipiplugin_telepathy
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

configure_file(kipiplugin_telepathy.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/kipiplugin_telepathy.desktop)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/kipiplugin_telepathy.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
