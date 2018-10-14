
#include "WindowContainer.h"
#include "RenderWindow.h"

bool RenderWindow::Initialize(WindowContainer * pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(this->window_class); //wide string representation of class

	this->RegisterWindowClass();

	this->handle = CreateWindowEx(0, //extended windows style, we use default
		this->window_class_wide.c_str(), //window class name
		this->window_title_wide.c_str(), //window title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //windows style
		0, //windows x position
		0, //windows y position
		this->width, //window width
		this->height, //window height
		NULL, //handle to parent of window
		NULL, //handle to menu or child window identifier
		this->hInstance, //handle to the instance of module to be used with this window
		pWindowContainer); //param to create window

	if (this->handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "CreateWindowEx Failed for window: " + this->window_title);
		return false;
	}

	//bring the window up on the screen and set it as main focus.
	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::ProcessMessages()
{
	//handle the windows messages
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); //initialize the message structure.

	while (PeekMessage(&msg,  //where to store message (if one exists)
		this->handle, //handle to window we are checking messages for
		0, //minimum filter Msg value
		0, //maximum filter Msg value
		PM_REMOVE)) //remove message after capturing it via PeekMessage.
	{
		TranslateMessage(&msg); //translate message from virtual key messages into character messages
		DispatchMessage(&msg); //Dispatch message to our Window Proc for this window
	}

	// check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL; //message processing loop takes care of destroying this window
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

HWND RenderWindow::GetHWND() const
{
	return this->handle;
}

RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}

LRESULT CALLBACK HandleMsgredirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// all other messages
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	default:
	{
		//retrieve ptr to window class
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		// forward message to window class handler
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer * pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr) //sanity check
		{
			ErrorLogger::Log("Critical Error: Pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgredirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc; //Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	wc.lpfnWndProc = HandleMessageSetup; //Pointer to Window Proc function for handling messages from this window
	wc.cbClsExtra = 0; //# of extra bytes to allocate following the window-class structure. We are not currently using this.
	wc.cbWndExtra = 0; //# of extra bytes to allocate following the window instance. We are not currently using this.
	wc.hInstance = this->hInstance; //Handle to the instance that contains the Window Procedure
	wc.hIcon = NULL;   //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
	wc.hIconSm = NULL; //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
	wc.hbrBackground = NULL; //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
	wc.lpszMenuName = NULL; //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
	wc.lpszClassName = this->window_class_wide.c_str(); //Pointer to null terminated string of our class name for this window.
	wc.cbSize = sizeof(WNDCLASSEX); //Need to fill in the size of our struct for cbSize
	RegisterClassEx(&wc); // Register the class so that it is usable.
}