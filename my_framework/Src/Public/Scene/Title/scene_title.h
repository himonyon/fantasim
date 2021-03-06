#pragma once

#include "Behaviour/TitleManager.h"

namespace nsTitle {
	class SceneTitle : public Scene {
	private:
		noDel_ptr<GameObject> pBg;

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
