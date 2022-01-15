#pragma once
/*-----------------------------------------------------------

	SpriteRendererクラス
		スプライトを描画するクラス

-------------------------------------------------------------*/

class SpriteRenderer : public Renderer2D {
private:
	//頂点バッファ
	ID3D11Buffer* pVertexBuffer = 0;

	static const int indexNum = 6;

public:
	float color[4] = { 1,1,1,1 };

public:
	SpriteRenderer();
	~SpriteRenderer(void);

	//画面の上下左右の位置座標
	static float WorldWHPos[4];

	//頂点情報
	stVertex3D vtx[indexNum];

	//コンポーネント処理
	void Execute() override;

	void SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

	void SetColor(float r, float g, float b, float a) override;
	void SetColor(stColor4 color) override;
	stColor4 GetColor() override;

	void SetDefaultUV(); //スプライトの状態を初期状態に戻す(ColorとUV)

private:
	void Render(void); //描画

	void SetVertexState();

	XMMATRIX GetPosMatrix();
	XMMATRIX GetRotMatrix();
	XMMATRIX GetScaleMatrix();

	// XZ平面とスクリーン座標の交点算出関数
	static XMVECTOR CalcScreenToWorld(
		XMVECTOR& pout,
		int Sx,
		int Sy,
		float fZ,
		int Screen_w,
		int Screen_h,
		XMMATRIX& View,
		XMMATRIX& Prj
	);

	static XMVECTOR CalcScreenToXZ(
		XMVECTOR& pout,
		int Sx,
		int Sy,
		int Screen_w,
		int Screen_h,
		XMMATRIX& View,
		XMMATRIX& Prj
	);
};