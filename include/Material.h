#pragma once

#include "Object.h"
#include "Vec3.h"

////////////////////////////////////// ABSTRACT MATERIAL CLASS //////////////////////////////////////
class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& inputRay, const Hit& rec, Vec3& attenuation, Ray& scatteredRay) const {
        return false;
    }
};

////////////////////////////////////// LAMBERTIAN MATERIAL CLASS //////////////////////////////////////
class Lambertian : public Material {
private:
    Vec3 m_albedo;
public:
    Lambertian (const Vec3& albedo);
    bool scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const override;
};

////////////////////////////////////// METAL MATERIAL CLASS //////////////////////////////////////
class Metal : public Material {
private:
    Vec3 m_albedo;
    double m_fuzz;
    
public:
    Metal(const Vec3& albedo, double fuzz);
    bool scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const override;
};

////////////////////////////////////// DIELECTRIC MATERIAL CLASS //////////////////////////////////////
class Dielectric : public Material {
private:
    double m_refractionIndex;

    static double reflectance(double cosine, double refractionIndex);

public:
    Dielectric(double refractionIndex);
    bool scatter(const Ray& inputRay, const Hit& hitRecord, Vec3& attenuation, Ray& scatteredRay) const override;
};
