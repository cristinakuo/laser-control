#ifndef _FUNCTION_HPP_INCLUDED_
#define _FUNCTION_HPP_INCLUDED_

#include <Arduino.h>

#define ARCHIMEDEAN_PARAM_F 10

class Function
{
public:
	virtual ~Function();
	virtual long unsigned eval(size_t i) = 0;
};

// TODO: change confusing name
class Archimedean: public Function
{
public:
    ~Archimedean();
    Archimedean(float X0_measured, float a, float b, float X_min, float dx);
    long unsigned eval(size_t i);
    float getX0() const;
private:
    float _X0_measured;
    float _X_min;
    float _dx;
    float _X0;
    float _a;
    float _b;
    float _F;
};

class Linear: public Function
{
public:
    ~Linear();
    Linear(float X0_measured, float linear_speed, float angular_speed, float dx);
    long unsigned eval(size_t i);
    
private:
    float _X0_measured; // TODO: maybe not necessary
    float _linearSpeed;
    float _angularSpeed; // TODO: make table for angular speed (target motor)
    float _dx;
    float _X0;
};
#endif