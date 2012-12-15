class fixed
{
  public:
  fixed(void);
  fixed(long);
  long value;
  bool isone; //not used currently
};

class lfixed
{
  public:
  lfixed(void);
  lfixed(long long);
  long long value;
};

fixed::fixed(void)
{
  fixed::value=0;
  bool isone=0;
}

fixed::fixed(long x)
{
  fixed::value=x;
  bool isone=0;
}

lfixed::lfixed(void)
{}

lfixed::lfixed(long long x)
{lfixed::value=x;}

/*fixed newfixed(long x)
{
  fixed z;
  z.value=x;
  return z;
}

lfixed newlfixed(long long x)
{
  lfixed z;
  z.value=x;
  return z;
}*/

const fixed one = ((signed long)((1UL<<31)-1))+1;

bool operator==(fixed x, fixed y)
{return x.value==y.value ? true : false;}

bool operator!=(fixed x, fixed y)
{return x.value!=y.value ? true : false;}

bool operator>(fixed x, fixed y)
{return (x.value>y.value)||((x==one)&&(y!=one)) ? true : false;}

bool operator>=(fixed x, fixed y)
{return (x.value>=y.value)||(x==one) ? true : false;}

bool operator<(fixed x, fixed y)
{return (x.value<y.value)||((y==one)&&(x!=one)) ? true : false;}

bool operator<=(fixed x, fixed y)
{return (x.value<=y.value)||(y==one) ? true : false;}


bool operator==(lfixed x, lfixed y)
{return x.value==y.value ? true : false;}

bool operator!=(lfixed x, lfixed y)
{return x.value!=y.value ? true : false;}

bool operator>(lfixed x, lfixed y)
{return x.value>y.value ? true : false;}

bool operator>=(lfixed x, lfixed y)
{return x.value>=y.value ? true : false;}

bool operator<(lfixed x, lfixed y)
{return x.value<y.value ? true : false;}

bool operator<=(lfixed x, lfixed y)
{return x.value<=y.value ? true : false;}

/*bool operator==(fixed x, long y)
{return x.value==y ? true : false;}

bool operator!=(fixed x, long y)
{return x.value!=y ? true : false;}

bool operator>(fixed x, long y)
{return x.value>y ? true : false;}

bool operator>=(fixed x, long y)
{return x.value>=y ? true : false;}

bool operator<(fixed x, long y)
{return x.value<y ? true : false;}

bool operator<=(fixed x, long y)
{return x.value<=y ? true : false;}
*/

fixed operator+(fixed x, fixed y)
{
  fixed z;
  z.value=x.value+y.value;
  return z;
}

/*fixed operator+(long x, fixed y)
{
  fixed z;
  z.value=x+y.value;
  return z;
}

fixed operator+(fixed x, long y)
{
  fixed z;
  z.value=x.value+y;
  return z;
}
*/
fixed operator-(fixed x, fixed y)
{
  fixed z;
  z.value=x.value-y.value;
  return z;
}

/*fixed operator-(long x, fixed y)
{
  fixed z;
  z.value=x-y.value;
  return z;
}

fixed operator-(fixed x, long y)
{
  fixed z;
  z.value=x.value-y;
  return z;
}
*/
fixed operator-(fixed x)
{
  fixed z;
  if (x==one)
    z.value=((signed long)((1L<<31)-1))+2;
  else
    z.value=-x.value;
  return z;
}

fixed operator<<(fixed x, char y)
{
  fixed z;
  z.value=x.value<<y;
  if(z<0 && x>0)
    z=one;
  return z;
}

fixed operator>>(fixed x, char y)
{
  fixed z;
  if(y==0)
    return x;
  if(x==one)
    z.value=1L<<(31-y);
  else
    z.value=x.value>>y;
  return z;
}


lfixed operator+(lfixed x, lfixed y)
{
  lfixed z;
  z.value=x.value+y.value;
  return z;
}

/*lfixed operator+(long long x, lfixed y)
{
  lfixed z;
  z.value=x+y.value;
  return z;
}

lfixed operator+(lfixed x, long long y)
{
  lfixed z;
  z.value=x.value+y;
  return z;
}
*/
lfixed operator-(lfixed x, lfixed y)
{
  lfixed z;
  z.value=x.value-y.value;
  return z;
}

/*lfixed operator-(long long x, lfixed y)
{
  lfixed z;
  z.value=x-y.value;
  return z;
}

lfixed operator-(lfixed x, long long y)
{
  lfixed z;
  z.value=x.value-y;
  return z;
}
*/
lfixed operator-(lfixed x)
{
  lfixed z;
  z.value=-x.value;
  return z;
}

lfixed operator<<(lfixed x, byte y)
{
  lfixed z;
  z.value=x.value<<y;
  return z;
}

lfixed operator>>(lfixed x, byte y)
{
  lfixed z;
  z.value=x.value>>y;
  return z;
}

fixed operator*(fixed x, fixed y) //multiply and conquer!  //to be rewritten
{
  fixed z;
  if(y==one)
    z.value=x.value;
  else if(x==one)
    z.value=y.value;
  else
    z.value=(x.value>0) ? ((long long)(y.value)*((unsigned long)(x.value)<<1)+0x80000000)>>32 : -(((long long)(y.value)*((unsigned long)(-x.value)<<1)+0x80000000)>>32);
  return z;
}

lfixed operator%(fixed x, fixed y) //NOTE that it is NOT a division
{
  lfixed z;
  if (y==one && x==one)
    z.value=1LL<<62;
  else if (y==one)
    z.value=(long long)x.value<<31;
  else if (x==one)
    z.value=(long long)y.value<<31;
  else
    z.value=(long long)x.value*(long long)y.value;
  return z;
}

fixed operator/(lfixed x, fixed y)
{
  fixed z;
  if(y==one)
    z.value=x.value>>31;
  else
    {z.value=x.value/y.value;
    if (z.value==one && x.value<0)
      z.value=one.value+1;
    }
  return z;
}

fixed operator/(lfixed x, lfixed y)
{
  fixed z;
  unsigned long long a,b;
  byte sign=0;
//  if(y.value==0)
//    return one;
  if(x.value<0)
  {
    a=-x.value;
    sign^=1;
  }
  else
    a=x.value;
  if(y.value<0)
  {
    b=-y.value;
    sign^=1;
  }
  else
    b=y.value;
  if(b<a)
    return one;

  z.value=0;
  for(byte i=30; i!=255; i--)
  {
    a<<=1;
    if(b<a)
    {
      z.value|=1UL<<i;
      a-=b;
    }
  }
  if(b<a<<1)
    z.value++; //rounding
  return sign?-z:z;
//  if(sign==1)
//    z.value=-z.value;
//  return z;
}

fixed tofixed(lfixed x)
{
//  fixed z;
  if (x>=0x4000000000000000LL)
    return one;
  if (x<=-0x3FFFFFFF80000000LL)
    return -one;
  return x>0?(x.value+0x40000000)>>31:-((-x.value+0x40000000)>>31);
//  return z;
}

lfixed tolfixed(fixed x)
{
  if(x==one)
    return 0x4000000000000000LL;
  if(x==-one)
    return -0x4000000000000000LL;
  return x>0? ((long long)x.value)<<31:-(((long long)x.value)<<31);
}

lfixed ldiv(lfixed x, lfixed y)
{
  lfixed z;
  unsigned long long a,b;
  byte sign=0;
//  print("ldiv: a",a);
//  print("ldiv: b",b);
  if(x.value<0)
  {
    a=-x.value;
    sign^=1;
  }
  else
    a=x.value;
  if(y.value<0)
  {
    b=-y.value;
    sign^=1;
  }
  else
    b=y.value;
  if(b<a>>1)
    return 0;

  z.value=0;
  for(byte i=62; i!=255; i--)
  {
    if(b<a)
    {
      z.value|=1ULL<<i;
      a-=b;
    }
    a<<=1;
  }
  if (b<a)
    x.value++; //rounding
  return sign?-z:z;
}

fixed operator/(fixed x, int y)
{
  fixed z;
  if(x==one)
    z.value=(one.value-1)/y;
  else
    z.value=x.value/y;
  return z;
}

lfixed operator*(lfixed x, lfixed y)
{
  byte sign=0;
  lfixed z=0;
//  unsigned long long b=y.value;
//Serial.println((long)(b>>32),HEX);
  if(x.value<0)
    sign^=1;
  if(y.value<0)
    {sign^=1;
    y.value=-y.value;
    }

  for(byte i=62; i!=255; i--)
  {
    if(x.value&(1ULL<<i))
      z=z+y;
      //Serial.println("bingo");
    //}
    y=y>>1;
//    Serial.println((long)((x.value&(1ULL<<i))>>32),HEX);
//    Serial.println((long)(b>>32),HEX);
  }
  return sign?-z:z;
}

lfixed operator*(lfixed x, fixed y)
{
  return x*tolfixed(y);
}

