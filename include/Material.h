#pragma once

#include "Vec3.h"
#include "Hit.h"
class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& inputRay, const Hit& rec, Vec3& attenuation, Ray& scatteredRay) const {
        return false;
    }
};

// Lambertian Material
class Lambertian : public Material {
private:
    Vec3 m_albedo;
public:
    Lambertian (const Vec3& albedo);
    bool scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const override;
};

// Metal Material
class Metal : public Material {
private:
    Vec3 m_albedo;
    double m_fuzz;
    
public:
    Metal(const Vec3& albedo, double fuzz);
    bool scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const override;
};

// Dielectric Material
class Dielectric : public Material {
private:
    double m_refractionIndex;

    static double reflectance(double cosine, double refractionIndex);

public:
    Dielectric(double refractionIndex);
    bool scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const override;
};
