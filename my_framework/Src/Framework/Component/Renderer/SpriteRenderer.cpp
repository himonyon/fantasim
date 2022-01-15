#include "../../../../framework.h"
#include "../../../../environment.h"

float SpriteRenderer::WorldWHPos[4] = { 0,0,0,0 };

SpriteRenderer::SpriteRenderer()
{
	for (int i = 0; i < 6; i++) {
		vtx[i].pos = { 0,0,0 };
		vtx[i].nor = { 1,1,1 };
		vtx[i].tex = { 0,0 };
	}
}

SpriteRenderer::~SpriteRenderer() {
	SAFE_RELEASE(pVertexBuffer);
}

//�R���|�[�l���g�̏�����
void SpriteRenderer::SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite) {

	this->sizeX = sizeX;
	this->sizeY = sizeY;

	if (sprite != nullptr) {
		pRenderSprite = sprite;
		SetDefaultUV();
	}

	SetVertexState();

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(stVertex3D) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vtx;
	Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
}


void SpriteRenderer::Execute() {
	Render();
}

void SpriteRenderer::SetVertexState() {
	float PI = 3.14f;

	//�e���_�̍��W��ݒ�
	vtx[0].pos.x = -(sizeX / 2);
	vtx[0].pos.y = (sizeY / 2);

	vtx[1].pos.x = (sizeX / 2);
	vtx[1].pos.y = (sizeY / 2);

	vtx[2].pos.x = -(sizeX / 2);
	vtx[2].pos.y = -(sizeY / 2);
	
	vtx[3].pos.x = (sizeX / 2);
	vtx[3].pos.y = (sizeY / 2);

	vtx[4].pos.x = -(sizeX / 2);
	vtx[4].pos.y = -(sizeY / 2);

	vtx[5].pos.x = (sizeX / 2);
	vtx[5].pos.y = -(sizeY / 2);
}

void SpriteRenderer::Render() {
	if (pRenderSprite == NULL) return;

	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProj;

	mWorld = XMMatrixIdentity();
	mView = XMMatrixIdentity();
	mProj = XMMatrixIdentity();

	// <summary>
	//�ړ��E��]�E�X�P�[�����O
	// </summary>
	//�X�P�[�����O
	XMMATRIX mScale = GetScaleMatrix();
	mWorld = mWorld * mScale;
	//��]
	XMMATRIX mRot = GetRotMatrix();
	mWorld = mWorld * mRot;
	//�ړ�
	XMMATRIX mPos = GetPosMatrix();
	mWorld = mWorld * mPos;

	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	XMFLOAT3 _EyePt(0.0f, 0.0f, -0.2f); //�J�����i���_�j�ʒu
	XMFLOAT3 _LookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	XMFLOAT3 _UpVec(0.0f, 1.0f, 0.0f);//����ʒu
	mView = XMMatrixLookAtLH(ToXMVECTOR(_EyePt), ToXMVECTOR(_LookatPt), ToXMVECTOR(_UpVec));
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	mProj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);


	XMVECTOR screenHT;
	XMVECTOR screenRB;
	CalcScreenToXZ(screenHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, mView, mProj);
	CalcScreenToXZ(screenRB, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, mView, mProj);
	WorldWHPos[0] = screenHT.m128_f32[0]; WorldWHPos[1] = screenRB.m128_f32[0];
	WorldWHPos[2] = screenHT.m128_f32[1]; WorldWHPos[3] = screenRB.m128_f32[1];


	//�g�p����V�F�[�_�[�̓o�^	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), NULL, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer0 cb;
		//���[���h�s���n��
		cb.mW = mWorld;
		cb.mW = XMMatrixTranspose(cb.mW);
		//���[���h�A�J�����A�ˉe�s���n��
		XMMATRIX m = mWorld * mView * mProj;
		cb.mWVP = m;
		cb.mWVP = XMMatrixTranspose(cb.mWVP);
		//���C�g�̕�����n��
		XMFLOAT3 vLightDir(0, 0, 0);
		cb.lightDir = XMFLOAT4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);
		//���_�ʒu��n��
		cb.eye = XMFLOAT4(_EyePt.x, _EyePt.y, _EyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer0));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer0, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer0);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &pConstantBuffer0);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D::getDeviceContext()->IASetInputLayout(pInputLayout1);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//���X�^���C�U�̃Z�b�g
	Direct3D::getDeviceContext()->RSSetState(pRasterizerState);
	//�u�����h�X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetBlendState(pBlendState, NULL, 0xffffffff);
	//�[�x�X�e���V���X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetDepthStencilState(pDepthStencilState, 0);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(stVertex3D);
	UINT offset = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	Direct3D::getDeviceContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//�}�e���A���̕`��
	//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
	D3D11_MAPPED_SUBRESOURCE pData2 = {};
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData2)))
	{
		ConstantBuffer1 cb;
		cb.ambient = { 1,1,1,1 };//�A���r�G���g�����V�F�[�_�[�ɓn��
		cb.diffuse = { color[0],color[1],color[2],color[3] };//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
		cb.specular = { 1,1,1,1 };//�X�y�L�����[���V�F�[�_�[�ɓn��
		memcpy_s(pData2.pData, pData2.RowPitch, (void*)&cb, sizeof(ConstantBuffer1));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer1, 0);
	}
	Direct3D::getDeviceContext()->VSSetConstantBuffers(1, 1, &pConstantBuffer1);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(1, 1, &pConstantBuffer1);

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	if (pRenderSprite->GetTexture() != NULL)
	{
		Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSamplerState);
		Direct3D::getDeviceContext()->PSSetShaderResources(0, 1, &pRenderSprite->pTextureView);
	}
	//�v���~�e�B�u�������_�����O
	Direct3D::getDeviceContext()->DrawIndexed(6, 0, 0);
}


void SpriteRenderer::SetColor(float r, float g, float b, float a) {
	color[0] = r; color[1] = g; color[2] = b; color[3] = a;
}
void SpriteRenderer::SetColor(stColor4 color) {
	this->color[0] = color.r; this->color[1] = color.g; this->color[2] = color.b; this->color[3] = color.a;
}
stColor4 SpriteRenderer::GetColor() {
	return { color[0], color[1], color[2], color[3] };
}
void SpriteRenderer::SetDefaultUV() {
	if (pRenderSprite == NULL) return; //�X�v���C�g���Ȃ��ꍇreturn
	
	vtx[0].tex.x = pRenderSprite->GetVertexState(0).u;
	vtx[0].tex.y = pRenderSprite->GetVertexState(0).v;
	vtx[1].tex.x = pRenderSprite->GetVertexState(1).u;
	vtx[1].tex.y = pRenderSprite->GetVertexState(1).v;
	vtx[2].tex.x = pRenderSprite->GetVertexState(2).u;
	vtx[2].tex.y = pRenderSprite->GetVertexState(2).v;
	vtx[3].tex.x = pRenderSprite->GetVertexState(1).u;
	vtx[3].tex.y = pRenderSprite->GetVertexState(1).v;
	vtx[4].tex.x = pRenderSprite->GetVertexState(2).u;
	vtx[4].tex.y = pRenderSprite->GetVertexState(2).v;
	vtx[5].tex.x = pRenderSprite->GetVertexState(3).u;
	vtx[5].tex.y = pRenderSprite->GetVertexState(3).v;
}

XMMATRIX SpriteRenderer::GetPosMatrix() {
	stVector3 pos;
	pos.x = transform->position.x;
	pos.y = transform->position.y;
	pos.z = transform->position.z;
	//�J�������W�������ăX�N���[�����W��ݒ肷��
	if (Camera::main != nullptr) {
		pos.x -= Camera::main->transform->position.x;
		pos.y -= Camera::main->transform->position.y;
		pos.z -= Camera::main->transform->position.z;
	}
	return XMMatrixTranslation(pos.x, pos.y, pos.z);
}
XMMATRIX SpriteRenderer::GetRotMatrix() {
	stVector3 rot;
	rot.x = transform->rotation.x;
	rot.y = transform->rotation.y;
	rot.z = transform->rotation.z;

	if (Camera::main != nullptr) {
		rot.x -= Camera::main->transform->rotation.x;
		rot.y -= Camera::main->transform->rotation.y;
		rot.z -= Camera::main->transform->rotation.z;
	}

	XMMATRIX mPitch, mHeading, mBank;//��]�s��p
	mPitch = XMMatrixRotationX(rot.x);
	mHeading = XMMatrixRotationY(rot.y);
	mBank = XMMatrixRotationZ(rot.z);
	return mPitch * mHeading * mBank;
}

XMMATRIX SpriteRenderer::GetScaleMatrix() {
	return XMMatrixScaling(transform->scale.x, transform->scale.y, transform->scale.z);
}

XMVECTOR SpriteRenderer::CalcScreenToWorld(
	XMVECTOR& pout,
	int Sx,
	int Sy,
	float fZ,
	int Screen_w,
	int Screen_h,
	XMMATRIX& View,
	XMMATRIX& Prj) {

	//�����[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	float _x = Camera::main != NULL ? Camera::main->transform->position.x : 0;
	float _y = Camera::main != NULL ? Camera::main->transform->position.y : 0;
	float _z = Camera::main != NULL ? Camera::main->transform->position.z : 0;
	if (-0.2f + _z == 0) _z += -0.2f; //All0���
	XMFLOAT3 tEyePt(0.0f, 0.0f, -0.2f + _z); //�J�����i���_�j�ʒu
	XMFLOAT3 tLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	XMFLOAT3 tUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	XMMATRIX tView = XMMatrixLookAtLH(ToXMVECTOR(tEyePt), ToXMVECTOR(tLookatPt), ToXMVECTOR(tUpVec));
	// �e�s��̋t�s����Z�o
	XMMATRIX InvView, InvPrj, VP, InvViewport;
	InvView = XMMatrixInverse(NULL, tView);
	InvPrj = XMMatrixInverse(NULL, Prj);
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = Screen_w / 2.0f; VP.r[1].m128_f32[1] = -Screen_h / 2.0f;
	VP.r[3].m128_f32[0] = Screen_w / 2.0f; VP.r[3].m128_f32[1] = Screen_h / 2.0f;
	InvViewport = XMMatrixInverse(NULL, VP);

	// �t�ϊ�
	XMMATRIX tmp = InvViewport * InvPrj * InvView;
	XMVECTOR temp = XMVECTOR{ (float)Sx,(float)Sy,fZ, 0 };
	pout = XMVector3TransformCoord(temp, tmp);

	return pout;
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
}

XMVECTOR SpriteRenderer::CalcScreenToXZ(
	XMVECTOR& pout,
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h,
	XMMATRIX& View,
	XMMATRIX& Prj
) {
	XMVECTOR nearpos;
	XMVECTOR farpos;
	XMVECTOR ray;
	float tem = (float)abs(Camera::main->transform->position.z + 0.2f) / 110.0f;
	CalcScreenToWorld(nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	ray = XMVector3Normalize(ray);

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.m128_f32[2] > 0) {
		// ����_
		XMVECTOR temp = XMVECTOR{ 0,0,1,1 };
		XMVECTOR Lray = XMVector3Dot(ray, temp);
		XMVECTOR LP0 = XMVector3Dot((-nearpos), temp);
		pout = nearpos + (LP0 / Lray) * ray;
		pout.m128_f32[0] += Camera::main != NULL ? Camera::main->transform->position.x : 0;
		pout.m128_f32[1] += Camera::main != NULL ? Camera::main->transform->position.y : 0;
	}
	else {
		pout = farpos;
	}

	return pout;
}