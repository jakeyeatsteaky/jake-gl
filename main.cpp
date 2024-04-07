#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "App.hpp"

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


int main(int argc, char** argv)
{
   print_args(argc, argv);

   App app;

   app.Setup();

   while(app.IsRunning())
   {
      app.Input();
      app.Update();
      app.Render();
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
   }


   

}
