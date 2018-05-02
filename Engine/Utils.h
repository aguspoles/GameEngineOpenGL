#pragma once
enum PositionInFrustum { OUTSIDE, INSIDE, INTERSECT};

struct Plane {
	float _a, _b, _c, _d;
	Plane(float a, float b, float c, float d) : _a(a), _b(b), _c(c), _d(d) {
		Normalize();
	}
	void Normalize() {
		float mag = sqrt(_a*_a + _b * _b + _c * _c);
		_a /= mag;
		_b /= mag;
		_c /= mag;
		_d /= mag;
	}
	int Distance(glm::vec3 point) {
		int res = _a * point.x + _b * point.y + _c * point.z + _d;
		return res;
	}
};
