#include "Renderer.hpp"

#include <GLFW/glfw3.h>

#include <cstring>

Renderer::Renderer():
    m_instance(NULL)
{

}

Renderer::~Renderer()
{
    std::cout << "Destroying Renderer: " << std::endl;
    vkDestroyInstance(m_instance, nullptr);
    std::cout << "\t Destroyed instance" << std::endl;
}

void Renderer::AddValidationLayers()
{
    m_validationLayers.push_back("VK_LAYER_KHRONOS_validation");
}

bool Renderer::CheckValidationLayers() const
{
    bool layerFound = false;
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> validation_layer_properties(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, validation_layer_properties.data());

    for(const char* layerName : m_validationLayers)
    {
        layerFound = false;
        for(const auto& layerProperty : validation_layer_properties)
        {
            if(strcmp(layerName, layerProperty.layerName) == 0)
            {
                std::cout << "Layer Found: " << layerName << std::endl;
                layerFound = true;
                break;
            }
        }
        if(!layerFound)
            std::cout << "Validation Layer Requested, but not found: " << layerName << std::endl;
    }
    return layerFound;
}

void Renderer::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if(util::DEBUG_BUILD)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}

void Renderer::CreateInstance()
{
    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "Hello Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t extCount = 0;
    CheckExtensions(&extCount);

    const char** requiredExts = glfwGetRequiredInstanceExtensions(&extCount);
    createInfo.enabledExtensionCount = extCount;
    createInfo.ppEnabledExtensionNames = requiredExts;
    createInfo.enabledLayerCount = 0;

    if(!m_validationLayers.empty())
    {
        createInfo.enabledLayerCount = (uint32_t)m_validationLayers.size();
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
    }
    else
        createInfo.enabledLayerCount = 0;

    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance), "Vk Create Instance", true);
}

void Renderer::CheckExtensions(uint32_t* count, bool printInfo)
{
    std::vector<VkExtensionProperties> ext_props {};
    vkEnumerateInstanceExtensionProperties(nullptr, count, nullptr);
    ext_props.resize(*count);
    vkEnumerateInstanceExtensionProperties(nullptr, count, ext_props.data());

    if(!printInfo)
        return;

    std::cout << "Available Extensions:" << std::endl;
    for(auto& ext: ext_props)
    {
        std::cout <<"\t" << ext.extensionName << std::endl;
    }
}






















