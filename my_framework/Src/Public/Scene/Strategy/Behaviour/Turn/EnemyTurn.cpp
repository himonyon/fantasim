#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void EnemyTurn::Start() {
	//�Q�[���}�l�[�W���[
	pGM = gameObject->FindGameObject("gameManager")->GetComponent<GameManager>();

	//�o�g���p�l��
	pBtPanel = gameObject->FindGameObject("battlePanel")->GetComponent<BattlePanel>();
	pInfoPanel = gameObject->FindGameObject("infoPanel")->GetComponent<InfoPanel>();

	//�v���C���[�̍�
	for (int i = 0; i < TERRITORY_NUM; i++) {
		noDel_ptr<Country> _temp = dynamic_noDel_cast<Country>(pGM->GetCity(i));
		if (_temp == NULL) continue;
		if (_temp->IsPlayer() == false) continue;
		pPlayerCountry = _temp;
		break;
	}
}

void EnemyTurn::Update() {
	if (stopSimulate) return;
	if (pInfoPanel->IsOpen()) return;

	//�^�C�}�[����
	timer.Execute();

	//�������I����Ă���Ύ��Ԃ܂ŉ������Ȃ�
	if (isSimulated) {
		//���̊X���Z�b�g
		if (timer.time >= 200){
			//�X�V
			pTargetCity->FocusCity(false);
			isSimulated = false;
			while (true) {
				curCityID++;
				//���ׂĊ���������^�[���؂�ւ�
				if (curCityID >= TERRITORY_NUM) {
					pGM->ChangeTurn();
					return;
				}
				//�^�[�Q�b�g�Z�b�g
				pTargetCity = pGM->GetCity(curCityID);
				if (pTargetCity->pCountry != pPlayerCountry) break;
			}
			//�^�C�}�[�X�^�[�g
			timer.Start();
		}
		return;
	}

	//�^�[�Q�b�g�̊X�̃V�~�����[�g
	Simulate();
}

void EnemyTurn::TurnInit() {
	if (pGM == NULL) Start();
	curCityID = 0;
	//�^�[�Q�b�g�Z�b�g
	while (true) {
		pTargetCity = pGM->GetCity(curCityID);
		if (pTargetCity->pCountry != pPlayerCountry) break;
		curCityID++;
	}
	timer.Start();
}
void EnemyTurn::TurnInit(int id) {
	if (pGM == NULL) Start();
	//�r������
	curCityID = id + 1;
	//���ׂĊ���������^�[���؂�ւ�
	if (curCityID >= TERRITORY_NUM) {
		pGM->ChangeTurn();
		return;
	}
	//�^�[�Q�b�g�Z�b�g
	while (true) {
		pTargetCity = pGM->GetCity(curCityID);
		if (pTargetCity->pCountry != pPlayerCountry) break;
		curCityID++;
	}
	timer.Start();
}

////�^�[�Q�b�g�̊X�̃V�~�����[�g
void EnemyTurn::Simulate() {
	//�摜�̃t�H�[�J�X
	pTargetCity->FocusCity(true);

	//����-----------------------------------
	//�퓬
	if (Battle()) isSimulated = true;
	if (isSimulated) return;

	//�L�����h��
	if (CharaMove()) isSimulated = true;
	if (isSimulated) return;

	//�L��������
	if (CharaEnhance()) isSimulated = true;
	if (isSimulated) return;

	//�����ړ�
	if (MoneyMove()) isSimulated = true;
	if (isSimulated) return;

	//���Y�͌���
	if (EconomicUp()) isSimulated = true;
	if (isSimulated) return;

	//����
	if (PoliticalUp()) isSimulated = true;
	if (isSimulated) return;

	//�ЊQ�΍�
	if (DisasterUp()) isSimulated = true;
	if (isSimulated) return;

	//����
	if (SupportUp()) isSimulated = true;
	if (isSimulated) return;

	isSimulated = true;
}

bool EnemyTurn::Battle() {
	/*����----------------------------------------------------
	-�L�����̍��v�p���[����-30����Ȃ�퓬���d�|����
	-�m����40%
	--------------------------------------------------------*/
	//�L�����̍��v�p���[
	int _cityPower = 0;
	for (auto& chara : pTargetCity->vOwnChara) {
		_cityPower += chara->power;
	}

	//�ׂ荇���G�̒n�̍��v�L�����p���[���r
	for (auto& neigh : pTargetCity->GetNeighbor()) {
		//�������Ȃ�X�L�b�v
		if (neigh->pCountry == pTargetCity->pCountry) continue;
		//�׍��̍��v�L�����p���[�Z�o
		int _neighPower = 0;
		for (auto& chara : neigh->vOwnChara) {
			_neighPower += chara->power;
		}
		//�p���[����-�R�O���傫�����m�F
		if (_cityPower - _neighPower <= -30) continue;
		//40�p�[�Z���g�̊m��
		srand((unsigned int) time(NULL));
		int _rand = rand() % 100;
		if (_rand >= 40) continue;
		//�퓬
		//�v���C���[�ւ̐퓬�̏ꍇ
		if (neigh->pCountry == pPlayerCountry) {
			//�퓬�p�l�����J��
			pBtPanel->Open(neigh, pTargetCity, false);
			stopSimulate = true;
		}
		//�G���m�Ő퓬���s���ꍇ
		else {
			//�L�����̍��v�p���[�������ق��Ɋm�����X����
			int _rate = 0;
			if (_cityPower > _neighPower) _rate = 70;
			else _rate = 30;

			//�퓬���ʂ𔽉f
			_rand = rand() % 100;
			//�V�~�����[�g���Ă��鍑�̏���
			if (_rand < _rate) neigh->ChangeBelongCountry(pTargetCity->pCountry);
			//�׍��̏���
			else if (_rand < _rate) pTargetCity->ChangeBelongCountry(neigh->pCountry);
		}

		return true;
	}

	return false;
}
bool EnemyTurn::CharaEnhance() {
	/*����----------------------------------------------------
	-������10000�ȏ�
	-�m��20%
	--------------------------------------------------------*/

	return false;
}
bool EnemyTurn::CharaMove() {
	/*����----------------------------------------------------
	-�ׂ̗̒n�ɓG�̒n���Ȃ�
	-�ׂ̗̒n�̃L������5�̖���
	-�m��20%
	--------------------------------------------------------*/

	//�L���������Ȃ���Ώ������Ȃ�
	if (pTargetCity->vOwnChara.size() == 0) return false;

	//�ׂɗ̒n�����邩�m�F
	for (auto& neigh : pTargetCity->GetNeighbor()) {
		if (neigh->pCountry != pTargetCity->pCountry) return false;
	}

	//�ׂ̗̒n�̃L������5�̖���
	for (auto& neigh : pTargetCity->GetNeighbor()) {
		int _charaNum = (int)neigh->vOwnChara.size();
		if (_charaNum >= OWN_CHARACTOR_NUM) continue;
		//�m��20%
		srand((unsigned int)time(NULL));
		int _rand = rand() % 100;
		if (_rand >= 20) continue;

		//�L�����ړ�----------------
		//�X����L�����𔲂�
		noDel_ptr<Charactor> _pTempChara = pTargetCity->vOwnChara[0];
		pTargetCity->vOwnChara.erase(pTargetCity->vOwnChara.begin());
		//�L�������ړ�
		neigh->vOwnChara.emplace_back(_pTempChara);
		return true;
	}

	return false;
}
bool EnemyTurn::MoneyMove() {
	/*����----------------------------------------------------
	-�ׂ̊X�̎������I�𒆂̊X�̔����ȉ�
	-�m��50%
	--------------------------------------------------------*/

	//�ׂ̊X�̎������I�𒆂̊X�̔����ȉ�
	for (auto& neigh : pTargetCity->GetNeighbor()) {
		if (neigh->pCountry != pTargetCity->pCountry) continue;
		if (neigh->money > pTargetCity->money / 2) continue;
		//�m��20%
		srand((unsigned int)time(NULL));
		int _rand = rand() % 100;
		if (_rand >= 50) continue;
		//�����ړ�
		int _moveMoney = pTargetCity->money / 4;
		pTargetCity->money -= _moveMoney;
		neigh->money += _moveMoney;
		return true;
	}
	
	return false;;
}
bool EnemyTurn::EconomicUp() {
	/*����----------------------------------------------------
	-������2000�ȏ�
	-�m��30%
	--------------------------------------------------------*/

	if (pTargetCity->money <= 2000) return false;
	//�m��30%
	srand((unsigned int)time(NULL));
	int _rand = rand() % 100;
	if (_rand >= 30) return false;
	//�o�ϗ͌���
	pTargetCity->money -= 1000;
	pTargetCity->economic += 1000 / 10;

	return true;
}
bool EnemyTurn::PoliticalUp() {
	/*����----------------------------------------------------
	-������2000�ȏ�
	-�m��15%
	--------------------------------------------------------*/

	if (pTargetCity->money <= 2000) return false;
	//�m��15%
	srand((unsigned int)time(NULL));
	int _rand = rand() % 100;
	if (_rand >= 15) return false;
	//�������
	pTargetCity->money -= 1000;
	pTargetCity->political += 1000 / 10;

	return true;
}
bool EnemyTurn::DisasterUp() {
	/*����----------------------------------------------------
	-������500�ȏ�
	-�ЊQ�ϐ�60�ȉ���50%
	-�ЊQ�ϐ�80�ȉ���20%
	-�ЊQ�ϐ�90�ȉ���10%
	--------------------------------------------------------*/

	if (pTargetCity->money <= 500) return false;
	//�m��
	int _rate = 0;
	if (pTargetCity->disaster <= 60) _rate = 50;
	else if (pTargetCity->disaster <= 80) _rate = 20;
	else if (pTargetCity->disaster <= 90) _rate = 10;

	srand((unsigned int)time(NULL));
	int _rand = rand() % 100;
	if (_rand >= _rate) return false;
	//�ЊQ�ϐ�����
	pTargetCity->money -= 500;
	pTargetCity->disaster += 500 / 100;

	return true;
}
bool EnemyTurn::SupportUp() {
	/*����----------------------------------------------------
	-������500�ȏ�
	-�x����60�ȉ���50%
	-�x����80�ȉ���20%
	-�x����90�ȉ���10%
	--------------------------------------------------------*/

	if (pTargetCity->money <= 500) return false;
	//�m��
	int _rate = 0;
	if (pTargetCity->support <= 60) _rate = 50;
	else if (pTargetCity->support <= 80) _rate = 20;
	else if (pTargetCity->support <= 90) _rate = 10;

	srand((unsigned int)time(NULL));
	int _rand = rand() % 100;
	if (_rand >= _rate) return false;
	//�ЊQ�ϐ�����
	pTargetCity->money -= 500;
	pTargetCity->support += 500 / 100;

	return true;
}