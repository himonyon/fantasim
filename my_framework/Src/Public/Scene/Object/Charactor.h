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
	std::unordered_map<eBuffType, std::shared_ptr<stBuff>> umBuff;

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
	

	//バフのセット
	void SetBuff(const stBuff buff);
	//バフ取得
	std::unordered_map<eBuffType, std::shared_ptr<stBuff>> GetBuff() { return umBuff; };
	std::shared_ptr<stBuff> GetBuff(eBuffType type);
	//バフの削除
	void DeleteBuff(eBuffType type);
};