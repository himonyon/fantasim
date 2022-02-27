/*-----------------------------------------------------------

	CityPanel�N���X
		�����œn���ꂽ�X�̏���\������p�l��

-------------------------------------------------------------*/
#pragma once
#include "SelectNeighborPanel.h"
#include "InvestPanel.h"
#include "BattlePanel.h"
#include "CharaSelectPanel.h"
#include "CharaEnhancePanel.h"
#include "OptionPanel.h"
#include "OptionMenu.h"
#include "InfoPanel.h"
#include "EventPanel.h"

namespace nsStrategy {

	enum class eCommand {
		Economic,
		Political,
		Disaster,
		Support,
		MoneyMove,
		CharaMove,
		CharaEnhance,
		Battle,
		Max
	};

	class CityPanel : public Panel {
	private:
		//Find����ϐ�
		noDel_ptr<SoundManager> pSoundManager;
		noDel_ptr<GameObject> pMapCursor;

	private:
		//�\������X
		noDel_ptr<City> pCity;
		//�J�[�\��
		noDel_ptr<GameObject> pSelectCursor;
		int cursorNum = 0;

		bool isPlayerCity = false; //�J���ꂽ�p�l�����v���C���[�̏��L����

		//�e�L�X�g---------------------------------------------------------
		//�X�̊�{�X�e�[�^�X�\���p�l��
		noDel_ptr<GameObject> pCityInfoPanel; 
		noDel_ptr<GameObject> pCityNameText;
		noDel_ptr<GameObject> pCityStatusText;

		//�L�����ꗗ�\���p�l��
		noDel_ptr<GameObject> pCharaInfoPanel; 
		noDel_ptr<GameObject> pCharaInfoText[OWN_CHARACTOR_NUM];
		noDel_ptr<GameObject> pAttrObj[OWN_CHARACTOR_NUM];

		//�s���ꗗ�\���p�l��
		noDel_ptr<GameObject> pBorderText[2];
		noDel_ptr<GameObject> pCommandPanel;
		noDel_ptr<GameObject> pCommandTitleText;
		noDel_ptr<GameObject> pCommand[(int)eCommand::Max];
		noDel_ptr<GameObject> pCommandTexts[(int)eCommand::Max];

	private:
		void Awake() override;
		void Update() override;

		void Close() override;

		//�X�̏����Z�b�g����
		void SetCityStatus(float top, float left);
		void SetCommandPanel(float top, float left);

		//�Z���N�g�J�[�\���ړ�
		void MoveSelectCursor();

		//���̃p�l���֑J��
		void TransOtherState();

	public:
		//�p�l�����J��
		void Open(noDel_ptr<City> city);

		//�v���C���[�̑���
		void PlayerCityFunc();
	};
}
