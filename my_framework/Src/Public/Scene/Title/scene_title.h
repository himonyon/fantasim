#pragma once

#include "Behaviour/TitleManager.h"

namespace nsTitle {
	class SceneTitle : public Scene {
	private:
		noDel_ptr<GameObject> pBg;

	private:
		//������
		void Initialize();

		//�폜
		void Terminate();

		//����
		void Execute();

		//�`��
		void Render();
	};
}
