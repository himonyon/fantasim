/*-----------------------------------------------------------

	�f�o�b�O�p

-------------------------------------------------------------*/
#pragma once

class Debug {
public:
	static noDel_ptr<nsStrategy::City> pCities[TERRITORY_NUM];
	static int scene;

	//�L�����N�^�[�X�e�[�^�X�i�O�̃V�[����������p���j
	static std::vector<noDel_ptr<Charactor>> vChara;

	static bool open;

	static Sprite* pBg;

	//�J�[�\��
	static int rowCursor;
	static int colCursor;

public:
	//�f�o�b�O����
	static void Execute();

	//�X�g���e�W�[�ł̃f�o�b�O���
	static void Strategy();
	static void SetStrategy(noDel_ptr<nsStrategy::City>* cities);

	//�o�g���V�[���ł̃f�o�b�O���
	static void Battle();
	static void SetBattle(std::vector<noDel_ptr<Charactor>> p, std::vector<noDel_ptr<Charactor>> e);
};