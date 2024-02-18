#include "renderer/vulkan/vulkan_backend.h"
#include "renderer/vulkan/vulkan_types.inl"
#include "renderer/vulkan/vulkan_platform.h"

#include "core/logger.h"
#include "core/kstring.h"

#include "containers/darray.h"

static vulkan_context context;

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data);

b8 vulkan_renderer_backend_initialize(struct renderer_backend* backend, const char* application_name, struct platform_state* plat_state) {
    
    // TODO: custom allocator
    context.allocator = 0;

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = application_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Test Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;

    const char** required_extensions = darray_create(const char*);
    darray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME); // GEneric surface extension
    platform_get_required_extension_names(&required_extensions); // Platform specific extension(s)

#if defined(_DEBUG)
    darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Debugging extension

    KDEBUG("Required extensions:");
    u32 length = darray_length(required_extensions);
    for (u32 i = 0; i < length; ++i) {
        KDEBUG(required_extensions[i]);
    }
#endif

    create_info.enabledExtensionCount = darray_length(required_extensions);
    create_info.ppEnabledExtensionNames = required_extensions;

    const char** required_validation_layer_names = 0;
    u32 required_validation_layer_names_count = 0;

#if defined(_DEBUG)
    KINFO("Validation layers enable. Enumerating...");

    // validation layers required
    required_validation_layer_names = darray_create(const char*);
    darray_push(required_validation_layer_names, &"VK_LAYER_KHRONOS_validation");
    required_validation_layer_names_count = darray_length(required_validation_layer_names);

    // Obtener una lista de las capas de validación disponibles
    u32 available_layer_count = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
    VkLayerProperties* available_layers = darray_reserve(VkLayerProperties, available_layer_count);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers));

    // Verificar todas las capas de validación disponibles
    for (u32 i = 0; i < required_validation_layer_names_count; ++i) {
        KINFO("Searching for layer: %s...", required_validation_layer_names[i]);
        b8 found = FALSE;
        for (u32 j = 0; j < available_layer_count; ++j) {
            if (strings_equal(required_validation_layer_names[i], available_layers[j].layerName)) {
                found = TRUE;
                KINFO("Found");
                break;
            }
        }

        if (!found) {
            KFATAL("Required validation layer is missing: %s", required_validation_layer_names[i]);
            return FALSE;
        }
    }

    KINFO("All required validation layers found");
#endif

    create_info.ppEnabledLayerNames = required_validation_layer_names;
    create_info.enabledLayerCount = required_validation_layer_names_count;

    VK_CHECK(vkCreateInstance(&create_info, context.allocator, &context.instance));
    KINFO("Vulkan Instance created");

    // Debugger
#if defined(_DEBUG)
    KDEBUG("Creating Vulkan debugger...");
    u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity = log_severity;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;
    
    PFN_vkCreateDebugUtilsMessengerEXT func = 
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
    KASSERT_MSG(func, "Failed to create debug_messenger!");
    VK_CHECK(func(context.instance, &debug_create_info, context.allocator, &context.debug_messenger));
    KDEBUG("Vulkan debugger created successfully");
#endif
    

    KINFO("Vulkan instance created successfully");
    return TRUE;
}

void vulkan_renderer_backend_shutdown(struct renderer_backend* backend) {

#if defined(_DEBUG)
    KDEBUG("Destroying Vulkan debugger...");
    if (context.debug_messenger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = 
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debug_messenger, context.allocator);    
    }
#endif

    KDEBUG("Destroying Vulkan instance...");
    vkDestroyInstance(context.instance, context.allocator);
}


void vulkan_renderer_backend_resized(struct renderer_backend* backend, u16 width, u16 height) {
    
}


b8 vulkan_renderer_backend_begin_frame(struct renderer_backend* backend, f32 delta_time) {
    return TRUE;
}

b8 vulkan_renderer_backend_end_frame(struct renderer_backend* backend, f32 delta_time) {
    return TRUE;   
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data) {

    switch (message_severity) {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            KERROR(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            KWARN(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            KINFO(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            KDEBUG(callback_data->pMessage);
            break;
    }
    return VK_FALSE;
}