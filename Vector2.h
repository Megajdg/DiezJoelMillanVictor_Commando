#pragma once
#include <cmath>

class Vector2
{
public:
	float x, y;

	Vector2() { x = 0.f; y = 0.f; }					// Constructor por defecto se inicializa a 0,0
	Vector2(float x, float y) :x(x), y(y) {}		// Constructor con valores explicitos
	Vector2(float v) { x = y = v; }					// Constructor que pone ambos componentes al mismo valor
	Vector2(const Vector2& v) { x = v.x; y = v.y; }	// Constructor copia

	// Magnitud del vector (longitud)
	float Module() const
	{
		return std::sqrt(x * x + y * y);
	}

	// Magnitud al cuadrado (evita sqrt, util para comparaciones rapidas)
	float SquareModule() const
	{
		return x * x + y * y;
	}

	// Devuelve una copia normalizada del vector
	Vector2 Normalized() const
	{
		return Vector2(x / Module(), y / Module());
	}

	// Normaliza el vector actual y lo devuelve
	Vector2 normalize()
	{
		float m = Module();
		x /= m;
		y /= m;

		return *this;
	}

	// Rota el vector un numero de grados alrededor del origen
	Vector2 rotate(float degrees) const
	{
		float rads = degrees * 3.14159 / 180;
		float _x = x * cos(rads) - y * sin(rads);
		float _y = x * sin(rads) + y * cos(rads);

		return Vector2(_x, _y);
	}

	// OPERADORES
	// Suma acumulativa
	Vector2 operator+=(const Vector2& v2)
	{
		x += v2.x;
		y += v2.y;

		return *this;
	}

	// Resta acumulativa
	Vector2 operator-=(const Vector2& v2)
	{
		x -= v2.x;
		y -= v2.y;

		return *this;
	}

	// Suma de vectores
	Vector2 operator+(const Vector2& v2) const
	{
		return Vector2(x + v2.x, y + v2.y);
	}

	// Resta de vectores
	Vector2 operator-(const Vector2& v2) const
	{
		return Vector2(x - v2.x, y - v2.y);
	}

	// Multiplicacion por escalar
	Vector2 operator*(float scl) const
	{
		return Vector2(x * scl, y * scl);
	}

	// Division por escalar
	Vector2 operator/(float scl) const
	{
		float inv = 1 / scl;
		return *this * inv;
	}
};