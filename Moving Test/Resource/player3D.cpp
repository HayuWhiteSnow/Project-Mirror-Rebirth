//============================================================================
// 
// 3Dプレイヤー [player3D.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player3D.h"
#include "manager.h"
#include "block3D.h"
#include "explosion3D.h"

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
const float CPlayer3D::MAX_VELOCITY = 0.5f;		// 加速度上限
const float CPlayer3D::JUMP_FORCE = 6.0f;			// ジャンプ力
const float CPlayer3D::BRAKING_FORCE = 0.9f;	// 制動力

//============================================================================
// コンストラクタ
//============================================================================
CPlayer3D::CPlayer3D() : CObjectX(static_cast<int>(LAYER::FRONT_MIDDLE))
{
	m_nLeftNumJump = 0;						// ジャンプ可能回数
	m_velocity = { 0.0f, 0.0f, 0.0f };		// 加速度
	m_posTarget = { 0.0f, 0.0f, 0.0f };		// 目標位置
	m_rotTarget = { 0.0f, 0.0f, 0.0f };		// 目標向き

	m_nTestExplosionCnt = 0;	// これはテスト用です
}

//============================================================================
// デストラクタ
//============================================================================
CPlayer3D::~CPlayer3D()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CPlayer3D::Init()
{
	// 基底クラスの初期設定
	HRESULT hr = CObjectX::Init();

	return hr;
}

//============================================================================
// 終了処理
//============================================================================
void CPlayer3D::Uninit()
{
	// 基底クラスの終了処理
	CObjectX::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer3D::Update()
{
	// 現在位置を取得、以降このコピーを目標位置として変更を加えていく
	m_posTarget = CObjectX::GetPos();

	// 操作
	Control();

	// 回転
	Rotation();

	// 制動調整
	Braking();

	// 重力加速
	GravityFall();

	// 位置を調整、この処理の終わりに目標位置を反映させる
	AdjustPos();

	// 基底クラスの更新
	CObjectX::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CPlayer3D::Draw()
{
	// 基底クラスの描画処理
	CObjectX::Draw();
}

//============================================================================
// 生成
//============================================================================
CPlayer3D* CPlayer3D::Create(D3DXVECTOR3 pos)
{
	// インスタンスを生成
	CPlayer3D* pPlayer3D = DBG_NEW CPlayer3D;

	if (pPlayer3D == nullptr)
	{ // 生成失敗
		assert(false);
	}

	pPlayer3D->SetType(TYPE::PLAYER);	// タイプを設定

	pPlayer3D->Init();			// 基底クラスの初期設定
	pPlayer3D->SetPos(pos);		// 中心位置の設定

	// モデルを設定
	pPlayer3D->BindModel(CManager::GetRenderer()->GetModelInstane()->GetModel(CModel::MODEL_TYPE::MODEL_PLAYER));

	return pPlayer3D;
}

//============================================================================
// 操作
//============================================================================
void CPlayer3D::Control()
{
	// 左スティック取得
	CInputPad* pPad = CManager::GetPad();
	CInputPad::JOYSTICK Stick = pPad->GetJoyStickL();

	// 入力があれば移動
	if (Stick.X != 0 || Stick.Y != 0)
	{
		// 移動量と目標回転量を設定
		m_velocity.x += sinf(atan2f((float)Stick.X, (float)-Stick.Y));
		m_velocity.z += cosf(atan2f((float)Stick.X, (float)-Stick.Y));
		m_rotTarget.y = atan2f((float)-Stick.X, (float)Stick.Y);
	}

	// キーボード取得
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();

	// 移動方向用
	bool bMove = 0;
	float X = 0.0f;
	float Z = 0.0f;

	// X軸
	if (pKeyboard->GetPress(DIK_A))
	{
		X = -1.0f;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{
		X = 1.0f;
	}

	// Z軸
	if (pKeyboard->GetPress(DIK_W))
	{
		Z = 1.0f;
	}
	else if (pKeyboard->GetPress(DIK_S))
	{
		Z = -1.0f;
	}

	// 何か入力していれば移動判定を出す
	if (X != 0.0f || Z != 0.0f)
	{
		bMove = true;
	}
	else
	{ // テスト用
		m_nTestExplosionCnt = 19;
	}

	if (bMove)
	{
		// 移動量と目標回転量を設定
		m_velocity.x += sinf(atan2f(X, Z) + CManager::GetCamera()->GetRot().y) * 0.1f;
		m_velocity.z += cosf(atan2f(X, Z) + CManager::GetCamera()->GetRot().y) * 0.1f;
		m_rotTarget.y = atan2f(-X, -Z) + CManager::GetCamera()->GetRot().y;

		// テスト用
		m_nTestExplosionCnt > 20 ? m_nTestExplosionCnt = 0, (int)CExplosion3D::Create({ GetPos().x, GetPos().y + 3.0f, GetPos().z }, { 7.5f, 0.0f, 7.5f }) : m_nTestExplosionCnt++;
	}

	// ジャンプ
	if (pKeyboard->GetTrigger(DIK_SPACE))
	{
		if (m_nLeftNumJump > 0)
		{ // ジャンプ可能回数が残っていれば

			// Y軸方向の加速度を上方向へ固定
			m_velocity.y = JUMP_FORCE;

			// ジャンプ可能回数を減少
			m_nLeftNumJump--;
		}
	}
}

//============================================================================
// 回転
//============================================================================
void CPlayer3D::Rotation()
{
	// 向き情報取得
	D3DXVECTOR3 rot = GetRot();

	// ブレーキ力
	float fStopEnergy = 0.1f;

	// 回転反映と回転量の減衰
	if (m_rotTarget.y - rot.y > D3DX_PI)
	{
		rot.y += ((m_rotTarget.y - rot.y) * fStopEnergy + (D3DX_PI * 1.8f));
	}
	else if (m_rotTarget.y - rot.y < -D3DX_PI)
	{
		rot.y += ((m_rotTarget.y- rot.y) * fStopEnergy + (D3DX_PI * -1.8f));
	}
	else
	{
		rot.y += ((m_rotTarget.y - rot.y) * fStopEnergy);
	}

	// 向き情報設定
	SetRot(rot);
}

//============================================================================
// 制動調整
//============================================================================
void CPlayer3D::Braking()
{
	// 
	//// 加速度上限に到達で速度固定
	//if (m_velocity.x > CPlayer3D::MAX_VELOCITY)
	//{
	//	m_velocity.x = CPlayer3D::MAX_VELOCITY;
	//}
	//else if (m_velocity.x < -CPlayer3D::MAX_VELOCITY)
	//{
	//	m_velocity.x = -CPlayer3D::MAX_VELOCITY;
	//}

	//if (m_velocity.z > CPlayer3D::MAX_VELOCITY)
	//{
	//	m_velocity.z = CPlayer3D::MAX_VELOCITY;
	//}
	//else if (m_velocity.z < -CPlayer3D::MAX_VELOCITY)
	//{
	//	m_velocity.z = -CPlayer3D::MAX_VELOCITY;
	//}

	// 少しずつ加速度を失う
	m_velocity = m_velocity * CPlayer3D::BRAKING_FORCE;
}

//============================================================================
// 重力落下
//============================================================================
void CPlayer3D::GravityFall()
{
	// 重力分、下方向への加速度増加
	m_velocity.y = m_velocity.y - GRAVITY_FORCE;
}

//============================================================================
// 位置調整
//============================================================================
void CPlayer3D::AdjustPos()
{
	// 加速度分位置を変動
	m_posTarget += m_velocity;

	// 当たり判定
	Collision();

	// 画面の下端に到達で下降制限
	if (m_posTarget.y < 0.0f)
	{
		// ジャンプ可能回数を設定
		m_nLeftNumJump = 2;

		// 位置を下端に設定
		m_posTarget.y = 0.0f;

		// Y軸方向の加速度をリセット
		m_velocity.y = 0.0f;
	}

	// 位置を設定
	SetPos(m_posTarget);
}

//============================================================================
// 当たり判定
//============================================================================
void CPlayer3D::Collision()
{
	// ブロックサイズ
	float fHalfSizeBlock = 10.0f;

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

				// オブジェクトクラスを3Dブロッククラスへダウンキャスト
				CBlock3D* pBlock3D = dynamic_cast<CBlock3D*>(pObject);

				if (pBlock3D == nullptr)
				{ // ダウンキャスト失敗
					assert(false);
				}

				// ブロックと衝突する場合
				if (m_posTarget.x + fHalfSizeBlock >= pBlock3D->GetPos().x - fHalfSizeBlock &&
					m_posTarget.x - fHalfSizeBlock <= pBlock3D->GetPos().x + fHalfSizeBlock &&
					m_posTarget.z + fHalfSizeBlock >= pBlock3D->GetPos().z - fHalfSizeBlock &&
					m_posTarget.z - fHalfSizeBlock <= pBlock3D->GetPos().z + fHalfSizeBlock &&
					m_posTarget.y + fHalfSizeBlock >= pBlock3D->GetPos().y - fHalfSizeBlock &&
					m_posTarget.y - fHalfSizeBlock <= pBlock3D->GetPos().y + fHalfSizeBlock)
				{
					// 過去の位置で垂直幅に衝突していたのか判定
					if (GetPos().y + fHalfSizeBlock > pBlock3D->GetPos().y - fHalfSizeBlock &&
						GetPos().y - fHalfSizeBlock < pBlock3D->GetPos().y + fHalfSizeBlock)
					{
						// 過去の位置がどちらかの軸方向に重なっていたかで処理分岐
						if (GetPos().x + fHalfSizeBlock > pBlock3D->GetPos().x - fHalfSizeBlock &&
							GetPos().x - fHalfSizeBlock < pBlock3D->GetPos().x + fHalfSizeBlock)
						{
							if (GetPos().z < pBlock3D->GetPos().z)
							{
								// 位置をこのブロックの後端に設定
								m_posTarget.z = -fHalfSizeBlock + (pBlock3D->GetPos().z - fHalfSizeBlock);
							}
							else if (GetPos().z > pBlock3D->GetPos().z)
							{
								// 位置をこのブロックの前端に設定
								m_posTarget.z = fHalfSizeBlock + (pBlock3D->GetPos().z + fHalfSizeBlock);
							}

							// Z軸方向の加速度をリセット
							m_velocity.z = 0.0f;
						}
						else if (GetPos().z + fHalfSizeBlock > pBlock3D->GetPos().z - fHalfSizeBlock &&
							GetPos().z - fHalfSizeBlock < pBlock3D->GetPos().z + fHalfSizeBlock)
						{
							if (GetPos().x < pBlock3D->GetPos().x)
							{
								// 位置をこのブロックの左端に設定
								m_posTarget.x = -fHalfSizeBlock + (pBlock3D->GetPos().x - fHalfSizeBlock);
							}
							else if (GetPos().x > pBlock3D->GetPos().x)
							{
								// 位置をこのブロックの右端に設定
								m_posTarget.x = fHalfSizeBlock + (pBlock3D->GetPos().x + fHalfSizeBlock);
							}

							// X軸方向の加速度をリセット
							m_velocity.x = 0.0f;
						}
					}
					else if(GetPos().x + fHalfSizeBlock > pBlock3D->GetPos().x - fHalfSizeBlock &&
						GetPos().x - fHalfSizeBlock < pBlock3D->GetPos().x + fHalfSizeBlock &&
						GetPos().z + fHalfSizeBlock > pBlock3D->GetPos().z - fHalfSizeBlock &&
						GetPos().z - fHalfSizeBlock < pBlock3D->GetPos().z + fHalfSizeBlock)
					{
						if (GetPos().y < pBlock3D->GetPos().y)
						{
							// 位置をこのブロックの下端に設定
							m_posTarget.y = -fHalfSizeBlock + (pBlock3D->GetPos().y - fHalfSizeBlock);
						}
						else if (GetPos().y > pBlock3D->GetPos().y)
						{
							// 位置をこのブロックの上端に設定
							m_posTarget.y = fHalfSizeBlock + (pBlock3D->GetPos().y + fHalfSizeBlock);

							// ジャンプ可能回数を設定
							m_nLeftNumJump = 2;
						}

						// Y軸方向の加速度をリセット
						m_velocity.y = 0.0f;
					}
				}
			}
		}
	}
}