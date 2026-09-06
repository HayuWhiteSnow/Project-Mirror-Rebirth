//============================================================================
// 
// パーティクル、ヘッダファイル [particle.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _PARTICLE_H_
#define _PARTICLE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object2D.h"

//****************************************************
// パーティクルクラス
//****************************************************
class CParticle : public CObject2D
{
public:

	CParticle();			// コンストラクタ
	~CParticle() override;	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CParticle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fFlyAngle);	// 生成

private:

	void Translate();	// 移動
	bool Progress();	// 期間経過

	float m_fFlyAngle;	// 飛ぶ角度
};

#endif // _EFFECT_H_