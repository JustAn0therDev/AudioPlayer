#include <Windows.h>
#include <playsoundapi.h>

const int WIDTH = 1024;
const int HEIGHT = 720;

HWND hwndPlayButton = { 0 };
HWND hwndMain = { 0 };
HWND hwndStopButton = { 0 };

LRESULT BtnPlayWndProc(
	HWND hInstance,
	UINT msg,
	WPARAM wAddMsgInfo,
	LPARAM lAddMsgInfo
);

LRESULT BtnStopWndProc(
	HWND hInstance,
	UINT msg,
	WPARAM wAddMsgInfo,
	LPARAM lAddMsgInfo
);

LRESULT WndProc(
	HWND hInstance,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch (msg) {
		case WM_COMMAND: {
			if (lParam == (LPARAM)hwndPlayButton) {
				return BtnPlayWndProc(hInstance, WM_LBUTTONDOWN, wParam, lParam);
			} else if (lParam == (LPARAM)hwndStopButton) {
				return BtnStopWndProc(hInstance, WM_LBUTTONDOWN, wParam, lParam);
			}
		}

		case WM_CLOSE: {
			PostQuitMessage(0);
			return 0;
		}

		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}

		case WM_PAINT: {
			PAINTSTRUCT ps;

			HDC hdc = BeginPaint(hInstance, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BACKGROUND + 1));

			EndPaint(hInstance, &ps);

			return 0;
		}

		default: {
			return DefWindowProc(hInstance, msg, wParam, lParam);
		}
	}
}

LRESULT BtnPlayWndProc(
	HWND hInstance,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch (msg) {
	case WM_LBUTTONDOWN: {
		PlaySound(
			L"D:\\repos\\AudioPlayer\\AudioPlayer\\TestAudio\\PinkPanther30.wav",
			0,
			SND_FILENAME | SND_ASYNC | SND_LOOP);

		return 0;
	}

	default:
	{
		return DefWindowProc(hInstance, msg, wParam, lParam);
	}
	}
}

LRESULT BtnStopWndProc(
	HWND hInstance,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch (msg) {
		case WM_LBUTTONDOWN: {
			PlaySound(0, 0, 0);
			return 0;
		}

		default:
		{
			return DefWindowProc(hInstance, msg, wParam, lParam);
		}
	}
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {

	// Register the window class.
	const wchar_t CLASS_NAME[] = L"MainWndClass";

	WNDCLASS wc = { 0 };

	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	hwndMain = CreateWindow(
		CLASS_NAME,
		L"AudioPlayer",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WIDTH,
		HEIGHT,
		0,
		0,
		hInstance,
		0);

	if (hwndMain == 0) {
		PostQuitMessage(0);
		return 1;
	}

	hwndPlayButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"PLAY",    // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles 
		(WIDTH / 2) - 250,         // x position 
		(HEIGHT / 2) - 55,        // y position 
		100,				      // button size (width)
		100,				      // button size (height)
		hwndMain,				  // Parent window
		0,                        // No menu.
		(HINSTANCE)GetWindowLongPtr(hwndMain, GWLP_HINSTANCE),
		0);                       // Pointer not needed.

	if (hwndPlayButton == 0) {
		PostQuitMessage(0);
		return 1;
	}

	hwndStopButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"STOP",    // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles 
		(WIDTH / 2) + 200,         // x position 
		(HEIGHT / 2) - 55,        // y position 
		100,				      // button size (width)
		100,				      // button size (height)
		hwndMain,				  // Parent window
		0,                        // No menu.
		(HINSTANCE)GetWindowLongPtr(hwndMain, GWLP_HINSTANCE),
		0);                       // Pointer not needed.

	if (hwndStopButton == 0) {
		PostQuitMessage(0);
		return 1;
	}

	ShowWindow(hwndMain, nCmdShow);

	MSG msg;

	GetMessage(&msg, 0, 0, 0);

	while (GetMessage(&msg, 0, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyWindow(hwndMain);
	DestroyWindow(hwndPlayButton);
	DestroyWindow(hwndStopButton);

	return (int)msg.wParam;
}
