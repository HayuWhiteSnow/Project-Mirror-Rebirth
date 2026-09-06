//============================================================================
// 
// 2Dオブジェクト管理、ヘッダファイル [object2D.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// 2Dオブジェクトクラス
//****************************************************
class CObject2D : public CObject
{
public:
	
	CObject2D(int nPriority = static_cast<int>(LAYER::UI) - 1);	// コンストラクタ
	~CObject2D() override;											// デストラクタ

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

	static CObject2D* Create();	// 生成

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9 m_pTex;			// テクスチャのポインタ

	D3DXVECTOR3 m_pos;	// 中心位置
	D3DXVECTOR3 m_rot;	// 向き
	D3DXVECTOR3 m_size;	// サイズ
	float m_fLength;	// 展開用対角線
	float m_fAngle;		// 対角線用角度
	float m_fTexWidth;	// 横テクスチャ分割幅
	float m_fTexHeight;	// 縦テクスチャ分縦幅
	int m_nNowPatternU;	// 現在の横テクスチャ種類
	int m_nNowPatternV;	// 現在の縦テクスチャ種類
};

#endif // _OBJECT2D_H_