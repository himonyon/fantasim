#pragma once

//カラーを16進数へ変換
unsigned int ColorConvert16(stColor4);

//XMVectorをXMFLOAT3へ変換
_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
    return DirectX::XMLoadFloat3(&vec);
}

//小数点を3位までになるよう四捨五入する
stVector3 RoundDec3forVector(stVector3);
float RoundDec3forFloat(float);

//自作ポインタのダイナミックキャスト
template <typename U, typename T>
noDel_ptr<U> dynamic_noDel_cast(noDel_ptr<T> ptr) {
    T* _ptr = ptr.get();
    return noDel_ptr<U>(dynamic_cast<U*>(_ptr));
}

//自作ポインタのスタティックキャスト
template <typename U, typename T>
noDel_ptr<U> static_noDel_cast(noDel_ptr<T> ptr) {
    T* _ptr = ptr.get();
    return noDel_ptr<U>(static_cast<U*>(_ptr));
}

//文字列を区切る
void Split(char split_char, char* buffer, std::vector<std::string>& out);
//文字列を置き換える
void Replace(char search_char, char replace_char, char* buffer);

//int型の数字の＋１ー１を指定の幅(0~maxNum)で行う
int CountUpInRange(int target, int maxRange);
int CountDownInRange(int target, int maxRange);