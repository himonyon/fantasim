/*-----------------------------------------------------------

	OptionPanel�N���X
		�Q�[���ݒ���

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class OptionPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

		//�e�L�X�g
		noDel_ptr<Font> pSoundText[2];
		noDel_ptr<Font> pDeviceText[3];

		//�I�v�V�����̍���
		enum class eOptionSeed {
			Sound,
			Device,
			Max,
		};

		//�I�����ꂽ�ݒ�
		bool sound = true;
		eDeviceType deviceType = eDeviceType::Both;

		//�J�[�\��
		int colSelecter = 0; //�c��̃J�[�\��
		int rowSelecter = 0; //����̃J�[�\��

	private:
		void Awake() override;
		void Update() override;

		//�I�v�V�������ڂ�I��
		void MoveColSelecter();
		void MoveRowSelecter();

		//�T�E���h���ڂ̑I��
		void SoundSetting(bool sound);
		//�f�o�C�X���ڂ̑I��
		void DeviceSetting(int type);

	public:
		void Open();
		void Close();
	};
}
