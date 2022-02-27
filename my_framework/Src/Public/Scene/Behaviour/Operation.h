#pragma once
/*-----------------------------------------------------------

	Operator�N���X(Behaviour�h���N���X)
	��������̕\��

-------------------------------------------------------------*/
#pragma once

class Operation : public Behaviour{
private:
	struct stOprText {
		bool trigger; //�g�p�ς݂�
		noDel_ptr<Font> pText;
	};

	//��������̍ő吔
	static const int OprNum = 5;

	//����{�^���ꗗ�i�R���g���[���[�j
	std::unordered_map<std::string, std::wstring> umJoyButton;
	//����{�^���ꗗ�i�L�[�{�[�h���}�E�X�j
	std::unordered_map<std::string, std::wstring> umKeyMouButton;

	//��������e�L�X�g
	stOprText oprText[OprNum];

private:
	//������
	void Awake() override;

	//�����e�L�X�g�̍쐬
	void CreateOperation();

	//����{�^���ꗗ�쐬�i�R���g���[���[�j
	void CreateJoyButton();
	//����{�^���ꗗ�쐬�i�L�[�}�E�X)
	void CreateKeyMouButton();

public:
	//�����I�u�W�F�N�g���Z�b�g
	void AddOperation(std::string type, std::wstring text);
	//��������I�u�W�F�N�g�̃��Z�b�g
	void ResetOperation();
	//�\����\��
	void Open();
	void Hide();

};