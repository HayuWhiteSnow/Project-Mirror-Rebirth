//============================================================================
// 
// パーティクル [particle.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "particle.h"
#include "manager.h"

//============================================================================
// コンストラクタ
//============================================================================
CParticle::CParticle() : CObject2D(static_cast<int>(LAYER::FRONT_MIDDLE))
{
	m_fFlyAngle = 0.0f;	// 飛ぶ角度
}

//============================================================================
// デストラクタ
//============================================================================
CParticle::~CParticle()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CParticle::Init()
{
	// 基底クラスの初期設定
	HRESULT hr = CObject2D::Init();

	return hr;
}

//============================================================================
// 終了処理
//============================================================================
void CParticle::Uninit()
{
	// 基底クラスの終了処理
	CObject2D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CParticle::Update()
{
	// 移動
	Translate();

	// 期間経過
	if (!Progress())
	{ // 破棄されていたら更新終了
		return;
	}

	// 基底クラスの更新
	CObject2D::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CParticle::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CManager::GetRenderer()->GetDeviece();

	// アルファブレンディングを加算合成に設定
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 基底クラスの描画処理
	CObject2D::Draw();

	// アルファブレンディングをの設定を戻す
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================================
// 生成
//============================================================================
CParticle* CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fFlyAngle)
{
	// インスタンスを生成
	CParticle* pParticle = new CParticle;

	if (pParticle == nullptr)
	{ // 生成失敗
		assert(false);
	}

	pParticle->SetType(TYPE::NONE);	// タイプを設定

	pParticle->Init();			// 基底クラスの初期設定
	pParticle->SetPos(pos);		// 中心位置の設定
	pParticle->SetSize(size);	// サイズの設定

	pParticle->m_fFlyAngle = fFlyAngle;	// 飛ぶ角度の設定

	// テクスチャを設定
	pParticle->BindTex(CManager::GetRenderer()->GetTextureInstane()->GetTexture(CTexture::TEX_TYPE::EFFECT_000));

	return pParticle;
}

//============================================================================
// 移動
//============================================================================
void CParticle::Translate()
{
	// 中心位置情報を取得
	D3DXVECTOR3 pos = CObject2D::GetPos();

	// 設定された角度に飛んでいく
	pos.x += sinf(m_fFlyAngle) * 1.0f;
	pos.y += cosf(m_fFlyAngle) * 1.0f;

	// 中心位置を設定
	CObject2D::SetPos(pos);
}

//============================================================================
// 期間経過
//============================================================================
bool CParticle::Progress()
{
	// サイズを取得
	D3DXVECTOR3 size = CObject2D::GetSize();

	// 縮小
	size.x += -0.25f;
	size.y += -0.25f;

	// 裏返ったタイミングで消滅
	if (size.x <= 0)
	{
		// 自身を破棄
		CObject::Release();

		// 終了
		return false;
	}

	// サイズを設定
	CObject2D::SetSize(size);

	return true;
}