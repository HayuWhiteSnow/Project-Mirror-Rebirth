//============================================================================
// 
// マネージャー [manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "manager.h"

//****************************************************
// 静的メンバの初期化
//****************************************************
CRenderer* CManager::m_pRenderer = nullptr;			// レンダラー管理
CCamera* CManager::m_pCamera = nullptr;				// カメラ管理
CLight* CManager::m_pLight = nullptr;				// ライト管理
CInputKeyboard* CManager::m_pKeyboard = nullptr;	// キーボード管理
CInputPad* CManager::m_pPad = nullptr;				// パッド管理
CSound* CManager::m_pSound = nullptr;				// サウンド管理
CScene* CManager::m_pScene = nullptr;				// シーン管理

//============================================================================
// コンストラクタ
//============================================================================
CManager::CManager()
{
	m_pRenderer = nullptr;
	m_pCamera = nullptr;
	m_pLight = nullptr;
	m_pKeyboard = nullptr;
	m_pPad = nullptr;
	m_pSound = nullptr;
	m_pScene = nullptr;
}

//============================================================================
// デストラクタ
//============================================================================
CManager::~CManager()
{
	
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダラーの生成
	m_pRenderer = DBG_NEW CRenderer;

	if (m_pRenderer == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// レンダラーの初期化
	m_pRenderer->Init(hWnd, bWindow);

	// カメラの生成
	m_pCamera = DBG_NEW CCamera;

	if (m_pCamera == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// カメラの初期化
	m_pCamera->Init();

	// ライトの生成
	m_pLight = DBG_NEW CLight;

	if (m_pLight == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// ライトの初期化
	m_pLight->Init();

	// キーボードの生成
	m_pKeyboard = DBG_NEW CInputKeyboard;

	if (m_pKeyboard == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// キーボードの初期化
	m_pKeyboard->Init(hInstance, hWnd);

	// パッドの生成
	m_pPad = DBG_NEW CInputPad;

	if (m_pPad == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// キーボードの初期化
	m_pPad->Init();

	// サウンドの生成
	m_pSound = DBG_NEW CSound;

	if (m_pSound == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// サウンドの初期化
	m_pSound->Init(hWnd);

	// 最初のシーン設定
	SetScene(CScene::MODE::TITLE);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CManager::Uninit()
{
	// レンダラーの破棄
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();	// 終了処理
		delete m_pRenderer;		// メモリを解放
		m_pRenderer = nullptr;	// ポインタを初期化
	}

	// カメラの破棄
	if (m_pCamera != nullptr)
	{
		delete m_pCamera;		// メモリを解放
		m_pCamera = nullptr;	// ポインタを初期化
	}

	// ライトの破棄
	if (m_pLight != nullptr)
	{
		delete m_pLight;		// メモリを解放
		m_pLight = nullptr;		// ポインタを初期化
	}

	// キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();	// 終了処理
		delete m_pKeyboard;		// メモリを解放
		m_pKeyboard = nullptr;	// ポインタを初期化
	}

	// パッドの破棄
	if (m_pPad != nullptr)
	{
		m_pPad->Uninit();	// 終了処理
		delete m_pPad;		// メモリを解放
		m_pPad = nullptr;	// ポインタを初期化
	}

	// サウンドの破棄
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();	// 終了処理
		delete m_pSound;	// メモリを解放
		m_pSound = nullptr;	// ポインタを初期化
	}

	// シーンの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();	// 終了処理
		delete m_pScene;	// メモリを解放
		m_pScene = nullptr;	// ポインタを初期化
	}
}

//============================================================================
// 更新処理
//============================================================================
void CManager::Update()
{
	// レンダラーの更新
	m_pRenderer->Update();

	// シーンの更新
	m_pScene->Update();

	// カメラの更新
	m_pCamera->Update();

	// ライトの更新
	m_pLight->Update();

	// キーボードの更新
	m_pKeyboard->Update();

	// パッドの更新
	m_pPad->Update();
}

//============================================================================
// 描画処理
//============================================================================
void CManager::Draw()
{
	// レンダラーの描画
	m_pRenderer->Draw();
}

//============================================================================
// レンダラー取得
//============================================================================
CRenderer* CManager::GetRenderer()
{ 
	return m_pRenderer;
}

//============================================================================
// カメラ取得
//============================================================================
CCamera* CManager::GetCamera()
{
	return m_pCamera;
}

//============================================================================
// ライト取得
//============================================================================
CLight* CManager::GetLight()
{
	return m_pLight;
}

//============================================================================
// キーボード取得
//============================================================================
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pKeyboard;
}

//============================================================================
// パッド取得
//============================================================================
CInputPad* CManager::GetPad()
{
	return m_pPad;
}

//============================================================================
// サウンド取得
//============================================================================
CSound* CManager::GetSound()
{
	return m_pSound;
}

//============================================================================
// シーン取得
//============================================================================
CScene* CManager::GetScene()
{
	return m_pScene;
}

//============================================================================
// シーン設定
//============================================================================
void CManager::SetScene(CScene::MODE mode)
{
	// 現在のシーンを破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	// 新たなシーンを設定
	if (m_pScene == nullptr)
	{
		m_pScene = CScene::Create(mode);
	}
}