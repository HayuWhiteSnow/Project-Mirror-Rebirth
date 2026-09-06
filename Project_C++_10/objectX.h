//============================================================================
// 
// Xオブジェクト、ヘッダファイル [object3D.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"
#include "main.h"

//****************************************************
// Xオブジェクトクラス
//****************************************************
class CObjectX : public CObject
{
public:

	CObjectX(int nPriority = static_cast<int>(LAYER::FRONT));	// コンストラクタ
	~CObjectX() override;										// デストラクタ

	HRESULT Init() override;		// 初期設定
	void Uninit() override;			// 終了処理
	void Update() override;			// 更新処理
	void Draw() override;			// 描画処理

	void BindModel(MODEL* pModel);	// モデル割当

	D3DXVECTOR3 GetPos();			// 中心位置取得
	void SetPos(D3DXVECTOR3 pos);	// 中心位置設定

	D3DXVECTOR3 GetRot();			// 向き取得
	void SetRot(D3DXVECTOR3 rot);	// 向き設定

	virtual CObjectX* Create();	// 生成

private:

	void SetMtxWorld();	// ワールド行列設定

	MODEL* m_pModel;		// モデル情報
	D3DXVECTOR3 m_pos;		// 中心位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXMATRIX m_mtxWorld;	// ワールド行列
};

#endif // _OBJECTX_H_