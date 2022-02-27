/*-----------------------------------------------------------

	Turnクラス
		ターンの基底クラス

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	//ターン状態
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
	//現在の状態と一つ前の状態を持った構造体
	struct stTurnState {
		eTurnState state = eTurnState::None;
		std::weak_ptr<stTurnState> bef;

		stTurnState() {};
		stTurnState(eTurnState st) {
			state = st;
		}
	};

	//ターンクラス
	class Turn : public Behaviour{
	protected:
		//パネル
		noDel_ptr<InfoPanel> pInfoPanel;
		//対象とする街
		noDel_ptr<City> pTargetCity;
		//当たり判定のある街
		noDel_ptr<City> pFocusCity;
		//ターンの状態
		std::shared_ptr<stTurnState> turnState;
		//ターン状態一覧
		std::unordered_map<eTurnState, std::shared_ptr<stTurnState>> umTurnStates;

	public:
		//ターン状態セット
		void SetTurnState(eTurnState state, int backNum);

		//選択された街のセット
		void SetFocusCity(noDel_ptr<City> city) { pFocusCity = city; }

		//ターゲットにする街のセット
		void SetTargetCity(noDel_ptr<City> city) { pTargetCity = city; }

		//ゲーム終了
		void EndGame(bool isClear);
	};
}