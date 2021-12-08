set(qtquickcontrols2imaginestyleplugin_FOUND FALSE)

# note: _third_party_deps example: "ICU\\;FALSE\\;1.0\\;i18n uc data;ZLIB\\;FALSE\\;\\;"
set(_third_party_deps "")

foreach(_target_dep ${_third_party_deps})
    list(GET _target_dep 0 pkg)
    list(GET _target_dep 1 is_optional)
    list(GET _target_dep 2 version)
    list(GET _target_dep 3 components)
    set(find_package_args "${pkg}")
    if(version)
        list(APPEND find_package_args "${version}")
    endif()
    if(components)
        string(REPLACE " " ";" components "${components}")
        list(APPEND find_package_args COMPONENTS ${components})
    endif()

    if(is_optional)
        if(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
            list(APPEND find_package_args QUIET)
        endif()
        find_package(${find_package_args})
    else()
        find_dependency(${find_package_args})
    endif()
endforeach()

set(__qt_use_no_default_path_for_qt_packages "NO_DEFAULT_PATH")
if(QT_DISABLE_NO_DEFAULT_PATH_IN_QT_PACKAGES)
    set(__qt_use_no_default_path_for_qt_packages "")
endif()

# note: target_deps example: "Qt6Core\;5.12.0;Qt6Gui\;5.12.0"
set(_target_deps "Qt6Core\;6.0.1;Qt6Gui\;6.0.1;Qt6Qml\;6.0.1;Qt6Quick\;6.0.1")
foreach(_target_dep ${_target_deps})
    list(GET _target_dep 0 pkg)
    list(GET _target_dep 1 version)

    if (NOT ${pkg}_FOUND)
        find_dependency(${pkg} ${version}
            PATHS
                ${CMAKE_CURRENT_LIST_DIR}/../..
                ${QT_ADDITIONAL_PACKAGES_PREFIX_PATH}
                ${QT_EXAMPLES_CMAKE_PREFIX_PATH}
            ${__qt_use_no_default_path_for_qt_packages}
        )
    endif()
endforeach()

set(qtquickcontrols2imaginestyleplugin_FOUND TRUE)
