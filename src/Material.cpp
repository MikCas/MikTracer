#include "Material.h"

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
Metal::Metal(const Vec3& albedo, double fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

bool Metal::scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const {
    Vec3 reflectedRay = reflect(inputRay.direction(), hitRecord.normal);
    reflectedRay = normalise(reflectedRay) + (m_fuzz * randomUnitVector());
    scatteredRay = Ray(hitRecord.position, reflectedRay);
    attenuation = m_albedo;
    
    // The scattered ray is only valid if it is not pointing against the normal
    return (dot(scatteredRay.direction(), hitRecord.normal) > 0);
}

////////////////////////////////////// DIELECTRIC MATERIAL CLASS //////////////////////////////////////
Dielectric::Dielectric(double refractionIndex) : m_refractionIndex(refractionIndex) {}

bool Dielectric::scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const {
    attenuation = Vec3(1.0, 1.0, 1.0);

    // Depending on which way the ray intersects, the refractive index will be standard or inversed
    double refractiveIndex = hitRecord.frontFace ? (1.0 / m_refractionIndex) : m_refractionIndex;
    
    Vec3 direction = normalise(inputRay.direction());
    double cosTheta = std::fmin(dot(-direction, hitRecord.normal), 1.0);
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    bool noRefraction = refractiveIndex * sinTheta > 1.0;
    bool shouldReflect = reflectance(cosTheta, refractiveIndex) > randomDouble();

    // Total internal reflection
    if (noRefraction || shouldReflect){
        direction = reflect(direction, hitRecord.normal);
    }
    else {
        direction = refract(direction, hitRecord.normal, refractiveIndex);
    }

    scatteredRay = Ray(hitRecord.position, direction);

    return true;
}

double Dielectric::reflectance(double cosine, double refractionIndex) {
    // Use Schlick's approximation for reflectance
    double r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}