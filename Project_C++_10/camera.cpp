//============================================================================
// 
// カメラ [camera.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************s
// インクルードファイル
//****************************************************
#include "camera.h"
#include "main.h"
#include "manager.h"

//============================================================================
// コンストラクタ
//============================================================================
CCamera::CCamera()
{
	m_pos = { 0.0f, 0.0f, 0.0f };			// 中心位置
	m_posTarget = { 0.0f, 0.0f, 0.0f };		// 目標中心位置
	m_posV = { 0.0f, 0.0f, 0.0f };			// 視点位置
	m_posTargetV = { 0.0f, 0.0f, 0.0f };	// 目標視点位置
	m_posR = { 0.0f, 0.0f, 0.0f };			// 注視点位置
	m_posTargetR = { 0.0f, 0.0f, 0.0f };	// 目標注視点位置
	m_rot = { 0.0f, 0.0f, 0.0f };			// 向き
	m_rotTarget = { 0.0f, 0.0f, 0.0f };		// 目標向き
	m_fDistance = 150.0f;					// 視点 -> 注視点間の距離
	m_vecU = { 0.0f, 1.0f, 0.0f };			// 上方向ベクトル
	D3DXMatrixIdentity(&m_mtxProjection);	// プロジェクションマトリックス
	D3DXMatrixIdentity(&m_mtxView);			// ビューマトリックス
}

//============================================================================
// デストラクタ
//============================================================================
CCamera::~CCamera()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CCamera::Init()
{
	return S_OK;
}

//============================================================================
// 更新処理
//============================================================================
void CCamera::Update()
{
	// カメラ操作
	Control();

	// 回転
	Rotation();

	// 移動
	Translation();

	// ヨー角の範囲を制限
	RestrictYaw();

	// ピッチ角の範囲を制限
	RestrictPitch();

	// 視点位置を算出
	CalcPosV();

	// 注視点位置を算出
	CalcPosR();
}

//============================================================================
// カメラをセット
//============================================================================
void CCamera::SetCamera()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CManager::GetRenderer()->GetDeviece();

	// 画面バッファクリア
	pDev->Clear(0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// プロジェクション行列を計算
	CalcMtxProjection();
	
	// ビュー行列を計算
	CalcMtxView();
}

//============================================================================
// カメラ操作
//============================================================================
void CCamera::Control()
{
	// ズームイン / アウト
	if (CManager::GetKeyboard()->GetPress(DIK_AT) && m_fDistance > 10.0f)
	{
		m_fDistance -= 10.0f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_COLON))
	{
		m_fDistance += 10.0f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_BACKSLASH))
	{
		// 距離間リセット
		m_fDistance = 200.0f;
	}

	// 左右
	if (CManager::GetKeyboard()->GetPress(DIK_RIGHT))
	{
		m_rotTarget.y += 0.025f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_LEFT))
	{
		m_rotTarget.y -= 0.025f;
	}

	// 上下
	if (CManager::GetKeyboard()->GetPress(DIK_UP))
	{
		m_rotTarget.x += 0.025f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_DOWN))
	{
		m_rotTarget.x -= 0.025f;
	}
}

//============================================================================
// 回転
//============================================================================
void CCamera::Rotation()
{
	// 回転量減衰
	m_rotTarget *= 0.8f;

	// 回転量反映
	m_rot += m_rotTarget * 0.5f;
}

//============================================================================
// 移動
//============================================================================
void CCamera::Translation()
{
	// 移動量減衰
	m_posTarget *= 0.8f;

	// 移動量反映
	m_pos += m_posTarget * 0.5f;
}

//============================================================================
// ヨー角の範囲を制限
//============================================================================
void CCamera::RestrictYaw()
{
	// 回転の制限
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y = D3DX_PI;
	}
}

//============================================================================
// ピッチ角の範囲を制限
//============================================================================
void CCamera::RestrictPitch()
{
	if (m_rot.x + m_rotTarget.x > D3DX_PI * 0.48f)
	{
		m_rot.x = D3DX_PI * 0.48f;
	}
	else if (m_rot.x + m_rotTarget.x < -D3DX_PI * 0.48f)
	{
		m_rot.x = -D3DX_PI * 0.48f;
	}
}

//============================================================================
// 視点位置を算出
//============================================================================
void CCamera::CalcPosV()
{
	// 視点は対象物の後方に
	m_posV =
	{
		m_pos.x - (sinf(m_rot.y) * cosf(m_rot.x) * (m_fDistance)),
		m_pos.y - (sinf(m_rot.x) * (m_fDistance)),
		m_pos.z - (cosf(m_rot.y) * cosf(m_rot.x) * (m_fDistance))
	};
}

//============================================================================
// 注視点位置を算出
//============================================================================
void CCamera::CalcPosR()
{
	// 注視点を対象物の前方に
	m_posR =
	{
		m_pos.x + (sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance),
		m_pos.y + (sinf(m_rot.x) * m_fDistance),
		m_pos.z + (cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance)
	};
}

//============================================================================
// プロジェクション行列計算
//============================================================================
void CCamera::CalcMtxProjection()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CManager::GetRenderer()->GetDeviece();

	// プロジェクション行列の初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクション行列を生成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		5000.0f);

	// プロジェクション行列の設定
	pDev->SetTransform(D3DTS_PROJECTION,
		&m_mtxProjection);
}

//============================================================================
// ビュー行列計算
//============================================================================
void CCamera::CalcMtxView()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CManager::GetRenderer()->GetDeviece();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// 視点位置の調整用
	D3DXVECTOR3 posV = m_posV;
	D3DXVECTOR3 posR = m_posR;

	// カメラを俯瞰気味に
	posV.y += 50.0f;

	// ビューマトリックスの生成
	D3DXMatrixLookAtLH(&m_mtxView,
		&posV,
		&posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDev->SetTransform(D3DTS_VIEW,
		&m_mtxView);
}