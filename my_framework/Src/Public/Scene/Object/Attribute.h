#pragma once
/*-----------------------------------------------------------

	Attributeクラス
	　属性管理クラス

-------------------------------------------------------------*/
#pragma once

class Attribute {
private:
	static std::unordered_map<std::wstring, Attribute*> attrs;

private:
	std::wstring name = L"";
	Sprite* pSprite;

	std::unordered_map<std::wstring, float> damage_rate = {};

public:
	static void InitAttribute();
	static void DestroyAttribute();

	~Attribute();

	static noDel_ptr<Attribute> GetAttr(std::wstring name) { return noDel_ptr<Attribute>(attrs[name]); }

	std::wstring GetName() { return name; }
	noDel_ptr<Sprite> GetSprite() { return noDel_ptr<Sprite>(pSprite); }
	float GetDamageRate(std::wstring name) { return damage_rate[name]; }

private:
	void SetSprite();
};