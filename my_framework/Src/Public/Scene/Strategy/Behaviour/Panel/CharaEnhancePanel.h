/*-----------------------------------------------------------

	CharaEnhancePanel
		�L�����N�^�[�����p�l��

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class CharaEnhancePanel : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

		//�X�e�[�^�X�̏㏸�l
		struct stUpValue {
			const int level = 1;
			const int power = 5;
			const int hp = 10;
			const int mp = 8;
			const int def = 2;
			const int evd = 2;
			const int move = 1;
		};
		//�㏸�l
		stUpValue upValue;

		//�Ώۂ̊X
		noDel_ptr<City> pSelectCity;

		//�摜
		noDel_ptr<ImageRenderer> pBorder;
		noDel_ptr<ImageRenderer> pCharaImage;

		//�m�F��ʃp�l��
		noDel_ptr<GameObject> pConfirmPanel;
		noDel_ptr<Font> pConfirmText;
		noDel_ptr<Font> pYesNoText;

		//�Z���N�g�J�[�\��
		noDel_ptr<ImageRenderer> pCursor;
		int cursorNum;

		//�e�L�X�g---------------------------------
		//��ڃe�L�X�g
		noDel_ptr<Font> pTopText;
		//�������z�e�L�X�g
		noDel_ptr<Font> pNeedMoneyText;
		//�L������
		noDel_ptr<Font> pNameText[OWN_CHARACTOR_NUM];
		//����
		noDel_ptr<Font> pAttrText;
		//���x��
		noDel_ptr<Font> pLevelText;
		//�̒l
		noDel_ptr<Font> pPowerText;
		//HP
		noDel_ptr<Font> pHpText;
		//MP
		noDel_ptr<Font> pMpText;
		//�h���
		noDel_ptr<Font> pDefText;
		//���
		noDel_ptr<Font> pEvdText;
		//�ړ���
		noDel_ptr<Font> pMoveText;

		
	public:
		void Awake() override;
		void Update() override;

		void Open(noDel_ptr<City> city);

	private:

		//�X�e�[�^�X�e�L�X�g�Z�b�g
		void SetStatusText(noDel_ptr<Charactor> chara);

		//�J�[�\���ړ�
		void MoveCursor();

		//����
		void Enhance();

		//�����\��
		bool IsEnhance(noDel_ptr<Charactor> chara);

		//�K�v���z�v�Z
		int GetNeedMoney(int level);
	};
}