#include "Plane.h"

bool Plane::Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDist, float maxDist)
{
    

    if (t <= minDist || t >= maxDist) return false;

    raycastHit.distance = t;
    raycastHit.normal = glm::normalize(m_normal);
    raycastHit.point = ray.at(t);
    raycastHit.material = GetMaterial();

    return true;
}
