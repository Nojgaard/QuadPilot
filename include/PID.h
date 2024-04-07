#pragma once

#include <Vector3.h>

class PID {
    public:
    PID(float scaleDerivative, float scaleProportional, float scaleIntegral);
    void update(const Vector3f& target, const Vector3f& measure, float dt);
    const Vector3f& error();

    private:
    Vector3f _error;
    Vector3f _errorTemp;
    Vector3f _errorDerivative;
    Vector3f _errorProportional;
    Vector3f _errorIntegral;
    float _scaleDerivative;
    float _scaleProportional;
    float _scaleIntegral;
    float _triggerIntegral;
};