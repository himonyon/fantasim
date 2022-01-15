/*-----------------------------------------------------------

	Charactorクラス
	　キャラクター管理クラス

-------------------------------------------------------------*/
#pragma once

#define MAX_OWN_SKILL (4)

class Charactor {
private:
	int id = 0;
	std::wstring name = L""; //名前
	std::wstring sp_name = L""; //スプライトの名前
	Sprite* pSprite = 0; //スプライト
	noDel_ptr<Attribute> pAttr = 0; //属性
	std::vector<noDel_ptr<Skill>> vpSkill; //スキル
	std::vector<stBuff> vBuff;

public:
	int level = 1;
	int power = 0; //個体値
	int maxHp = 0; //HP
	int hp = 0; //HP
	int maxMp = 0; //MP
	int mp = 0; //MP
	int def = 0; //防御力
	int evd = 0; //回避力
	int move = 0; //移動量

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