class quaternion
{
  public:
  quaternion(void);
  quaternion(fixed, fixed, fixed, fixed);
  quaternion(fixed, fixed, fixed);
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

quaternion operator*(quaternion a, quaternion b)
{
  quaternion c;
  c.x=a.y*b.z-a.z*b.y+a.w*b.x+a.x*b.w;
  c.y=a.z*b.x-a.x*b.z+a.w*b.y+a.y*b.w;
  c.z=a.x*b.y-a.y*b.x+a.w*b.z+a.y*b.w;
  c.w=a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z;
  return c;
}

