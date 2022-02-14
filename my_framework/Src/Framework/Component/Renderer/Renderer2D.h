#pragma once
/*-----------------------------------------------------------

	SpriteRendererクラス
		スプライトを描画するクラス

-------------------------------------------------------------*/

class Renderer2D : public Component {
private:
	static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Sprite[];
	static D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[];

public:
	//描画対象スプライト
	noDel_ptr<Sprite> pRenderSprite = NULL;

	//画像の幅
	float sizeX = 0;
	float sizeY = 0;


protected:
	static ID3D11Buffer* pConstantBuffer; //Image用
	static ID3D11Buffer* pConstantBuffer0; //Sprite行列用
	static ID3D11Buffer* pConstantBuffer1; //Spriteマテリアル用
	static ID3D11Buffer* pIndexBuffer;
	static ID3D11RasterizerState* pRasterizerState;
	static ID3D11SamplerState* pSamplerState;
	static ID3D11BlendState* pBlendState;
	static ID3D11DepthStencilState* pDepthStencilState;
	static ID3D11InputLayout* pInputLayout; //Image用
	static ID3D11InputLayout* pInputLayout1; //Sprite用
	static UINT	VertexStrides;
	static UINT	VertexOffsets;

	int renderPriority = 0;

public:
	static bool Initialize();
	static void Destroy();

	Renderer2D();
	virtual ~Renderer2D(void);

	void SetSize(float width, float height);

	void SetRenderPriority(int value); //描画順位の設定
	virtual int GetRenderPriority(); //描画順位の所得

	virtual void SetColor(float r, float g, float b, float a) {};
	virtual void SetColor(stColor4 color) {};
	virtual stColor4 GetColor() { return { 0,0,0,0 }; };

	virtual void SetDefaultUV() {};

private:
	virtual void Render(void) {}; //描画
};