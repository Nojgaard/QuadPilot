#include <PID.h>

PID::PID(float scaleDerivative, float scaleProportional, float scaleIntegral, float triggerIntegral):
    _scaleDerivative(scaleDerivative), _scaleProportional(scaleProportional), _scaleIntegral(scaleIntegral),
    _triggerIntegral(triggerIntegral) {}

void PID::update(const Vector3f& target, const Vector3f& measure, float dt) {
    // update terms
    _errorTemp.set(_errorProportional);
    
    _errorProportional.set(target).sub(measure);
    
    _errorDerivative.set(_errorProportional)
        .sub(_errorTemp)
        .scale(1/dt);

    if (_errorProportional.getMagnitude() <= _triggerIntegral) {
        _errorTemp.set(_errorProportional).scale(dt);
        _errorIntegral.add(_errorTemp);
    } else {
        _errorIntegral.clear();
    }

    // update total error
    _error.set(_errorProportional).scale(_scaleProportional);
    
    _errorTemp.set(_errorDerivative).scale(_scaleDerivative);
    _error.add(_errorTemp);

    _errorTemp.set(_errorIntegral).scale(_scaleIntegral);
    _error.add(_errorTemp);
}

const Vector3f& PID::error() const {
    return _error;
}