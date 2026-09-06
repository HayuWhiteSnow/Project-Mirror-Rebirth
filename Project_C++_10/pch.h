//============================================================================
// 
// プリコンパイル済みヘッダー [pch.h]
// Author : 福田歩希
// 
//============================================================================
#pragma once	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include <stdio.h>
#include <assert.h>
#include <windows.h>	// Windowsアプリケーション動作用
#include <d3dx9.h>		// DirectX9動作用

#define DIRECTINPUT_VERSION	0x0800	// DirectInputのバージョン
#include <dinput.h>					// DirectInput動作用
#include <xinput.h>					// コントローラー動作用
#include <xaudio2.h>				// サウンド動作用

// 標準ライブラリ
#include <fstream>
#include <string>
#include <vector>

//****************************************************
// ライブラリのリンク
//****************************************************
#pragma	comment(lib,"d3d9.lib")		// 描画処理用
#pragma	comment(lib,"d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma	comment(lib,"dxguid.lib")	// DirectXのコーポネント使用のため
#pragma	comment(lib,"dinput8.lib")	// DirectInput動作用
#pragma comment(lib,"xinput.lib")	// コントローラ動作用

//****************************************************
// マクロ定義
//****************************************************

// 頂点フォーマット
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// ウィンドウサイズ
#define SCREEN_WIDTH	1280	// ウインドウの幅
#define SCREEN_HEIGHT	720		// ウインドウの高さ