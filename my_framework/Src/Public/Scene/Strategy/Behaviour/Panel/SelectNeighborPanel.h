/*-----------------------------------------------------------

	SelectNeighborPanel�N���X
		�ߐڂ��Ă��鍑�܂��͊X��I������p�l���N���X

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class SelectNeighborPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

		//�Ώۂ̍��A�X
		noDel_ptr<City> pCity;

		int commandType = 0;

		//�אڂ��Ă��đΏۂƂȂ������E�X
		std::vector<noDel_ptr<City>> vSelectableCities;

		//�I�����Ă���ߗח̒n�̐���
		int selectNum = 0;

		//�e�L�X�g
		noDel_ptr<GameObject> pDescText;
		noDel_ptr<GameObject> pNameText;
		noDel_ptr<GameObject> pArrowText;

	private:
		void Awake() override;
		void Update() override;

	public:
		void Open(noDel_ptr<City> city, int cmd);
		void Close() override;
	};
}
