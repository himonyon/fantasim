#include "../../../../framework.h"
#include "../../../../environment.h"

#include "Attribute.h"

std::unordered_map<std::wstring, Attribute*> Attribute::attrs;

Attribute::~Attribute() {
	delete pSprite;
}

void Attribute::InitAttribute() {
	FILE* fp;
	fopen_s(&fp, "Data/Status/Common/attribute.txt", "r");
	if (fp == NULL) return;

	char _key[256] = "";
	std::vector<std::wstring> _attrName;

	int _count = 0;
	
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));

		if (strcmp(_key, "name") == 0) {
			//�o�^���ꂽ�z�񏇂Ƀ_���[�W�䗦map�ɒl���i�[
			Attribute* instance = new Attribute();
			fscanf_s(fp, "%s", _key, (int)sizeof(_key)); //�������͕ێ����Ă���̂ŃX�L�b�v

			//���O��ݒ�
			instance->name = _attrName[_count];
			instance->SetSprite();
			
			for (auto& name : _attrName) {
				float _rate = 0;
				fscanf_s(fp, "%f", &_rate);
				instance->damage_rate[name] = _rate;
			}

			attrs[instance->name] = instance;
			_count++;
		}
		else {
			//�z��ɑ����ꗗ���i�[
			WCHAR _temp[8] = L"��";
			MultiByteToWideChar(
				CP_ACP,
				MB_PRECOMPOSED,
				(LPCCH)_key,
				_countof(_key),
				_temp,
				_countof(_temp));

			_attrName.emplace_back(_temp);
		}

	}

	fclose(fp);
}

void Attribute::DestroyAttribute() {
	for (auto& attr : attrs) {
		delete attr.second;
	}
	attrs.clear();
}

void Attribute::SetSprite() {
	if (this->name == L"��") this->pSprite = new Sprite(L"Data/Image/Common/attribute.spr", L"none");
	if (this->name == L"��") this->pSprite = new Sprite(L"Data/Image/Common/attribute.spr", L"fire");
	if (this->name == L"��") this->pSprite = new Sprite(L"Data/Image/Common/attribute.spr", L"water");
	if (this->name == L"��") this->pSprite = new Sprite(L"Data/Image/Common/attribute.spr", L"grass");
	if (this->name == L"��") this->pSprite = new Sprite(L"Data/Image/Common/attribute.spr", L"light");
	if (this->name == L"��") this->pSprite = new Sprite(L"Data/Image/Common/attribute.spr", L"dark");
}