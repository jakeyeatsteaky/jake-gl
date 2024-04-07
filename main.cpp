#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>



int main(int argc, char** argv)
{

   std::cout << "Hello world" << std::endl;
   std::cout << "arg count: " << argc << std::endl;
   for (int i = 0; i < argc; i++) {
      std::cout << "arg: " << i << " -> " << argv[i] << std::endl;
   }

   glfwInit();

   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
   GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

   uint32_t extensionCount = 0;
   vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

   std::cout << extensionCount << " extensions supported\n";

   glm::mat4 matrix;
   glm::vec4 vec;
   [[maybe_unused]] auto test = matrix * vec;

   while(!glfwWindowShouldClose(window)) {
         glfwPollEvents();
   }

   glfwDestroyWindow(window);
   glfwTerminate();

   return 0;

}
