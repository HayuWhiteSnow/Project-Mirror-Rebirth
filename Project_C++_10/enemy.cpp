//============================================================================
// 
// エネミー [enemy.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "enemy.h"
#include "manager.h"
#include "bullet.h"
#include "particle.h"

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
const float CEnemy::MAX_VELOCITY = 2.0f;	// 加速度上限
const float CEnemy::BRAKING_FORCE = 0.9f;	// 制動力

//============================================================================
// コンストラクタ
//============================================================================
CEnemy::CEnemy() : CObject2D(static_cast<int>(LAYER::BACK_MIDDLE))
{
	m_velocity = { 0.0f, 0.0f, 0.0f };	// 加速度
	m_posTarget = { 0.0f, 0.0f, 0.0f };	// 目標位置
	m_rotTarget = { 0.0f, 0.0f, 0.0f };	// 目標向き
}

//============================================================================
// デストラクタ
//============================================================================
CEnemy::~CEnemy()
{
	// パーティクルを生成
	for (int i = 0; i < 10; i++)
	{
		CParticle::Create(
			CObject2D::GetPos(),	// 中心位置
			CObject2D::GetSize(),	// サイズ
			atan2f((float)(rand() % 314), (float)(rand() % 314)) * (rand() % 314));	// 飛ぶ角度
	}
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CEnemy::Init()
{
	// 基底クラスの初期設定
	HRESULT hr = CObject2D::Init();

	return hr;
}

//============================================================================
// 終了処理
//============================================================================
void CEnemy::Uninit()
{
	// 基底クラスの終了処理
	CObject2D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CEnemy::Update()
{
	// 現在位置を取得、以降このコピーを目標位置として変更を加えていく
	m_posTarget = CObject2D::GetPos();

	// 移動
	Translation();

	// 制動調整
	Braking();

	// 位置を調整、この処理の終わりに目標位置を反映させる
	AdjustPos();

	// 基底クラスの更新
	CObject2D::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CEnemy::Draw()
{
	// 基底クラスの描画処理
	CObject2D::Draw();
}

//============================================================================
// 生成
//============================================================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// インスタンスを生成
	CEnemy* pEnemy = new CEnemy;

	if (pEnemy == nullptr)
	{ // 生成失敗
		assert(false);
	}

	pEnemy->SetType(TYPE::ENEMY);	// タイプを設定

	pEnemy->Init();			// 基底クラスの初期設定
	pEnemy->SetPos(pos);	// 中心位置の設定
	pEnemy->SetSize(size);	// サイズの設定

	// テクスチャを設定
	pEnemy->BindTex(CManager::GetRenderer()->GetTextureInstane()->GetTexture(CTexture::TEX_TYPE::ENEMY_000));

	return pEnemy;
}

//============================================================================
// 移動
//============================================================================
void CEnemy::Translation()
{
	// 移動
	m_velocity.x += 5.0f;
	m_velocity.y += -0.0f;
}

//============================================================================
// 制動調整
//============================================================================
void CEnemy::Braking()
{
	// 加速度上限に到達で速度固定
	if (m_velocity.x > CEnemy::MAX_VELOCITY)
	{
		m_velocity.x = CEnemy::MAX_VELOCITY;
	}
	else if (m_velocity.x < -CEnemy::MAX_VELOCITY)
	{
		m_velocity.x = -CEnemy::MAX_VELOCITY;
	}

	if (m_velocity.y > CEnemy::MAX_VELOCITY)
	{
		m_velocity.y = CEnemy::MAX_VELOCITY;
	}
	else if (m_velocity.y < -CEnemy::MAX_VELOCITY)
	{
		m_velocity.y = -CEnemy::MAX_VELOCITY;
	}

	// 少しずつ加速度を失う
	m_velocity = m_velocity * CEnemy::BRAKING_FORCE;
}

//============================================================================
// 位置調整
//============================================================================
void CEnemy::AdjustPos()
{
	// 加速度分位置を変動
	m_posTarget += m_velocity;

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

	// 画面の上下端に到達でそれぞれループ
	if (m_posTarget.y - fSize.y > SCREEN_HEIGHT)
	{
		// 位置を上端に設定
		m_posTarget.y = 0.0f - fSize.y;
	}
	else if (m_posTarget.y + fSize.y < 0.0f)
	{
		// 位置を下端に設定
		m_posTarget.y = SCREEN_HEIGHT + fSize.y;
	}

	// 中心位置情報を設定
	CObject2D::SetPos(m_posTarget);
}
