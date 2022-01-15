#pragma once
/*-----------------------------------------------------------

	BuffSkill�N���X(Skill�h���N���X)
	 Buff�X�L���̏ڍ׏��

-------------------------------------------------------------*/

//�o�t�^�C�v
enum class eBuffType {
	AT,
	DEF,
	EVD
};

struct stBuff {
	eBuffType type; //�o�t�^�C�v
	float value; //�o�t�̒l
	int count; //�o�t�̌p���^�[����
};

class BuffSkill : public Skill {
private:
	stBuff buffInfo = {};

public:
	static void InitBuffSkill();

	stBuff GetBuff() { return buffInfo; }
};