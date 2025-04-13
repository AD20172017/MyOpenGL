#pragma once


namespace GLRT {

	class Hittable
	{
	public:
		//virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	};

	class HittableList :public Hittable
	{

	};

};
