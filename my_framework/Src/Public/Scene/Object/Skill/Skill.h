#pragma once
/*-----------------------------------------------------------

	Skillクラス(基底クラス)
	 スキル全般の共通クラス

-------------------------------------------------------------*/

enum class eSkillType {
	Attack,
	Heal,
	Buff
};

class Skill {
protected:
	static std::unordered_map<int, Skill*> umSkill;

protected:
	int id = 0;
	eSkillType skillType = eSkillType::Attack;
	std::wstring name = L"";
	noDel_ptr<Attribute> pAttr = 0;
	int consumeMP = 0;
	int min_dis = 0;
	int max_dis = 0;

public:
	virtual ~Skill() {};

	static void DestroySkill();
	static noDel_ptr<Skill> GetSkill(int id) { return noDel_ptr<Skill>(umSkill[id]); }

	//Getter,Setter
	eSkillType GetSkillType() { return skillType; }
	std::wstring GetName() { return name; }
	noDel_ptr<Attribute> GetAttr() { return pAttr; }
	int GetConsumeMP() { return consumeMP; }
	int GetMinDis() { return min_dis; }
	int GetMaxDis() { return max_dis; }
};