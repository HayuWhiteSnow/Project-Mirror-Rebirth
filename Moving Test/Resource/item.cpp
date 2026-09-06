//============================================================================
// 
// アイテム [item.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "item.h"
#include "manager.h"
#include "block.h"
#include "particle.h"

//============================================================================
// コンストラクタ
//============================================================================
CItem::CItem() : CObject2D(static_cast<int>(LAYER::BACK_MIDDLE))
{
	m_velocity = { 0.0f, 0.0f, 0.0f };	// 加速度
	m_pos_tgt = { 0.0f, 0.0f, 0.0f };	// 目標位置
}

//============================================================================
// デストラクタ
//============================================================================
CItem::~CItem()
{
	// パーティクルを生成
	for (int i = 0; i < 10; i++)
	{
		CParticle::Create(
			GetPos(),	// 中心位置
			GetSize(),	// サイズ
			atan2f((float)(rand() % 314), (float)(rand() % 314)) * (rand() % 314));	// 飛ぶ角度
	}
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CItem::Init()
{
	// 基底クラスの初期設定
	HRESULT hr = CObject2D::Init();

	return hr;
}

//============================================================================
// 終了処理
//============================================================================
void CItem::Uninit()
{
	// 基底クラスの終了処理
	CObject2D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CItem::Update()
{
	// 現在位置を取得、以降このコピーを目標位置として変更を加えていく
	m_pos_tgt = GetPos();

	// 移動
	Translation();

	// 重力加速
	GravityFall();

	// 位置を調整、この処理の終わりに目標位置を反映させる
	AdjustPos();

	// 基底クラスの更新
	CObject2D::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CItem::Draw()
{
	// 基底クラスの描画処理
	CObject2D::Draw();
}

//============================================================================
// 生成
//============================================================================
CItem* CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// インスタンスを生成
	CItem* pItem = DBG_NEW CItem;

	if (pItem == nullptr)
	{ // 生成失敗
		assert(false);
	}

	pItem->SetType(TYPE::ITEM);	// タイプを設定

	pItem->Init();			// 基底クラスの初期設定
	pItem->SetPos(pos);		// 中心位置の設定
	pItem->SetSize(size);	// サイズの設定

	// テクスチャを設定
	pItem->BindTex(CManager::GetRenderer()->GetTextureInstane()->GetTexture(CTexture::TEX_TYPE::ITEM_000));

	return pItem;
}

//============================================================================
// 移動
//============================================================================
void CItem::Translation()
{
	// 仮の移動
	m_pos_tgt.x += -0.5f;
}

//============================================================================
// 重力落下
//============================================================================
void CItem::GravityFall()
{
	// 重力分、下方向への加速度増加
	m_velocity.y = m_velocity.y + GRAVITY_FORCE;
}

//============================================================================
// 位置調整
//============================================================================
void CItem::AdjustPos()
{
	// 加速度分位置を変動
	m_pos_tgt += m_velocity;

	// 当たり判定
	Collision();

	// サイズを取得
	D3DXVECTOR3 fSize = GetSize();

	// 画面の左右端に到達でそれぞれループ
	if (m_pos_tgt.x - fSize.x > SCREEN_WIDTH)
	{
		// 位置を左端へ設定
		m_pos_tgt.x = 0.0f - fSize.x;
	}
	else if (m_pos_tgt.x + fSize.x < 0.0f)
	{
		// 位置を右端へ設定
		m_pos_tgt.x = SCREEN_WIDTH + fSize.x;
	}

	// 画面の下端に到達で下降制限
	if (m_pos_tgt.y + fSize.y > SCREEN_HEIGHT)
	{
		// 位置を下端に設定
		m_pos_tgt.y = SCREEN_HEIGHT - fSize.y;

		// Y軸方向の加速度をリセット
		m_velocity.y = 0.0f;
	}

	// 位置を設定
	SetPos(m_pos_tgt);
}

//============================================================================
// 当たり判定
//============================================================================
void CItem::Collision()
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

			if (pObject->GetType() != CObject::TYPE::BLOCK)
			{ // ブロックタイプ以外はコンティニュー
				continue;
			}

			// オブジェクトクラスをブロッククラスへダウンキャスト
			CBlock* pBlock = dynamic_cast<CBlock*>(pObject);

			if (pBlock == nullptr)
			{ // ダウンキャスト失敗
				assert(false);
			}

			// ブロックと衝突する場合
			if (m_pos_tgt.x + GetSize().x >= pBlock->GetPos().x - pBlock->GetSize().x &&
				m_pos_tgt.x - GetSize().x <= pBlock->GetPos().x + pBlock->GetSize().x &&
				m_pos_tgt.y + GetSize().y >= pBlock->GetPos().y - pBlock->GetSize().y &&
				m_pos_tgt.y - GetSize().y <= pBlock->GetPos().y + pBlock->GetSize().y)
			{
				// 過去の位置がどちらかの軸方向に重なっていたかで処理分岐
				if (GetPos().x + GetSize().x > pBlock->GetPos().x - pBlock->GetSize().x &&
					GetPos().x - GetSize().x < pBlock->GetPos().x + pBlock->GetSize().x)
				{
					if (GetPos().y < pBlock->GetPos().y)
					{
						// 位置をこのブロックの上端に設定
						m_pos_tgt.y = -GetSize().y + (pBlock->GetPos().y - pBlock->GetSize().y);

						// Y軸方向の加速度をリセット
						m_velocity.y = 0.0f;
					}
					else if (GetPos().y > pBlock->GetPos().y)
					{
						// 位置をこのブロックの下端に設定
						m_pos_tgt.y = GetSize().y + (pBlock->GetPos().y + pBlock->GetSize().y);
					}
				}
				else
				{
					if (GetPos().x < pBlock->GetPos().x)
					{
						// 位置をこのブロックの左端に設定
						m_pos_tgt.x = -GetSize().x + (pBlock->GetPos().x - pBlock->GetSize().x);
					}
					else if (GetPos().x > pBlock->GetPos().x)
					{
						// 位置をこのブロックの右端に設定
						m_pos_tgt.x = GetSize().x + (pBlock->GetPos().x + pBlock->GetSize().x);
					}
				}
			}
		}
	}
}