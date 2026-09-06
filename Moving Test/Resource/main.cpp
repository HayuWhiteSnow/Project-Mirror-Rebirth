//============================================================================
// 
// ポリゴンの描画 5_16 [main.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "main.h"
#include "manager.h"

// メモリリーク検出用
#include <crtdbg.h>
#include <stdlib.h>

//****************************************************
// マクロ定義
//****************************************************
#define CLASS_NAME	"WindowClass"			// ウインドウクラスの名前
#define WINDOW_NAME	"ウインドウテンプレ"	// ウインドウの名前

// メモリリーク検出用
#define _CRTDBG_MAP_ALLOC

//****************************************************
// グローバル宣言
//****************************************************
CManager* g_pManager = nullptr;	// マネージャー管理

//****************************************************
// プロトタイプ宣言
//****************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ

//****************************************************************************
// メイン関数
//****************************************************************************
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// CRTメモリリーク検出用
	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						// ウインドウのスタイル
		WindowProc,						// ウインドウプロシージャ
		0,								// 何かを0にする
		0,								// 何かを0にする
		hInstance,						// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW),			// クライアント領域の背景色
		NULL,							// メニューバー
		CLASS_NAME,						// ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// ファイルのアイコン
	};

	HWND hWnd;	// ウインドウハンドル(識別子)
	MSG msg;	// メッセージを格納する変数

	// 画面サイズの設定用
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx(0,	// 拡張ウインドウのスタイル
		CLASS_NAME,				// ウインドウクラスの名前
		WINDOW_NAME,			// ウインドウの名前
		WS_OVERLAPPEDWINDOW,	// ウインドウのスタイル
		125,					// ウインドウの左上のX座標
		50,						// ウインドウの左上のY座標
		SCREEN_WIDTH,			// ウインドウの幅
		SCREEN_HEIGHT,			// ウインドウの高さ
		NULL,					// 親ウインドウのハンドル
		NULL,					// メニューバーまたは子ウインドウID
		hInstance,				// インスタンスハンドル
		NULL);					// ウインドウ作成データ

	// ウインドウの表示
	ShowWindow(hWnd, SW_NORMAL);	// ウインドウの表示状態を設定
	UpdateWindow(hWnd);				// クライアント領域を更新

	// マネージャーの生成
	g_pManager = DBG_NEW CManager;

	if (g_pManager == nullptr)
	{ // 生成失敗
		return -1;
	}

	if (FAILED(g_pManager->Init(hInstance, hWnd, TRUE)))
	{ // 初期化処理が失敗した場合
		return -1;
	}

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windowsの処理
			if (msg.message == WM_QUIT)
			{
				// WM_QUITのメッセージを受け取ると、メッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウインドウプロシージャへメッセージを創出
			}
		}
		else
		{
			// 更新処理
			g_pManager->Update();

			// 描画処理
			g_pManager->Draw();
		}
	}

	// マネージャーの破棄
	if (g_pManager != nullptr)
	{
		g_pManager->Uninit();	// 終了処理
		delete g_pManager;		// メモリを解放
		g_pManager = nullptr;	// ポインタを初期化
	}

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// CRTメモリリーク箇所検出
	//_CrtSetBreakAlloc();

	return (int)msg.wParam;
}

//****************************************************************************
// ウインドウプロシージャ関数
//****************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 返り値格納先
	int nID;

	switch (uMsg)
	{
		// ウインドウ破棄のメッセージを送る
	case WM_DESTROY:

		PostQuitMessage(0);

		break;

		// ×マークを押しているメッセージを送る
	case WM_CLOSE:

		// メッセージボックスを表示
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	// 「WM_DESTROYというメッセージを送る」
		}
		else
		{
			return 0;
		}

		break;

		// キーが押されているメッセージを送る
	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:

			// メッセージボックスを表示
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	// 「WM_DESTROYというメッセージを送る」
			}

			break;
		}

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を返す
}