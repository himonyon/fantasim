#include "../../../../framework.h"
#include "../../../../environment.h"

SoundManager::SoundManager() : Component(eComponentType::Sound) {
}
SoundManager::~SoundManager() {
	for (auto& sound : umSound) {
		delete sound.second;
	}
	umSound.clear();
}

//�炷
void SoundManager::Play(std::string name) {
	umSound[name]->Play();
}
//�ĊJ
void SoundManager::Resume(std::string name) {
	umSound[name]->Resume();
}
//�~�߂�(�S��)
void SoundManager::Stop() {
	for (auto& sound : umSound) {
		sound.second->Stop();
	}
}
//�~�߂�(�P��)
void SoundManager::Stop(std::string name) {
	umSound[name]->Stop();
}
//���ʒ���(�S��)
void SoundManager::SetVolume(float volume) {
	for (auto& sound : umSound) {
		sound.second->SetVolume(volume);
	}
}
//���ʒ���(�P��)
void SoundManager::SetVolume(std::string name, float volume) {
	umSound[name]->SetVolume(volume);
}
//���Ă��邩(�P��)
bool SoundManager::IsPlaying(std::string name) {
	return umSound[name]->isPlaying();
}

//�T�E���h��������
void SoundManager::AddSound(std::string name, LPCWSTR file) {
	umSound[name] = new Sound(file);
}