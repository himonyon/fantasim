#pragma once
/*-----------------------------------------------------------

	SpriteRenderer�N���X
		�X�v���C�g��`�悷��N���X

-------------------------------------------------------------*/

class SpriteRenderer : public Renderer2D {
private:
	//���_�o�b�t�@
	ID3D11Buffer* pVertexBuffer = 0;

	static const int indexNum = 6;

public:
	float color[4] = { 1,1,1,1 };

public:
	SpriteRenderer();
	~SpriteRenderer(void);

	//��ʂ̏㉺���E�̈ʒu���W
	static float WorldWHPos[4];

	//���_���
	stVertex3D vtx[indexNum];

	//�R���|�[�l���g����
	void Execute() override;

	void SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite);

	void SetColor(float r, float g, float b, float a) override;
	void SetColor(stColor4 color) override;
	stColor4 GetColor() override;

	void SetDefaultUV(); //�X�v���C�g�̏�Ԃ�������Ԃɖ߂�(Color��UV)

private:
	void Render(void); //�`��

	void SetVertexState();

	XMMATRIX GetPosMatrix();
	XMMATRIX GetRotMatrix();
	XMMATRIX GetScaleMatrix();

	// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
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