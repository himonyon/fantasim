#pragma once
/*-----------------------------------------------------------

	Save���X
	�@Strategy�̃Z�[�u�Ǘ�

-------------------------------------------------------------*/

namespace nsStrategy{
	class Save {
	public:
		//�̒n
		noDel_ptr<City> pCities[TERRITORY_NUM];
		//�L�����N�^�[
		std::unordered_map<int, Charactor*> umCharactors;

		//�v���C���[���ۊ�
		void SaveBaceData();
		//�X�̃X�e�[�^�X�Z�[�u
		void SaveCities();
		//�L�����̃X�e�[�^�X�Z�[�u
		void SaveCharactor();
		//�퓬����X��ID��ۊ�(p_id = �v���C���[�̊XID e_id = �G�̊XID)
		void SaveBattleCities(int p_id, int e_id, bool playerTurn);
	};
};