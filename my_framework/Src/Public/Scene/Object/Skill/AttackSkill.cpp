#include "../../../../../framework.h"
#include "../../../../../environment.h"

#include "AttackSkill.h"

void AttackSkill::InitAttackSkill(){
	FILE* fp;
	fopen_s(&fp, "Data/Status/Common/at_skill.txt", "r");
	if (fp == NULL) return;

	char _key[256] = "";
	AttackSkill* _pInstance = 0;

	//ŠeƒXƒLƒ‹î•ñ‚ð“Ç‚Ýž‚Þ
	while (!feof(fp))
	{
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));

		if (strcmp(_key, "id") == 0) {
			_pInstance = new AttackSkill();
			_pInstance->skillType = eSkillType::Attack;
			fscanf_s(fp, "%d", &_pInstance->id);
		}
		if (_pInstance != NULL) {
			if (strcmp(_key, "name") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				WCHAR _temp[256] = L"";
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)_key, _countof(_key), _temp, _countof(_temp));
				_pInstance->name = _temp;
			}
			else if (strcmp(_key, "attr") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				WCHAR _attrName[8] = L"";
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)_key, _countof(_key), _attrName, _countof(_attrName));
				_pInstance->pAttr = Attribute::GetAttr(_attrName);
			}
			else if (strcmp(_key, "consume") == 0) {
				fscanf_s(fp, "%d", &_pInstance->consumeMP);
			}
			else if (strcmp(_key, "value") == 0) {
				fscanf_s(fp, "%d", &_pInstance->at);
			}
			else if (strcmp(_key, "min_dis") == 0) {
				fscanf_s(fp, "%d", &_pInstance->min_dis);
			}
			else if (strcmp(_key, "max_dis") == 0) {
				fscanf_s(fp, "%d", &_pInstance->max_dis);
			}
			else if (strcmp(_key, "hit") == 0) {
				fscanf_s(fp, "%d", &_pInstance->hit);
				umSkill[_pInstance->id] = _pInstance;
				_pInstance = NULL;
			}
		}
	}

	fclose(fp);
}