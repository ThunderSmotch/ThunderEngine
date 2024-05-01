export module Easing;

import std;
import ThunderLib.Base;

export enum class EasingType
{
	Linear = 0,
	InOutCubic,
	InOutBack,

	Length
};

f32 easeInOutCubic(f32 t)
{
	return t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2.0f;
}

f32 easeInOutBack(f32 t)
{
	auto c1 = 1.70158f;
	auto c2 = c1 * 1.525;

return t < 0.5
  ? (pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
  : (pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
}

export f32 ease(f32 t, EasingType type)
{
	switch (type)
	{
	case EasingType::Linear:
		return t;
	case EasingType::InOutCubic:
		return easeInOutCubic(t);
	case EasingType::InOutBack:
		return easeInOutBack(t);
	default:
		return t;
	}

}

