#ifndef QUATERNIONLIB_H
#define QUATERNIONLIB_H

//#include "fplib.h"

class quaternion
{
  public:
  quaternion(void);
  quaternion(fixed, fixed, fixed, fixed);
  quaternion(fixed, fixed, fixed);
  void normalize(void);
  fixed x, y, z, w;
};

quaternion::quaternion(void)
{
  quaternion::x=0;
  quaternion::y=0;
  quaternion::z=0;
  quaternion::w=one;
}

quaternion::quaternion(fixed x, fixed y, fixed z, fixed w)
{
  quaternion::x=x;
  quaternion::y=y;
  quaternion::z=z;
  quaternion::w=w;
}

quaternion::quaternion(fixed x, fixed y, fixed z)
{
  quaternion::x=x;
  quaternion::y=y;
  quaternion::z=z;
  quaternion::w=0;
}

bool operator==(quaternion a, quaternion b)
{return (a.x==b.x && a.y==b.y && a.z==b.z && a.w==b.w) ? true : false;}

bool operator!=(quaternion a, quaternion b)
{return (a.x!=b.x || a.y!=b.y || a.z!=b.z || a.w!=b.w) ? true : false;}

quaternion operator+(quaternion a, quaternion b)
{
  quaternion c;
  c.x=a.x+b.x;
  c.y=a.y+b.y;
  c.z=a.z+b.z;
  c.w=a.w+b.w;
  return c;
}

quaternion operator-(quaternion a, quaternion b)
{
  quaternion c;
  c.x=a.x-b.x;
  c.y=a.y-b.y;
  c.z=a.z-b.z;
  c.w=a.w-b.w;
  return c;
}

quaternion operator*(fixed a, quaternion b)
{
  quaternion c;
  c.x=a*b.x;
  c.y=a*b.y;
  c.z=a*b.z;
  c.w=a*b.w;
  return c;
}

quaternion operator*(quaternion b, fixed a)
{return a*b;}

quaternion operator/(quaternion a, fixed b)
{
  quaternion c;
  c.x=tolfixed(a.x)/b;
  c.y=tolfixed(a.y)/b;
  c.z=tolfixed(a.z)/b;
  c.w=tolfixed(a.w)/b;
  return c;
}

quaternion operator*(quaternion a, quaternion b)
{
  quaternion c;
  c.x=a.y*b.z-a.z*b.y+a.w*b.x+a.x*b.w;
  c.y=a.z*b.x-a.x*b.z+a.w*b.y+a.y*b.w;
  c.z=a.x*b.y-a.y*b.x+a.w*b.z+a.z*b.w;
  c.w=a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z;
  return c;
}

quaternion operator-(quaternion a)
{
  quaternion c;
  c.x=-a.x;
  c.y=-a.y;
  c.z=-a.z;
  c.w=-a.w; //see also conjugate()
  return c;
}

fixed norm(quaternion a)
{return a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w;}

lfixed lnorm(quaternion a)
{return a.x%a.x+a.y%a.y+a.z%a.z+a.w%a.w;}

fixed magnitude(quaternion a)
{return sqrt(lnorm(a));}

lfixed lmagnitude(quaternion a)
{return lsqrt(lnorm(a));}

void quaternion::normalize(void)
{
  lfixed nm=lmagnitude(*this);
  this->x=this->x%one/nm;
  this->y=this->y%one/nm;
  this->z=this->z%one/nm;
  this->w=this->w%one/nm;
}

quaternion conjugate(quaternion a)
{
  quaternion c;
  c.x=-a.x;
  c.y=-a.y;
  c.z=-a.z;
  c.w=a.w;
  return c;
}

quaternion inverse(quaternion a)
{return conjugate(a)/norm(a);}

fixed inner(quaternion a, quaternion b)
{return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;}

#endif
