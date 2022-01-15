#pragma once
/*-----------------------------------------------------------

	BattleChara�N���X(Behaviour�̔h��)
	�o�g���Ŏg�p����L�����̋���

-------------------------------------------------------------*/
namespace nsBattle {
	class PlayerTurn : public Turn {
	private:
		//�R�}���h�p�l��
		noDel_ptr<CommandPanel> pCommandPanel;

		//�����I�u�W�F�N�g----------------------
		noDel_ptr<GameObject> pStatusPanel;
		//�J�[�\��
		noDel_ptr<Cursor> pCursor;

	private:
		void Start() override;

		void Update() override;

		void SelectChara();

		//�ʏ�s��
		void NormalFunc();

		//�L�����̃J�[�\��������Ď��̋���
		void ShowCharaInfo();

	};
}
