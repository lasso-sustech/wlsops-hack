cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(wlsops-hack VERSION 0.1.0)

add_executable(wlsctrl wlsctrl_test.c wlsctrl.c)
# install(TARGETS wlsctrl DESTINATION bin)
