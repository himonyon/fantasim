/*-----------------------------------------------------------

	�X�v���C�g�A�j���[�V�����N���X
		�L�[���t���[�����Ƌ��ɐݒ肵�Ċe�L�[�̎��ԂɂȂ��
		���̏������s���A�j���[�V�����@�\

-------------------------------------------------------------*/
#pragma once

class SpriteAnimation {
private:
	bool isLoop = false;

	bool isEnd = false;

	float frameCount = 0; //�i�񂾃t���[����

	int curKeyIndex = -1; //���ݓ��B���Ă���L�[�t���[���z��ԍ�

	noDel_ptr<Renderer2D> pAnimRenderer = NULL; //�\������X�v���C�g�����_���[

private:
	//�A�j���[�V�����̃L�[�t���[��
	class KeyFrame {
	public:
		Sprite* pSprite = 0;
		float frame = 0;
		float x = 0, y = 0;
		stColor4 color = {1,1,1,1};
		float scaleX = 1, scaleY = 1;
		float rot = 0;

		//�Ӑ}�I�ɃA�j���[�V���������镔�ʂ����ׂ�g���K�[
		struct stTrigger {
		public:
			bool scale = false;
			bool rotation = false;
			bool color = false;
		};

		stTrigger trigger;

	public:
		KeyFrame() {};
		~KeyFrame();
	};

	

private:
	std::vector<KeyFrame*> keyFrames;

	//�L�[���̃f�t�H���g�l�Z�b�g�A�b�v
	void SetUpDefaultValue(KeyFrame*);

	//�L�[�t���[���̃X�v���C�g��Ԃ̔��f
	void SetKeySpriteState(KeyFrame*);

	//����̃L�[�t���[����Ԃɐݒ�(���W�ȊO)
	void SetKeyState(KeyFrame*);

	//���̃L�[�Ɍ����ĕω��@rate�͕ω���
	void TransPos(KeyFrame* key);
	void TransCol(KeyFrame* key, float rate);
	void TransScl(KeyFrame* key, float rate);
	void TransRot(KeyFrame* key, float rate);

public:
	SpriteAnimation(const WCHAR* animation_file, bool loop = false);
	~SpriteAnimation();

	void AnimOn(); //�A�j���[�V�������s
	void AnimOff(); //�A�j���[�V�����I��

	//�A�j���[�V����������Ώۃ����_���[�ݒ�
	void SetAnimRenderer(noDel_ptr<Renderer2D>);

	bool IsLoop() { return isLoop; }
	bool IsEnd() { return isEnd; }
};