//============================================================================
// 
// 弾 [bullet.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "bullet.h"
#include "manager.h"
#include "block.h"
#include "enemy.h"
#include "effect.h"
#include "explosion.h"
#include "score.h"

//============================================================================
// コンストラクタ
//============================================================================
CBullet::CBullet() : CObject2D(static_cast<int>(LAYER::BACK_MIDDLE))
{
	m_nRemain = 0;		// 使用期間
	m_fFlyAngle = 0;	// 飛ぶ角度
}

//============================================================================
// デストラクタ
//============================================================================
CBullet::~CBullet()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CBullet::Init()
{
	// 基底クラスの初期設定
	HRESULT hr = CObject2D::Init();

	return hr;
}

//============================================================================
// 終了処理
//============================================================================
void CBullet::Uninit()
{
	// 基底クラスの終了処理
 	CObject2D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CBullet::Update()
{
	// 移動
	Translation();

	// 当たり判定
	if (!CollisionEnemy())
	{ // 破棄されていたら更新終了
		return;
	}

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
void CBullet::Draw()
{
	// 基底クラスの描画処理
	CObject2D::Draw();
}

//============================================================================
// 生成
//============================================================================
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nRemain, float fFlyAngle)
{
	// インスタンスを生成
	CBullet* pBullet = DBG_NEW CBullet;

	if (pBullet == nullptr)
	{ // 生成失敗
		assert(false);
	}

	pBullet->SetType(TYPE::NONE);	// タイプを設定

	pBullet->Init();		// 基底クラスの初期設定
	pBullet->SetPos(pos);	// 中心位置の設定
	pBullet->SetSize(size);	// サイズの設定

	pBullet->m_nRemain = nRemain;		// 使用期間の設定
	pBullet->m_fFlyAngle = fFlyAngle;	// 飛ぶ角度の設定

	// テクスチャを設定
	pBullet->BindTex(CManager::GetRenderer()->GetTextureInstane()->GetTexture(CTexture::TEX_TYPE::BULLET_000));

	return pBullet;
}

//============================================================================
// 移動
//============================================================================
void CBullet::Translation()
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	// 設定された角度に飛んでいく
	pos.x += sinf(m_fFlyAngle) * 3.0f;
	pos.y += cosf(m_fFlyAngle) * 3.0f;

	// 位置を設定
	SetPos(pos);
}

//============================================================================
// 当たり判定
//============================================================================
bool CBullet::CollisionEnemy()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < CObject::MAX_OBJ; nCntObj++)
		{
			// オブジェクト情報を取得
			CObject* pObject = CObject::GetObject(nCntPriority, nCntObj);

			if (pObject == nullptr)
			{ // 情報がなければコンティニュー
				continue;
			}

			if (pObject->GetType() == CObject::TYPE::ENEMY)
			{ // エネミータイプなら

				// オブジェクトクラスをエネミークラスへダウンキャスト
				CEnemy* pEnemy = dynamic_cast<CEnemy*>(pObject);

				if (pEnemy == nullptr)
				{ // ダウンキャスト失敗
					assert(false);
				}

				// 敵と衝突したら
				if (GetPos().x + GetSize().x >= pEnemy->GetPos().x - pEnemy->GetSize().x &&
					GetPos().x - GetSize().x <= pEnemy->GetPos().x + pEnemy->GetSize().x &&
					GetPos().y + GetSize().y >= pEnemy->GetPos().y - pEnemy->GetSize().y &&
					GetPos().y - GetSize().y <= pEnemy->GetPos().y + pEnemy->GetSize().y)
				{
					// スコアインスタンスを取得
					CObject* pFindObject = FindScoreInstance();

					// オブジェクトクラスをスコアクラスへダウンキャスト
					CScore* pScore = dynamic_cast<CScore*>(pFindObject);

					if (pScore == nullptr)
					{ // ダウンキャスト失敗
						assert(false);
					}

					// スコアを加算
					pScore->SetScore(pScore->GetScore() + 123456);

					// 爆発を生成
					CExplosion::Create(
						GetPos(),					// 位置
						{ 40.0f, 40.0f, 0.0f });	// サイズ

					// 自身を破棄
					CObject::Release();

					// エネミーを破棄
					pObject->Release();

					// 終了
					return false;
				}
			}
			else if (pObject->GetType() == CObject::TYPE::BLOCK)
			{ // ブロックタイプなら

				// オブジェクトクラスをブロッククラスへダウンキャスト
				CBlock* pBlock = dynamic_cast<CBlock*>(pObject);

				if (pBlock == nullptr)
				{ // ダウンキャスト失敗
					assert(false);
				}

				// ブロックと衝突したら
				if (GetPos().x + GetSize().x >= pBlock->GetPos().x - pBlock->GetSize().x &&
					GetPos().x - GetSize().x <= pBlock->GetPos().x + pBlock->GetSize().x &&
					GetPos().y + GetSize().y >= pBlock->GetPos().y - pBlock->GetSize().y &&
					GetPos().y - GetSize().y <= pBlock->GetPos().y + pBlock->GetSize().y)
				{
					// スコアインスタンスを取得
					CObject* pFindObject = FindScoreInstance();

					// オブジェクトクラスをスコアクラスへダウンキャスト
					CScore* pScore = dynamic_cast<CScore*>(pFindObject);

					if (pScore == nullptr)
					{ // ダウンキャスト失敗
						assert(false);
					}

					// スコアを加算
					pScore->SetScore(pScore->GetScore() + 62);

					// 爆発を生成
					CExplosion::Create(
						GetPos(),					// 位置
						{ 40.0f, 40.0f, 0.0f });	// サイズ

					// 自身を破棄
					CObject::Release();

					// ブロックを破棄
					pObject->Release();

					// 終了
					return false;
				}
			}
		}
	}

	return true;
}

//============================================================================
// 期間経過
//============================================================================
bool CBullet::Progress()
{
	// 使用期間カウントダウン
	m_nRemain--;

	// エフェクト生成
	CEffect::Create(GetPos(),	// 位置
		GetSize());				// サイズ

	// 使用期間がなくなると消滅
	if (m_nRemain <= 0)
	{
		// 自身を破棄
		CObject::Release();

		// 爆発を生成
		CExplosion::Create(
			GetPos(),					// 位置
			{ 40.0f, 40.0f, 0.0f });	// サイズ

		// 終了
		return false;
	}

	return true;
}