#pragma once

unsigned int ColorConvert16(stColor4);

_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
    return DirectX::XMLoadFloat3(&vec);
}

stVector3 RoundDec3forVector(stVector3);
float RoundDec3forFloat(float);

template <typename U, typename T>
noDel_ptr<U> dynamic_noDel_cast(noDel_ptr<T> ptr) {
    T* _ptr = ptr.get();
    return noDel_ptr<U>(dynamic_cast<U*>(_ptr));
}

template <typename U, typename T>
noDel_ptr<U> static_noDel_cast(noDel_ptr<T> ptr) {
    T* _ptr = ptr.get();
    return noDel_ptr<U>(static_cast<U*>(_ptr));
}

//int^ΜΜ{P[PπwθΜ(0~maxNum)Εs€
int CountUpInRange(int target, int maxRange);
int CountDownInRange(int target, int maxRange);