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

class Archimedean: public Function
{
public:
    ~Archimedean();
    Archimedean(float X0_measured, float a, float b, float X_min, float dx);
    long unsigned eval(size_t i);

private:
    float _X0_measured;
    float _X_min;
    float _dx;
    float _X0;
    float _a;
    float _b;
    float _F;
};

#endif