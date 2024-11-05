#include "Plane.h"

bool Plane::Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDist, float maxDist) {
    float t;
    if (!Raycast(ray, m_transform.position, m_transform.GetUp(), minDist, maxDist, t)) return false;
    
    raycastHit.distance = t;
    raycastHit.normal = glm::normalize(m_transform.GetUp());
    raycastHit.point = ray.at(t);
    raycastHit.material = GetMaterial();

    return true;
}

bool Plane::Raycast(const ray_t& ray, const glm::vec3& point, const glm::vec3& normal, float minDist, float maxDist, float& t) {
    // check dot product of ray direction and plane normal, if result is 0 then ray direction if parallel to plane so it never hits plane
    // the dot product is 0 if the two vectors are perpendicular (90 degrees)
    float denominator = glm::dot(ray.direction, normal);
    if (Approximately(denominator, 0.0f)) {
        return false; // ray direction and plane parallel, no intersection
    }

    // get t (distance) along ray direction to hit point
    t = glm::dot(point - ray.origin, normal / denominator);
    if (t < 0)
    {
        return false; // plane behind ray's origin, no intersection
    }

    if (t <= minDist || t >= maxDist) return false;  
    
    return true;
}