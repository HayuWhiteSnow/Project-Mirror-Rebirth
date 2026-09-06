//============================================================================
// 
// プレイヤー [player.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player.h"
#include "manager.h"
#include "block.h"
#include "bullet.h"
#include "enemy.h"
#include "item.h"
#include "particle.h"

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
const int CPlayer::TEXTURE_DIVIDE_U = 30;		// テクスチャのU方向分割数
const int CPlayer::TEXTURE_DIVIDE_V = 1;		// テクスチャのV方向分割数
const float CPlayer::MAX_VELOCITY = 3.0f;		// 加速度上限
const float CPlayer::JUMP_FORCE = -10.;			// ジャンプ力
const float CPlayer::BRAKING_FORCE = 0.9f;		// 制動力

//============================================================================
// コンストラクタ
//============================================================================
CPlayer::CPlayer() : CObject2D(static_cast<int>(LAYER::FRONT_MIDDLE))
{
	m_nCntTexChange = 0;					// テクスチャ変更管理
	m_nLeftNumJump = 0;						// ジャンプ可能回数
	m_velocity = { 0.0f, 0.0f, 0.0f };		// 加速度
	m_posTarget = { 0.0f, 0.0f, 0.0f };		// 目標位置
	m_rotTarget = { 0.0f, 0.0f, 0.0f };		// 目標向き

	m_bAnimationLock = false;	// ( 試験的 )
}

//============================================================================
// デストラクタ
//============================================================================
CPlayer::~CPlayer()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CPlayer::Init()
{
	// 基底クラスの初期設定
	HRESULT hr = CObject2D::Init();

	return hr;
}

//============================================================================
// 終了処理
//============================================================================
void CPlayer::Uninit()
{
	// 基底クラスの終了処理
	CObject2D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer::Update()
{
	// 現在位置を取得、以降このコピーを目標位置として変更を加えていく
	m_posTarget = CObject2D::GetPos();

	// 操作
	Control();

	// 制動調整
	Braking();

	// 重力加速
	GravityFall();

	// 位置を調整、この処理の終わりに目標位置を反映させる
	AdjustPos();

	// アニメーション
	Animation();

	// 基底クラスの更新
	CObject2D::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CPlayer::Draw()
{
	// 基底クラスの描画処理
	CObject2D::Draw();
}

//============================================================================
// 生成
//============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// インスタンスを生成
	CPlayer* pPlayer = new CPlayer;

	if (pPlayer == nullptr)
	{ // 生成失敗
		assert(false);
	}

	pPlayer->SetType(TYPE::PLAYER);	// タイプを設定

	pPlayer->Init();		// 基底クラスの初期設定
	pPlayer->SetPos(pos);	// 中心位置の設定
	pPlayer->SetSize(size);	// サイズの設定

	pPlayer->SetTexWidth(1.0f / (float)CPlayer::TEXTURE_DIVIDE_U);	// 横テクスチャ分割幅
	pPlayer->SetTexHeight(1.0f / (float)CPlayer::TEXTURE_DIVIDE_V);	// 縦テクスチャ分縦幅

	// テクスチャを設定
	pPlayer->BindTex(CManager::GetRenderer()->GetTextureInstane()->GetTexture(CTexture::TEX_TYPE::PLAYER_000));

	return pPlayer;
}

//============================================================================
// 操作
//============================================================================
void CPlayer::Control()
{
	// 左スティック取得
	CInputPad* pPad = CManager::GetPad();
	CInputPad::JOYSTICK Stick = pPad->GetJoyStickL();

	// 入力があれば移動
	if (Stick.X != 0 || Stick.Y != 0)
	{
		// 移動量と目標回転量を設定
		m_velocity.x += sinf(atan2f((float)Stick.X, (float)-Stick.Y));
	}

	// キーボード取得
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();

	// 移動方向用
	bool bMove = 0;
	float X = 0.0f;
	float Y = 0.0f;

	// Y軸
	if (pKeyboard->GetPress(DIK_W))
	{
		Y = -1.0f;
	}
	else if (pKeyboard->GetPress(DIK_S))
	{
		Y = 1.0f;
	}

	// X軸
	if (pKeyboard->GetPress(DIK_A))
	{
		X = -1.0f;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{
		X = 1.0f;
	}

	// 何か入力していれば移動判定を出す
	if (X != 0.0f || Y != 0.0f)
	{
		bMove = true;
	}

	if (bMove)
	{
		// 移動量と目標回転量を設定
		m_velocity.x += sinf(atan2f(X, Y));
	}

	// ジャンプ
	if (pKeyboard->GetTrigger(DIK_SPACE) || pPad->GetTrigger(CInputPad::JOYKEY::X))
	{
		if (m_nLeftNumJump > 0)
		{ // ジャンプ可能回数が残っていれば

			// Y軸方向の加速度を上方向へ固定
			m_velocity.y = CPlayer::JUMP_FORCE;

			// ジャンプ可能回数を減少
			m_nLeftNumJump--;
		}
	}

	// デバッグ用にサウンド再生 (キーボード、パッド取得があるのでここで)
	if (pKeyboard->GetTrigger(DIK_RETURN) || pPad->GetTrigger(CInputPad::JOYKEY::B))
	{
		CSound* pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_00);

		// おまけに弾の生成
		CBullet::Create(
			m_posTarget,			// 中心位置
			{ 15.0f, 15.0f, 0.0f },	// サイズ
			100,					// 使用期間
			0.0f);					// 飛ぶ角度
	}
}

////============================================================================
//// 拡縮
////============================================================================
//void CPlayer::Scaling()
//{
//
//}
//
////============================================================================
//// 回転
////============================================================================
//void CPlayer::Rotation()
//{
//	//// 向き情報取得
//	//D3DXVECTOR3 rot = CObject2D::GetRot();
//
//	//// ブレーキ力
//	//float fStopEnergy = 0.1f;
//
//	//// 回転反映と回転量の減衰
//	//if (m_rotTarget.z - rot.z > D3DX_PI)
//	//{
//	//	rot.z += ((m_rotTarget.z - rot.z) * fStopEnergy + (D3DX_PI * 1.8f));
//	//}
//	//else if (m_rotTarget.z - rot.z < -D3DX_PI)
//	//{
//	//	rot.z += ((m_rotTarget.z - rot.z) * fStopEnergy + (D3DX_PI * -1.8f));
//	//}
//	//else
//	//{
//	//	rot.z += ((m_rotTarget.z - rot.z) * fStopEnergy);
//	//}
//
//	//// 向き情報設定
//	//CObject2D::SetRot(rot);
//}
//
////============================================================================
//// 移動
////============================================================================
//void CPlayer::Translation()
//{
//
//}

//============================================================================
// 制動調整
//============================================================================
void CPlayer::Braking()
{
	// 加速度上限に到達で速度固定
	if (m_velocity.x > CPlayer::MAX_VELOCITY)
	{
		m_velocity.x = CPlayer::MAX_VELOCITY;
	}
	else if (m_velocity.x < -CPlayer::MAX_VELOCITY)
	{
		m_velocity.x = -CPlayer::MAX_VELOCITY;
	}

	// 少しずつX軸方向への加速度を失う
	m_velocity.x = m_velocity.x * CPlayer::BRAKING_FORCE;
}

//============================================================================
// 重力落下
//============================================================================
void CPlayer::GravityFall()
{
	// 重力分、下方向への加速度増加
	m_velocity.y = m_velocity.y + CObject::GRAVITY_FORCE;
}

//============================================================================
// 位置調整
//============================================================================
void CPlayer::AdjustPos()
{
	// 加速度分位置を変動
	m_posTarget += m_velocity;

	// 当たり判定
	Collision();

	// サイズを取得
	D3DXVECTOR3 fSize = CObject2D::GetSize();

	// 画面の左右端に到達でそれぞれループ
	if (m_posTarget.x - fSize.x > SCREEN_WIDTH)
	{
		// 位置を左端へ設定
		m_posTarget.x = 0.0f - fSize.x;
	}
	else if (m_posTarget.x + fSize.x < 0.0f)
	{
		// 位置を右端へ設定
		m_posTarget.x = SCREEN_WIDTH + fSize.x;
	}

	// 画面の下端に到達で下降制限
	if (m_posTarget.y + fSize.y > SCREEN_HEIGHT)
	{
		// ジャンプ可能回数を設定
		m_nLeftNumJump = 2;

		// 位置を下端に設定
		m_posTarget.y = SCREEN_HEIGHT - fSize.y;

		// Y軸方向の加速度をリセット
		m_velocity.y = 0.0f;
	}

	// 中心位置情報を設定
	CObject2D::SetPos(m_posTarget);
}

//============================================================================
// 当たり判定
//============================================================================
void CPlayer::Collision()
{
	D3DXVECTOR3 copy = m_posTarget;

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

			if (pObject->GetType() == CObject::TYPE::BLOCK)
			{ // ブロックタイプなら

				// オブジェクトクラスをブロッククラスへダウンキャスト
				CBlock* pBlock = dynamic_cast<CBlock*>(pObject);

				if (pBlock == nullptr)
				{ // ダウンキャスト失敗
					assert(false);
				}

#if 0
				// ブロックと衝突する場合
				if (m_posTarget.x + CObject2D::GetSize().x >= pBlock->GetPos().x - pBlock->GetSize().x &&
					m_posTarget.x - CObject2D::GetSize().x <= pBlock->GetPos().x + pBlock->GetSize().x &&
					m_posTarget.y + CObject2D::GetSize().y >= pBlock->GetPos().y - pBlock->GetSize().y &&
					m_posTarget.y - CObject2D::GetSize().y <= pBlock->GetPos().y + pBlock->GetSize().y)
				{
					// お互いの距離を算出
					D3DXVECTOR3 dist = pBlock->GetPos() - CObject2D::GetPos();

					// 衝突前からその軸方向にいる場合
					if (dist.x * dist.x > dist.y * dist.y)
					{
						if (CObject2D::GetPos().x < pBlock->GetPos().x)
						{
							// 位置をこのブロックの左端に設定
							m_posTarget.x = -CObject2D::GetSize().x + (pBlock->GetPos().x - pBlock->GetSize().x);
						}
						else if (CObject2D::GetPos().x > pBlock->GetPos().x)
						{
							// 位置をこのブロックの右端に設定
							m_posTarget.x = CObject2D::GetSize().x + (pBlock->GetPos().x + pBlock->GetSize().x);
						}
					}
					else
					{
						if (CObject2D::GetPos().y < pBlock->GetPos().y)
						{
							// ジャンプ可能回数を設定
							m_nLeftNumJump = 2;

							// 位置をこのブロックの上端に設定
							m_posTarget.y = -CObject2D::GetSize().y + (pBlock->GetPos().y - pBlock->GetSize().y);

							// Y軸方向の加速度をリセット
							m_velocity.y = 0.0f;
						}
						else if (CObject2D::GetPos().y > pBlock->GetPos().y)
						{
							// 位置をこのブロックの下端に設定
							m_posTarget.y = CObject2D::GetSize().y + (pBlock->GetPos().y + pBlock->GetSize().y);
						}
					}
				}
#else
				// ブロックと衝突する場合
				if (copy.x + CObject2D::GetSize().x >= pBlock->GetPos().x - pBlock->GetSize().x &&
					copy.x - CObject2D::GetSize().x <= pBlock->GetPos().x + pBlock->GetSize().x &&
					copy.y + CObject2D::GetSize().y >= pBlock->GetPos().y - pBlock->GetSize().y &&
					copy.y - CObject2D::GetSize().y <= pBlock->GetPos().y + pBlock->GetSize().y)
				{
					// 過去の位置がどちらかの軸方向に重なっていたかで処理分岐
					if (CObject2D::GetPos().x + CObject2D::GetSize().x > pBlock->GetPos().x - pBlock->GetSize().x &&
						CObject2D::GetPos().x - CObject2D::GetSize().x < pBlock->GetPos().x + pBlock->GetSize().x)
					{
						if (CObject2D::GetPos().y < pBlock->GetPos().y)
						{
							// ジャンプ可能回数を設定
							m_nLeftNumJump = 2;

							// 位置をこのブロックの上端に設定
							m_posTarget.y = -CObject2D::GetSize().y + (pBlock->GetPos().y - pBlock->GetSize().y);
						}
						else if (CObject2D::GetPos().y > pBlock->GetPos().y)
						{
							// 位置をこのブロックの下端に設定
							m_posTarget.y = CObject2D::GetSize().y + (pBlock->GetPos().y + pBlock->GetSize().y);
						}

						// Y軸方向の加速度をリセット
						m_velocity.y = 0.0f;
					}
					else if (CObject2D::GetPos().y + CObject2D::GetSize().y > pBlock->GetPos().y - pBlock->GetSize().y &&
						CObject2D::GetPos().y - CObject2D::GetSize().y < pBlock->GetPos().y + pBlock->GetSize().y)
					{
						if (CObject2D::GetPos().x < pBlock->GetPos().x)
						{
							// 位置をこのブロックの左端に設定
							m_posTarget.x = -CObject2D::GetSize().x + (pBlock->GetPos().x - pBlock->GetSize().x);
						}
						else if (CObject2D::GetPos().x > pBlock->GetPos().x)
						{
							// 位置をこのブロックの右端に設定
							m_posTarget.x = CObject2D::GetSize().x + (pBlock->GetPos().x + pBlock->GetSize().x);
						}

						// X軸方向の加速度をリセット
						m_velocity.x = 0.0f;
					}
				}
#endif
			}
			//else if (pObject->GetType() == CObject::TYPE::ENEMY)
			//{ // エネミータイプなら

			//	// オブジェクトクラスをブロッククラスへダウンキャスト
			//	CEnemy* pEnemy = dynamic_cast<CEnemy*>(pObject);

			//	if (pEnemy == nullptr)
			//	{ // ダウンキャスト失敗
			//		assert(false);
			//	}

			//	// テスト
			//	auto てすと = pEnemy->GetVelocity();

			//	// エネミーと衝突する場合
			//	if (m_posTarget.x + CObject2D::GetSize().x >= pEnemy->GetPos().x - pEnemy->GetSize().x &&
			//		m_posTarget.x - CObject2D::GetSize().x <= pEnemy->GetPos().x + pEnemy->GetSize().x &&
			//		m_posTarget.y + CObject2D::GetSize().y >= pEnemy->GetPos().y - pEnemy->GetSize().y &&
			//		m_posTarget.y - CObject2D::GetSize().y <= pEnemy->GetPos().y + pEnemy->GetSize().y)
			//	{
			//		// 過去の位置がどちらかの軸方向に重なっていたかで処理分岐
			//		if (CObject2D::GetPos().x + CObject2D::GetSize().x > pEnemy->GetPos().x - pEnemy->GetSize().x &&
			//			CObject2D::GetPos().x - CObject2D::GetSize().x < pEnemy->GetPos().x + pEnemy->GetSize().x)
			//		{
			//			if (CObject2D::GetPos().y < pEnemy->GetPos().y)
			//			{
			//				// ジャンプ可能回数を設定
			//				m_nLeftNumJump = 2;

			//				m_posTarget.x += てすと.x;

			//				// 位置をこのブロックの上端に設定
			//				m_posTarget.y = -CObject2D::GetSize().y + (pEnemy->GetPos().y - pEnemy->GetSize().y);

			//				// Y軸方向の加速度をリセット
			//				m_velocity.y = 0.0f;
			//			}
			//			else if (CObject2D::GetPos().y > pEnemy->GetPos().y)
			//			{
			//				// 位置をこのブロックの下端に設定
			//				m_posTarget.y = CObject2D::GetSize().y + (pEnemy->GetPos().y + pEnemy->GetSize().y);
			//			}
			//		}
			//		else if (CObject2D::GetPos().y + CObject2D::GetSize().y > pEnemy->GetPos().y - pEnemy->GetSize().y - てすと.y &&
			//			CObject2D::GetPos().y - CObject2D::GetSize().y < pEnemy->GetPos().y + pEnemy->GetSize().y - てすと.y)
			//		{
			//			if (CObject2D::GetPos().x < pEnemy->GetPos().x)
			//			{
			//				// 位置をこのブロックの左端に設定
			//				m_posTarget.x = -CObject2D::GetSize().x + (pEnemy->GetPos().x - pEnemy->GetSize().x);
			//			}
			//			else if (CObject2D::GetPos().x > pEnemy->GetPos().x)
			//			{
			//				// 位置をこのブロックの右端に設定
			//				m_posTarget.x = CObject2D::GetSize().x + (pEnemy->GetPos().x + pEnemy->GetSize().x);
			//			}
			//		}

			//		int aa = 0;
			//	}
			//}
			else if (pObject->GetType() == CObject::TYPE::ITEM)
			{ // アイテムタイプなら

				// オブジェクトクラスをアイテムクラスへダウンキャスト
				CItem* pItem = dynamic_cast<CItem*>(pObject);

				if (pItem == nullptr)
				{ // ダウンキャスト失敗
					assert(false);
				}

				// アイテムと衝突する場合
				if (m_posTarget.x + CObject2D::GetSize().x >= pItem->GetPos().x - pItem->GetSize().x &&
					m_posTarget.x - CObject2D::GetSize().x <= pItem->GetPos().x + pItem->GetSize().x &&
					m_posTarget.y + CObject2D::GetSize().y >= pItem->GetPos().y - pItem->GetSize().y &&
					m_posTarget.y - CObject2D::GetSize().y <= pItem->GetPos().y + pItem->GetSize().y)
				{
					// パーティクルを生成
					for (int i = 0; i < 10; i++)
					{
						CParticle::Create(
							m_posTarget,			// 中心位置
							pItem->GetSize(),	// サイズ
							atan2f((float)(rand() % 314), (float)(rand() % 314)) * (rand() % 314));	// 飛ぶ角度
					}

					// 試験的にテクスチャを変更
					LPDIRECT3DTEXTURE9 pTex = CManager::GetRenderer()->GetTextureInstane()->GetTexture(CTexture::TEX_TYPE::PLAYER_001);

					if (pTex == nullptr)
					{ // 取得失敗
						assert(false);
					}

					// テクスチャを設定
					CObject2D::BindTex(pTex);

					// アニメーションをロック
					m_bAnimationLock = true;

					// アイテムを破棄
					pObject->Release();
				}
			}
		}
	}
}

//============================================================================
// アニメーション
//============================================================================
void CPlayer::Animation()
{
	if (!m_bAnimationLock)
	{
		// テクスチャ変更管理カウントアップ
		m_nCntTexChange++;

		if (m_nCntTexChange >= 15)
		{ // 一定時間経過により

			// 横テクスチャ種類情報取得
			int nTexPatternU = CObject2D::GetNowPatternU();

			// 横テクスチャ種類変更
			nTexPatternU++;

			if (nTexPatternU >= CPlayer::TEXTURE_DIVIDE_U)
			{ // U方向の分割数を超えると

				// テクスチャパターンをリセット
				nTexPatternU = 0;
			}

			// 横テクスチャ種類情報設定
			CObject2D::SetNowPatternU(nTexPatternU);

			// 変更管理カウントをリセット
			m_nCntTexChange = 0;
		}
	}
	else
	{
		// 一旦、強制的にテクスチャサイズを設定
		CObject2D::SetTexWidth(1.0f);
		CObject2D::SetNowPatternU(1);
	}
}