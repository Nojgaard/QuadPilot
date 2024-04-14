#pragma once

#include <Vector3.h>
#include <PID.h>

struct ControlInputs {
    float targetThrust;
    Vector3f targetEulerAngles;
};

class Controller {
    public:
    void update(const Vector3f& measuredEulerAngles, const PID& pidEulerAngles, const PID& pidVelocities, long targetRPSSqr[]);

    private:
    void updateInputs(const Vector3f& measuredEulerAngles, const PID& pidEulerAngles, const PID& pidVelocities);
    void clampInputs();
    void computeTargetRPM(long targetRPSSqr[]);

    ControlInputs _inputs;
};