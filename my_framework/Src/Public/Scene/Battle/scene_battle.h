#pragma once
/*-----------------------------------------------------------

	バトルシーンクラス(Sceneの派生)

-------------------------------------------------------------*/

#include "Behaviour/Cursor.h"
#include "Behaviour/SelectSquare.h"
#include "Object/Square.h"
#include "Object/AStar.h"
#include "Behaviour/Charactor/BattleChara.h"
#include "Behaviour/Charactor/PlayerChara.h"
#include "Behaviour/Charactor/EnemyChara.h"
#include "Behaviour/Panel/StatusPanel.h"
#include "Behaviour/Panel/SelectSkillPanel.h"
#include "Behaviour/Panel/Command.h"
#include "Behaviour/Turn/Turn.h"
#include "Behaviour/Turn/PlayerTurn.h"
#include "Behaviour/Turn/EnemyTurn.h"
#include "Behaviour/FieldManager.h"

namespace nsBattle {
	enum class eRenderOrder {
		Backgorund,
		Object = 10,
		UI = 20,
	};

	class SceneBattle : public Scene {
	private:
		//カメラ
		noDel_ptr<GameObject> pCam;

		//カーソル
		noDel_ptr<GameObject> pCursor;

		//背景
		noDel_ptr<GameObject> pBg;

		//フィールドマップマネージャー
		noDel_ptr<GameObject> pFieldManager;

		//ステータスパネル
		noDel_ptr<GameObject> pStatusPanel;

	public:
		//キャラクターステータス（前のシーンから引き継ぐ）
		std::vector<noDel_ptr<Charactor>> vPlayerCharaBace;
		std::vector<noDel_ptr<Charactor>> vEnemyCharaBace;

	private:
		//初期化
		void Initialize();

		//削除
		void Terminate();

		//処理
		void Execute();

		//描画
		void Render();
	};
}
