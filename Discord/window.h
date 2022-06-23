#pragma once
#include "menu.h"


void setup_window()
{
	win = FindWindow(XorStr("CEF-OSC-WIDGET").c_str(), XorStr("NVIDIA GeForce Overlay").c_str());

	if (!win)
	{
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		printf(XorStr("\n\n  [/X] Couldn't find overlay, please contact support.").c_str());
		Sleep(5000);
		exit(1);
	}

	ShowWindow(win, SW_SHOW);

	SetWindowLong(win, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	SetWindowLongW(
		win,
		-20,
		static_cast<LONG_PTR>(
			static_cast<int>(GetWindowLongW(win, -20)) | 0x20
			)
	);

	//transparency
	MARGINS margin = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(
		win,
		&margin
	);

	SetLayeredWindowAttributes(
		win,
		NULL,
		0xFF,
		0x02
	);

	// top most
	SetWindowPos(
		win,
		HWND_TOPMOST,
		0, 0, 0, 0,
		0x0002 | 0x0001
	);

	UpdateWindow(win);

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.hDeviceWindow = win;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.Windowed = TRUE;

	p_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, win, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice);

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsClassic();

	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, 1.0f);
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 255);
	colors[ImGuiCol_Button] = ImColor(35, 35, 41);
	colors[ImGuiCol_ButtonHovered] = ImColor(35, 35, 41);
	colors[ImGuiCol_ButtonActive] = ImColor(0, 255, 0);
	colors[ImGuiCol_FrameBg] = ImColor(35, 35, 41);
	colors[ImGuiCol_FrameBgHovered] = ImColor(35, 35, 41);
	colors[ImGuiCol_FrameBgActive] = ImColor(35, 35, 41);
	colors[ImGuiCol_CheckMark] = ImColor(0, 255, 0);
	colors[ImGuiCol_ChildBg] = ImColor(0, 67, 91);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.0f, 1.00f, 0.0f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 1.00f, 0.0f, 1.00f);


	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 0.0f;
	style.ChildRounding = 0.0f;
	style.FrameRounding = 0.0f;
	style.GrabRounding = 0.0f;
	style.PopupRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;

	ImGui_ImplWin32_Init(win);
	ImGui_ImplDX9_Init(D3dDevice);

	m_pFont = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\ProggyClean.ttf").c_str(), 13.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	l_pFont = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\Calibri.ttf").c_str(), 13.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	font1 = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\segoepr.ttf").c_str(), 25.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	font2 = io.Fonts->AddFontFromFileTTF(XorStr("C:\\Windows\\Fonts\\ProggyClean.ttf").c_str(), 13.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

	D3DXCreateTextureFromFileInMemory(D3dDevice, icon, sizeof(icon), &bgtext);
	p_Object->Release();

}

void mainloop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, win, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();

		if (hwnd_active == windowid) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(win, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(win, &rc);
		ClientToScreen(win, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = win;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			d3dpp.BackBufferWidth = Width;
			d3dpp.BackBufferHeight = Height;
			SetWindowPos(win, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			D3dDevice->Reset(&d3dpp);
		}

		io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
		io.MouseDown[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
		io.MouseDown[3] = GetAsyncKeyState(VK_XBUTTON1) & 0x8000;
		io.MouseDown[4] = GetAsyncKeyState(VK_XBUTTON2) & 0x8000;

		for (int i = 6; i < 512; i++) {
			io.KeysDown[i] = (GetKeyState(i) & 0x8000) != 0;
		}

		menu();

		Sleep(1);
	}
	features::menu = false;
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
