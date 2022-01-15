#pragma once
/*-----------------------------------------------------------

	HealSkill�N���X(Skill�h���N���X)
	 Heal�X�L���̏ڍ׏��

-------------------------------------------------------------*/

enum class eHealType {
	HP,
	ALL_HP,
	MP,
	ALL_MP
};

class HealSkill : public Skill {
private:
	eHealType healType = eHealType::HP;
	bool isFixed = false; //�Œ�̒l���̒l�ˑ���
	int heal = 0;

public:
	static void InitHealSkill();

	bool IsFixed() { return isFixed; }
	int GetHeal() { return heal; }
	eHealType GetHealType() { return healType; }
};