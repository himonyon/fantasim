#include "Main.h"

//�Œ�t���[�����[�g�̂��߂̃p�t�H�[�}���X�J�E���^�[
bool performanceCounter;
LARGE_INTEGER freq, startCount, finishCount;
float count;

std::mutex mem_mutex_;

bool Main::Init(void* hWnd) {
	//�p�t�H�[�}���X�J�E���^�̏�����
	memset(&freq, 0, sizeof(freq));
	memset(&startCount, 0, sizeof(startCount));
	memset(&finishCount, 0, sizeof(finishCount));
	count = 0.0f;

	performanceCounter = QueryPerformanceFrequency(&freq);

	//�}�E�X�J�[�\�����\���ɂ���
	ShowCursor(false);

	//--------------------------------------------------

	//------------------------------------

	//�t���[�����[�N-------------------------------
	Direct3D::InitD3D(hWnd);
	Font::Initialize(hWnd);
	Shader::InitShader();
	Renderer2D::Initialize();
	Renderer3D::Initialize();
	Sound::InitSound();
	DirectInput::InitInput(hWnd);
	InputConfig::SetUpConfig();
	
	//�f�o�b�O
	DebugFont::Initialize(hWnd);


	//�Q�[��------------------------------------------------
	Attribute::InitAttribute(); //�����쐬
	AttackSkill::InitAttackSkill(); //�X�L���쐬
	HealSkill::InitHealSkill(); //�X�L���쐬
	BuffSkill::InitBuffSkill(); //�X�L���쐬

	noDel_ptr<AttackSkill> at = dynamic_noDel_cast<AttackSkill>(Skill::GetSkill(6));

	//�V�[���쐬
	SceneManager::SwitchScene();

	return true;
}

void Main::Destroy() {
	//�Q�[��------------------------------------------------
	SceneManager::DeleteScene();
	Skill::DestroySkill(); //�X�L���j��
	Attribute::DestroyAttribute(); //�����j��

	//�f�o�b�O
	DebugFont::Destroy();

	//�t���[�����[�N-------------------------------
	InputConfig::DestroyConfig();
	DirectInput::DestroyInput();
	Sound::DestroySound();
	Renderer2D::Destroy();
	Renderer3D::Destroy();
	Shader::DestroyShader();
	Font::Destroy();
	Direct3D::DestroyD3D();
}

void Main::App() {
	if (performanceCounter)
	{	//�t���[���̊J�n���Ԃ��擾
		QueryPerformanceCounter(&startCount);
	}

	//�t���[���^�C���v��
	Timer::FrameTimeExecute();

	//���C������
	Execute();

	//�`��N���A
	Direct3D::Clear();

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT	Viewport;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = (float)WINDOW_WIDTH;
	Viewport.Height = (float)WINDOW_HEIGHT;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Direct3D::getDeviceContext()->RSSetViewports(1, &Viewport);

	//�`��
	Render();

	//�V�[���؂�ւ��m�F
	SceneManager::SwitchScene();

	//�t���[�����[�g����
	if (performanceCounter)
	{	//�t���[���̏I�����Ԃ��擾
		static float frame_msec = 1.0f / 60.0f;
		QueryPerformanceCounter(&finishCount);
		count = ((float)(finishCount.QuadPart - startCount.QuadPart) / (float)freq.QuadPart);

		if (count < frame_msec)
		{	//1/60�b���o�܂ő҂�
			DWORD wait = (DWORD)((frame_msec - count) * 1000.0f - 0.5f);
			timeBeginPeriod(1);
			Sleep(wait);
			timeEndPeriod(1);
		}
	}

	Direct3D::getSwapChain()->Present(1, 0);
}

void Main::Execute() {
	DirectInput::KeyManager();

	//�f�o�b�O�[�|�|�|�|�|
	if (Debug::open) {
		Debug::Execute();
		return;
	}
	//----------------------

	if(SceneManager::GetMainScene() != nullptr) SceneManager::GetMainScene()->Execute();
}

void Main::Render() {
	if (SceneManager::GetMainScene() != nullptr)SceneManager::GetMainScene()->Render();

	//�f�o�b�O
	DebugFont::Render();
}