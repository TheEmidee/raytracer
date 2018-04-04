#include "bvhnode.h"

#include <assert.h>

#include "hitinfos.h"
#include "maths.h"

struct HitableSorter
{
public:

    bool operator() (const std::shared_ptr< Hitable > & left, const std::shared_ptr< Hitable > & right)
    {
        aabb box_left, box_right;

        if (!left->GetBoundingBox(box_left)
            || !right->GetBoundingBox(box_right)
            )
        {
            assert(false);
        }

        return Sort( box_left, box_right );
    }

protected:

    virtual bool Sort(const aabb & box_left, const aabb & box_right) const = 0;
};

struct SortByX : public HitableSorter
{
protected:

    virtual bool Sort(const aabb & box_left, const aabb & box_right) const override
    {
        return box_left.min.x < box_right.min.x;
    }

} sortByX;

struct SortByY : public HitableSorter
{
protected:

    virtual bool Sort(const aabb & box_left, const aabb & box_right) const override
    {
        return box_left.min.y < box_right.min.y;
    }

} sortByY;

struct SortByZ : public HitableSorter
{
protected:

    virtual bool Sort(const aabb & box_left, const aabb & box_right) const override
    {
        return box_left.min.z < box_right.min.z;
    }

} sortByZ;

BVHNode::BVHNode(const std::vector< std::shared_ptr< Hitable > > & hitables_, uint32_t & state)
{
    auto sorted_hitables = hitables_;

    auto rand_axis = static_cast<int>(3 * RandomFloat01(state));

    if (rand_axis == 0)
    {
        std::sort(sorted_hitables.begin(), sorted_hitables.end(), sortByX);
    }
    else if (rand_axis == 1)
    {
        std::sort(sorted_hitables.begin(), sorted_hitables.end(), sortByY);
    }
    else
    {
        std::sort(sorted_hitables.begin(), sorted_hitables.end(), sortByZ);
    }

    if (sorted_hitables.size() == 1)
    {
        left = right = sorted_hitables[0];
    }
    else if (sorted_hitables.size() == 2)
    {
        left = sorted_hitables[0];
        right = sorted_hitables[1];
    }
    else
    {
        const auto left_hitables = std::vector<std::shared_ptr< Hitable >>(sorted_hitables.begin(), sorted_hitables.begin() + (sorted_hitables.size() / 2));
        const auto right_hitables = std::vector<std::shared_ptr< Hitable >>(sorted_hitables.begin() + (sorted_hitables.size() / 2), sorted_hitables.end());

        left = std::make_shared< BVHNode >(left_hitables, state);
        right = std::make_shared< BVHNode >(right_hitables, state);
    }

    aabb box_left, box_right;

    if (!left->GetBoundingBox(box_left)
        ||!right->GetBoundingBox( box_right)
        )
    {
        assert(false);
    }

    box = aabb::GetSurroundingAABB(box_left, box_right);
}

bool BVHNode::Hit(const Ray & ray, float min_time, float max_time, HitInfos & hit_infos) const
{
    if (box.Hit(ray, min_time, max_time))
    {
        HitInfos left_hit_infos, right_hit_infos;

        bool hit_left = left->Hit(ray, min_time, max_time, left_hit_infos);
        bool hit_right = right->Hit(ray, min_time, max_time, right_hit_infos);

        if (hit_left && hit_right)
        {
            if (left_hit_infos.Time < right_hit_infos.Time)
            {
                hit_infos = left_hit_infos;
            }
            else
            {
                hit_infos = right_hit_infos;
            }

            return true;
        }
        else if (hit_left)
        {
            hit_infos = left_hit_infos;
            return true;
        }
        else if (hit_right)
        {
            hit_infos = right_hit_infos;
            return true;
        }
    }

    return false;
}

bool BVHNode::GetBoundingBox(aabb & box) const
{
    box = this->box;
    return true;
}