#pragma once
/*-----------------------------------------------------------

	AttackSkill�N���X(Skill�h���N���X)
	 Attack�X�L���̏ڍ׏��

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