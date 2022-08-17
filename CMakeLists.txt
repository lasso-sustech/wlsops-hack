cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(wlsops-hack VERSION 0.1.0)
add_subdirectory("wlsctrl")

# setup variables
execute_process(COMMAND uname -r OUTPUT_VARIABLE KERNVER
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(KDIR "/lib/modules/${KERNVER}/build")
set(INSTALL_DIRECTORY "/lib/modules/${KERNVER}/updates")

# failure check
find_file(kernel_makefile NAMES Makefile
            PATHS ${KDIR} NO_DEFAULT_PATH)
if(NOT kernel_makefile)
    message(FATAL_ERROR "There is no Makefile in ${KDIR}!")
endif()

# call Kbuild for building
set(DRIVER_NAME wlsops_hook)
set(DRIVER_FILE wlsops_hook.ko)
set(KBUILD_CMD $(MAKE) -C ${KDIR} modules M=${CMAKE_CURRENT_BINARY_DIR} src=${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(
        OUTPUT ${DRIVER_FILE}
        COMMAND ${KBUILD_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS hack_entry.c hack_mmap.c wlsops.c VERBATIM)
add_custom_target(module ALL DEPENDS ${DRIVER_FILE})   #defautl target
add_custom_target(cleanup COMMAND ${KBUILD_CMD} clean) #clean

# install target
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DRIVER_FILE}
        DESTINATION ${INSTALL_DIRECTORY})
        
# add custom target
add_custom_target(insmod COMMAND sudo insmod ${DRIVER_FILE})
add_custom_target(rmmod-inotify COMMAND sudo rmmod ${DRIVER_NAME})
add_custom_target(dmesg COMMAND sudo dmesg | grep --color "LAB525:")