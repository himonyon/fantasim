#pragma once

#include "Behaviour/TitleManager.h"

namespace nsTitle {
	class SceneTitle : public Scene {
	private:
		noDel_ptr<GameObject> pBg;

	private:
		//‰Šú‰»
		void Initialize();

		//íœ
		void Terminate();

		//ˆ—
		void Execute();

		//•`‰æ
		void Render();
	};
}
