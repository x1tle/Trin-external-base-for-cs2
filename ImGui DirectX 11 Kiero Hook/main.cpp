#include "includes.h"
#include "config.h"
#include "cheat/cs2/ent.h"
#include "cheat/draw.h"
#include "cheat/math/vector.h"
#include "cheat/mem/mem.h"
#include "cheat/cs2/offsets.hpp"
#include "cheat/cs2/client.hpp"
#include "cheat/cs2/bones.hpp"
#include "cheat/cs2/weapon.hpp"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;


void tab(int id) {
	//soon
}
void styleMENU() {
	//soon
}


HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{

	
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);

	}
	auto mem = Memory("cs2.exe");
	const auto client = mem.GetModuleAdress("client.dll");

	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	

	auto flags = ImGuiConfigFlags_NoMouseCursorChange | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		config::menu = !config::menu;
	}
	if (config::menu) {
		ImGui::Begin("Trin || external base",nullptr,flags);

		if (ImGui::Button("AimBot", ImVec2(80.f, 15.f)))
			config::tab = 1;
		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button("Visuals", ImVec2(80.f, 15.f)))
			config::tab = 2;
		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button("Misc", ImVec2(80.f, 15.f)))
			config::tab = 3;
		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button("Cofnig", ImVec2(80.f, 15.f)))
			config::tab = 4;
	    
		if (config::tab == 1) {

		}
		if (config::tab == 2) {

		}
		if (config::tab == 3) {

		}
		if (config::tab == 4) {

		}

		ImGui::End();
	}
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}