/*-----------------------------------------------------------

	Turn�N���X
		�^�[���̊��N���X

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	//�^�[�����
	enum class eTurnState {
		None,
		Back,
		Field,
		Save,
		TurnEnd,
		OptionMenu,
		Option,
		Command,
		InvEconomic,
		InvPolitical,
		InvDisaster,
		InvSupport,
		SelMoneyMoveToCity,
		MoveMoney,
		SelCharaMoveToCity,
		MoveChara,
		SelBattleCity,
		Battle,
		CharaEnhance,
	};
	//���݂̏�Ԃƈ�O�̏�Ԃ��������\����
	struct stTurnState {
		eTurnState state = eTurnState::None;
		std::weak_ptr<stTurnState> bef;

		stTurnState() {};
		stTurnState(eTurnState st) {
			state = st;
		}
	};

	//�^�[���N���X
	class Turn : public Behaviour{
	protected:
		//�p�l��
		noDel_ptr<InfoPanel> pInfoPanel;
		//�ΏۂƂ���X
		noDel_ptr<City> pTargetCity;
		//�����蔻��̂���X
		noDel_ptr<City> pFocusCity;
		//�^�[���̏��
		std::shared_ptr<stTurnState> turnState;
		//�^�[����Ԉꗗ
		std::unordered_map<eTurnState, std::shared_ptr<stTurnState>> umTurnStates;

	public:
		//�^�[����ԃZ�b�g
		void SetTurnState(eTurnState state, int backNum);

		//�I�����ꂽ�X�̃Z�b�g
		void SetFocusCity(noDel_ptr<City> city) { pFocusCity = city; }

		//�^�[�Q�b�g�ɂ���X�̃Z�b�g
		void SetTargetCity(noDel_ptr<City> city) { pTargetCity = city; }

		//�Q�[���I��
		void EndGame(bool isClear);
	};
}