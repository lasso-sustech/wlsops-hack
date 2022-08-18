cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(iwlmvm-mod VERSION 0.1.0)
set(FETCHCONTENT_QUIET FALSE)

# download Linux kernel v5.15
set(LINUX_URL "https://github.com/torvalds/linux/archive/refs/tags/v5.15.zip")
set(LINUX_NAME "linux-5.15")
set(LINUX_ARCHIVE ${CMAKE_CURRENT_BINARY_DIR}/linux-kernel.zip)
set(LINUX_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/linux-kernel)
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/iwlwifi)
    if (NOT EXISTS ${LINUX_ARCHIVE})
        file(DOWNLOAD ${LINUX_URL} ${LINUX_ARCHIVE}
                SHOW_PROGRESS)
    endif()
    if (NOT EXISTS ${LINUX_FOLDER})
        file(ARCHIVE_EXTRACT INPUT ${LINUX_ARCHIVE}
                DESTINATION ${LINUX_FOLDER} VERBOSE)
    endif()
    ##
    file(COPY ${LINUX_FOLDER}/${LINUX_NAME}/drivers/net/wireless/intel/iwlwifi
            DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
    file(REMOVE_RECURSE ${LINUX_FOLDER})
    ##
    execute_process(COMMAND bash -c "patch -p1 < ../iwlmvm-mod.patch"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/iwlwifi)
endif()

# call Kbuild for building
execute_process(COMMAND uname -r OUTPUT_VARIABLE KERNVER
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(KDIR "/lib/modules/${KERNVER}/build")
set(DRIVER_FILES iwlmvm.ko)
set(KBUILD_CMD $(MAKE) -C ${KDIR} modules M=${CMAKE_CURRENT_BINARY_DIR} src=${CMAKE_CURRENT_SOURCE_DIR}/iwlwifi/mvm)
add_custom_command(
        OUTPUT ${DRIVER_FILES}
        COMMAND ${KBUILD_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM)
add_custom_target(module ALL DEPENDS ${DRIVER_FILES})  #defautl target
add_custom_target(cleanup COMMAND ${KBUILD_CMD} clean) #clean
