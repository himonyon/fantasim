#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void Turn::SetTurnState(eTurnState state, int backNum) {
	//�߂��Ԃ�����ꂽ���O�̏�Ԃɖ߂�
	if (state == eTurnState::Back) {
		for (int i = 0; i < backNum; i++) {
			turnState = turnState->bef.lock();
		}
		return;
	}

	//�ꗗ�ɂȂ��ꍇ�A�V�K�o�^
	if (umTurnStates.find(state) == umTurnStates.end()) {
		std::shared_ptr<stTurnState> _temp = std::make_shared<stTurnState>(stTurnState(state));
		umTurnStates[state] = _temp;
	}
	//���݂̏�Ԃ��Z�b�g
	if (turnState != NULL) umTurnStates[state]->bef = turnState; //�ȑO�̏�Ԃ��Z�b�g
	turnState = umTurnStates[state]; //���݂̏�Ԃ��Z�b�g
}