#pragma once
/*-----------------------------------------------------------

	HealSkillクラス(Skill派生クラス)
	 Healスキルの詳細情報

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
	bool isFixed = false; //固定の値か個体値依存か
	int heal = 0;

public:
	static void InitHealSkill();

	bool IsFixed() { return isFixed; }
	int GetHeal() { return heal; }
	eHealType GetHealType() { return healType; }
};