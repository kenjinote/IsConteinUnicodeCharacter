#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

BOOL IsConteinUnicodeCharacter(LPCWSTR lpszText)
{
	if (!lpszText || !lpszText[0])return FALSE;
	DWORD dwSize = WideCharToMultiByte(CP_ACP, 0, lpszText, -1, 0, 0, 0, 0);
	LPSTR lpszTextA = (LPSTR)GlobalAlloc(0, dwSize);
	WideCharToMultiByte(CP_ACP, 0, lpszText, -1, lpszTextA, dwSize, 0, 0);
	dwSize = MultiByteToWideChar(CP_ACP, 0, lpszTextA, -1, 0, 0);
	LPWSTR lpszTextW = (LPWSTR)GlobalAlloc(0, sizeof(WCHAR)*dwSize);
	MultiByteToWideChar(CP_ACP, 0, lpszTextA, -1, lpszTextW, dwSize);
	GlobalFree(lpszTextA);
	const BOOL bReturn = lstrcmpW(lpszText, lpszTextW) != 0;
	GlobalFree(lpszTextW);
	return bReturn;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;
	static HWND hStatic;
	switch (msg)
	{
	case WM_CREATE:
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), 0, WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 10, 10, 512, 32, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hStatic = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), 0, WS_VISIBLE | WS_CHILD, 10, 50, 512, 32, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE)
		{
			DWORD dwSize = GetWindowTextLengthW(hEdit);
			LPWSTR lpszText = (LPWSTR)GlobalAlloc(0, sizeof(WCHAR)*(dwSize + 1));
			GetWindowTextW(hEdit, lpszText, dwSize + 1);
			SetWindowText(hStatic, IsConteinUnicodeCharacter(lpszText) ? TEXT("Unicode固有文字が含まれています。") : TEXT("Unicode固有文字は含まれていません。"));
			GlobalFree(lpszText);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("入力された文字列にUnicode固有文字が含まれているか"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
