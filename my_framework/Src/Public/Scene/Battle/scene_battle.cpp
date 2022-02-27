#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace nsBattle;

//������
void SceneBattle::Initialize() {
	//�J����
	pCam = CreateObject(0, 0, -12.0f);
	pCam->AddComponent<Camera>();
	pCam->GetComponent<Camera>()->SetMain();

	//�J�[�\���A�w�i
	pCursor = CreateObject(0, 0, 0, 0.9f, 0.9f, CreateSprite(new Sprite(L"Data/Image/Battle/cursor01.spr")), NULL, "cursor");
	pCursor->AddComponent<Cursor>();
	pCursor->AddComponent<Collider2D>();
	pCursor->AddComponent<Physics2D>();
	pCursor->GetComponent<SpriteRenderer>()->SetRenderPriority((int)eRenderOrder::FrontObject + 1);
	pCursor->GetComponent<Collider2D>()->SetUpCollider2D(0.2f, 0.2f, false);

	pBg = CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, SCREEN_WIDTH, SCREEN_HEIGHT, CreateSprite(new Sprite(L"Data/Image/Battle/bg.spr")));
	pBg->GetComponent<ImageRenderer>()->isFrontImg = false;

	//�p�l��-------------------------------------------------------------------
	//�X�e�[�^�X
	pStatusPanel = CreateImageObject(0, 0, 400, 250, CreateSprite(new Sprite(L"Data/Image/Battle/status_bg.spr")), NULL, "statusPanel");
	pStatusPanel->AddComponent<StatusPanel>();

	//�t�B�[���h�}�l�[�W���[�i�L�����N�^�[��n���j------------------------------------------
	pFieldManager = CreateObject(0, 0, 0, NULL, "fieldManager");
	pFieldManager->AddComponent<FieldManager>();
	noDel_ptr<FieldManager> fieldManager = pFieldManager->GetComponent<FieldManager>();
	fieldManager->vPlayerCharaBace = vPlayerCharaBace;
	fieldManager->vEnemyCharaBace = vEnemyCharaBace;

	//�f�o�b�O�ݒ�---------------------------------------------------------------------------
	Debug::SetBattle(vPlayerCharaBace, vEnemyCharaBace);
	Debug::scene = (int)eSceneTable::Battle;


	//�o�g��
	pBattlePanel = CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, SCREEN_WIDTH, SCREEN_HEIGHT,
		CreateSprite(new Sprite(L"Data/Image/Battle/battle_bg_0.spr")), NULL, "battlePanel");
	pBattlePanel->AddComponent<BattlePanel>();

	isInitialized = true;
}

void SceneBattle::Terminate() {
}

//����
void SceneBattle::Execute() {
	
	Scene::Execute();
}

//�`��
void SceneBattle::Render() {
	Scene::Render();
} 

 