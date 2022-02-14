#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace nsStrategy;


void Save::SaveBaceData() {
	FILE* fp;
	fopen_s(&fp, "Data/SaveData/base.txt", "w");
	if (fp == NULL) return;
	fprintf_s(fp, "save");

	fclose(fp);
}
void Save::SaveCities() {
	FILE* fp;
	_wfopen_s(&fp, L"Data/SaveData/city.txt", L"w");
	if (fp == NULL) return;
	for (int i = 0; i < TERRITORY_NUM; i++) {
		//����
		fprintf_s(fp, "new ");
		//ID
		fprintf_s(fp, "id %d ", pCities[i]->GetID());
		//���O
		char _name[256];
		WideCharToMultiByte(CP_ACP, 0, pCities[i]->GetName().c_str(), pCities[i]->GetName().length() *4, _name, _countof(_name), NULL, NULL);
		fprintf_s(fp, "name %s ", _name);
		//���̏ꍇ
		if (i < COUNTRY_NUM) {
			//�ח����Ă��邩
			if (dynamic_noDel_cast<Country>(pCities[i])->IsFallen()) {
				fprintf_s(fp, "fallen ");
			}
		}
		//������ID
		fprintf_s(fp, "country %d ", pCities[i]->pCountry->GetID());
		//�ʒu
		fprintf_s(fp, "position %f %f ", pCities[i]->transform->position.x, pCities[i]->transform->position.y);
		//���̏ꍇ
		if (i < COUNTRY_NUM) {
			//�v���C���[�̍���
			if (dynamic_noDel_cast<Country>(pCities[i])->IsPlayer()) {
				fprintf_s(fp, "player ");
			}
		}
		//�׍�ID
		fprintf_s(fp, "neighbor ");
		for (auto& neigh : pCities[i]->GetNeighbor()) {
			fprintf_s(fp, "%d ", neigh->GetID());
		}
		//����
		fprintf_s(fp, "money %d ", pCities[i]->money);
		//�o�ϗ�
		fprintf_s(fp, "economic %d ", pCities[i]->economic);
		//������
		fprintf_s(fp, "political %d ", pCities[i]->political);
		//�ЊQ�ϐ�
		fprintf_s(fp, "disaster %d ", pCities[i]->disaster);
		//�x����
		fprintf_s(fp, "support %d ", pCities[i]->support);
		//�i�C
		fprintf_s(fp, "bussiness %f ", pCities[i]->bussiness);
		//�����L����ID
		fprintf_s(fp, "chara ");
		for (auto& chara : pCities[i]->vOwnChara) {
			fprintf_s(fp, "%d ", chara->GetID());
		}

		//���s
		fprintf_s(fp, "\n");
	}

	fclose(fp);
}
void Save::SaveCharactor() {
	FILE* fp;
	_wfopen_s(&fp, L"Data/SaveData/chara.txt", L"w");
	if (fp == NULL) return;
	for (auto& ch: umCharactors) {
		Charactor* chara = ch.second;

		//����
		fprintf_s(fp, "new ");
		//ID
		fprintf_s(fp, "id %d ", chara->GetID());
		//���O
		char _name[256];
		WideCharToMultiByte(CP_ACP, 0, chara->GetName().c_str(), _countof(_name), _name, 256, NULL, NULL);
		fprintf_s(fp, "name %s ", _name);
		//�摜�̖��O
		WideCharToMultiByte(CP_ACP, 0, chara->GetSpriteName().c_str(), _countof(_name), _name, 256, NULL, NULL);
		fprintf_s(fp, "sp_name %s ", _name);
		//������
		WideCharToMultiByte(CP_ACP, 0, chara->GetAttr()->GetName().c_str(), _countof(_name), _name, 256, NULL, NULL);
		fprintf_s(fp, "attr %s ", _name);
		//���x��
		fprintf_s(fp, "level %d ", chara->level);
		//�퓬��
		fprintf_s(fp, "power %d ", chara->power);
		//HP
		fprintf_s(fp, "hp %d ", chara->maxHp);
		//MP
		fprintf_s(fp, "mp %d ", chara->maxMp);
		//DEF
		fprintf_s(fp, "def %d ", chara->def);
		//EVD
		fprintf_s(fp, "evd %d ", chara->evd);
		//MOVE
		fprintf_s(fp, "move %d ", chara->move);
		//SKILL
		fprintf_s(fp, "skill ");
		for (auto& skill : chara->GetSkills()) {
			fprintf_s(fp, "%d ", skill->GetID());
		}

		//���s
		fprintf_s(fp, "\n");
	}

	fclose(fp);
}
void Save::SaveBattleCities(int p_id, int e_id, bool playerTurn) {
	//�S�̕ۑ�
	SaveBaceData();
	SaveCities();
	SaveCharactor();

	//�o�g�����̕ۑ�
	FILE* fp;
	fopen_s(&fp, "Data/SaveData/battleCity.txt", "w");
	if (fp == NULL) return;

	//�ǂ���̃^�[����
	fprintf_s(fp, "turn ");
	if(playerTurn) fprintf_s(fp, "player\n");
	else fprintf_s(fp, "enemy\n");

	//�퓬����XID�ۑ�
	fprintf_s(fp, "p_city %d\n", p_id);
	fprintf_s(fp, "e_city %d\n", e_id);

	fclose(fp);
}