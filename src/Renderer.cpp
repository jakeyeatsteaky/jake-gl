#include "Renderer.hpp"

#include <GLFW/glfw3.h>

#include <cstring>

Renderer::Renderer():
    m_instance(NULL),
    m_validationLayers({}),
    m_extensions({}),
    m_physicalDevice(VK_NULL_HANDLE)
{

}

Renderer::~Renderer()
{
    std::cout << "Destroying Renderer: " << std::endl;
    DestroyDebugUtilsMessengerEXT(m_instance, nullptr, m_debugMessenger);
    std::cout << "\t Destroyed Debug Messenger" << std::endl;
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

    for(uint32_t i = 0; i < glfwExtensionCount; i++)
    {
        m_extensions.push_back(glfwExtensions[i]);
    }

    if(util::DEBUG_BUILD)
        m_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);


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

    createInfo.enabledExtensionCount = (uint32_t)m_extensions.size();
    createInfo.ppEnabledExtensionNames = m_extensions.data();
    createInfo.enabledLayerCount = 0;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};
    if(!m_validationLayers.empty())
    {
        PopulateDebugMessengerCreateInfo(debugCreateInfo);

        createInfo.enabledLayerCount = (uint32_t)m_validationLayers.size();
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance), "Vk Create Instance", true);
}

void Renderer::SetupDebugMessenger()
{
    if(m_validationLayers.empty())
        return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo {};
    PopulateDebugMessengerCreateInfo(createInfo);
    VK_CHECK(CreateDebugUtilsMessengerEXT(
            m_instance,
            &createInfo,
            nullptr,
            &m_debugMessenger), "VK Create Debug Messenger", true);

}

VkResult Renderer::CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
    if(function)
        return function(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}


void Renderer::DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT debugMessenger)
{
    auto function = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
    if(function)
        function(instance, debugMessenger, pAllocator);
}


void Renderer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
    createInfo.pUserData = nullptr;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT sev,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    [[maybe_unused]] void* pUserData)
{
    // 0x1 Gen, 0x2 validation, 0x4 perf
    std::string msgType = (type & 0b1) ? "General" : ((type & 0b10) ? "Validation" : ((type & 0b100) ? "Performance" : ""));

    switch(sev)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    {
        std::cerr << msgType << " Layer (ERROR): " << pCallbackData->pMessage << std::endl;
        break;
    }

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    {
        std::cerr << msgType << " Layer (WARNING): " << pCallbackData->pMessage << std::endl;
        break;
    }

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    {
        std::cerr << msgType << " Layer (VERBOSE): " << pCallbackData->pMessage << std::endl;
        break;
    }

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    {
        std::cerr << msgType << " Layer (INFO): " << pCallbackData->pMessage << std::endl;
        break;
    }

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
    {
        break;
    }

    default:
        break;
    }

    return VK_FALSE;
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

void Renderer::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr), "Get Physical Device Count", true);

    std::vector<VkPhysicalDevice> physical_devices(deviceCount);
    VK_CHECK(vkEnumeratePhysicalDevices(m_instance, &deviceCount, physical_devices.data()), "Enumerate Physical Devices", true);

    VK_CHECK(FindSuitablePhy(physical_devices), "Find a Suitable Phy", true);




}

VkResult Renderer::FindSuitablePhy(const std::vector<VkPhysicalDevice>& physical_devices)
{
    VkResult result = VK_NOT_READY;

    for(const auto& device: physical_devices)
    {
        if(IsDeviceSuitable(device)){
            m_physicalDevice = device;
            result = VK_SUCCESS;
            break;
        }
    }

    return result;
}

bool Renderer::IsDeviceSuitable(const VkPhysicalDevice& device)
{
    bool ret = false;
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // Specifically use the discrete gpu, the feature is just an example

    ret = (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader);
    return ret;

    // Idea for picking highest rated GPU
//      #include <map>

//     ...

//         void pickPhysicalDevice() {
//         ...

//             // Use an ordered map to automatically sort candidates by increasing score
//             std::multimap<int, VkPhysicalDevice> candidates;

//         for (const auto& device : devices) {
//             int score = rateDeviceSuitability(device);
//             candidates.insert(std::make_pair(score, device));
//         }

//         // Check if the best candidate is suitable at all
//         if (candidates.rbegin()->first > 0) {
//             physicalDevice = candidates.rbegin()->second;
//         } else {
//             throw std::runtime_error("failed to find a suitable GPU!");
//         }
//     }

//     int rateDeviceSuitability(VkPhysicalDevice device) {
//         ...

//             int score = 0;

//         // Discrete GPUs have a significant performance advantage
//         if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
//             score += 1000;
//         }

//         // Maximum possible size of textures affects graphics quality
//         score += deviceProperties.limits.maxImageDimension2D;

//         // Application can't function without geometry shaders
//         if (!deviceFeatures.geometryShader) {
//             return 0;
//         }

//         return score;
//     }

}
















