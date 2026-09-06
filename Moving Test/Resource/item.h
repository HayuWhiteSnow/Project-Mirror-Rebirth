//============================================================================
// 
// アイテム、ヘッダファイル [item.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _ITEM_H_
#define _ITEM_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object2D.h"

//****************************************************
// アイテムクラス
//****************************************************
class CItem : public CObject2D
{
public:

	CItem();			// コンストラクタ
	~CItem() override;	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CItem* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 生成

private:

	void Translation();	// 移動
	void GravityFall();	// 重力加速
	void AdjustPos();	// 位置を調整
	void Collision();	// 当たり判定

	D3DXVECTOR3 m_velocity;	// 加速度
	D3DXVECTOR3 m_pos_tgt;	// 目標位置
};

#endif // _ITEM_H_