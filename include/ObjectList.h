#pragma once

#include "Object.h"

#include <vector>

using std::make_shared;
using std::shared_ptr;

class ObjectList : public Object {
private:
    std::vector<shared_ptr<Object>> objects;

public:
    ObjectList();
    ObjectList(shared_ptr<Object> object);

    void clear();
    void add(shared_ptr<Object> object);

    bool hit(const Ray& r, double tMin, double tMax, Hit& hitRecord) const override;
};