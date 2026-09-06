//============================================================================
// 
// エネミー、ヘッダファイル [enemy.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object2D.h"

//****************************************************
// エネミークラス
//****************************************************
class CEnemy : public CObject2D
{
public:

	CEnemy();			// コンストラクタ
	~CEnemy() override;	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 生成

	D3DXVECTOR3 GetVelocity() { return m_velocity; }

private:

	static const float MAX_VELOCITY;	// 加速度上限
	static const float JUMP_FORCE;		// ジャンプ力
	static const float BRAKING_FORCE;	// 制動力

	void Translation();	// 移動
	void Braking();		// 制動調整
	void AdjustPos();	// 位置調整

	D3DXVECTOR3 m_velocity;		// 加速度
	D3DXVECTOR3 m_posTarget;	// 目標位置
	D3DXVECTOR3 m_rotTarget;	// 目標向き
};

#endif // _ENEMY_H_