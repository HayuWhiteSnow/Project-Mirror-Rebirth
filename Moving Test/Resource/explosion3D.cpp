//============================================================================
// 
// 3D爆発 [explosion3D.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "explosion3D.h"
#include "manager.h"
#include "bullet.h"

//============================================================================
// コンストラクタ
//============================================================================
CExplosion3D::CExplosion3D() : CObjectBillboard(static_cast<int>(LAYER::FRONT_MIDDLE))
{
	m_nCntTexChange = 0;	// テクスチャ変更管理
}

//============================================================================
// デストラクタ
//============================================================================
CExplosion3D::~CExplosion3D()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CExplosion3D::Init()
{
	// 基底クラスの初期設定
	HRESULT hr = CObjectBillboard::Init();

	return hr;
}

//============================================================================
// 終了処理
//============================================================================
void CExplosion3D::Uninit()
{
	// 基底クラスの終了処理
	CObjectBillboard::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CExplosion3D::Update()
{
	// アニメーション
	if (!Animation())
	{ // 破棄されていたら更新終了
		return;
	}

	// 基底クラスの更新
	CObjectBillboard::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CExplosion3D::Draw()
{
	// 基底クラスの描画処理
	CObjectBillboard::Draw();
}

//============================================================================
// 生成
//============================================================================
CExplosion3D* CExplosion3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// インスタンスを生成
	CExplosion3D* pExplosion = DBG_NEW CExplosion3D;

	if (pExplosion == nullptr)
	{ // 生成失敗
		assert(false);
	}

	pExplosion->SetType(TYPE::NONE);	// タイプを設定

	pExplosion->Init();			// 基底クラスの初期設定
	pExplosion->SetPos(pos);	// 中心位置の設定
	pExplosion->SetSize(size);	// サイズの設定

	pExplosion->SetTexWidth(1.0f / 8.0f);	// 横テクスチャ分割幅
	pExplosion->SetTexHeight(1.0f);			// 縦テクスチャ分縦幅

	// テクスチャを設定
	pExplosion->BindTex(CManager::GetRenderer()->GetTextureInstane()->GetTexture(CTexture::TEX_TYPE::EXPLOSION_000));

	return pExplosion;
}

//============================================================================
// アニメーション
//============================================================================
bool CExplosion3D::Animation()
{
	// テクスチャ変更管理カウントアップ
	m_nCntTexChange++;

	if (m_nCntTexChange >= 10)
	{
		// 横テクスチャ種類情報取得
		int nTexPatternU = GetNowPatternU();

		// 横テクスチャ種類変更
		nTexPatternU++;

		if (nTexPatternU >= 8)
		{
			// 自身を破棄
			CObject::Release();

			// 終了
			return false;
		}

		// 横テクスチャ種類情報設定
		SetNowPatternU(nTexPatternU);

		// 変更管理カウントをリセット
		m_nCntTexChange = 0;
	}

	return true;
}