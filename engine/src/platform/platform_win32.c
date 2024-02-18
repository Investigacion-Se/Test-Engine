#include "platform/platform.h"

// Esta es la capa de windows, por lo que solo debería tener esta implementación
// si se esta usando windows
#if KPLATFORM_WINDOWS

#include "core/logger.h"
#include "core/input.h"

#include "containers/darray.h"

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

typedef struct internal_state {
    HINSTANCE h_instance;
    HWND hwnd;
} internal_state;

static f64 clock_frecuency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

b8 platform_startup(
    platform_state* plat_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height) {

   plat_state->internal_state = malloc(sizeof(internal_state)); 
   internal_state *state = (internal_state*) plat_state->internal_state;

   state->h_instance = GetModuleHandleA(0);

   HICON icon = LoadIcon(state->h_instance, IDI_APPLICATION);
   WNDCLASSA wc;
   memset(&wc, 0, sizeof(wc));
   wc.style = CS_DBLCLKS; // Soporta doble click
   wc.lpfnWndProc = win32_process_message;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hInstance = state->h_instance;
   wc.hIcon = icon;
   wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
   wc.hbrBackground = NULL;
   wc.lpszClassName = "kohi_window_class";

   if (!RegisterClassA(&wc)) {
       MessageBoxA(0, "Window registration failed (platform_win32.c)", "Error", MB_ICONEXCLAMATION | MB_OK);
       return FALSE;
   }

    // crear la ventana
    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    // Consiguiendo el tamaño del borde
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    window_x += border_rect.left;
    window_y += border_rect.top;

    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowExA(
        window_ex_style, "kohi_window_class", application_name,
        window_style, window_x, window_y, window_width, window_height,
        0, 0, state->h_instance, 0);

    if (handle == 0) {
        MessageBoxA(0, "Window creation failed (platform_win32.c)", "Error", MB_ICONEXCLAMATION | MB_OK);

        KFATAL("Window creation failed (platform_win32.c)");
        return FALSE;
    }

    state->hwnd = handle;

    // mostrar ventana
    b32 should_activate = 1; // TODO: Si la ventana no debería aceptar inputs entonces deberia ser false
    i32 show_window_command_flags = (should_activate) ? SW_SHOW : SW_SHOWNOACTIVATE;
    // si esta minimizado usar SW_MNIMIZE : SW_SHOWMINNOACTIVE
    // si esta maximizada usar SW_SHOWMAXIMIZED : SW_MAXMIZE
    ShowWindow(state->hwnd, show_window_command_flags);

    // Settear el clock
    LARGE_INTEGER frecquency;
    QueryPerformanceFrequency(&frecquency);
    clock_frecuency = 1.0 / (f64)frecquency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return TRUE;
}

void platform_shutdown(platform_state* plat_state) {
    internal_state* state = (internal_state*) plat_state->internal_state;

    if (state->hwnd) {
        DestroyWindow(state->hwnd);
        state->hwnd = 0;
    }

    free(state);
}

b8 platform_pump_messages(platform_state* plat_state) {
    MSG message;
    
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return TRUE;
}

void* platform_allocate(u64 size, b8 aligned) {
    return malloc(size);
}

void platform_free(void* block, b8 aligned) {
    free(block);
}

void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}

void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}

void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void platform_console_write(const char* message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
}

void platform_console_write_error(const char* message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
}

f64 platform_get_absolute_time() {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);

    return (f64)now_time.QuadPart * clock_frecuency;
}

void platform_sleep(u64 ms) {
    Sleep(ms);
}

void platform_get_required_extension_names(const char*** names_darray) {
    darray_push(*names_darray, &"VK_KHR_win32_surface");
}

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_ERASEBKGND:
            // notifica al so que el fondo lo va a borrar la aplicación
            return 1;
        case WM_CLOSE:
            // TODO: Mandar una notificación a la aplicación para que se cierre
            return 0; 
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // obtenemos el tamaño nuevo de la ventana
            // RECT rect;
            // GetClientRect(hwnd, &rect);
            // u32 width = rect.right - rect.left;
            // u32 height = rect.bottom - rect.top;

            // TODO: Mandar una notificación a la aplicación para que sepa que la ventana cambio de tamaño
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // obtener si se presionó o se soltó la tecla
            b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            
            keys key = (u16)w_param;
            input_process_key(key, pressed);
        } break;
        case WM_MOUSEMOVE: {
            // Posicion del mouse
            i32 x_position = GET_X_LPARAM(l_param);
            i32 y_position = GET_Y_LPARAM(l_param);
            
            input_process_mouse_move(x_position, y_position);
        } break;
        case WM_MOUSEWHEEL: {
            i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            if (z_delta != 0) {
                // simplificar el valor a (-1, 1)
                z_delta = (z_delta > 0) ? 1 : -1;
                
                input_process_mouse_wheel((i8)z_delta);
            }
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            b8 pressed = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN);
            
            buttons mouse_buttons = BUTTON_MAX_BUTTONS;
            switch (msg){
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP: {
                    mouse_buttons = BUTTON_LEFT;
                } break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP: {
                    mouse_buttons = BUTTON_MIDDLE;
                } break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP: {
                    mouse_buttons = BUTTON_RIGHT;
                } break;
            }

            if (mouse_buttons != BUTTON_MAX_BUTTONS) {
                input_process_button(mouse_buttons, pressed);
            }                
        } break;
    }

    return DefWindowProcA(hwnd, msg, w_param, l_param);
}

#endif // KPLATFORM_WINDOWS