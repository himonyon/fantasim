/*-----------------------------------------------------------

	BattlePanel�N���X
		�o�g���Ɉڍs����O�̊m�F���

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class BattlePanel : public Panel {
	private:
		//Findobject
		noDel_ptr<GameObject> pCityPanel;
	private:
		//�Ώۂ̍��A�X
		noDel_ptr<City> pPlayerCity;
		noDel_ptr<City> pEnemyCity;

		noDel_ptr<GameObject> pPlayerCityNameText;
		noDel_ptr<GameObject> pEnemyCityNameText;

		noDel_ptr<GameObject> pStartText;

		noDel_ptr<GameObject> pPlayerCharaText[OWN_CHARACTOR_NUM];
		noDel_ptr<GameObject> pEnemyCharaText[OWN_CHARACTOR_NUM];

		//�J�[�\��
		noDel_ptr<GameObject> pSelectCursor;

		//�J�ڒ��̃g���K�[
		bool isSwitchingScene = false;

	public:
		void Awake() override;
		void Update() override;

		void Open(noDel_ptr<City> p_city, noDel_ptr<City> e_city);
		void Close() override;
	};
}