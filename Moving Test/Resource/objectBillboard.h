//============================================================================
// 
// ビルボードオブジェクト管理、ヘッダファイル [objectBillboard.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// ビルボードオブジェクトクラス
//****************************************************
class CObjectBillboard : public CObject
{
public:

	CObjectBillboard(int nPriority = static_cast<int>(LAYER::FRONT));	// コンストラクタ
	~CObjectBillboard() override;										// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	void BindTex(LPDIRECT3DTEXTURE9 pTex);	// テクスチャ割当

	D3DXVECTOR3 GetPos();			// 中心位置取得
	void SetPos(D3DXVECTOR3 pos);	// 中心位置設定

	D3DXVECTOR3 GetRot();			// 向き取得
	void SetRot(D3DXVECTOR3 rot);	// 向き設定

	D3DXVECTOR3 GetSize();			// サイズ取得
	void SetSize(D3DXVECTOR3 size);	// サイズ設定

	float GetLength();	// 展開用対角線取得

	void SetTexWidth(float fWidth);		// 横テクスチャ分割幅設定
	void SetTexHeight(float fHeight);	// 縦テクスチャ分割幅設定

	int GetNowPatternU();					// 現在のテクスチャ横分割幅取得
	void SetNowPatternU(int nNowPatternU);	// 現在のテクスチャ横分割幅設定

	int GetNowPatternV();					// 現在のテクスチャ縦分割幅取得
	void SetNowPatternV(int nNowPatternV);	// 現在のテクスチャ縦分割幅設定

	static CObjectBillboard* Create();	// 生成

private:

	void SetMtxWorld();	// ワールド行列設定

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9 m_pTex;			// テクスチャのポインタ

	D3DXVECTOR3 m_pos;		// 中心位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_size;		// サイズ
	float m_fLength;		// 展開用対角線
	float m_fAngle;			// 対角線用角度
	float m_fTexWidth;		// 横テクスチャ分割幅
	float m_fTexHeight;		// 縦テクスチャ分縦幅
	int m_nNowPatternU;		// 現在の横テクスチャ種類
	int m_nNowPatternV;		// 現在の縦テクスチャ種類
	D3DXMATRIX m_mtxWorld;	// ワールド行列
};

#endif // _OBJECT3D_H_