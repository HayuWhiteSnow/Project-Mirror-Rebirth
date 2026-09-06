//============================================================================
// 
// 弾、ヘッダファイル [bullet.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _BULLET_H_
#define _BULLET_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object2D.h"

//****************************************************
// バレットクラス
//****************************************************
class CBullet : public CObject2D
{
public:

	CBullet();				// コンストラクタ
	~CBullet() override;	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CBullet* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nRemain, float fFlyAngle);	// 生成

private:

	void Translation();		// 移動
	bool CollisionEnemy();	// 当たり判定
	bool Progress();		// 期間経過

	int m_nRemain;		// 使用期間
	float m_fFlyAngle;	// 飛ぶ角度
};

#endif // _BULLET_H_