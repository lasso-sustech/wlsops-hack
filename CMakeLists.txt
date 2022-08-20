cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(wlsops-hack VERSION 0.1.0)
set(FETCHCONTENT_QUIET FALSE)

set(LINUX_VERSION 5.15)
set(LINUX_URL "https://github.com/torvalds/linux/archive/refs/tags/v${LINUX_VERSION}.zip")
set(LINUX_NAME "linux-${LINUX_VERSION}")
set(LINUX_ARCHIVE ${CMAKE_CURRENT_BINARY_DIR}/linux-kernel.zip)
set(LINUX_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/linux-kernel)
set(IWLMVM_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/iwlmvm)
set(WLSOPS_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wlsops)

# fetch the Linux source code
if (NOT EXISTS ${IWLMVM_WORKING_DIRECTORY}/iwlwifi)
    ## download Linux kernel
    if (NOT EXISTS ${LINUX_ARCHIVE})
        file(DOWNLOAD ${LINUX_URL} ${LINUX_ARCHIVE}
                SHOW_PROGRESS)
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

add_subdirectory("iwlmvm")
add_subdirectory("wlsops")
add_subdirectory("wlsctrl")
