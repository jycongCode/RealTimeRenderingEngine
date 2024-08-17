//
// Created by Lenovo on 2024/8/14.
//

#ifndef WINDOWCALLBACK_H
#define WINDOWCALLBACK_H
#include <GLFW/glfw3.h>
struct WindowCallback{
    GLFWframebuffersizefun framebuffer_size_callback = nullptr;
    GLFWcursorposfun mouse_pos_callback = nullptr;
    GLFWmousebuttonfun mouse_button_callback = nullptr;
    GLFWscrollfun  mouse_scroll_callback = nullptr;
};
#endif //WINDOWCALLBACK_H
