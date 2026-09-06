//============================================================================
// 
// プレイヤー、ヘッダファイル [player.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object2D.h"

//****************************************************
// プレイヤークラス
//****************************************************
class CPlayer : public CObject2D
{
public:

	CPlayer();				// コンストラクタ
	~CPlayer() override;	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 生成

private:

	static const int TEXTURE_DIVIDE_U;	// テクスチャのU方向分割数
	static const int TEXTURE_DIVIDE_V;	// テクスチャのV方向分割数
	static const float MAX_VELOCITY;	// 加速度上限
	static const float JUMP_FORCE;		// ジャンプ力
	static const float BRAKING_FORCE;	// 制動力

	void Control();			// 操作
	void Braking();			// 制動調整
	void GravityFall();		// 重力加速
	void AdjustPos();		// 位置を調整
	void Collision();		// 当たり判定
	void Animation();		// アニメーション
	
	int m_nCntTexChange;		// テクスチャ変更管理
	int m_nLeftNumJump;			// ジャンプ可能回数
	D3DXVECTOR3 m_velocity;		// 加速度
	D3DXVECTOR3 m_posTarget;	// 目標位置
	D3DXVECTOR3 m_rotTarget;	// 目標向き

	bool m_bAnimationLock;	// ( 試験的 )
};

#endif // _PALYER_H_