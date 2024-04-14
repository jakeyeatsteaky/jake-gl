#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "App.hpp"
#include "Utility.hpp"


void print_args(int argc, char** argv, bool print = false)
{
   if(!print)
      return;

   std::cout << "Hello world" << std::endl;
   std::cout << "arg count: " << argc << std::endl;
   for (int i = 0; i < argc; i++) {
      std::cout << "arg: " << i << " -> " << argv[i] << std::endl;
   }
}

#if 0
============================= TODO ==================================
 [ ] https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Validation_layers
    X-> Message callback -- start here

 [ ] Log the validation layers and print to console only the important ones.  Right now, every message is being printed to the console
#endif


int main(int argc, char** argv)
{
   print_args(argc, argv);

   App app;

   app.Setup();

   try {
      app.Run();
   } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return util::eERR_FAILED;
   }

   return util::eERR_SUCCESS;

}
