//============================================================================
// 
// カメラ、ヘッダファイル [camera.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_	// 二重インクルード防止

//****************************************************
// カメラクラス
//****************************************************
class CCamera
{
public:

	CCamera();	// コンストラクタ
	~CCamera();	// デストラクタ

	HRESULT Init();		// 初期設定
	void Update();		// 更新処理
	void SetCamera();	// カメラをセット

private:

	void Control();				// カメラ操作
	void Rotation();			// 回転
	void Translation();			// 移動
	void RestrictYaw();			// ヨー角の範囲を制限
	void RestrictPitch();		// ピッチ角の範囲を制限
	void CalcPosV();			// 視点位置を算出
	void CalcPosR();			// 注視点位置を算出
	void CalcMtxProjection();	// プロジェクション行列計算
	void CalcMtxView();			// ビュー行列計算

	D3DXVECTOR3 m_pos;			// 中心位置
	D3DXVECTOR3 m_posTarget;	// 目標中心位置
	D3DXVECTOR3 m_posV;			// 視点位置
	D3DXVECTOR3 m_posTargetV;	// 目標視点位置
	D3DXVECTOR3 m_posR;			// 注視点位置
	D3DXVECTOR3 m_posTargetR;	// 目標注視点位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_rotTarget;	// 目標向き
	float m_fDistance;			// 視点 -> 注視点間の距離
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		// ビューマトリックス
};

#endif // _CAMERA_H_