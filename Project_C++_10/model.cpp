//============================================================================
// 
// モデル管理 [model.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "model.h"
#include "manager.h"

//****************************************************
// 静的メンバの初期化
//****************************************************
MODEL CModel::m_aModelTemp[static_cast<int>(MODEL_TYPE::MAX)];	// モデル管理

//============================================================================
// コンストラクタ
//============================================================================
CModel::CModel()
{
	for (int i = 0; i < static_cast<int>(MODEL_TYPE::MAX); i++)
	{
		// モデル情報の初期化
		m_aModelTemp[i].pMesh = nullptr;
		m_aModelTemp[i].pBuffMat = nullptr;
		m_aModelTemp[i].dwNumMat = 0;
		m_aModelTemp[i].ppTex = nullptr;
	}
}

//============================================================================
// デストラクタ
//============================================================================
CModel::~CModel()
{

}

//============================================================================
// モデル読み込み
//============================================================================
HRESULT CModel::Load()
{
	// モデル名管理ファイルを展開
	std::ifstream file("data\\TXT\\model_path.txt");

	if (!file)
	{ // 展開に失敗
		assert(false);
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CManager::GetRenderer()->GetDeviece();

	for (int nCntModel = 0; nCntModel < static_cast<int>(MODEL_TYPE::MAX); nCntModel++)
	{
		// モデル名格納先
		std::string filename;

		// モデル名を取得する
		std::getline(file, filename);

		// モデルファイルの取得
		D3DXLoadMeshFromX(filename.c_str(),
			D3DXMESH_SYSTEMMEM,
			pDev,
			nullptr,
			&m_aModelTemp[nCntModel].pBuffMat,
			nullptr,
			&m_aModelTemp[nCntModel].dwNumMat,
			&m_aModelTemp[nCntModel].pMesh);

		// マテリアルデータへのポインタを取得
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_aModelTemp[nCntModel].pBuffMat->GetBufferPointer();

		// マテリアルの数分のテクスチャポインタを確保
		m_aModelTemp[nCntModel].ppTex = new LPDIRECT3DTEXTURE9 [static_cast<int>(m_aModelTemp[nCntModel].dwNumMat)];

		// マテリアル分テクスチャの有無を確認
		for (int nCntMat = 0; nCntMat < static_cast<int>(m_aModelTemp[nCntModel].dwNumMat); nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != nullptr)
			{
				// テクスチャを読み取れたら生成
				D3DXCreateTextureFromFileA(pDev,
					pMat[nCntMat].pTextureFilename,
					&m_aModelTemp[nCntModel].ppTex[nCntMat]);
			}
			else
			{
				// 読み取れなければ初期化
				m_aModelTemp[nCntModel].ppTex[nCntMat] = nullptr;
			}
		}

		// モデル情報をすべて取得できたかチェック
		if (!LoadCheck())
		{ // 取得失敗
			file.close();	// ファイルを閉じる
			assert(false);
		}
	}

	file.close();	// ファイルを閉じる

	return S_OK;
}

//============================================================================
// モデル破棄
//============================================================================
void CModel::Unload()
{
	for (int i = 0; i < static_cast<int>(MODEL_TYPE::MAX); i++)
	{
		// テクスチャポインタの破棄
		if (m_aModelTemp[i].ppTex != nullptr)
		{
			// テクスチャの破棄
			for (int nCntMat = 0; nCntMat < static_cast<int>(m_aModelTemp[i].dwNumMat); nCntMat++)
			{
				if (m_aModelTemp[i].ppTex[nCntMat] != nullptr)
				{
					m_aModelTemp[i].ppTex[nCntMat]->Release();
					m_aModelTemp[i].ppTex[nCntMat] = nullptr;
				}
			}

			delete[] m_aModelTemp[i].ppTex;
			m_aModelTemp[i].ppTex = nullptr;
		}

		// メッシュの破棄
		if (m_aModelTemp[i].pMesh != nullptr)
		{
			m_aModelTemp[i].pMesh->Release();
			m_aModelTemp[i].pMesh = nullptr;
		}

		// マテリアルの破棄
		if (m_aModelTemp[i].pBuffMat != nullptr)
		{
			m_aModelTemp[i].pBuffMat->Release();
			m_aModelTemp[i].pBuffMat = nullptr;
		}
	}
}

//============================================================================
// モデルを取得
//============================================================================
MODEL* CModel::GetModel(MODEL_TYPE type)
{
	if (&m_aModelTemp[static_cast<int>(type)].pMesh == nullptr)
	{ // モデル取得不能
		assert(false);
	}

	return &m_aModelTemp[static_cast<int>(type)];
}

//============================================================================
// 読み込みチェック
//============================================================================
bool CModel::LoadCheck()
{
	/*---*/

	return true;
}