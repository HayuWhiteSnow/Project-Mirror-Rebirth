//============================================================================
// 
// テクスチャ管理 [texture.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "texture.h"
#include "manager.h"

//****************************************************
// 静的メンバの初期化
//****************************************************
LPDIRECT3DTEXTURE9 CTexture::m_apTexTemp[static_cast<int>(TEX_TYPE::MAX)];	// テクスチャ管理

//============================================================================
// コンストラクタ
//============================================================================
CTexture::CTexture()
{
	for (int i = 0; i < static_cast<int>(TEX_TYPE::MAX); i++)
	{
		m_apTexTemp[i] = nullptr;	// テクスチャ情報の初期化
	}
}

//============================================================================
// デストラクタ
//============================================================================
CTexture::~CTexture()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CTexture::Load()
{
	// テクスチャ名管理ファイルを展開
	std::ifstream file("data\\TXT\\texture_path.txt");

	if (!file)
	{ // 展開に失敗
		assert(false);
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CManager::GetRenderer()->GetDeviece();

	for (int i = 0; i < static_cast<int>(TEX_TYPE::MAX); i++)
	{
		// テクスチャ名格納先
		std::string filename;

		// テクスチャ名を取得する
		std::getline(file, filename);

		// テクスチャの生成
		HRESULT hr = D3DXCreateTextureFromFileA(pDev,
			filename.c_str(),
			&m_apTexTemp[i]);

		if (FAILED(hr))
		{ // テクスチャ生成失敗
			file.close();	// ファイルを閉じる
			assert(false);
		}
	}

	file.close();	// ファイルを閉じる

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTexture::Unload()
{
	for (int i = 0; i < static_cast<int>(TEX_TYPE::MAX); i++)
	{
		// テクスチャの破棄
		if (m_apTexTemp[i] != nullptr)
		{
			m_apTexTemp[i]->Release();
			m_apTexTemp[i] = nullptr;
		}
	}
}

//============================================================================
// テクスチャを取得
//============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEX_TYPE type)
{
	if (m_apTexTemp[static_cast<int>(type)] == nullptr)
	{ // テクスチャ取得不能
		assert(false);
	}

	return m_apTexTemp[static_cast<int>(type)];
}