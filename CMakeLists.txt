cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(wlsops-hack VERSION 0.1.0)
OPTION(BUILD_IWLMVM "Build iwlmvm driver" TRUE)
OPTION(BUILD_IWLDVM "Build iwldvm driver" OFF)

set(LINUX_VERSION 5.15)
set(LINUX_URL "https://github.com/torvalds/linux/archive/refs/tags/v${LINUX_VERSION}.zip")
set(LINUX_NAME "linux-${LINUX_VERSION}")
set(LINUX_ARCHIVE ${CMAKE_CURRENT_BINARY_DIR}/linux-kernel.zip)
set(LINUX_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/linux-kernel)
set(IWLMVM_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/iwlmvm)
set(IWLDVM_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/iwldvm)
set(WLSOPS_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wlsops)

# fetch the Linux source code
if (${BUILD_IWLMVM} AND NOT EXISTS ${IWLMVM_WORKING_DIRECTORY}/iwlwifi  OR  NOT EXISTS ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    ## download Linux kernel
    if (NOT EXISTS ${LINUX_ARCHIVE})
        file(DOWNLOAD ${LINUX_URL} ${LINUX_ARCHIVE} SHOW_PROGRESS)
    endif()
    ## extract the archive
    if (NOT EXISTS ${LINUX_FOLDER})
        file(ARCHIVE_EXTRACT INPUT ${LINUX_ARCHIVE}
                DESTINATION ${LINUX_FOLDER})
    endif()
    ## copy iwlwifi drvier into folder `iwlmvm` and apply the patch
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/drivers/net/wireless/intel/iwlwifi
            DESTINATION ${IWLMVM_WORKING_DIRECTORY})
    execute_process(COMMAND bash -c "patch -p1 < ../iwlmvm-${LINUX_VERSION}.patch"
            WORKING_DIRECTORY ${IWLMVM_WORKING_DIRECTORY}/iwlwifi)
    ## update the intrinsic files
    file(MAKE_DIRECTORY ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/net/mac80211/ieee80211_i.h
            DESTINATION ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/net/mac80211/sta_info.h
            DESTINATION ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/net/mac80211/key.h
            DESTINATION ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/net/mac80211/debug.h
            DESTINATION ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    ## remove the unarchived temporary files
    file(REMOVE_RECURSE ${LINUX_FOLDER})
endif()

# fetch the Linux source code
if (${BUILD_IWLDVM} AND NOT EXISTS ${IWLDVM_WORKING_DIRECTORY}/iwlwifi  OR  NOT EXISTS ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    ## download Linux kernel
    if (NOT EXISTS ${LINUX_ARCHIVE})
        file(DOWNLOAD ${LINUX_URL} ${LINUX_ARCHIVE} SHOW_PROGRESS)
    endif()
    ## extract the archive
    if (NOT EXISTS ${LINUX_FOLDER})
        file(ARCHIVE_EXTRACT INPUT ${LINUX_ARCHIVE}
                DESTINATION ${LINUX_FOLDER})
    endif()
    ## copy iwlwifi drvier into folder `iwldvm` and apply the patch
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/drivers/net/wireless/intel/iwlwifi
            DESTINATION ${IWLDVM_WORKING_DIRECTORY})
    execute_process(COMMAND bash -c "patch -p1 < ../iwldvm-${LINUX_VERSION}.patch"
            WORKING_DIRECTORY ${IWLDVM_WORKING_DIRECTORY}/iwlwifi)
    ## update the intrinsic files
    file(MAKE_DIRECTORY ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/net/mac80211/ieee80211_i.h
            DESTINATION ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/net/mac80211/sta_info.h
            DESTINATION ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/net/mac80211/key.h
            DESTINATION ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/net/mac80211/debug.h
            DESTINATION ${WLSOPS_WORKING_DIRECTORY}/intrinsic)
    ## remove the unarchived temporary files
    file(REMOVE_RECURSE ${LINUX_FOLDER})
endif()

if (${BUILD_IWLMVM})
    add_subdirectory("iwlmvm")
endif()

if (${BUILD_IWLDVM})
    add_subdirectory("iwldvm")
endif()


add_subdirectory("wlsops")
add_subdirectory("wlsctrl")

# add custom target
if (${BUILD_IWLMVM})
add_custom_target(insmod
        COMMAND sudo rmmod iwlmvm
        COMMAND sudo insmod ${CMAKE_CURRENT_BINARY_DIR}/iwlmvm/iwlmvm.ko
        COMMAND sleep 0.5
        COMMAND sudo insmod ${CMAKE_CURRENT_BINARY_DIR}/wlsops/wlsops_hack.ko)
add_custom_target(rmmod
        COMMAND sudo rmmod wlsops_hack
        COMMAND sudo rmmod iwlmvm
        COMMAND sudo modprobe iwlmvm)
endif()

if (${BUILD_IWLDVM})
add_custom_target(insmod
        COMMAND sudo rmmod iwldvm
        COMMAND sudo insmod ${CMAKE_CURRENT_BINARY_DIR}/iwlmvm/iwldvm.ko
        COMMAND sleep 0.5
        COMMAND sudo insmod ${CMAKE_CURRENT_BINARY_DIR}/wlsops/wlsops_hack.ko)
add_custom_target(rmmod
        COMMAND sudo rmmod wlsops_hack
        COMMAND sudo rmmod iwldvm
        COMMAND sudo modprobe iwldvm)
endif()
