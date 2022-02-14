#pragma once
/*-----------------------------------------------------------

	SpriteRendererクラス
		スプライトを描画するクラス

-------------------------------------------------------------*/
class ImageRenderer : public Renderer2D {
public:
	bool isFrontImg= true;

public:
	ImageRenderer();
	~ImageRenderer(void);

	//頂点情報
	stVertex2D vtx[Sprite::VertexNum];

	//コンポーネント処理
	void Execute() override;

	void SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

	void SetColor(float r, float g, float b, float a) override;
	void SetColor(stColor4 color) override;
	stColor4 GetColor() override;

	//FrontObjなら+10000した値、BackObjなら-10000した値を返す
	int GetRenderPriority() override;

	void SetDefaultUV() override; //スプライトの状態を初期状態に戻す(ColorとUV)

private:
	void Render(void); //描画

	void SetVertexState();
};