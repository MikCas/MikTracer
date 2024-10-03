#include "../include/ObjectList.h"

ObjectList::ObjectList() {}

ObjectList::ObjectList(shared_ptr<Object> object) {
    add(object);
}

void ObjectList::clear() {
    objects.clear();
}

void ObjectList::add(shared_ptr<Object> object) {
    objects.push_back(object);
}

bool ObjectList::hit(const Ray& r, double tMin, double tMax, Hit& hitRecord) const {
    Hit tempHit;
    bool hitAnything = false;
    double closestSoFar = tMax;

    for (const auto& object : objects) {
        if (object->hit(r, tMin, closestSoFar, tempHit)) {
            hitAnything = true;
            closestSoFar = tempHit.t;
            hitRecord = tempHit;
        }
    }

    return hitAnything;
}