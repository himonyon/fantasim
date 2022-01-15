#pragma once
/*-----------------------------------------------------------

	AttackSkillクラス(Skill派生クラス)
	 Attackスキルの詳細情報

-------------------------------------------------------------*/

class AttackSkill : public Skill {
private:
	int at = 0;
	int hit = 0;

public:
	static void InitAttackSkill();

	int GetAt() { return at; }
	int GetHit() { return hit; }
};