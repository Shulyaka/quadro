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
  fixed w, x, y, z;

  bool operator==(const quaternion&) const;
  bool operator!=(const quaternion&) const;
  
  quaternion& operator+=(const quaternion&);
  quaternion& operator-=(const quaternion&);
  const quaternion operator+(const quaternion&) const;
  const quaternion operator-(const quaternion&) const;
};

quaternion::quaternion(void)
{
  quaternion::w=one;
  quaternion::x=0;
  quaternion::y=0;
  quaternion::z=0;
}

quaternion::quaternion(fixed w, fixed x, fixed y, fixed z)
{
  quaternion::w=w;
  quaternion::x=x;
  quaternion::y=y;
  quaternion::z=z;
}

quaternion::quaternion(fixed x, fixed y, fixed z)
{
  quaternion::w=0;
  quaternion::x=x;
  quaternion::y=y;
  quaternion::z=z;
}

const quaternion ident=quaternion(one, 0, 0, 0);

bool quaternion::operator==(const quaternion &b) const
{return (this->w==b.w && this->x==b.x && this->y==b.y && this->z==b.z) ? true : false;}

bool quaternion::operator!=(const quaternion &b) const
{return (this->w!=b.w || this->x!=b.x || this->y!=b.y || this->z!=b.z) ? true : false;}

quaternion& quaternion::operator+=(const quaternion &b)
{
  this->w+=b.w;
  this->x+=b.x;
  this->y+=b.y;
  this->z+=b.z;
  return *this;
}

quaternion& quaternion::operator-=(const quaternion &b)
{
  this->w-=b.w;
  this->x-=b.x;
  this->y-=b.y;
  this->z-=b.z;
  return *this;
}

const quaternion quaternion::operator+(const quaternion &b) const
{
  //return quaternion(*this)+=b;
  quaternion c;
  c.w=this->w+b.w;
  c.x=this->x+b.x;
  c.y=this->y+b.y;
  c.z=this->z+b.z;
  return c;
}

const quaternion quaternion::operator-(const quaternion &b) const
{
  //return quaternion(*this)-=b;
  quaternion c;
  c.w=this->w-b.w;
  c.x=this->x-b.x;
  c.y=this->y-b.y;
  c.z=this->z-b.z;
  return c;
}




quaternion operator*(fixed a, quaternion b)
{
  quaternion c;
  c.w=a*b.w;
  c.x=a*b.x;
  c.y=a*b.y;
  c.z=a*b.z;
  return c;
}

quaternion operator*(quaternion b, fixed a)
{return a*b;}

quaternion operator>>(quaternion a, char b)
{
  quaternion c;
  c.w=a.w>>b;
  c.x=a.x>>b;
  c.y=a.y>>b;
  c.z=a.z>>b;
  return c;
}

quaternion operator<<(quaternion a, char b)
{
  quaternion c;
  c.w=a.w<<b;
  c.x=a.x<<b;
  c.y=a.y<<b;
  c.z=a.z<<b;
  return c;
}

quaternion operator/(quaternion a, fixed b)
{
  quaternion c;
  c.w=lfixed(a.w)/b;
  c.x=lfixed(a.x)/b;
  c.y=lfixed(a.y)/b;
  c.z=lfixed(a.z)/b;
  return c;
}

quaternion operator*(quaternion a, quaternion b)
{
  quaternion c;
  c.w=a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z;
  c.x=a.y*b.z-a.z*b.y+a.w*b.x+a.x*b.w;
  c.y=a.z*b.x-a.x*b.z+a.w*b.y+a.y*b.w;
  c.z=a.x*b.y-a.y*b.x+a.w*b.z+a.z*b.w;
  return c;
}

quaternion operator-(quaternion a)
{
  quaternion c;
  c.w=-a.w; //see also conjugate()
  c.x=-a.x;
  c.y=-a.y;
  c.z=-a.z;
  return c;
}

fixed norm(quaternion a)
{return a.w*a.w+a.x*a.x+a.y*a.y+a.z*a.z;}

lfixed lnorm(quaternion a)
{return a.w%a.w+a.x%a.x+a.y%a.y+a.z%a.z;}

fixed magnitude(quaternion a)
{return sqrt(lnorm(a));}

lfixed lmagnitude(quaternion a)
{return lsqrt(lnorm(a));}

//void print(const char *name, fixed val);

void quaternion::normalize(void)
{
  //quaternion z;
  //print("ln",lnorm(*this));
  lfixed nm=lmagnitude(*this);
  //print("nm",nm);
  this->w=this->w%one/nm;
  this->x=this->x%one/nm;
  this->y=this->y%one/nm;
  this->z=this->z%one/nm;
  
  if(this->w<0)
  {
    this->w=-this->w;
    this->x=-this->x;
    this->y=-this->y;
    this->z=-this->z;
  }
  //return *this;
}

quaternion conjugate(quaternion a)
{
  quaternion c;
  c.w=a.w;
  c.x=-a.x;
  c.y=-a.y;
  c.z=-a.z;
  return c;
}

quaternion inverse(quaternion a)
{return conjugate(a)/norm(a);}

fixed inner(quaternion a, quaternion b)
{return a.w*b.w+a.x*b.x+a.y*b.y+a.z*b.z;}

quaternion sqrt(quaternion a)
{
  //quaternion z=(a>>1)+(ident>>1);
  quaternion z=quaternion((a.w>>1)+(one>>1), a.x>>1, a.y>>1, a.z>>1);
  z.normalize();
  return z;
}

#endif
