cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(iwlmvm-mod VERSION 0.1.0)

set(IWLMVM_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/iwlwifi)
if (NOT EXISTS ${IWLMVM_WORKING_DIRECTORY})
    message(FATAL_ERROR "Folder `iwlwifi` not exists. Please fetch the source code in the upper CMake firstly.")
endif()

# call Kbuild for building
execute_process(COMMAND uname -r OUTPUT_VARIABLE KERNVER
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(KDIR "/lib/modules/${KERNVER}/build")
set(DRIVER_FILES iwlmvm.ko)
set(KBUILD_CMD $(MAKE) -C ${KDIR} modules M=${CMAKE_CURRENT_BINARY_DIR} src=${IWLMVM_WORKING_DIRECTORY}/mvm)
add_custom_command(
        OUTPUT ${DRIVER_FILES}
        COMMAND rm -f ${DRIVER_FILES}
        COMMAND ${KBUILD_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM)
add_custom_target(module_iwlmvm ALL DEPENDS ${DRIVER_FILES})  #defautl target
add_custom_target(cleanup_iwlmvm COMMAND ${KBUILD_CMD} clean) #clean
