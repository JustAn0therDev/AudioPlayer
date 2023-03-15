#include <Windows.h>

LRESULT Wndproc(
                HWND hInstance,
                UINT uMsg,
                WPARAM wAddMsgInfo,
                LPARAM lAddMsgInfo
                ) {
  switch (uMsg) {
  case WM_SIZE: {
    OutputDebugStringA("RESIZING!\n");
    return DefWindowProc(hInstance, uMsg, wAddMsgInfo, lAddMsgInfo);
  }
  case WM_CLOSE: {
    OutputDebugStringA("CLOSING!\n");
    return DefWindowProc(hInstance, uMsg, wAddMsgInfo, lAddMsgInfo);
  }

  case WM_PAINT: {
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hInstance, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

    EndPaint(hInstance, &ps);
    
    return 0;
  }

  default: {
    return DefWindowProc(hInstance, uMsg, wAddMsgInfo, lAddMsgInfo);
  }
  }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow) {

  // Register the window class.
  const wchar_t CLASS_NAME[] = L"Sample Window Class";

  WNDCLASS wc = { 0 };

  wc.lpfnWndProc = Wndproc;
  wc.hInstance = hInstance;
  wc.lpszClassName = L"MainWndClass";

  RegisterClass(&wc);

  HWND hwndMain = CreateWindow(
                               L"MainWndClass",
                               L"Sample",
                               WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               0,
                               0,
                               hInstance,
                               0);

  if (hwndMain == NULL) {
    return 0;
  }

  ShowWindow(hwndMain, nCmdShow);
  UpdateWindow(hwndMain);

  MSG msg;
  BOOL close = 0;

  GetMessage(&msg, NULL, 0, 0);

  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    UpdateWindow(hwndMain);
  }

  return msg.wParam;
}
