#include "../include/Material.h"

////////////////////////////////////// LAMBERTIAN MATERIAL CLASS //////////////////////////////////////
Lambertian::Lambertian (const Vec3& albedo) : m_albedo(albedo) {}

bool Lambertian::scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const {
    Vec3 scatterDirection = hitRecord.normal + randomUnitVector();

    if(scatterDirection.nearZero()){
        scatterDirection = hitRecord.normal;
    }
    
    scatteredRay = Ray(hitRecord.position, scatterDirection);
    attenuation = m_albedo;
    return true;
}

////////////////////////////////////// METAL MATERIAL CLASS //////////////////////////////////////
Metal::Metal(const Vec3& albedo) : m_albedo(albedo), m_fuzz(0) {}

bool Metal::scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const {
    Vec3 reflectedRay = reflect(normalise(inputRay.direction()), hitRecord.normal);
    scatteredRay = Ray(hitRecord.position, reflectedRay);
    attenuation = m_albedo;
    return true;
}