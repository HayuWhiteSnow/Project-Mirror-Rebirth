//============================================================================
// 
// オブジェクト管理 [object.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"
#include "main.h"

//****************************************************
// 静的メンバの初期化
//****************************************************
const float CObject::GRAVITY_FORCE = 0.15f;									// 重力
CObject* CObject::m_apObject[static_cast<int>(LAYER::MAX)][MAX_OBJ] = {};	// オブジェクト管理
int CObject::m_nNumAll = 0;													// オブジェクト総数

//============================================================================
// コンストラクタ
//============================================================================
CObject::CObject(int nPriority) : m_nPriority(nPriority), m_nID(0), m_type(TYPE::NONE)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{
		if (m_apObject[m_nPriority][nCntObj] == nullptr)
		{
			m_apObject[m_nPriority][nCntObj] = this;	// 自分自身のポインタを代入
			m_nID = nCntObj;							// 自分自身のIDを代入
			m_nNumAll++;								// 総数をカウントアップ
			return;										// 終了
		}
	}

	// オブジェクト上限に到達し、新規生成に失敗
	assert(false);
}

//============================================================================
// デストラクタ
//============================================================================
CObject::~CObject()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject::Init()
{
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject::Uninit()
{

}

//============================================================================
// 更新処理
//============================================================================
void CObject::Update()
{

}

//============================================================================
// 描画処理
//============================================================================
void CObject::Draw()
{

}

//============================================================================
// 全解放処理
//============================================================================
void CObject::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] != nullptr)
			{
				m_apObject[nCntPriority][nCntObj]->Release();	// 解放処理
			}
		}
	}
}

//============================================================================
// 全更新処理
//============================================================================
void CObject::UpdateAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] != nullptr)
			{
				m_apObject[nCntPriority][nCntObj]->Update();	// 更新処理
			}
		}
	}
}

//============================================================================
// 全描画処理
//============================================================================
void CObject::DrawAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] != nullptr)
			{
				m_apObject[nCntPriority][nCntObj]->Draw();	// 描画処理
			}
		}
	}
}

//============================================================================
// オブジェクト情報取得
//============================================================================
CObject* CObject::GetObject(int nPriority, int nID)
{
	return m_apObject[nPriority][nID];
}

//============================================================================
// オブジェクト総数取得
//============================================================================
int CObject::GetNumAll()
{
	return m_nNumAll;
}

//============================================================================
// タイプ取得
//============================================================================
CObject::TYPE CObject::GetType()
{
	return m_type;
}

//============================================================================
// スコアインスタンスを探す
//============================================================================
CObject* CObject::FindScoreInstance()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] == nullptr)
			{ // 情報がなければコンティニュー
				continue;
			}

			if (m_apObject[nCntPriority][nCntObj]->GetType() == TYPE::SCORE)
			{ // スコアタイプならリターン
				return m_apObject[nCntPriority][nCntObj];
			}
		}
	}

	// 発見できなければエラー
	assert(false);

	return nullptr;
}

//============================================================================
// タイプ設定
//============================================================================
void CObject::SetType(TYPE type) 
{
	m_type = type;
}

//============================================================================
// 解放処理
//============================================================================
void CObject::Release()
{
	int nPriority = m_nPriority;	// プライオリティをコピーしておく
	int nID = m_nID;				// IDをコピーしておく

	if (m_apObject[nPriority][nID] != nullptr)
	{
		m_apObject[nPriority][nID]->Uninit();	// 終了処理
		delete m_apObject[nPriority][nID];		// メモリを解放
		m_apObject[nPriority][nID] = nullptr;	// ポインタを初期化
		m_nNumAll--;							// 総数をカウントダウン
	}
}