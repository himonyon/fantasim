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
	std::unordered_map<eBuffType, std::shared_ptr<stBuff>> umBuff;

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
	Charactor(int id, std::string file);
	Charactor(const Charactor&);
	~Charactor();

	//getter,setter
	int GetID() { return id; }
	std::wstring GetName() { return name; }
	std::wstring GetSpriteName() { return sp_name; }
	noDel_ptr<Sprite> GetSprite() { return noDel_ptr<Sprite>(pSprite); }
	noDel_ptr<Attribute> GetAttr() { return pAttr; }
	std::vector<noDel_ptr<Skill>> GetSkills() const { return vpSkill; }
	

	//�o�t�̃Z�b�g
	void SetBuff(const stBuff buff);
	//�o�t�擾
	std::unordered_map<eBuffType, std::shared_ptr<stBuff>> GetBuff() { return umBuff; };
	std::shared_ptr<stBuff> GetBuff(eBuffType type);
	//�o�t�̍폜
	void DeleteBuff(eBuffType type);
};