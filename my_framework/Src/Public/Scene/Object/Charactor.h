/*-----------------------------------------------------------

	Charactor�N���X
	�@�L�����N�^�[�Ǘ��N���X

-------------------------------------------------------------*/
#pragma once

#define MAX_OWN_SKILL (4)

class Charactor {
private:
	int id = 0;
	std::wstring name = L""; //���O
	std::wstring sp_name = L""; //�X�v���C�g�̖��O
	Sprite* pSprite = 0; //�X�v���C�g
	noDel_ptr<Attribute> pAttr = 0; //����
	std::vector<noDel_ptr<Skill>> vpSkill; //�X�L��
	std::vector<stBuff> vBuff;

public:
	int level = 1;
	int power = 0; //�̒l
	int maxHp = 0; //HP
	int hp = 0; //HP
	int maxMp = 0; //MP
	int mp = 0; //MP
	int def = 0; //�h���
	int evd = 0; //����
	int move = 0; //�ړ���

public:
	Charactor(int id);
	Charactor(const Charactor&);
	~Charactor();

	//getter,setter
	int GetID() { return id; }
	std::wstring GetName() { return name; }
	noDel_ptr<Sprite> GetSprite() { return noDel_ptr<Sprite>(pSprite); }
	noDel_ptr<Attribute> GetAttr() { return pAttr; }
	std::vector<noDel_ptr<Skill>> GetSkills() const { return vpSkill; }
};