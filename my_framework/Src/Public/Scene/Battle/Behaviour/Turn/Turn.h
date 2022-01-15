#pragma once
/*-----------------------------------------------------------

	BattleCharaクラス(Behaviourの派生)
	バトルで使用するキャラの挙動

-------------------------------------------------------------*/
namespace nsBattle {
	enum class eFieldState {
		None,
		Field,
		Command,
	};

	class Turn : public Behaviour {
	public:
		//ゲームの進行状態
		static eFieldState curState;


	protected:
		//選択中のキャラクタ
		noDel_ptr<BattleChara> pSelectChara;

	public:
		//キャラクターのBattleCharaクラス取得
		std::vector<noDel_ptr<BattleChara>> vBattleChara;

	protected:

		//カメラ移動
		void MoveCamera();
	};
}
