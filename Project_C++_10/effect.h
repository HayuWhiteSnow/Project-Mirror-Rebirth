//============================================================================
// 
// エフェクト、ヘッダファイル [effect.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object2D.h"

//****************************************************
// エネミークラス
//****************************************************
class CEffect : public CObject2D
{
public:

	CEffect();				// コンストラクタ
	~CEffect() override;	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 生成

private:

	bool Progress();	// 期間経過
};

#endif // _EFFECT_H_