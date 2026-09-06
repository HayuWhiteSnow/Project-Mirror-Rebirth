//============================================================================
// 
// オブジェクト管理、ヘッダファイル [object.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_	// 二重インクルード防止

//****************************************************
// オブジェクトクラス
//****************************************************
class CObject
{
public:

	static const int MAX_OBJ = 256;		// オブジェクト最大数
	static const float GRAVITY_FORCE;	// 重力

	//****************************************************
	// オブジェクトの種類識別
	//****************************************************
	enum class TYPE
	{
		NONE = 0,	// 無し
		PLAYER,		// プレイヤー
		ENEMY,		// エネミー
		BLOCK,		// ブロック
		ITEM,		// アイテム
		SCORE,		// スコア
		MAX,
	};

	//****************************************************
	// オブジェクトのレイヤー識別
	//****************************************************
	enum class LAYER
	{
		NONE = 0,		// 無し
		BACK,			// 背景
		BACK_MIDDLE,	// 後ろ側
		FRONT_MIDDLE,	// 前川
		FRONT,			// 前景
		UI,				// UI
		MAX,
	};

	CObject(int nPriority = static_cast<int>(LAYER::MAX) - 1);	// コンストラクタ
	virtual ~CObject() = 0;										// デストラクタ

	virtual HRESULT Init() = 0;	// 初期設定
	virtual void Uninit() = 0;	// 終了処理
	virtual void Update() = 0;	// 更新処理
	virtual void Draw() = 0;	// 描画処理

	static void ReleaseAll();	// 全オブジェクト解放処理
	static void UpdateAll();	// 全オブジェクト更新処理
	static void DrawAll();		// 全オブジェクト描画処理

	static CObject* GetObject(int nPriority, int nID);	// オブジェクト情報取得
	static int GetNumAll();								// オブジェクト総数取得

	void Release();	// 個別解放処理 (仮)
	
	TYPE GetType();					// タイプ取得
	CObject* FindScoreInstance();	// スコアのインスタンスを探す

protected:

	void SetType(TYPE type);	// タイプ設定

private:

	static CObject* m_apObject[static_cast<int>(LAYER::MAX)][MAX_OBJ];	// オブジェクト管理
	static int m_nNumAll;												// オブジェクト総数

	int m_nPriority;	// 描画優先度
	int m_nID;			// 自分自身のID
	TYPE m_type;		// タイプ識別 
};

#endif // _OBJECT_H_