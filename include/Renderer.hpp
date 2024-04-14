#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <vulkan/vulkan.h>

#include <vector>
#include <stdexcept>
#include <iostream>

#include "Utility.hpp"

// VK_CHECK(vkCreateFunction, "String of Message", Bool print verbose);
#define VK_CHECK(x, y, z)                                       \
do                                                              \
    {                                                           \
        VkResult err = x;                                       \
        if (err != VK_SUCCESS)                                  \
        {                                                       \
            throw std::runtime_error(y);                        \
        }                                                       \
        else {                                                  \
            if(z)                                               \
                std::cout << "\tSUCCESS: " << y << std::endl;}  \
    } while (0)

class Renderer
{
private:
    VkInstance m_instance;
    std::vector<const char*> m_validationLayers;
    std::vector<const char*> m_extensions;
    VkDebugUtilsMessengerEXT m_debugMessenger;

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT sev,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    VkResult CreateDebugUtilsMessengerEXT(
                            VkInstance instance,
                            const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
                            const VkAllocationCallbacks* pAllocator,
                            VkDebugUtilsMessengerEXT* pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT debugMessenger);

public:
    Renderer();
    ~Renderer();
    void AddValidationLayers();
    bool CheckValidationLayers() const;
    void GetRequiredExtensions();
    void CheckExtensions(uint32_t* count, bool printInfo = true);
    bool IsInitialized();
    void CreateInstance();
    void SetupDebugMessenger();
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);


};


#endif // end renderer.hpp
