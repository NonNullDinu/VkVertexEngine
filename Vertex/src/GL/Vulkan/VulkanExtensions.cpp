#include "VulkanExtensions.h"

// VK_EXT_debug_utils
//////////////////////////////////

static PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXTDispatchTable;
static PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXTDispatchTable;
static PFN_vkSubmitDebugUtilsMessageEXT SubmitDebugUtilsMessageEXTDispatchTable;

static void loadDebugUtilsCommands( VkInstance instance ){
    PFN_vkVoidFunction temp_fp;

    temp_fp = vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" );
    if( !temp_fp ) throw "Failed to load vkCreateDebugUtilsMessengerEXT"; // check shouldn't be necessary (based on spec)
    CreateDebugUtilsMessengerEXTDispatchTable = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>( temp_fp );

    temp_fp = vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" );
    if( !temp_fp ) throw "Failed to load vkDestroyDebugUtilsMessengerEXT"; // check shouldn't be necessary (based on spec)
    DestroyDebugUtilsMessengerEXTDispatchTable = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>( temp_fp );

    temp_fp = vkGetInstanceProcAddr( instance, "vkSubmitDebugUtilsMessageEXT" );
    if( !temp_fp ) throw "Failed to load vkSubmitDebugUtilsMessageEXT"; // check shouldn't be necessary (based on spec)
    SubmitDebugUtilsMessageEXTDispatchTable = reinterpret_cast<PFN_vkSubmitDebugUtilsMessageEXT>( temp_fp );
}

static void unloadDebugUtilsCommands( VkInstance instance ){
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pMessenger
){
    auto dispatched_cmd = CreateDebugUtilsMessengerEXTDispatchTable;
    return dispatched_cmd( instance, pCreateInfo, pAllocator, pMessenger );
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks* pAllocator
){
    auto dispatched_cmd = DestroyDebugUtilsMessengerEXTDispatchTable;
    return dispatched_cmd( instance, messenger, pAllocator );
}

VKAPI_ATTR void VKAPI_CALL vkSubmitDebugUtilsMessageEXT(
    VkInstance instance,
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData
){
    auto dispatched_cmd = SubmitDebugUtilsMessageEXTDispatchTable;
    return dispatched_cmd( instance, messageSeverity, messageTypes, pCallbackData );
}

void LoadVkExtensions(VkInstance instance)
{
    loadDebugUtilsCommands(instance);
}

void UnloadVkExtensions(VkInstance instance){
    unloadDebugUtilsCommands(instance);
}