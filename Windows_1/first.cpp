// https://docs.microsoft.com/ko-kr/windows/win32/learnwin32/your-first-windows-program
#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// ������ Ŭ���� ���
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc	= WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// ������ ����
	HWND hwnd = CreateWindowEx(
		0,								// �������� ������ ��Ÿ��
		CLASS_NAME,						// ������ Ŭ����
		L"Learn to Program Windows",	// ������ �ؽ�Ʈ
		WS_OVERLAPPEDWINDOW,			// ������ ��Ÿ��

		// ������, ��ġ
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		nullptr,			// �θ� ������
		nullptr,			// �޴�
		hInstance,			// �ν��Ͻ� �ڵ�
		nullptr				// �������� ���� ������
	);
	
	if (hwnd == nullptr)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// �޽��� ���� ����
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
