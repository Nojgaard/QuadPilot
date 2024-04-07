#include <Arduino.h>
#include <Controller.h>
#include <Specifications.h>

void Controller::update(const Vector3f& measuredEulerAngles, const PID& pidEulerAngles, const PID& pidVelocities, long targetRPMSqr[]) {
    updateInputs(measuredEulerAngles, pidEulerAngles, pidVelocities);
    clampInputs();
    computeTargetRPM(targetRPMSqr);
}

void Controller::updateInputs(const Vector3f& measuredEulerAngles, const PID& pidEulerAngles, const PID& pidVelocities) {
    float m = Specifications::MASS;
    float g = Specifications::GRAVITY;

    const Vector3f& angleError = pidEulerAngles.error();
    const Vector3f& velocityError = pidVelocities.error();

    _inputs.targetThrust = (m * g) / (cos(measuredEulerAngles.x) * cos(measuredEulerAngles.y)) + velocityError.z;
    _inputs.targetEulerAngles.set(angleError);
}

void Controller::clampInputs() {
    float b = Specifications::DRAG_TORQUE_COEFFECIENT;
    float k = Specifications::THRUST_COEFFECIENT;
    float l = Specifications::ARM_LENGTH;
    int wmin = Specifications::MIN_MOTOR_RPM_SQR;
    int wmax = Specifications::MAX_MOTOR_RPM_SQR;

    constrain(_inputs.targetThrust, 4 * k * wmin, 4 * k * wmax); // thrust
    constrain(_inputs.targetEulerAngles.x, -l * k * wmax, l * k * wmax); // roll torque
    constrain(_inputs.targetEulerAngles.y, -l * k * wmax, l * k * wmax); // pitch torque
    constrain(_inputs.targetEulerAngles.z, -2 * b * wmax, 2 * b * wmax); // yaw torque
}

void Controller::computeTargetRPM(long targetRPMSqr[]) {
    float thrust = _inputs.targetThrust;
    const Vector3f& a = _inputs.targetEulerAngles;

    float b = Specifications::DRAG_TORQUE_COEFFECIENT;
    float k = Specifications::THRUST_COEFFECIENT;
    float l = Specifications::ARM_LENGTH;

    targetRPMSqr[0] = thrust / (4 * k) + a.y / (2 * l * k) + a.z / (4 * b);
    targetRPMSqr[1] = thrust / (4 * k) - a.x / (2 * l * k) - a.z / (4 * b);
    targetRPMSqr[2] = thrust / (4 * k) - a.y / (2 * l * k) + a.z / (4 * b);
    targetRPMSqr[3] = thrust / (4 * k) + a.x / (2 * l * k) - a.z / (4 * b);

    for (int i = 0; i < Specifications::NUM_MOTORS; i++) {
        constrain(targetRPMSqr[i], Specifications::MIN_MOTOR_RPM_SQR, Specifications::MAX_MOTOR_RPM_SQR);
    }
}