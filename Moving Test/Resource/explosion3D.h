//============================================================================
// 
// 3D爆発、ヘッダファイル [explosion3D.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _EXPLOSION3D_H_
#define _EXPLOSION3D_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "objectBillboard.h"

//****************************************************
// 3Dエクスプロージョンクラス
//****************************************************
class CExplosion3D : public CObjectBillboard
{
public:

	CExplosion3D();			// コンストラクタ
	~CExplosion3D() override;	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CExplosion3D* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 生成

private:

	bool Animation();	// アニメーション

	int m_nCntTexChange;	// テクスチャ変更管理
};

#endif // _EXPLOSION3D_H_