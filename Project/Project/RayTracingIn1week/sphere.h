#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
	sphere();
	sphere(point3 cen, double r) : center(cen), radius(r) {};

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
	point3 center;
	double radius;
};

/*
(t^2)b·b + 2tb·(a - c) +  (a - c)· (a - c) - r^2 = 0 , t = unknown
Simplifying if b = 2h
-b +- √ (b^2 - 4 ac) / 2a -> -h +- √(h^2 - ac) / a

Intersection
-h +-√ (h^2 -ac) / a
*/
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	vec3 A_C = r.origin() - center;
	vec3 B = r.direction();
	float a = dot(B, B);
	float b = dot(A_C, B);
	float c = dot(A_C, A_C) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}