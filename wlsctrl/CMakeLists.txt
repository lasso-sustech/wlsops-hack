cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(wlsops-hack VERSION 0.1.0)

add_library(wlsctrl SHARED wlsctrl_usr.c wlsctrl.c)
add_executable(wlsctrl_usr wlsctrl_usr.c wlsctrl.c)
# install(TARGETS wlsctrl DESTINATION bin)
