#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable
{
public:
	hittable_list() {}
	hittable_list(std::shared_ptr<hittable> obj, int n) { add(obj); list_size = n; }

	void clear() { objs.clear(); }
	void add(std::shared_ptr<hittable> obj) { objs.push_back(obj); }

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
	std::vector<std::shared_ptr<hittable>> objs;
	int list_size;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& o : objs)
	{
		if (o->hit(r, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}