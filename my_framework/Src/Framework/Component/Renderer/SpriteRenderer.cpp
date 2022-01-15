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

//コンポーネントの初期化
void SpriteRenderer::SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite) {

	this->sizeX = sizeX;
	this->sizeY = sizeY;

	if (sprite != nullptr) {
		pRenderSprite = sprite;
		SetDefaultUV();
	}

	SetVertexState();

	//バーテックスバッファー作成
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

	//各頂点の座標を設定
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
	//移動・回転・スケーリング
	// </summary>
	//スケーリング
	XMMATRIX mScale = GetScaleMatrix();
	mWorld = mWorld * mScale;
	//回転
	XMMATRIX mRot = GetRotMatrix();
	mWorld = mWorld * mRot;
	//移動
	XMMATRIX mPos = GetPosMatrix();
	mWorld = mWorld * mPos;

	// ビュートランスフォーム（視点座標変換）
	XMFLOAT3 _EyePt(0.0f, 0.0f, -0.2f); //カメラ（視点）位置
	XMFLOAT3 _LookatPt(0.0f, 0.0f, 0.0f);//注視位置
	XMFLOAT3 _UpVec(0.0f, 1.0f, 0.0f);//上方位置
	mView = XMMatrixLookAtLH(ToXMVECTOR(_EyePt), ToXMVECTOR(_LookatPt), ToXMVECTOR(_UpVec));
	// プロジェクショントランスフォーム（射影変換）
	mProj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);


	XMVECTOR screenHT;
	XMVECTOR screenRB;
	CalcScreenToXZ(screenHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, mView, mProj);
	CalcScreenToXZ(screenRB, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, mView, mProj);
	WorldWHPos[0] = screenHT.m128_f32[0]; WorldWHPos[1] = screenRB.m128_f32[0];
	WorldWHPos[2] = screenHT.m128_f32[1]; WorldWHPos[3] = screenRB.m128_f32[1];


	//使用するシェーダーの登録	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), NULL, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer0 cb;
		//ワールド行列を渡す
		cb.mW = mWorld;
		cb.mW = XMMatrixTranspose(cb.mW);
		//ワールド、カメラ、射影行列を渡す
		XMMATRIX m = mWorld * mView * mProj;
		cb.mWVP = m;
		cb.mWVP = XMMatrixTranspose(cb.mWVP);
		//ライトの方向を渡す
		XMFLOAT3 vLightDir(0, 0, 0);
		cb.lightDir = XMFLOAT4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);
		//視点位置を渡す
		cb.eye = XMFLOAT4(_EyePt.x, _EyePt.y, _EyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer0));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer0, 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer0);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &pConstantBuffer0);
	//頂点インプットレイアウトをセット
	Direct3D::getDeviceContext()->IASetInputLayout(pInputLayout1);
	//プリミティブ・トポロジーをセット
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//ラスタライザのセット
	Direct3D::getDeviceContext()->RSSetState(pRasterizerState);
	//ブレンドステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetBlendState(pBlendState, NULL, 0xffffffff);
	//深度ステンシルステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetDepthStencilState(pDepthStencilState, 0);
	//バーテックスバッファーをセット
	UINT stride = sizeof(stVertex3D);
	UINT offset = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	
	//インデックスバッファーをセット
	Direct3D::getDeviceContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//マテリアルの描画
	//マテリアルの各要素をエフェクト（シェーダー）に渡す
	D3D11_MAPPED_SUBRESOURCE pData2 = {};
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData2)))
	{
		ConstantBuffer1 cb;
		cb.ambient = { 1,1,1,1 };//アンビエントををシェーダーに渡す
		cb.diffuse = { color[0],color[1],color[2],color[3] };//ディフューズカラーをシェーダーに渡す
		cb.specular = { 1,1,1,1 };//スペキュラーをシェーダーに渡す
		memcpy_s(pData2.pData, pData2.RowPitch, (void*)&cb, sizeof(ConstantBuffer1));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer1, 0);
	}
	Direct3D::getDeviceContext()->VSSetConstantBuffers(1, 1, &pConstantBuffer1);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(1, 1, &pConstantBuffer1);

	//テクスチャーをシェーダーに渡す
	if (pRenderSprite->GetTexture() != NULL)
	{
		Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSamplerState);
		Direct3D::getDeviceContext()->PSSetShaderResources(0, 1, &pRenderSprite->pTextureView);
	}
	//プリミティブをレンダリング
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
	if (pRenderSprite == NULL) return; //スプライトがない場合return
	
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
	//カメラ座標を加えてスクリーン座標を設定する
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

	XMMATRIX mPitch, mHeading, mBank;//回転行列用
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

	//実験ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	float _x = Camera::main != NULL ? Camera::main->transform->position.x : 0;
	float _y = Camera::main != NULL ? Camera::main->transform->position.y : 0;
	float _z = Camera::main != NULL ? Camera::main->transform->position.z : 0;
	if (-0.2f + _z == 0) _z += -0.2f; //All0回避
	XMFLOAT3 tEyePt(0.0f, 0.0f, -0.2f + _z); //カメラ（視点）位置
	XMFLOAT3 tLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	XMFLOAT3 tUpVec(0.0f, 1.0f, 0.0f);//上方位置
	XMMATRIX tView = XMMatrixLookAtLH(ToXMVECTOR(tEyePt), ToXMVECTOR(tLookatPt), ToXMVECTOR(tUpVec));
	// 各行列の逆行列を算出
	XMMATRIX InvView, InvPrj, VP, InvViewport;
	InvView = XMMatrixInverse(NULL, tView);
	InvPrj = XMMatrixInverse(NULL, Prj);
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = Screen_w / 2.0f; VP.r[1].m128_f32[1] = -Screen_h / 2.0f;
	VP.r[3].m128_f32[0] = Screen_w / 2.0f; VP.r[3].m128_f32[1] = Screen_h / 2.0f;
	InvViewport = XMMatrixInverse(NULL, VP);

	// 逆変換
	XMMATRIX tmp = InvViewport * InvPrj * InvView;
	XMVECTOR temp = XMVECTOR{ (float)Sx,(float)Sy,fZ, 0 };
	pout = XMVector3TransformCoord(temp, tmp);

	return pout;
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
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

	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	if (ray.m128_f32[2] > 0) {
		// 床交点
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