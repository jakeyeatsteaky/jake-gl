#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <GLFW/glfw3.h>


namespace util
{
    constexpr bool DEBUG_BUILD = 1;

    enum
    {
        eERR_FAILED = -1,
        eERR_SUCCESS = 0
    }; 

    void DestroyWindow(GLFWwindow* window);
}


#endif // end utility.hpp
