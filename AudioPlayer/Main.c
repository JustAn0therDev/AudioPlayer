#include <stdlib.h>
#include <Windows.h>
#include <playsoundapi.h>
#include <devicetopology.h>

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 720;

const int BTN_WIDTH = 100;
const int BTN_HEIGHT = 100;

HWND hwndPlayButton = { 0 };
HWND hwndMain = { 0 };

BOOL playing = FALSE;

LPCWSTR lpszWAVEFileName = L"D:\\repos\\AudioPlayer\\AudioPlayer\\TestAudio\\PinkPanther30.wav";

LRESULT BtnPlayWndProc(
	HWND hInstance,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

LRESULT BtnStopWndProc(
	HWND hInstance,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
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

			FillRect(hdc, &ps.rcPaint, (HBRUSH)COLOR_WINDOW);

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
		if (playing == TRUE) {
			PlaySound(0, 0, 0);

			DestroyWindow(hwndPlayButton);

			hwndPlayButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"PLAY",    // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles 
				(WINDOW_WIDTH / 2) - (BTN_WIDTH / 2),        // x position 
				(WINDOW_HEIGHT / 2) - (BTN_HEIGHT),        // y position 
				BTN_WIDTH,				      // button size (width)
				BTN_HEIGHT,				      // button size (height)
				hwndMain,				  // Parent window
				0,                        // No menu.
				(HINSTANCE)GetWindowLongPtr(hwndMain, GWLP_HINSTANCE),
				0);                       // Pointer not needed.

			if (hwndPlayButton == 0) {
				return 1;
			}

			playing = FALSE;
		}
		else {
			PlaySound(
				lpszWAVEFileName,
				0,
				SND_FILENAME | SND_ASYNC);

			DestroyWindow(hwndPlayButton);

			hwndPlayButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"STOP",    // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles 
				(WINDOW_WIDTH / 2) - (BTN_WIDTH / 2),        // x position 
				(WINDOW_HEIGHT / 2) - (BTN_HEIGHT),        // y position 
				BTN_WIDTH,				      // button size (width)
				BTN_HEIGHT,				      // button size (height)
				hwndMain,				  // Parent window
				0,                        // No menu.
				(HINSTANCE)GetWindowLongPtr(hwndMain, GWLP_HINSTANCE),
				0);                       // Pointer not needed.

			if (hwndPlayButton == 0) {
				PostQuitMessage(0);
				return 1;
			}

			playing = TRUE;
		}

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
			// Stop any processing WAVE playing
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
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
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
		(WINDOW_WIDTH / 2) - (BTN_WIDTH / 2),        // x position 
		(WINDOW_HEIGHT / 2) - (BTN_HEIGHT),        // y position 
		BTN_WIDTH,				      // button size (width)
		BTN_HEIGHT,				      // button size (height)
		hwndMain,				  // Parent window
		0,                        // No menu.
		(HINSTANCE)GetWindowLongPtr(hwndMain, GWLP_HINSTANCE),
		0);                       // Pointer not needed.

	if (hwndPlayButton == 0) {
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

	return (int)msg.wParam;
}
