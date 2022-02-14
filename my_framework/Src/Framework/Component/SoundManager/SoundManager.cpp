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

//鳴らす
void SoundManager::Play(std::string name) {
	umSound[name]->Play();
}
//再開
void SoundManager::Resume(std::string name) {
	umSound[name]->Resume();
}
//止める(全体)
void SoundManager::Stop() {
	for (auto& sound : umSound) {
		sound.second->Stop();
	}
}
//止める(単体)
void SoundManager::Stop(std::string name) {
	umSound[name]->Stop();
}
//音量調節(全体)
void SoundManager::SetVolume(float volume) {
	for (auto& sound : umSound) {
		sound.second->SetVolume(volume);
	}
}
//音量調節(単体)
void SoundManager::SetVolume(std::string name, float volume) {
	umSound[name]->SetVolume(volume);
}
//鳴っているか(単体)
bool SoundManager::IsPlaying(std::string name) {
	return umSound[name]->isPlaying();
}

//サウンドを加える
void SoundManager::AddSound(std::string name, LPCWSTR file) {
	umSound[name] = new Sound(file);
}