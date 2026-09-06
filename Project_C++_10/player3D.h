//============================================================================
// 
// プレイヤー3D、ヘッダファイル [player3D.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _PLAYER3D_H_
#define _PLAYER3D_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "objectX.h"

//****************************************************
// プレイヤー3Dクラス
//****************************************************
class CPlayer3D : public CObjectX
{
public:

	CPlayer3D();			// コンストラクタ
	~CPlayer3D() override;	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CPlayer3D* Create(D3DXVECTOR3 pos);	// 生成

private:

	static const float MAX_VELOCITY;	// 加速度上限
	static const float BRAKING_FORCE;	// 制動力

	void Control();			// 操作
	void Rotation();		// 回転
	void Braking();			// 制動調整
	void AdjustPos();		// 位置を調整

	D3DXVECTOR3 m_velocity;		// 加速度
	D3DXVECTOR3 m_posTarget;	// 目標位置
	D3DXVECTOR3 m_rotTarget;	// 目標向き
};

#endif // _PALYER3D_H_