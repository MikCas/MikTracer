#include "../include/ObjectList.h"

ObjectList::ObjectList() {}

ObjectList::ObjectList(shared_ptr<Object> object) {
    add(object);
}

void ObjectList::clear() {
    m_objects.clear();
}

void ObjectList::add(shared_ptr<Object> object) {
    m_objects.push_back(object);
}

bool ObjectList::hit(const Ray& r, Interval hitInterval, Hit& hitRecord) const {
    Hit tempHit;
    bool hitAnything = false;
    double closestSoFar = hitInterval.max;

    for (const auto& object : m_objects) {
        if (object->hit(r, Interval(hitInterval.min, closestSoFar), tempHit)) {
            hitAnything = true;
            closestSoFar = tempHit.t;
            hitRecord = tempHit;
        }
    }

    return hitAnything;
}