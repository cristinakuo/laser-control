#include "Function.hpp"

Function::~Function() {
}

Archimedean::~Archimedean() {
}

Archimedean::Archimedean(float X0_measured, float a, float b, float X_min, float dx) {
    _X0_measured = X0_measured;
    _a = a;
    _b = b;
    _X_min = X_min;
    _dx = dx;
    _F = ARCHIMEDEAN_PARAM_F;
    _X0 = floor(X0_measured/dx) * dx;
}

long unsigned Archimedean::eval(size_t i) {
    return (PI/_a/_b/_F * (pow(_X0,2)-pow(_X0-_dx*i,2)) - PI/_a/_b/_F * (pow(_X0,2)-pow(_X0-_dx*(i-1),2)))*1000000;
}

float Archimedean::getX0() const {
    return _X0;
}

Linear::~Linear() {
}

Linear::Linear(float X0_measured, float linear_speed, float angular_speed, float dx) {
    _X0_measured = X0_measured;
    _linearSpeed = linear_speed;
    _angularSpeed = angular_speed;
    _dx = dx;
    _X0 = floor(X0_measured/dx) * dx;
}

long unsigned Linear::eval(size_t i) {
    return ((_dx*i)/_linearSpeed);
}