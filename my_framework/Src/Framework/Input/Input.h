/*-----------------------------------------------------------

	Inputクラス
		InputConfigでまとめた配列を利用して各デバイスの入力を検知する

-------------------------------------------------------------*/
#pragma once
#include <functional>

/// <summary>
/// ユーザーから独自設定されたInputクラス
/// </summary>
/// 

class Input {
private:
	enum class eDeviceType {
		Keyboard,
		Joystick,
		Both
	};
	static eDeviceType currentDevice;

	static std::function<bool(std::vector<int>*& inputs)> funcTrg;
	static std::function<bool(std::vector<int>*& inputs)> funcOn;
	static std::function<bool(std::vector<int>*& inputs)> funcRel;

	//移動量でボタンを設定する際,マイナス値で呼び出す値(txtファイルで設定)は移動量もマイナス方向を返す
	static std::function<float(std::vector<int>*& inputs)> funcDX;
	static std::function<float(std::vector<int>*& inputs)> funcDY;

	static bool BothDevTrg(std::vector<int>*& intuts);
	static bool KeyboardDevTrg(std::vector<int>*& intuts);
	static bool JoystickDevTrg(std::vector<int>*& intuts);

	static bool BothDevOn(std::vector<int>*& inputs);
	static bool KeyboardDevOn(std::vector<int>*& inputs);
	static bool JoystickDevOn(std::vector<int>*& inputs);

	static bool BothDevRel(std::vector<int>*& inputs);
	static bool KeyboardDevRel(std::vector<int>*& inputs);
	static bool JoystickDevRel(std::vector<int>*& inputs);

	static float BothDevDX(std::vector<int>*& inputs);
	static float KeyboardDevDX(std::vector<int>*& inputs);
	static float JoystickDevDX(std::vector<int>*& inputs);

	static float BothDevDY(std::vector<int>*& inputs);
	static float KeyboardDevDY(std::vector<int>*& inputs);
	static float JoystickDevDY(std::vector<int>*& inputs);


public:
	static float GetDX(std::vector<int>*& inputs);
	static float GetDY(std::vector<int>*& inputs);
	static bool Trg(std::vector<int>*& inputs);
	static bool On(std::vector<int>*& inputs);
	static bool Rel(std::vector<int>*& inputs);

	//マウスが動いているか
	static bool IsMouseMove();

	static eDeviceType GetDeviceType() { return currentDevice; }

	//デバイスの変更
	static void SetKeyboardDevice();
	static void SetJoystickDevice();
	static void SetBothDevice();
};