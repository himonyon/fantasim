#pragma once
/*-----------------------------------------------------------

	BuffSkillクラス(Skill派生クラス)
	 Buffスキルの詳細情報

-------------------------------------------------------------*/

//バフタイプ
enum class eBuffType {
	AT,
	DEF,
	EVD
};

struct stBuff {
	eBuffType type; //バフタイプ
	float value; //バフの値
	int count; //バフの継続ターン数
};

class BuffSkill : public Skill {
private:
	stBuff buffInfo = {};

public:
	static void InitBuffSkill();

	stBuff GetBuff() { return buffInfo; }
};