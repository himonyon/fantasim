#pragma once

//�J���[��16�i���֕ϊ�
unsigned int ColorConvert16(stColor4);

//XMVector��XMFLOAT3�֕ϊ�
_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
    return DirectX::XMLoadFloat3(&vec);
}

//�����_��3�ʂ܂łɂȂ�悤�l�̌ܓ�����
stVector3 RoundDec3forVector(stVector3);
float RoundDec3forFloat(float);

//����|�C���^�̃_�C�i�~�b�N�L���X�g
template <typename U, typename T>
noDel_ptr<U> dynamic_noDel_cast(noDel_ptr<T> ptr) {
    T* _ptr = ptr.get();
    return noDel_ptr<U>(dynamic_cast<U*>(_ptr));
}

//����|�C���^�̃X�^�e�B�b�N�L���X�g
template <typename U, typename T>
noDel_ptr<U> static_noDel_cast(noDel_ptr<T> ptr) {
    T* _ptr = ptr.get();
    return noDel_ptr<U>(static_cast<U*>(_ptr));
}

//���������؂�
void Split(char split_char, char* buffer, std::vector<std::string>& out);
//�������u��������
void Replace(char search_char, char replace_char, char* buffer);

//int�^�̐����́{�P�[�P���w��̕�(0~maxNum)�ōs��
int CountUpInRange(int target, int maxRange);
int CountDownInRange(int target, int maxRange);