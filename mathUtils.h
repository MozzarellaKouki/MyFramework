#pragma once
#include "cmath"
#include "main.h"


//Color構造体 ===============================
struct Color
{
	float r, g, b, a;
};


//円周率変換 ================================
constexpr float PI = 3.141592f;
inline float DegToRad(float degree) {return degree * PI / 180.0f;}
inline float RadToDeg(float rad){ return rad * 180.0f / PI;}


//乱数範囲生成 ==============================
inline float RandomRange(float min, float max)
{
	return min + (max - min) * (float)rand() / RAND_MAX;
}

//リープ補完 ================================
inline float Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

inline Color Lerp(const Color& a, const Color& b, float t)
{
	return
	{
		Lerp(a.r, b.r, t),
		Lerp(a.g, b.g, t),
		Lerp(a.b, b.b, t),
		Lerp(a.a, b.a, t)
	};
}











//Vector3 ==================================
class Vector3
{

public:
	
	float x, y, z;


	// コンストラクタ初期化
	Vector3() : x(0), y(0), z(0) {}

	// コピーコンストラクタ
	Vector3(const Vector3& a) : x(a.x), y(a.y), z(a.z) {}

	// 3つの値で作成する
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}


	// ====== 準的なオブジェクトの保守 =======

	//代入（Cの慣習に従い値への参照を返す）
	Vector3& operator = (const Vector3& a)
	{
		x = a.x; y = a.y; z = a.z;
		return *this;
	}

	//等しさのチェック
	bool operator == (const Vector3& a) const
	{
		return x == a.x && y == a.y && z == a.z;
	}

	bool operator != (const Vector3& a) const
	{
		return x != a.x || y != a.y || z != a.z;
	}

	// ===== ベクトル操作 ============

	//ベクトルを0に設定する
	void zero() { x = y = z = 0.0f; }



	//単項式のマイナスは、反転したベクトルを返す
	Vector3 operator - () const { return Vector3(-x, -y, -z); }



	//二項式の＋とーはベクトルを加算し、減算する
	Vector3 operator + (const Vector3& a) const { return Vector3(x + a.x, y + a.y, z + a.z); }

	Vector3 operator - (const Vector3& a) const { return Vector3(x - a.x, y - a.y, z - a.z); }



	//スカラーによる乗算と除算
	Vector3 operator * (float a) const { return Vector3(x * a, y * a, z * a); }

	Vector3 operator / (float a) const
	{
		float oneOverA = 1.0f / a; return Vector3(x * oneOverA, y * oneOverA, z * oneOverA);
	}



	// Cの表記法に準拠するための組み合わせ代入演算
	Vector3& operator += (const Vector3& a)
	{
		x += a.x; y += a.y; z += a.z;
		return *this;
	}

	Vector3& operator -=(const Vector3& a)
	{
		x -= a.x; y -= a.y; z -= a.z;
		return *this;
	}

	Vector3& operator *= (float a) { x *= a; y *= a; z *= a; return *this; }

	Vector3& operator /= (float a)
	{
		float oneOverA = 1.0f / a;
		x *= oneOverA; y *= oneOverA; 	z *= oneOverA;
		return *this;
	}



	//ベクトルを正規化する

	void normalize()
	{

		float magSq = x * x + y * y + z * z;
		if (magSq > 0.0f)
		{
			float oneOverMag = 1.0f / sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
	}

	void normalize() const { normalize(); }

	// ======= ベクトルの内積 ========

	//標準の乗算記号をこれにオーバーロードする
	float operator *(const Vector3& a) const 
	{
		return x * a.x + y * a.y + z * a.z;
	}

};