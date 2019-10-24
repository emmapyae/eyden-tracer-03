#pragma once

#include "types.h"
#include "ray.h"

struct Ray;

namespace
{
inline Vec3f Min3f(const Vec3f a, const Vec3f b)
{
	return Vec3f(MIN(a.val[0], b.val[0]), MIN(a.val[1], b.val[1]), MIN(a.val[2], b.val[2]));
}

inline Vec3f Max3f(const Vec3f a, const Vec3f b)
{
	return Vec3f(MAX(a.val[0], b.val[0]), MAX(a.val[1], b.val[1]), MAX(a.val[2], b.val[2]));
}
} // namespace

/**
 * @brief Bounding Box class
 */
class CBoundingBox
{
public:
	CBoundingBox(void) = default;
	~CBoundingBox(void) = default;

	/**
	 * @brief Resets the bounding box
	 * @details This function resets the member variables \b m_min and \b m_max
	 */
	void clear(void)
	{
		// --- PUT YOUR CODE HERE ---
		// set max and min oppositely
		m_min[0] = std::numeric_limits<float>::infinity();
		m_min[1] = std::numeric_limits<float>::infinity();
		m_min[2] = std::numeric_limits<float>::infinity();

		m_max[0] = (-1) * std::numeric_limits<float>::infinity();
		m_max[1] = (-1) * std::numeric_limits<float>::infinity();
		m_max[2] = (-1) * std::numeric_limits<float>::infinity();
	}

	/**
	 * @brief Extends the bounding box to contain point \b a
	 * @param a A point
	 */
	void extend(Vec3f a)
	{
		// --- PUT YOUR CODE HERE ---
		// using Vec3f defined in the namespace to save real min and max Vec3f
		m_min = Min3f(a, m_min);
		m_max = Max3f(a, m_max);
	}

	/**
	 * @brief Extends the bounding box to contain bounding box \b box
	 * @param box The second bounding box
	 */
	void extend(const CBoundingBox &box)
	{
		// --- PUT YOUR CODE HERE ---
		// extend the added box
		this->extend(box.m_min);
		this->extend(box.m_max);
	}

	/**
	 * @brief Checks if the current bounding box overlaps with the argument bounding box \b box
	 * @param box The secind bounding box to be checked with
	 */
	bool overlaps(const CBoundingBox &box)
	{
		// --- PUT YOUR CODE HERE ---
		bool check_0 = this->m_min[0] <= box.m_max[0] &&  this->m_max[0] >= box.m_min[0];
		bool check_1 = this->m_min[1] <= box.m_max[1] &&  this->m_max[1] >= box.m_min[1];
		bool check_2 = this->m_min[2] <= box.m_max[2] &&  this->m_max[2] >= box.m_min[2];
		return (check_0 && check_1 && check_2);
	}

	/**
	 * @brief Clips the ray with the bounding box
	 * @param[in] ray The ray
	 * @param[in,out] t0 The distance from ray origin at which the ray enters the bounding box
	 * @param[in,out] t1 The distance from ray origin at which the ray leaves the bounding box
	 */
	void clip(const Ray &ray, float &t0, float &t1)
	{
		// --- PUT YOUR CODE HERE ---
		// reference: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
		// https://tavianator.com/fast-branchless-raybounding-box-intersections
		float tfar_min = (-1) * std::numeric_limits<float>::infinity();
		float tnear_max = std::numeric_limits<float>::infinity();
		float tx_near, ty_near, tz_near, tx_far, ty_far, tz_far;

		if (ray.dir[0] != 0)
		{
			tx_near = (m_min[0] - ray.org[0]) / ray.dir[0];
			tx_far = (m_max[0] - ray.org[0]) / ray.dir[0];

			if (ray.dir[0] < 0)
				std::swap(tx_near, tx_far);
		}

		if (ray.dir[1] != 0)
		{
			ty_near = (m_min[1] - ray.org[1]) / ray.dir[1];
			ty_far = (m_max[1] - ray.org[1]) / ray.dir[1];

			if (ray.dir[1] < 0)
				std::swap(ty_near, ty_far);
		}

		if (ray.dir[2] != 0)
		{
			tz_near = (m_min[2] - ray.org[2]) / ray.dir[2];
			tz_far = (m_max[2] - ray.org[2]) / ray.dir[2];

			if (ray.dir[2] < 0)
				std::swap(tz_near, tz_far);
		}
		// finding the max and min as in the lecture slides
		tnear_max = std::max(tz_near, std::max(tx_near, ty_near));
		tfar_min = std::min(tz_far, std::min(tx_far, ty_far));

		// this is a false statement
		if (tnear_max <= tfar_min)
		{
			t0 = tnear_max;
			t1 = tfar_min;
		}
		
	}

public:
	Vec3f m_min; ///< The minimal point defying the size of the bounding box
	Vec3f m_max; ///< The maximal point defying the size of the bounding box
};
