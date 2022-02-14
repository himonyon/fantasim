/*-----------------------------------------------------------

	PlayerTurn�N���X(Turn�h��)
		�v���C���[�^�[���Ǘ�

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class PlayerTurn : public Turn {
	private:
		//�T�E���h�}�l�[�W���[
		noDel_ptr<SoundManager> pSoundManager;

		//�J�ڐ�̃p�l��
		noDel_ptr<CityPanel> pCityPanel;
		noDel_ptr<InvestPanel> pInvestPanel;
		noDel_ptr<SelectNeighborPanel> pSelectNeighborPanel;
		noDel_ptr<BattlePanel> pBattlePanel;
		noDel_ptr<CharaSelectPanel> pCharaSelectPanel;
		noDel_ptr<CharaEnhancePanel> pCharaEnhancePanel;

		//�I�����ꂽ�X
		noDel_ptr<City> pSelectCity;

	private:
		void Awake() override;

		void Start() override;

		void Update() override;

		//�K�v�ȃp�l���̍쐬
		void CreatePanels();

		//�e����
		void FieldFunc();
		void CommandFunc();
		void InvEconomicFunc();
		void InvPoliticalFunc();
		void InvDisasterFunc();
		void InvSupportFunc();
		void SelMoneyMoveToCityFunc();
		void MoveMoneyFunc();
		void SelCharaMoveToCityFunc();
		void MoveCharaFunc();
		void SelBattleCityFunc();
		void BattleFunc();
		void CharaEnhance();
	};
}