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
HWND hwndStopButton = { 0 };

BOOL playing = FALSE;

LPHWAVEOUT phwo = 0;

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
		PlaySound(
			L"D:\\repos\\AudioPlayer\\AudioPlayer\\TestAudio\\PinkPanther30.wav",
			0,
			SND_FILENAME | SND_ASYNC);

		// Information to pass in to the "waveOutOpen" function.
		WAVEFORMATEX pwfx;
		pwfx.cbSize = sizeof(WAVEFORMATEX);
		pwfx.wFormatTag = WAVE_FORMAT_PCM;
		pwfx.nChannels = 2;
		pwfx.nSamplesPerSec = 44100;
		pwfx.wBitsPerSample = 16;
		pwfx.nBlockAlign = pwfx.nChannels * pwfx.wBitsPerSample;
		pwfx.nAvgBytesPerSec = pwfx.nSamplesPerSec * pwfx.nBlockAlign;

		//MMRESULT waveOutOpen(
		//	LPHWAVEOUT      phwo,
		//	UINT            uDeviceID,
		//	LPCWAVEFORMATEX pwfx,
		//	DWORD_PTR       dwCallback,
		//	DWORD_PTR       dwInstance,
		//	DWORD           fdwOpen
		//);

		MMRESULT mmr = waveOutOpen(
			phwo, 
			WAVE_MAPPER,
			&pwfx,
			CALLBACK_NULL,
			0,
			WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE
			);

		switch (mmr) {
		case MMSYSERR_ALLOCATED:
		{
			OutputDebugStringA("Specified resource is already allocated.\n");
		}
		break;
		case MMSYSERR_BADDEVICEID:
		{
			OutputDebugStringA("Specified device identifier is out of range.\n");
		}
		break;
		case MMSYSERR_NODRIVER:
		{
			OutputDebugStringA("No device driver is present.\n");
		}
		break;
		case MMSYSERR_NOMEM:
		{
			OutputDebugStringA("Unable to allocate or lock memory.\n");
		}
		break;
		case WAVERR_BADFORMAT:
		{
			OutputDebugStringA("Attempted to open with an unsupported waveform-audio format.\n");
		}
		break;
		case WAVERR_SYNC:
		{
			OutputDebugStringA(
				"The device is synchronous but waveOutOpen was called without using the"\
				"WAVE_ALLOWSYNC flag.\n"
			);
		}
		break;
		case MMSYSERR_NOERROR:
		{
			OutputDebugStringA("Everything is A OKAY\n");
			
		}
		case MMSYSERR_ERROR:
		{
			OutputDebugStringA("Something bad happened\n");
		}
		case MMSYSERR_INVALFLAG:
		{
			OutputDebugStringA("Invalid flag!\n");
		}
		break;
		default:
			OutputDebugStringA("Nobody is sure what happened.\n");
			break;
		}

		if (playing == FALSE) {
			DestroyWindow(hwndPlayButton);

			hwndPlayButton = 0;

			hwndPlayButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"PAUSE",    // Button text 
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

			playing = TRUE;
		}
		else {
			DestroyWindow(hwndPlayButton);

			hwndPlayButton = 0;

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

	hwndStopButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"STOP",    // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles 
		(WINDOW_WIDTH / 2) - (BTN_WIDTH / 2),        // x position 
		(WINDOW_HEIGHT / 2) + 155,        // y position 
		BTN_WIDTH,				      // button size (width)
		BTN_HEIGHT,				      // button size (height)
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

	// This is called before handling any sound request
	phwo = malloc(sizeof(LPHWAVEOUT));

	GetMessage(&msg, 0, 0, 0);

	while (GetMessage(&msg, 0, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyWindow(hwndMain);
	free(phwo);

	return (int)msg.wParam;
}
