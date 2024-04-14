#include <Arduino.h>
#include <Controller.h>
#include <Specifications.h>

void Controller::update(const Vector3f& measuredEulerAngles, const PID& pidEulerAngles, const PID& pidVelocities, long targetRPSSqr[]) {
    updateInputs(measuredEulerAngles, pidEulerAngles, pidVelocities);
    clampInputs();
    computeTargetRPM(targetRPSSqr);
}

void Controller::updateInputs(const Vector3f& measuredEulerAngles, const PID& pidEulerAngles, const PID& pidVelocities) {
    float m = Specifications::MASS;
    float g = Specifications::GRAVITY;

    const Vector3f& angleError = pidEulerAngles.error();
    const Vector3f& velocityError = pidVelocities.error();

    _inputs.targetThrust = (m * g) / (cos(measuredEulerAngles.x) * cos(measuredEulerAngles.y));
    _inputs.targetEulerAngles.set(angleError);
}

void Controller::clampInputs() {
    float b = Specifications::DRAG_TORQUE_COEFFECIENT;
    float k = Specifications::THRUST_COEFFECIENT;
    float l = Specifications::ARM_LENGTH;
    long wmin = Specifications::MIN_MOTOR_RPS_SQR;
    long wmax = Specifications::MAX_MOTOR_RPS_SQR;

    _inputs.targetThrust = constrain(_inputs.targetThrust, 4 * k * wmin, 4 * k * wmax); // thrust
    _inputs.targetEulerAngles.x = constrain(_inputs.targetEulerAngles.x, -l * k * wmax, l * k * wmax); // roll torque
    _inputs.targetEulerAngles.y = constrain(_inputs.targetEulerAngles.y, -l * k * wmax, l * k * wmax); // pitch torque
    _inputs.targetEulerAngles.z = constrain(_inputs.targetEulerAngles.z, -2 * b * wmax, 2 * b * wmax); // yaw torque
}

void Controller::computeTargetRPM(long targetRPSSqr[]) {
    float thrust = _inputs.targetThrust;
    const Vector3f& a = _inputs.targetEulerAngles;

    float b = Specifications::DRAG_TORQUE_COEFFECIENT;
    float k = Specifications::THRUST_COEFFECIENT;
    float l = Specifications::ARM_LENGTH;

    targetRPSSqr[0] = thrust / (4 * k) + a.y / (2 * l * k) + a.z / (4 * b);
    targetRPSSqr[1] = thrust / (4 * k) - a.x / (2 * l * k) - a.z / (4 * b);
    targetRPSSqr[2] = thrust / (4 * k) - a.y / (2 * l * k) + a.z / (4 * b);
    targetRPSSqr[3] = thrust / (4 * k) + a.x / (2 * l * k) - a.z / (4 * b);

    for (int i = 0; i < Specifications::NUM_MOTORS; i++) {
        targetRPSSqr[i] = constrain(targetRPSSqr[i], Specifications::MIN_MOTOR_RPS_SQR, Specifications::MAX_MOTOR_RPS_SQR);
    }
}