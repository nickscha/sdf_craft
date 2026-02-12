/* win32_sdf_craft.c - v0.1 - public domain data structures - nickscha 2026

A C89 standard compliant, nostdlib (no C Standard Library) Raymarched SDF Modeler entirely running on the CPU. (SDF_CRAFT).

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/

/* #############################################################################
 * # [SECTION] TYPES & COMPILER SETTINGS
 * #############################################################################
 */
#if __STDC_VERSION__ >= 199901L
#define SDF_CRAFT_INLINE inline
typedef long long i64;
typedef unsigned long long u64;
#elif defined(__GNUC__) || defined(__clang__)
#define SDF_CRAFT_INLINE __inline__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
typedef long long i64;
typedef unsigned long long u64;
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#define SDF_CRAFT_INLINE __inline
typedef __int64 i64;
typedef unsigned __int64 u64;
#else
#define SDF_CRAFT_INLINE
typedef long i64;
typedef unsigned long u64;
#endif

#define SDF_CRAFT_API static

typedef char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef short i16;
typedef int i32;
typedef float f32;
typedef double f64;

#define TYPES_STATIC_ASSERT(c, m) typedef char types_assert_##m[(c) ? 1 : -1]
TYPES_STATIC_ASSERT(sizeof(s8) == 1, s8_size_must_be_1);
TYPES_STATIC_ASSERT(sizeof(u8) == 1, u8_size_must_be_1);
TYPES_STATIC_ASSERT(sizeof(u16) == 2, u16_size_must_be_2);
TYPES_STATIC_ASSERT(sizeof(i16) == 2, i16_size_must_be_2);
TYPES_STATIC_ASSERT(sizeof(u32) == 4, u32_size_must_be_4);
TYPES_STATIC_ASSERT(sizeof(i32) == 4, i32_size_must_be_4);
TYPES_STATIC_ASSERT(sizeof(f32) == 4, f32_size_must_be_4);
TYPES_STATIC_ASSERT(sizeof(f64) == 8, f64_size_must_be_8);
TYPES_STATIC_ASSERT(sizeof(u64) == 8, u64_size_must_be_8);
TYPES_STATIC_ASSERT(sizeof(i64) == 8, i64_size_must_be_8);

/* Unfortunaly "modern" compilers sometimes inject memset intrinsics in the generated code
 * even if the application does not call memset and even with -fno-builtin, ... set.
 * Therefore we have to provide our own memset function.
 */
#ifdef _MSC_VER
#pragma function(memset)
#endif
void *memset(void *dest, i32 c, u32 count)
{
  s8 *bytes = (s8 *)dest;
  while (count--)
  {
    *bytes++ = (s8)c;
  }
  return dest;
}

/* #############################################################################
 * # [SECTION] win32 "windows.h" subsitution for fast builds
 * #############################################################################
 */
#define WIN32_API(r) __declspec(dllimport) r __stdcall

#define GENERIC_WRITE (0x40000000L)
#define CREATE_ALWAYS 2
#define FILE_SHARE_READ 0x00000001
#define FILE_ATTRIBUTE_NORMAL 0x00000080

#define MEM_COMMIT 0x00001000
#define MEM_RESERVE 0x00002000
#define MEM_RELEASE 0x00008000
#define PAGE_READWRITE 0x04

#define WM_ERASEBKGND 0x0014
#define WM_CREATE 0x0001
#define WM_CLOSE 0x0010
#define WM_QUIT 0x0012
#define WM_SIZE 0x0005
#define WM_INPUT 0x00FF
#define WM_DEVICECHANGE 0x0219

#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205

#define DBT_DEVICEARRIVAL 0x8000
#define DBT_DEVICEREMOVECOMPLETE 0x8004
#define DBT_DEVNODES_CHANGED 0x0007

#define SIZE_MINIMIZED 1

#define CS_VREDRAW 0x0001
#define CS_HREDRAW 0x0002

#define HWND_TOPMOST ((void *)-1)

#define WS_CLIPSIBLINGS 0x04000000
#define WS_CLIPCHILDREN 0x02000000
#define WS_THICKFRAME 0x00040000L
#define WS_MINIMIZEBOX 0x00020000L
#define WS_MAXIMIZEBOX 0x00010000L
#define WS_CAPTION 0x00C00000L
#define WS_SYSMENU 0x00080000L

#define SWP_NOSIZE 0x0001
#define SWP_NOMOVE 0x0002
#define SW_SHOW 5

#define SRCCOPY 0x00CC0020

#define GWLP_USERDATA -21

#define PM_REMOVE 0x0001

#define MAKEINTRESOURCEA(i) ((s8 *)((u32)((u16)(i))))
#define IDC_ARROW MAKEINTRESOURCEA(32512)

#define RIDEV_INPUTSINK 0x00000100
#define RID_INPUT 0x10000003
#define RIM_TYPEMOUSE 0
#define RIM_TYPEKEYBOARD 1
#define RI_KEY_BREAK 1
#define RI_MOUSE_WHEEL 0x0400

#define WHEEL_DELTA 120

#define HIGH_PRIORITY_CLASS 0x80
#define THREAD_PRIORITY_HIGHEST 2
#define ES_SYSTEM_REQUIRED ((u32)0x00000001)
#define ES_DISPLAY_REQUIRED ((u32)0x00000002)
#define ES_CONTINUOUS ((u32)0x80000000)

typedef void *(*PROC)(void);
typedef i64 (*WNDPROC)(void *, u32, u64, i64);

typedef struct CREATESTRUCTA
{
  void *lpCreateParams;
  void *hInstance;
  void *hMenu;
  void *hwndParent;
  i32 cy;
  i32 cx;
  i32 y;
  i32 x;
  i32 style;
  s8 *lpszName;
  s8 *lpszClass;
  u32 dwExStyle;
} CREATESTRUCTA;

typedef struct WNDCLASSA
{
  u32 style;
  WNDPROC lpfnWndProc;
  i32 cbClsExtra;
  i32 cbWndExtra;
  void *hInstance;
  void *hIcon;
  void *hCursor;
  void *hbrBackground;
  s8 *lpszMenuName;
  s8 *lpszClassName;
} WNDCLASSA;

typedef struct POINT
{
  i32 x;
  i32 y;
} POINT;

typedef struct RECT
{
  i32 left;
  i32 top;
  i32 right;
  i32 bottom;
} RECT;

typedef struct MSG
{
  void *hwnd;
  u32 message;
  u64 wParam;
  i64 lParam;
  u32 time;
  POINT pt;
  u32 lPrivate;
} MSG;

typedef struct PIXELFORMATDESCRIPTOR
{
  u16 nSize;
  u16 nVersion;
  u32 dwFlags;
  u8 iPixelType;
  u8 cColorBits;
  u8 cRedBits;
  u8 cRedShift;
  u8 cGreenBits;
  u8 cGreenShift;
  u8 cBlueBits;
  u8 cBlueShift;
  u8 cAlphaBits;
  u8 cAlphaShift;
  u8 cAccumBits;
  u8 cAccumRedBits;
  u8 cAccumGreenBits;
  u8 cAccumBlueBits;
  u8 cAccumAlphaBits;
  u8 cDepthBits;
  u8 cStencilBits;
  u8 cAuxBuffers;
  u8 iLayerType;
  u8 bReserved;
  u32 dwLayerMask;
  u32 dwVisibleMask;
  u32 dwDamageMask;
} PIXELFORMATDESCRIPTOR;

typedef struct FILETIME
{
  u32 dwLowDateTime;
  u32 dwHighDateTime;
} FILETIME;

typedef struct WIN32_FILE_ATTRIBUTE_DATA
{
  u32 dwFileAttributes;
  FILETIME ftCreationTime;
  FILETIME ftLastAccessTime;
  FILETIME ftLastWriteTime;
  u32 nFileSizeHigh;
  u32 nFileSizeLow;
} WIN32_FILE_ATTRIBUTE_DATA;

typedef struct RAWINPUTDEVICE
{
  u16 usUsagePage;
  u16 usUsage;
  u32 dwFlags;
  void *hwndTarget;
} RAWINPUTDEVICE;

typedef struct RAWINPUTHEADER
{
  u32 dwType;
  u32 dwSize;
  void *hDevice;
  u64 wParam;
} RAWINPUTHEADER;

typedef struct RAWKEYBOARD
{
  u16 MakeCode;
  u16 Flags;
  u16 Reserved;
  u16 VKey;
  u32 Message;
  u32 ExtraInformation;
} RAWKEYBOARD;

typedef struct RAWMOUSE
{
  u16 usFlags;
  u16 usButtonFlags;
  u16 usButtonData;
  u32 ulRawButtons;
  i32 lLastX;
  i32 lLastY;
  u32 ulExtraInformation;
} RAWMOUSE;

typedef struct RAWHID
{
  u32 dwSizeHid;
  u32 dwCount;
  u8 bRawData[1];
} RAWHID;

typedef struct RAWINPUT
{
  RAWINPUTHEADER header;
  union
  {
    RAWMOUSE mouse;
    RAWKEYBOARD keyboard;
    RAWHID hid;
  } data;
} RAWINPUT;

typedef struct PROCESS_MEMORY_COUNTERS_EX
{
  u32 cb;
  u32 PageFaultCount;
  u64 PeakWorkingSetSize;
  u64 WorkingSetSize;
  u64 QuotaPeakPagedPoolUsage;
  u64 QuotaPagedPoolUsage;
  u64 QuotaPeakNonPagedPoolUsage;
  u64 QuotaNonPagedPoolUsage;
  u64 PagefileUsage;
  u64 PeakPagefileUsage;
  u64 PrivateUsage;
} PROCESS_MEMORY_COUNTERS_EX;

typedef struct THREADENTRY32
{
  u32 dwSize;
  u32 cntUsage;
  u32 th32ThreadID;
  u32 th32OwnerProcessID;
  i32 tpBasePri;
  i32 tpDeltaPri;
  u32 dwFlags;
} THREADENTRY32;

typedef struct WINDOWPLACEMENT
{
  u32 length;
  u32 flags;
  u32 showCmd;
  POINT ptMinPosition;
  POINT ptMaxPosition;
  RECT rcNormalPosition;
  RECT rcDevice;
} WINDOWPLACEMENT;

typedef struct MONITORINFO
{
  u32 cbSize;
  RECT rcMonitor;
  RECT rcWork;
  u32 dwFlags;
} MONITORINFO;

typedef struct BITMAPINFOHEADER
{
  u32 biSize;
  i32 biWidth;
  i32 biHeight;
  u16 biPlanes;
  u16 biBitCount;
  u32 biCompression;
  u32 biSizeImage;
  i32 biXPelsPerMeter;
  i32 biYPelsPerMeter;
  u32 biClrUsed;
  u32 biClrImportant;
} BITMAPINFOHEADER;

typedef struct RGBQUAD
{
  u8 rgbBlue;
  u8 rgbGreen;
  u8 rgbRed;
  u8 rgbReserved;
} RGBQUAD;

typedef struct BITMAPINFO
{
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[1];
} BITMAPINFO;

/* clang-format off */
WIN32_API(void *) GetStdHandle(u32 nStdHandle);
WIN32_API(i32)    CloseHandle(void *hObject);
WIN32_API(void *) LoadLibraryA(s8 *lpLibFileName);
WIN32_API(i32)    FreeLibrary(void *hLibModule);
WIN32_API(void *) GetProcAddress(void *hModule, char *lpProcName);
WIN32_API(i32)    SetProcessDPIAware(void);
WIN32_API(void *) VirtualAlloc(void *lpAddress, u32 dwSize, u32 flAllocationType, u32 flProtect);
WIN32_API(i32)    VirtualFree(void *lpAddress, u32 dwSize, u32 dwFreeType);
WIN32_API(void *) CreateFileA(s8 *lpFileName, u32 dwDesiredAccess, u32 dwShareMode, void *, u32 dwCreationDisposition, u32 dwFlagsAndAttributes, void *hTemplateFile);
WIN32_API(u32)    GetFileSize(void *hFile, u32 *lpFileSizeHigh);
WIN32_API(i32)    ReadFile(void *hFile, void *lpBuffer, u32 nNumberOfBytesToRead, u32 *lpNumberOfBytesRead, void *lpOverlapped);
WIN32_API(i32)    WriteFile(void *hFile, void *lpBuffer, u32 nNumberOfBytesToWrite, u32 *lpNumberOfBytesWritten, void *lpOverlapped);
WIN32_API(i32)    CompareFileTime(FILETIME *lpFileTime1, FILETIME *lpFileTime2);
WIN32_API(i32)    GetFileAttributesExA(s8 *lpFileName, u32 fInfoLevelId, void *lpFileInformation);
WIN32_API(void)   Sleep(u32 dwMilliseconds);
WIN32_API(void)   ExitProcess(u32 uExitCode);
WIN32_API(i32)    PeekMessageA(MSG* lpMsg, void *hWnd, u32 wMsgFilterMin, u32 wMsgFilterMax, u32 wRemoveMsg);
WIN32_API(i32)    GetMessageA(MSG* lpMsg, void *hWnd, u32 wMsgFilterMin, u32 wMsgFilterMax);
WIN32_API(i64)    DispatchMessageA(MSG *lpMsg);
WIN32_API(i64)    DefWindowProcA(void *hWnd, u32 Msg, u64 wParam, i64 lParam);
WIN32_API(i64)    SetWindowLongPtrA(void *hWnd, i32 nIndex, i64 dwNewLong);
WIN32_API(i64)    GetWindowLongPtrA(void *hWnd, i32 nIndex);
WIN32_API(void *) GetModuleHandleA(s8 *lpModuleName);
WIN32_API(void *) LoadCursorA(void *hInstance, s8 *lpCursorName);
WIN32_API(void *) LoadIconA(void *hInstance, s8 *lpIconName);
WIN32_API(u16)    RegisterClassA(WNDCLASSA *lpWndClass);
WIN32_API(void *) CreateWindowExA(u32 dwExStyle, s8 *lpClassName, s8 *lpWindowName, u32 dwStyle, i32 X, i32 Y, i32 nWidth, i32 nHeight, void *hWndParent, void *hMenu, void *hInstance, void *lpParam);
WIN32_API(void *) GetDC(void *hWnd);
WIN32_API(i32)    ReleaseDC(void *hWnd, void *hDC);
WIN32_API(i32)    SwapBuffers(void *unnamedParam1);
WIN32_API(i32)    ChoosePixelFormat(void *hdc, PIXELFORMATDESCRIPTOR *ppfd);
WIN32_API(i32)    SetPixelFormat(void *hdc, i32 format, PIXELFORMATDESCRIPTOR *ppfd);
WIN32_API(i32)    DescribePixelFormat(void *hdc, i32 iPixelFormat, u32 nBytes, PIXELFORMATDESCRIPTOR* ppfd);
WIN32_API(i32)    ShowWindow(void *hWnd, i32 nCmdShow);
WIN32_API(i32)    DestroyWindow(void *hWnd);
WIN32_API(i32)    AdjustWindowRect(RECT* lpRect, u32 dwStyle, i32 bMenu);
WIN32_API(i32)    QueryPerformanceCounter(i64 *lpPerformanceCount);
WIN32_API(i32)    QueryPerformanceFrequency(i64 *lpFrequency);
WIN32_API(s8 *)   GetCommandLineA(void);
WIN32_API(i32)    RegisterRawInputDevices(RAWINPUTDEVICE* pRawInputDevices, u32 uiNumDevices, u32 cbSize);
WIN32_API(u32)    GetRawInputData(void *hRawInput, u32 uiCommand, void *pData, u32 *pcbSize, u32 cbSizeHeader);
WIN32_API(i32)    GetCursorPos(POINT *lpPoint);
WIN32_API(i32)    ScreenToClient(void *hWnd, POINT *lpPoint);
WIN32_API(i32)    StretchDIBits(
 void* hdc,
 i32 xDest,
 i32 yDest,
 i32 DestWidth,
 i32 DestHeight,
 i32 xSrc,
 i32 ySrc,
 i32 SrcWidth,
 i32 SrcHeight,
 void *lpBits,
 BITMAPINFO *lpbmi,
 u32 iUsage,
 u32 rop
);

WIN32_API(i32)    GetWindowLongA(void *hWnd, i32 nIndex);
WIN32_API(i32)    GetWindowPlacement(void *hWnd, WINDOWPLACEMENT *lpwndpl);
WIN32_API(i32)    GetMonitorInfoA(void *hMonitor, MONITORINFO* lpmi);
WIN32_API(void *) MonitorFromWindow(void *hwnd, u32 dwFlags);
WIN32_API(i32)    SetWindowLongA(void *hWnd, i32 nIndex, i32 dwNewLong);
WIN32_API(i32)    SetWindowPos(void *hWnd, void *hWndInsertAfter, i32 X, i32 Y, i32 cx, i32 cy, u32 uFlags);
WIN32_API(i32)    SetWindowPlacement(void *hWnd, WINDOWPLACEMENT *lpwndpl);
WIN32_API(i32)    GetClientRect(void *hWnd, RECT* lpRect);

WIN32_API(void *) GetCurrentProcess(void);
WIN32_API(u32)    GetCurrentProcessId(void);
WIN32_API(i32)    SetPriorityClass(void *hProcess, u32 dwPriorityClass);
WIN32_API(void *) GetCurrentThread(void);
WIN32_API(i32)    SetThreadPriority(void *hThread, i32 nPriority);
WIN32_API(u32)    SetThreadExecutionState(u32 esFlags);
WIN32_API(i32)    GetProcessHandleCount(void* hProcess, u32* pdwHandleCount);
WIN32_API(void *) CreateToolhelp32Snapshot(u32 dwFlags, u32 th32ProcessID);
WIN32_API(i32)    Thread32First(void* hSnapshot, THREADENTRY32* lpte);
WIN32_API(i32)    Thread32Next(void* hSnapshot, THREADENTRY32* lpte);
/* clang-format on */

/* #############################################################################
 * # [SECTION] WIN32 specifiy functions
 * #############################################################################
 */
SDF_CRAFT_API void win32_print(s8 *str)
{
  static u32 written;
  static void *log_file;

  if (!log_file)
  {
    log_file = CreateFileA("sdf_craft.log", GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  }

  {
    s8 *p = str;
    u32 len = 0;

    while (*p++)
    {
      len++;
    }

    WriteFile(log_file, str, len, &written, 0);
  }
}

SDF_CRAFT_API SDF_CRAFT_INLINE u8 win32_enable_high_priority(void)
{
  if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
  {
    return 0;
  }

  if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST))
  {
    return 0;
  }

  if (!SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED))
  {
    return 0;
  }

  return 1;
}

SDF_CRAFT_API SDF_CRAFT_INLINE u8 win32_enable_dpi_awareness(void)
{
  /* Try Windows 10 / 11 (Per-Monitor V2) */
  void *user32 = GetModuleHandleA("user32.dll");

  void *shcore;

  if (user32)
  {
    typedef i32(__stdcall * SetProcessDpiAwarenessContextProc)(void *);
    SetProcessDpiAwarenessContextProc setProcessDpiAwarenessContext;

    *(void **)(&setProcessDpiAwarenessContext) = GetProcAddress(user32, "SetProcessDpiAwarenessContext");

    if (setProcessDpiAwarenessContext && setProcessDpiAwarenessContext((void *)-4)) /* DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2  */
    {
      return 1;
    }
  }

  shcore = LoadLibraryA("Shcore.dll");

  if (shcore)
  {
    typedef i32(__stdcall * SetProcessDpiAwarenessProc)(i32);
    SetProcessDpiAwarenessProc setDpiAwareness;

    *(void **)(&setDpiAwareness) = GetProcAddress(shcore, "SetProcessDpiAwareness");

    if (setDpiAwareness)
    {
      setDpiAwareness(2); /* PROCESS_PER_MONITOR_DPI_AWARE */
    }

    FreeLibrary(shcore);
  }
  else
  {
    SetProcessDPIAware();
  }

  return 1;
}

SDF_CRAFT_API SDF_CRAFT_INLINE u8 win32_enable_high_resolution_timer(void)
{
  void *winmm = LoadLibraryA("Winmm.dll");

  if (winmm)
  {
    typedef u32(__stdcall * timeBeginPeriodProc)(u32);
    timeBeginPeriodProc timeBeginPeriod;

    u32 res = 0;

    *(void **)(&timeBeginPeriod) = GetProcAddress(winmm, "timeBeginPeriod");

    if (!timeBeginPeriod)
    {
      return 0;
    }

    res = timeBeginPeriod(1);

    FreeLibrary(winmm);

    /* TIMERR_NOCANDO */
    if (res == 97)
    {
      return 0;
    }
  }

  return 1;
}

typedef struct process_memory_info
{
  u64 private_bytes; /* Commit charge (what you asked for) */
  u64 working_set;   /* RAM currently used */
  u64 peak_working_set;
} process_memory_info;

u8 win32_process_memory(process_memory_info *out)
{
  typedef i32(__stdcall * GetProcessMemoryInfo_Fn)(void *, PROCESS_MEMORY_COUNTERS_EX *, u32);
  static GetProcessMemoryInfo_Fn pGetMemInfo = 0;
  static i32 initialized = 0;

  PROCESS_MEMORY_COUNTERS_EX pmc;

  if (!initialized)
  {
    /* Win7+ */
    void *kernel32 = LoadLibraryA("kernel32.dll");

    if (kernel32)
    {
      *(void **)(&pGetMemInfo) = GetProcAddress(kernel32, "K32GetProcessMemoryInfo");
    }

    /* Vista and older */
    if (!pGetMemInfo)
    {
      void *psapi = LoadLibraryA("psapi.dll");

      if (psapi)
      {
        *(void **)(&pGetMemInfo) = GetProcAddress(psapi, "GetProcessMemoryInfo");
      }
    }

    initialized = 1;
  }

  if (!pGetMemInfo)
  {
    return 0;
  }

  pmc.cb = sizeof(pmc);

  if (!pGetMemInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
  {
    return 0;
  }

  out->private_bytes = (u64)pmc.PrivateUsage;
  out->working_set = (u64)pmc.WorkingSetSize;
  out->peak_working_set = (u64)pmc.PeakWorkingSetSize;

  return 1;
}

#define KEYS_COUNT 256

typedef struct win32_sdf_craft_state
{

  u32 window_width;
  u32 window_height;

  u32 window_width_pending;
  u32 window_height_pending;

  f32 window_clear_color_g;
  f32 window_clear_color_b;
  f32 window_clear_color_r;
  f32 window_clear_color_a;

  i32 iFrame;        /* Frames processed count               */
  f64 iTime;         /* Total elapsed time in seconds        */
  f64 iTimeDelta;    /* Current render frame time in seconds */
  f64 iFrameRate;    /* Frame Rate per second                */
  f64 iFrameRateRaw; /* Frame Rate per second raw (no cap)   */

  u8 running;
  u8 window_minimized;
  u8 window_size_changed;

  u32 target_frames_per_second;

  s8 *window_title;

  void *window_handle;
  void *device_context;

  /* Input state */
  i32 mouse_dx; /* Relative movement delta for x  */
  i32 mouse_dy; /* Relative movement delta for y  */
  i32 mouse_x;  /* Mouse position on screen for x */
  i32 mouse_y;  /* Mouse position on screen for y */
  f32 mouse_scroll;
  u8 mouse_left_is_down;
  u8 mouse_left_was_down;
  u8 mouse_right_is_down;
  u8 mouse_right_was_down;

  /* State Examples:
    Key Pressed:  state.keys_is_down[0x0D] && !state.keys_was_down[0x0D]
    Key Released: !state.keys_is_down[0x0D] && state.keys_was_down[0x0D]

    Example of a Toggle switch (when pressed first toggles on, when pressed second time toggles off):

    static u8 ui_enabled = 0;

    if (state.keys_is_down[0x70] && !state.keys_was_down[0x70])
    {
      ui_enabled = !ui_enabled;
    }
  */
  u8 keys_is_down[KEYS_COUNT];
  u8 keys_was_down[KEYS_COUNT];

  void *framebuffer;
  u32 framebuffer_width;
  u32 framebuffer_height;
  BITMAPINFO framebuffer_info;

} win32_sdf_craft_state;

SDF_CRAFT_API SDF_CRAFT_INLINE i64 win32_window_callback(void *window, u32 message, u64 wParam, i64 lParam)
{
  win32_sdf_craft_state *state = (win32_sdf_craft_state *)GetWindowLongPtrA(window, GWLP_USERDATA);

  i64 result = 0;

  switch (message)
  {
  case WM_ERASEBKGND:
    return 1;
  case WM_CREATE:
  {
    CREATESTRUCTA *cs = (CREATESTRUCTA *)lParam;
    state = (win32_sdf_craft_state *)cs->lpCreateParams;
    SetWindowLongPtrA(window, GWLP_USERDATA, (i64)state);

    /* Setup raw input for mouse and keyboard */
    {
      RAWINPUTDEVICE rid[2] = {0};

      (void)RIDEV_INPUTSINK; /* Receive input even when not focused */

      rid[0].usUsagePage = 0x01;
      rid[0].usUsage = 0x06; /* Keyboard */
      rid[0].dwFlags = 0;    /* Receive input only when focused */
      rid[0].hwndTarget = window;

      rid[1].usUsagePage = 0x01;
      rid[1].usUsage = 0x02; /* Mouse */
      rid[1].dwFlags = 0;    /* Receive input only when focused */
      rid[1].hwndTarget = window;

      if (!RegisterRawInputDevices(rid, 2, sizeof(rid[0])))
      {
        win32_print("[win32] Failed to register RAWINPUT device\n");
      }
    }
  }
  break;
  case WM_CLOSE:
  case WM_QUIT:
  {
    if (!state)
    {
      break;
    }

    state->running = 0;
  }
  break;
  case WM_SIZE:
  {
    if (!state)
    {
      break;
    }

    if (wParam == SIZE_MINIMIZED)
    {
      state->window_minimized = 1;
    }
    else
    {
      state->window_minimized = 0;
      state->window_size_changed = 1;
      state->window_width_pending = (u16)(((u64)(lParam)) & 0xffff);          /* Low Word  */
      state->window_height_pending = (u16)((((u64)(lParam)) >> 16) & 0xffff); /* High Word */
    }
  }
  break;
  case WM_INPUT:
  {
    static u8 rawBuffer[128];
    RAWINPUT *raw = (RAWINPUT *)rawBuffer;

    u32 dwSize = 0;

    if (!state)
    {
      break;
    }

    GetRawInputData((RAWINPUT *)lParam, RID_INPUT, (void *)0, &dwSize, sizeof(RAWINPUTHEADER));

    if (dwSize > sizeof(rawBuffer) ||
        GetRawInputData((RAWINPUT *)lParam, RID_INPUT, raw, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
    {
      return result;
    }

    if (raw->header.dwType == RIM_TYPEKEYBOARD)
    {
      RAWKEYBOARD *keyboard = &raw->data.keyboard;

      u16 vKey = keyboard->VKey;

      if (vKey < KEYS_COUNT)
      {
        /*key->was_down = key->is_down;*/
        state->keys_is_down[vKey] = !(keyboard->Flags & RI_KEY_BREAK); /* 1 if pressed, 0 if released */
      }
    }
    else if (raw->header.dwType == RIM_TYPEMOUSE)
    {
      RAWMOUSE *mouse = &raw->data.mouse;

      i32 dx = mouse->lLastX;
      i32 dy = mouse->lLastY;

      state->mouse_dx += dx;
      state->mouse_dy -= dy;

      /* Scroll wheel */
      if (mouse->usButtonFlags & RI_MOUSE_WHEEL)
      {
        i16 wheelDelta = (i16)mouse->usButtonData;
        state->mouse_scroll += (f32)wheelDelta / (f32)WHEEL_DELTA;
      }
    }
  }
  break;
  case WM_DEVICECHANGE:
  {
    /* Check if a device notification arrived in order to find out when to requery XInput controller states */
    /* DBT_DEVNODES_CHANGED is the most reliable for USB plugging/unplugging */
    if (wParam == DBT_DEVNODES_CHANGED || wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVICEREMOVECOMPLETE)
    {
      /* TODO(nickscha): XInput handling */
    }
  }
  break;
  case WM_LBUTTONDOWN:
    state->mouse_left_is_down = 1;
    break;
  case WM_LBUTTONUP:
    state->mouse_left_is_down = 0;
    break;
  case WM_RBUTTONDOWN:
    state->mouse_right_is_down = 1;
    break;
  case WM_RBUTTONUP:
    state->mouse_right_is_down = 0;
    break;
  default:
  {
    result = DefWindowProcA(window, message, wParam, lParam);
  }
  break;
  }

  return (result);
}

SDF_CRAFT_API SDF_CRAFT_INLINE i32 win32_create_window(win32_sdf_craft_state *state)
{
  void *window_instance = GetModuleHandleA(0);
  WNDCLASSA window_class = {0};
  u32 window_style = WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
  RECT rect = {0};

  window_class.style = CS_HREDRAW | CS_VREDRAW;
  window_class.lpfnWndProc = win32_window_callback;
  window_class.hInstance = window_instance;
  window_class.hCursor = LoadCursorA(0, IDC_ARROW);
  window_class.hIcon = LoadIconA(window_instance, MAKEINTRESOURCEA(1));
  window_class.hbrBackground = 0;
  window_class.lpszClassName = state->window_title;

  if (!RegisterClassA(&window_class))
  {
    return 0;
  }

  rect.right = (i32)state->window_width;
  rect.bottom = (i32)state->window_height;
  AdjustWindowRect(&rect, window_style, 0);

  state->window_handle = CreateWindowExA(
      0,
      window_class.lpszClassName,
      window_class.lpszClassName,
      window_style,
      0, 0,
      rect.right - rect.left,
      rect.bottom - rect.top,
      0, 0,
      window_instance,
      state /* Pass pointer to user data to the window callback */
  );

  if (!state->window_handle)
  {
    return 0;
  }

  /* Modal window */
  SetWindowPos(state->window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  ShowWindow(state->window_handle, SW_SHOW);

  state->device_context = GetDC(state->window_handle);

  return 1;
}

SDF_CRAFT_API void win32_resize_framebuffer(win32_sdf_craft_state *s, u32 w, u32 h)
{
  u32 bytes = w * h * 4;

  if (s->framebuffer)
  {
    VirtualFree(s->framebuffer, 0, MEM_RELEASE);
  }

  s->framebuffer = VirtualAlloc(0, bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  s->framebuffer_width = w;
  s->framebuffer_height = h;

  memset(&s->framebuffer_info, 0, sizeof(BITMAPINFO));
  s->framebuffer_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  s->framebuffer_info.bmiHeader.biWidth = (i32)w;
  s->framebuffer_info.bmiHeader.biHeight = (i32)h; /* h = down-top, -h = top-down */
  s->framebuffer_info.bmiHeader.biPlanes = 1;
  s->framebuffer_info.bmiHeader.biBitCount = 32;
}

SDF_CRAFT_API void framebuffer_clear(win32_sdf_craft_state *state)
{
  u32 *pixel = (u32 *)state->framebuffer;
  u32 x, y;

  u8 r = (u8)(255.0f * state->window_clear_color_r);
  u8 g = (u8)(255.0f * state->window_clear_color_g);
  u8 b = (u8)(255.0f * state->window_clear_color_b);

  for (y = 0; y < state->framebuffer_height; ++y)
  {
    for (x = 0; x < state->framebuffer_width; ++x)
    {
      *pixel++ = (r << 16) | (g << 8) | b;
    }
  }
}

SDF_CRAFT_API void framebuffer_debug(win32_sdf_craft_state *state)
{
  u32 *pixel = (u32 *)state->framebuffer;
  u32 x, y;

  for (y = 0; y < state->framebuffer_height; ++y)
  {
    for (x = 0; x < state->framebuffer_width; ++x)
    {
      if (y < 8 && x < 8)
      {
        *pixel++ = (0 << 16) | (255 << 8) | 0;
      }
      else if (y > state->framebuffer_height - 8 && x > state->framebuffer_width - 8)
      {
        *pixel++ = (255 << 16) | (0 << 8) | 0;
      }
      else
      {
        pixel++;
      }
    }
  }
}

#define SDF_MATH_PI2 6.28318530717958647692f
#define SDF_MATH_LUT_SIZE 256

static f32 sdf_math_lut[SDF_MATH_LUT_SIZE] = {
    0.0000f, 0.0245f, 0.0491f, 0.0736f, 0.0980f, 0.1224f, 0.1467f, 0.1710f,
    0.1951f, 0.2191f, 0.2430f, 0.2667f, 0.2903f, 0.3137f, 0.3369f, 0.3599f,
    0.3827f, 0.4052f, 0.4276f, 0.4496f, 0.4714f, 0.4929f, 0.5141f, 0.5350f,
    0.5556f, 0.5758f, 0.5957f, 0.6152f, 0.6344f, 0.6532f, 0.6716f, 0.6895f,
    0.7071f, 0.7242f, 0.7409f, 0.7572f, 0.7730f, 0.7883f, 0.8032f, 0.8176f,
    0.8315f, 0.8449f, 0.8577f, 0.8701f, 0.8819f, 0.8932f, 0.9040f, 0.9142f,
    0.9239f, 0.9330f, 0.9415f, 0.9495f, 0.9569f, 0.9638f, 0.9700f, 0.9757f,
    0.9808f, 0.9853f, 0.9892f, 0.9925f, 0.9952f, 0.9973f, 0.9988f, 0.9997f,
    1.0000f, 0.9997f, 0.9988f, 0.9973f, 0.9952f, 0.9925f, 0.9892f, 0.9853f,
    0.9808f, 0.9757f, 0.9700f, 0.9638f, 0.9569f, 0.9495f, 0.9415f, 0.9330f,
    0.9239f, 0.9142f, 0.9040f, 0.8932f, 0.8819f, 0.8701f, 0.8577f, 0.8449f,
    0.8315f, 0.8176f, 0.8032f, 0.7883f, 0.7730f, 0.7572f, 0.7409f, 0.7242f,
    0.7071f, 0.6895f, 0.6716f, 0.6532f, 0.6344f, 0.6152f, 0.5957f, 0.5758f,
    0.5556f, 0.5350f, 0.5141f, 0.4929f, 0.4714f, 0.4496f, 0.4276f, 0.4052f,
    0.3827f, 0.3599f, 0.3369f, 0.3137f, 0.2903f, 0.2667f, 0.2430f, 0.2191f,
    0.1951f, 0.1710f, 0.1467f, 0.1224f, 0.0980f, 0.0736f, 0.0491f, 0.0245f,
    0.0000f, -0.0245f, -0.0491f, -0.0736f, -0.0980f, -0.1224f, -0.1467f, -0.1710f,
    -0.1951f, -0.2191f, -0.2430f, -0.2667f, -0.2903f, -0.3137f, -0.3369f, -0.3599f,
    -0.3827f, -0.4052f, -0.4276f, -0.4496f, -0.4714f, -0.4929f, -0.5141f, -0.5350f,
    -0.5556f, -0.5758f, -0.5957f, -0.6152f, -0.6344f, -0.6532f, -0.6716f, -0.6895f,
    -0.7071f, -0.7242f, -0.7409f, -0.7572f, -0.7730f, -0.7883f, -0.8032f, -0.8176f,
    -0.8315f, -0.8449f, -0.8577f, -0.8701f, -0.8819f, -0.8932f, -0.9040f, -0.9142f,
    -0.9239f, -0.9330f, -0.9415f, -0.9495f, -0.9569f, -0.9638f, -0.9700f, -0.9757f,
    -0.9808f, -0.9853f, -0.9892f, -0.9925f, -0.9952f, -0.9973f, -0.9988f, -0.9997f,
    -1.0000f, -0.9997f, -0.9988f, -0.9973f, -0.9952f, -0.9925f, -0.9892f, -0.9853f,
    -0.9808f, -0.9757f, -0.9700f, -0.9638f, -0.9569f, -0.9495f, -0.9415f, -0.9330f,
    -0.9239f, -0.9142f, -0.9040f, -0.8932f, -0.8819f, -0.8701f, -0.8577f, -0.8449f,
    -0.8315f, -0.8176f, -0.8032f, -0.7883f, -0.7730f, -0.7572f, -0.7409f, -0.7242f,
    -0.7071f, -0.6895f, -0.6716f, -0.6532f, -0.6344f, -0.6152f, -0.5957f, -0.5758f,
    -0.5556f, -0.5350f, -0.5141f, -0.4929f, -0.4714f, -0.4496f, -0.4276f, -0.4052f,
    -0.3827f, -0.3599f, -0.3369f, -0.3137f, -0.2903f, -0.2667f, -0.2430f, -0.2191f,
    -0.1951f, -0.1710f, -0.1467f, -0.1224f, -0.0980f, -0.0736f, -0.0491f, -0.0245f};

SDF_CRAFT_API SDF_CRAFT_INLINE f32 sdf_math_sinf(f32 x)
{
  f32 index, frac;
  i32 i, i2;

  x -= SDF_MATH_PI2 * (f32)((i32)(x * (1.0f / SDF_MATH_PI2)));

  if (x < 0)
  {
    x += SDF_MATH_PI2;
  }

  index = x * (SDF_MATH_LUT_SIZE / SDF_MATH_PI2);
  i = (i32)index;
  frac = index - (f32)i;

  i &= (SDF_MATH_LUT_SIZE - 1);
  i2 = (i + 1) & (SDF_MATH_LUT_SIZE - 1);

  return (sdf_math_lut[i] + frac * (sdf_math_lut[i2] - sdf_math_lut[i]));
}

SDF_CRAFT_API SDF_CRAFT_INLINE u8 float_to_u8(f32 v)
{
  if (v <= 0.0f)
  {
    return 0;
  }
  if (v >= 1.0f)
  {
    return 255;
  }
  return (u8)(v * 255.0f + 0.5f);
}

typedef struct vec3
{
  float x;
  float y;
  float z;
} vec3;

/* GLSL mainImage pixel processing
 *
 * void mainImage(out vec4 outColor, in vec2 fragCoord)
 * {
 *   vec2 uv = fragCoord / iResolution.xy;
 *   float t = iTime;
 *   outColor = vec4(uv, 0.5 + 0.5 * sin(t), 1.0);
 * }
 */
SDF_CRAFT_API vec3 sdf_craft_main_image(win32_sdf_craft_state *state, f32 frag_coord_x, f32 frag_coord_y)
{
  vec3 out_color = {0};

  f32 uv_x = frag_coord_x / (f32)state->framebuffer_width;
  f32 uv_y = frag_coord_y / (f32)state->framebuffer_height;

  out_color.x = uv_x;
  out_color.y = uv_y;
  out_color.z = 0.5f + 0.5f * sdf_math_sinf((f32)state->iTime);

  return out_color;
}

/* GLSL main fragment shader entry point
 *
 * void main()
 * {
 *   vec2 fragCoord = gl_FragCoord.xy;
 *   mainImage(FragColor, fragCoord);
 * }
 */
SDF_CRAFT_API void sdf_craft_main(win32_sdf_craft_state *state)
{
  u32 *pixel = (u32 *)state->framebuffer;
  u32 x, y;

  for (y = 0; y < state->framebuffer_height; ++y)
  {
    for (x = 0; x < state->framebuffer_width; ++x)
    {
      vec3 color = sdf_craft_main_image(state, (f32)x + 0.5f, (f32)y + 0.5f);

      *pixel++ = (float_to_u8(color.x) << 16) | (float_to_u8(color.y) << 8) | float_to_u8(color.z);
    }
  }
}

/* #############################################################################
 * # [SECTION] Main Entry Point
 * #############################################################################
 */
SDF_CRAFT_API i32 start(i32 argc, u8 **argv)
{
  win32_sdf_craft_state state = {0};
  state.running = 1;
  state.window_title = "sdf_craft v0.1";
  state.window_width = 800;
  state.window_height = 600;
  state.window_clear_color_r = 0.2f;
  state.window_clear_color_g = 0.2f;
  state.window_clear_color_b = 0.2f;
  state.target_frames_per_second = 60; /* 60 FPS, 0 = unlimited */

  (void)argc;
  (void)argv;

  /******************************/
  /* Set Process Priorities     */
  /******************************/
  if (!win32_enable_high_priority())
  {
    win32_print("[WARNING] Failed to set high priority process\n");
  }

  /******************************/
  /* Set DPI aware mode         */
  /******************************/
  if (!win32_enable_dpi_awareness())
  {
    win32_print("[WARNING] Cannot set DPI awareness\n");
  }

  /******************************/
  /* HighRes timer for Sleep(1) */
  /******************************/
  if (!win32_enable_high_resolution_timer())
  {
    win32_print("[WARNING] Cannot set win32 high resolution timer using Winmm.dll (timeBeginPeriod)\n");
  }

  /******************************/
  /* Initialize window          */
  /******************************/
  if (!win32_create_window(&state))
  {
    win32_print("[ERROR] Cannot create window!\n");
    return 1;
  }

  {
    i64 perf_freq;
    i64 time_start;
    i64 time_start_fps_cap;
    i64 time_last;

    QueryPerformanceFrequency(&perf_freq);
    QueryPerformanceCounter(&time_start);
    QueryPerformanceCounter(&time_start_fps_cap);

    time_last = time_start;

    while (state.running)
    {
      i64 time_now;

      /******************************/
      /* Timing                     */
      /******************************/
      {
        QueryPerformanceCounter(&time_now);

        state.iTimeDelta = (f64)(time_now - time_last) / (f64)perf_freq;
        state.iTime = (f64)(time_now - time_start) / (f64)perf_freq;

        time_last = time_now;

        if (state.iTimeDelta > 0.0)
        {
          state.iFrameRate = 1.0 / state.iTimeDelta;
        }
      }

      /******************************/
      /* Idle when window minimized */
      /******************************/
      if (state.window_minimized)
      {
        MSG msg;
        GetMessageA(&msg, 0, 0, 0);
        DispatchMessageA(&msg);
        continue;
      }

      /******************************/
      /* Input Processing           */
      /******************************/
      {
        MSG message = {0};
        u32 i;

        u64 *src = (u64 *)state.keys_is_down;
        u64 *dst = (u64 *)state.keys_was_down;

        /* 256 bytes / 8 bytes (u64) = 32 chunks. */
        for (i = 0; i < 8; ++i)
        {
          *dst++ = *src++;
          *dst++ = *src++;
          *dst++ = *src++;
          *dst++ = *src++;
        }

        state.mouse_left_was_down = state.mouse_left_is_down;
        state.mouse_right_was_down = state.mouse_right_is_down;

        /* Reset accumulated mouse relative speeds every frame before processing new mouse messages */
        state.mouse_dx = 0;
        state.mouse_dy = 0;

        while (PeekMessageA(&message, state.window_handle, 0, 0, PM_REMOVE))
        {
          DispatchMessageA(&message);
        }
      }

      /* Get current frames mouse position */
      {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(state.window_handle, &p);

        state.mouse_x = p.x;
        state.mouse_y = (i32)state.window_height - 1 - p.y;
      }

      /******************************/
      /* Handle Window Size changes */
      /******************************/
      if (state.window_size_changed)
      {
        state.window_width = state.window_width_pending;
        state.window_height = state.window_height_pending;
        win32_resize_framebuffer(&state, state.window_width, state.window_height);
        state.window_size_changed = 0;
      }

      /******************************/
      /* Main Logic                 */
      /******************************/
      framebuffer_clear(&state);
      framebuffer_debug(&state);
      sdf_craft_main(&state);

      StretchDIBits(
          state.device_context,
          0, 0,
          (i32)state.window_width,
          (i32)state.window_height,
          0, 0,
          (i32)state.framebuffer_width,
          (i32)state.framebuffer_height,
          state.framebuffer,
          &state.framebuffer_info,
          0,
          SRCCOPY);

      /* Measure RAW FPS (rendering without cap)*/
      {
        i64 time_render_now;
        QueryPerformanceCounter(&time_render_now);
        state.iFrameRateRaw = 1.0 / ((f64)(time_render_now - time_last) / (f64)perf_freq);
      }

      /******************************/
      /* Frame Rate Limiting        */
      /******************************/
      if (state.target_frames_per_second > 0)
      {
        i64 time_end;

        f64 frame_time;
        f64 remaining;
        f64 target_frame_time = 1.0 / (f64)state.target_frames_per_second;

        QueryPerformanceCounter(&time_end);

        frame_time = (f64)(time_end - time_start_fps_cap) / (f64)perf_freq;
        remaining = target_frame_time - frame_time;

        if (remaining > 0.0)
        {
          /* Sleep most of it (milliseconds) */
          if (remaining > 0.0005)
          {
            u32 sleep_ms = (u32)((remaining - 0.00025) * 1000.0);

            if (sleep_ms > 0)
            {
              Sleep(sleep_ms);
            }
          }

          /* Spin for the rest */
          for (;;)
          {
            QueryPerformanceCounter(&time_end);

            frame_time = (f64)(time_end - time_start_fps_cap) / (f64)perf_freq;

            if (frame_time >= target_frame_time)
            {
              break;
            }
          }
        }

        /* Start timing next frame */
        time_start_fps_cap = time_end;
      }

      state.iFrame++;
    }
  }

  return 0;
}

/* #############################################################################
 * # [SECTION] nostdlib entry point
 * #############################################################################
 */
#ifdef __clang__
#elif __GNUC__
__attribute((externally_visible))
#endif
i32 WinMainCRTStartup(void)
{
  u8 *cmdline = (u8 *)GetCommandLineA();
  u8 *argv[8];
  i32 argc = 0;

  i32 return_code;

  /* Parse command line arguments into argv */
  while (*cmdline)
  {
    /* skip whitespace */
    while (*cmdline == ' ' || *cmdline == '\t')
    {
      cmdline++;
    }

    if (!*cmdline)
    {
      break;
    }

    if (argc < 9)
    {
      argv[argc++] = cmdline;
    }

    /* parse token (basic, no quote handling) */
    while (*cmdline && *cmdline != ' ' && *cmdline != '\t')
    {
      cmdline++;
    }

    if (*cmdline)
    {
      *cmdline++ = '\0';
    }
  }

  argv[argc] = (u8 *)0;

  /* Run the program and exit with return code */
  return_code = start(argc, argv);
  ExitProcess((u32)return_code);
  return return_code;
}

/*
   ------------------------------------------------------------------------------
   This software is available under 2 licenses -- choose whichever you prefer.
   ------------------------------------------------------------------------------
   ALTERNATIVE A - MIT License
   Copyright (c) 2026 nickscha
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   ------------------------------------------------------------------------------
   ALTERNATIVE B - Public Domain (www.unlicense.org)
   This is free and unencumbered software released into the public domain.
   Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
   software, either in source code form or as a compiled binary, for any purpose,
   commercial or non-commercial, and by any means.
   In jurisdictions that recognize copyright laws, the author or authors of this
   software dedicate any and all copyright interest in the software to the public
   domain. We make this dedication for the benefit of the public at large and to
   the detriment of our heirs and successors. We intend this dedication to be an
   overt act of relinquishment in perpetuity of all present and future rights to
   this software under copyright law.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
   ------------------------------------------------------------------------------
*/
