#include <PID.h>

void PID::update(const Vector3f& target, const Vector3f& measure, float dt) {
    _errorTemp.set(_errorProportional);
    
    _errorProportional.set(target).sub(measure);
    
    _errorDerivative.set(_errorProportional)
        .sub(_errorTemp)
        .scale(1/dt);

    // use as temp value
    _errorTemp.set(_errorProportional).scale(dt);
    _errorIntegral.add(_errorTemp);
}

const Vector3f& PID::error() {
    _error.set(_errorProportional).scale(_scaleProportional);
    
    _errorTemp.set(_errorDerivative).scale(_scaleDerivative);
    _error.add(_errorTemp);
    return _error;
}