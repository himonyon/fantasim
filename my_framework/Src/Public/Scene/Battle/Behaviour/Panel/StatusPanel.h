#pragma once
/*-----------------------------------------------------------

	StatusPanel�N���X(Panel�̔h��)
	�X�e�[�^�X�\���p�l���̊Ǘ�

-------------------------------------------------------------*/
namespace nsBattle {
	class StatusPanel : public Panel {
	private:
		//�����Ɩ��O
		noDel_ptr<GameObject> pAttrObj;
		noDel_ptr<GameObject> pNameText;

		//HP,MP�̕\���p
		noDel_ptr<GameObject> pHPBar;
		noDel_ptr<GameObject> pMPBar;
		noDel_ptr<GameObject> pHPText;
		noDel_ptr<GameObject> pMPText;
		noDel_ptr<GameObject> pHPFrame;
		noDel_ptr<GameObject> pMPFrame;

	private:
		void Awake() override;

	public:
		void Open(noDel_ptr<BattleChara> cbattleCharahara);
	};
}
