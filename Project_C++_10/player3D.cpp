//============================================================================
// 
// プレイヤー3D [player3D.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player3D.h"
#include "manager.h"

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
const float CPlayer3D::MAX_VELOCITY = 0.5f;		// 加速度上限
const float CPlayer3D::BRAKING_FORCE = 0.95f;	// 制動力

//============================================================================
// コンストラクタ
//============================================================================
CPlayer3D::CPlayer3D() : CObjectX(static_cast<int>(LAYER::FRONT))
{
	m_velocity = { 0.0f, 0.0f, 0.0f };		// 加速度
	m_posTarget = { 0.0f, 0.0f, 0.0f };		// 目標位置
	m_rotTarget = { 0.0f, 0.0f, 0.0f };		// 目標向き
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
	CPlayer3D* pPlayer3D = new CPlayer3D;

	if (pPlayer3D == nullptr)
	{ // 生成失敗
		assert(false);
	}

	pPlayer3D->SetType(TYPE::PLAYER);	// タイプを設定

	pPlayer3D->Init();			// 基底クラスの初期設定
	pPlayer3D->SetPos(pos);		// 中心位置の設定

	// モデルを設定
	pPlayer3D->BindModel(CManager::GetRenderer()->GetModelInstane()->GetModel(CModel::MODEL_TYPE::MODEL_000));

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

	if (bMove)
	{
		// 移動量と目標回転量を設定
		m_velocity.x += sinf(atan2f(X, Z));
		m_velocity.z += cosf(atan2f(X, Z));
		m_rotTarget.y = atan2f(-X, -Z);
	}
}

//============================================================================
// 回転
//============================================================================
void CPlayer3D::Rotation()
{
	// 向き情報取得
	D3DXVECTOR3 rot = CObjectX::GetRot();

	// ブレーキ力
	float fStopEnergy = 0.05f;

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
	CObjectX::SetRot(rot);
}

//============================================================================
// 制動調整
//============================================================================
void CPlayer3D::Braking()
{
	// 加速度上限に到達で速度固定
	if (m_velocity.x > CPlayer3D::MAX_VELOCITY)
	{
		m_velocity.x = CPlayer3D::MAX_VELOCITY;
	}
	else if (m_velocity.x < -CPlayer3D::MAX_VELOCITY)
	{
		m_velocity.x = -CPlayer3D::MAX_VELOCITY;
	}

	if (m_velocity.z > CPlayer3D::MAX_VELOCITY)
	{
		m_velocity.z = CPlayer3D::MAX_VELOCITY;
	}
	else if (m_velocity.z < -CPlayer3D::MAX_VELOCITY)
	{
		m_velocity.z = -CPlayer3D::MAX_VELOCITY;
	}

	// 少しずつ加速度を失う
	m_velocity = m_velocity * CPlayer3D::BRAKING_FORCE;
}

//============================================================================
// 位置調整
//============================================================================
void CPlayer3D::AdjustPos()
{
	// 加速度分位置を変動
	m_posTarget += m_velocity;

	// 中心位置情報を設定
	CObjectX::SetPos(m_posTarget);
}