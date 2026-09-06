//============================================================================
// 
// 数字、ヘッダファイル [number.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object2D.h"

//****************************************************
// ナンバークラス
//****************************************************
class CNumber : public CObject2D
{
public:

	CNumber();				// コンストラクタ
	~CNumber() override;	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理
	
	void SetNumber(int nNum);	// 数字を設定

	static CNumber* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 生成

private:

	void Animation();	// アニメーション

	int m_nNum;	// 数字の割り当て
};

#endif // _NUMBER_H_