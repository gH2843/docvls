cmake_minimum_required(VERSION 3.26)
project(docvls)

set(CMAKE_CXX_STANDARD 23)

add_executable(docvls main.cpp
        src/soft_wrap.h
        src/document.h
        src/doubly_linked_list.h
        src/windows/page_window.h
        src/windows/turn_page_bar_win.h)
target_link_libraries(docvls "-lncursesw")
