#include "common.h"

#include <tbb/tbb.h>
#include <iomanip>
#include <Eigen/Geometry>

std::string memString(size_t size, bool precise)
{
	double value = (double)size;
	const char *suffixes[] = {
		"B", "KiB", "MiB", "GiB", "TiB", "PiB"
	};
	int suffix = 0;
	while (suffix < 5 && value > 1024.0f) {
		value /= 1024.0f; ++suffix;
	}

	std::ostringstream os;
	os << std::setprecision(suffix == 0 ? 0 : (precise ? 4 : 1))
		<< std::fixed << value << " " << suffixes[suffix];

	return os.str();
}

void coordinate_system(const Vector3f &a, Vector3f &b, Vector3f &c) 
{
	if (std::abs(a.x()) > std::abs(a.y())) {
		Float invLen = 1.0f / std::sqrt(a.x() * a.x() + a.z() * a.z());
		c = Vector3f(a.z() * invLen, 0.0f, -a.x() * invLen);
	}
	else {
		Float invLen = 1.0f / std::sqrt(a.y() * a.y() + a.z() * a.z());
		c = Vector3f(0.0f, a.z() * invLen, -a.y() * invLen);
	}
	b = c.cross(a);
}

Float signum(Float value) 
{
	return std::copysign((Float)1, value);
}

Float modulo(Float a, Float b)
{
	Float r = std::fmod(a, b);
	return (r < 0.0) ? r + b : r;
}

float fast_acos(float x) 
{
	float negate = float(x < 0.0f);
	x = std::abs(x);
	float ret = -0.0187293f;
	ret *= x; ret = ret + 0.0742610f;
	ret *= x; ret = ret - 0.2121144f;
	ret *= x; ret = ret + 1.5707288f;
	ret = ret * std::sqrt(1.0f - x);
	ret = ret - 2.0f * negate * ret;
	return negate * (float)M_PI + ret;
}