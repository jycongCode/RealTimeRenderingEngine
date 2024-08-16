//
// Created by Lenovo on 2024/8/14.
//

#ifndef WINDOWCALLBACK_H
#define WINDOWCALLBACK_H
#include <GLFW/glfw3.h>
struct WindowCallback{
    GLFWframebuffersizefun framebuffer_size_callback;
    GLFWcursorposfun mouse_pos_callback;
    GLFWmousebuttonfun mouse_button_callback;
    GLFWscrollfun  mouse_scroll_callback;
};
#endif //WINDOWCALLBACK_H
