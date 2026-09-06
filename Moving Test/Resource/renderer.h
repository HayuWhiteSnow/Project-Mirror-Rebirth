//============================================================================
// 
// レンダラー管理、ヘッダファイル [renderer.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "texture.h"
#include "model.h"

//****************************************************
// レンダラークラス
//****************************************************
class CRenderer
{
public:

	CRenderer();	// コンストラクタ
	~CRenderer();	// デストラクタ

	HRESULT Init(HWND hWnd, BOOL bWindiw);	// 初期設定
	void Uninit();							// 終了処理
	void Update();							// 更新処理
	void Draw();							// 描画処理

	LPDIRECT3DDEVICE9 GetDeviece();	// デバイスの取得
	CTexture* GetTextureInstane();	// テクスチャ管理の取得
	CModel* GetModelInstane();		// モデル管理の取得

private:

	LPDIRECT3D9 m_pD3D;				// Direct3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// デバイス
	CTexture* m_pTexture;			// テクスチャ管理
	CModel* m_pModel;				// モデル管理
};

#endif // _RENDERER_H_