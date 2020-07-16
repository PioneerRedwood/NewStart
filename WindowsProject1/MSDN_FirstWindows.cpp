// https://docs.microsoft.com/ko-kr/windows/win32/learnwin32/your-first-windows-program
// 결과적으로 동작하지 않는다 하지만 많은 걸 배웠다
#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#define HIWORD(l) ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff))

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
inline StateInfo* GetAppState(HWND hwnd);

// 상태 정보를 보여주기 위한 구조체
struct StateInfo
{
	unsigned iResult;
	const wchar_t* name;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// 윈도우 클래스 등록
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	StateInfo* pState = new StateInfo;
	if (pState == nullptr)
	{
		return 0;
	}

	pState->iResult = 1;
	pState->name = L"We've got Windows";

	// 윈도우 생성
	HWND hwnd = CreateWindowEx(
		0,								// 선택적인 윈도우 스타일
		CLASS_NAME,						// 윈도우 클래스
		L"Learn to Program Windows",	// 윈도우 텍스트
		WS_OVERLAPPEDWINDOW,			// 윈도우 스타일

		// 사이즈, 위치
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		nullptr,			// 부모 윈도우
		nullptr,			// 메뉴
		hInstance,			// 인스턴스 핸들
		pState				// 선택적인 응용 데이터 // 구조체나 클래스 포인터를 전달 가능
	);

	if (hwnd == nullptr)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// 메시지 루프 실행
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
	StateInfo* pState;
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		pState = reinterpret_cast<StateInfo*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
	}
	else
	{
		pState = GetAppState(hwnd);
	}


	switch (uMsg)
	{
	//case WM_SIZE:
	//	// 매크로 사용
	//	int width = LOWORD(lParam);
	//	int height = LOWORD(lParam);

	//	OnSize(hwnd, (UINT)wParam, width, height);
	//	break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;
		
	case WM_CLOSE:
		if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL))
		{
			DestroyWindow(hwnd);
		}
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OnSize(HWND hwnd, UINT flag, int width, int height)
{
	// 크기 재조정 핸들

}

inline StateInfo* GetAppState(HWND hwnd)
{
	LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	StateInfo* pState = reinterpret_cast<StateInfo*>(ptr);
	return pState;
}