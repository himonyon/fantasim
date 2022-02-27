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
		//�������
		noDel_ptr<Operation> pOperation;

		//�J�ڐ�̃p�l��
		noDel_ptr<CityPanel> pCityPanel;
		noDel_ptr<InvestPanel> pInvestPanel;
		noDel_ptr<SelectNeighborPanel> pSelectNeighborPanel;
		noDel_ptr<BattlePanel> pBattlePanel;
		noDel_ptr<CharaSelectPanel> pCharaSelectPanel;
		noDel_ptr<CharaEnhancePanel> pCharaEnhancePanel;
		noDel_ptr<OptionMenu> pOptionMenu;
		noDel_ptr<OptionPanel> pOptionPanel;
		noDel_ptr<EventPanel> pEventPanel;

		//�I�����ꂽ�X
		noDel_ptr<City> pSelectCity;

		//�ύX���ꂽ���m�F�p
		eTurnState checkState = eTurnState::None;

	public:
		void TurnInit();

	private:
		void Awake() override;

		void Start() override;

		void Update() override;

		//�K�v�ȃp�l���̍쐬
		void CreatePanels();

		//�e����-----------------------------
		//�t�B�[���h���
		void FieldFunc();
		//�^�[���I��
		void TurnEndFunc();
		//�Z�[�u
		void SaveFunc();
		//�I�v�V�������j���[
		void OptionMenuFunc();
		//�I�v�V����
		void OptionFunc();
		//�X�̃R�}���h����
		void CommandFunc();
		//�����R�}���h�n
		void InvEconomicFunc();
		void InvPoliticalFunc();
		void InvDisasterFunc();
		void InvSupportFunc();
		//�����ړ��R�}���h
		void SelMoneyMoveToCityFunc();
		void MoveMoneyFunc();
		//�L�����ړ��R�}���h
		void SelCharaMoveToCityFunc();
		void MoveCharaFunc();
		//�퓬�R�}���h
		void SelBattleCityFunc();
		void BattleFunc();
		//�L��������
		void CharaEnhance();
	};
}