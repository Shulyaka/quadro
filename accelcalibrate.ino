fixed det(fixed a[3][3])
{ return a[0][0]*a[1][1]*a[2][2]-a[0][2]*a[1][1]*a[2][0]+a[0][1]*a[1][2]*a[2][0]-a[0][1]*a[1][0]*a[2][2]+a[0][2]*a[1][0]*a[2][1]-a[0][0]*a[1][2]*a[2][1]; }

fixed det(fixed a[4][4])
{
  fixed m[3][3];
  fixed d;
  m[0][0]=a[1][1]; m[0][1]=a[1][2]; m[0][2]=a[1][3];
  m[1][0]=a[2][1]; m[1][1]=a[2][2]; m[1][2]=a[2][3];
  m[2][0]=a[3][1]; m[2][1]=a[3][2]; m[2][2]=a[3][3];
  d=a[0][0]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][2]; m[0][2]=a[1][3];
  m[1][0]=a[2][0]; m[1][1]=a[2][2]; m[1][2]=a[2][3];
  m[2][0]=a[3][0]; m[2][1]=a[3][2]; m[2][2]=a[3][3];
  d=d-a[0][1]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][1]; m[0][2]=a[1][3];
  m[1][0]=a[2][0]; m[1][1]=a[2][1]; m[1][2]=a[2][3];
  m[2][0]=a[3][0]; m[2][1]=a[3][1]; m[2][2]=a[3][3];
  d=d+a[0][2]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][1]; m[0][2]=a[1][2];
  m[1][0]=a[2][0]; m[1][1]=a[2][1]; m[1][2]=a[2][2];
  m[2][0]=a[3][0]; m[2][1]=a[3][1]; m[2][2]=a[3][2];
  return d-a[0][3]*det(m);
}

fixed det(fixed a[5][5])
{
  fixed m[4][4];
  fixed d;
  m[0][0]=a[1][1]; m[0][1]=a[1][2]; m[0][2]=a[1][3]; m[0][3]=a[1][4];
  m[1][0]=a[2][1]; m[1][1]=a[2][2]; m[1][2]=a[2][3]; m[1][3]=a[2][4];
  m[2][0]=a[3][1]; m[2][1]=a[3][2]; m[2][2]=a[3][3]; m[2][3]=a[3][4];
  m[3][0]=a[4][1]; m[3][1]=a[4][2]; m[3][2]=a[4][3]; m[3][3]=a[4][4];
  d=a[0][0]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][2]; m[0][2]=a[1][3]; m[0][3]=a[1][4];
  m[1][0]=a[2][0]; m[1][1]=a[2][2]; m[1][2]=a[2][3]; m[1][3]=a[2][4];
  m[2][0]=a[3][0]; m[2][1]=a[3][2]; m[2][2]=a[3][3]; m[2][3]=a[3][4];
  m[3][0]=a[4][0]; m[3][1]=a[4][2]; m[3][2]=a[4][3]; m[3][3]=a[4][4];
  d=d-a[0][1]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][1]; m[0][2]=a[1][3]; m[0][3]=a[1][4];
  m[1][0]=a[2][0]; m[1][1]=a[2][1]; m[1][2]=a[2][3]; m[1][3]=a[2][4];
  m[2][0]=a[3][0]; m[2][1]=a[3][1]; m[2][2]=a[3][3]; m[2][3]=a[3][4];
  m[3][0]=a[4][0]; m[3][1]=a[4][1]; m[3][2]=a[4][3]; m[3][3]=a[4][4];
  d=d+a[0][2]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][1]; m[0][2]=a[1][2]; m[0][3]=a[1][4];
  m[1][0]=a[2][0]; m[1][1]=a[2][1]; m[1][2]=a[2][2]; m[1][3]=a[2][4];
  m[2][0]=a[3][0]; m[2][1]=a[3][1]; m[2][2]=a[3][2]; m[2][3]=a[3][4];
  m[3][0]=a[4][0]; m[3][1]=a[4][1]; m[3][2]=a[4][2]; m[3][3]=a[4][4];
  d=d-a[0][3]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][1]; m[0][2]=a[1][2]; m[0][3]=a[1][3];
  m[1][0]=a[2][0]; m[1][1]=a[2][1]; m[1][2]=a[2][2]; m[1][3]=a[2][3];
  m[2][0]=a[3][0]; m[2][1]=a[3][1]; m[2][2]=a[3][2]; m[2][3]=a[3][3];
  m[3][0]=a[4][0]; m[3][1]=a[4][1]; m[3][2]=a[4][2]; m[3][3]=a[4][3];
  return d+a[0][4]*det(m);
}

fixed det(fixed a[6][6])
{
  fixed m[5][5];
  fixed d;
  m[0][0]=a[1][1]; m[0][1]=a[1][2]; m[0][2]=a[1][3]; m[0][3]=a[1][4]; m[0][4]=a[1][5];
  m[1][0]=a[2][1]; m[1][1]=a[2][2]; m[1][2]=a[2][3]; m[1][3]=a[2][4]; m[1][4]=a[2][5];
  m[2][0]=a[3][1]; m[2][1]=a[3][2]; m[2][2]=a[3][3]; m[2][3]=a[3][4]; m[2][4]=a[3][5];
  m[3][0]=a[4][1]; m[3][1]=a[4][2]; m[3][2]=a[4][3]; m[3][3]=a[4][4]; m[3][4]=a[4][5];
  m[4][0]=a[5][1]; m[4][1]=a[5][2]; m[4][2]=a[5][3]; m[4][3]=a[5][4]; m[4][4]=a[5][5];
  d=a[0][0]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][2]; m[0][2]=a[1][3]; m[0][3]=a[1][4]; m[0][4]=a[1][5];
  m[1][0]=a[2][0]; m[1][1]=a[2][2]; m[1][2]=a[2][3]; m[1][3]=a[2][4]; m[1][4]=a[2][5];
  m[2][0]=a[3][0]; m[2][1]=a[3][2]; m[2][2]=a[3][3]; m[2][3]=a[3][4]; m[2][4]=a[3][5];
  m[3][0]=a[4][0]; m[3][1]=a[4][2]; m[3][2]=a[4][3]; m[3][3]=a[4][4]; m[3][4]=a[4][5];
  m[4][0]=a[5][0]; m[4][1]=a[5][2]; m[4][2]=a[5][3]; m[4][3]=a[5][4]; m[4][4]=a[5][5];
  d=d-a[0][1]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][1]; m[0][2]=a[1][3]; m[0][3]=a[1][4]; m[0][4]=a[1][5];
  m[1][0]=a[2][0]; m[1][1]=a[2][1]; m[1][2]=a[2][3]; m[1][3]=a[2][4]; m[1][4]=a[2][5];
  m[2][0]=a[3][0]; m[2][1]=a[3][1]; m[2][2]=a[3][3]; m[2][3]=a[3][4]; m[2][4]=a[3][5];
  m[3][0]=a[4][0]; m[3][1]=a[4][1]; m[3][2]=a[4][3]; m[3][3]=a[4][4]; m[3][4]=a[4][5];
  m[4][0]=a[5][0]; m[4][1]=a[5][1]; m[4][2]=a[5][3]; m[4][3]=a[5][4]; m[4][4]=a[5][5];
  d=d+a[0][2]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][1]; m[0][2]=a[1][2]; m[0][3]=a[1][4]; m[0][4]=a[1][5];
  m[1][0]=a[2][0]; m[1][1]=a[2][1]; m[1][2]=a[2][2]; m[1][3]=a[2][4]; m[1][4]=a[2][5];
  m[2][0]=a[3][0]; m[2][1]=a[3][1]; m[2][2]=a[3][2]; m[2][3]=a[3][4]; m[2][4]=a[3][5];
  m[3][0]=a[4][0]; m[3][1]=a[4][1]; m[3][2]=a[4][2]; m[3][3]=a[4][4]; m[3][4]=a[4][5];
  m[4][0]=a[5][0]; m[4][1]=a[5][1]; m[4][2]=a[5][2]; m[4][3]=a[5][4]; m[4][4]=a[5][5];
  d=d-a[0][3]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][1]; m[0][2]=a[1][2]; m[0][3]=a[1][3]; m[0][4]=a[1][5];
  m[1][0]=a[2][0]; m[1][1]=a[2][1]; m[1][2]=a[2][2]; m[1][3]=a[2][3]; m[1][4]=a[2][5];
  m[2][0]=a[3][0]; m[2][1]=a[3][1]; m[2][2]=a[3][2]; m[2][3]=a[3][3]; m[2][4]=a[3][5];
  m[3][0]=a[4][0]; m[3][1]=a[4][1]; m[3][2]=a[4][2]; m[3][3]=a[4][3]; m[3][4]=a[4][5];
  m[4][0]=a[5][0]; m[4][1]=a[5][1]; m[4][2]=a[5][2]; m[4][3]=a[5][3]; m[4][4]=a[5][5];
  d=d+a[0][4]*det(m);
  m[0][0]=a[1][0]; m[0][1]=a[1][1]; m[0][2]=a[1][2]; m[0][3]=a[1][3]; m[0][4]=a[1][4];
  m[1][0]=a[2][0]; m[1][1]=a[2][1]; m[1][2]=a[2][2]; m[1][3]=a[2][3]; m[1][4]=a[2][4];
  m[2][0]=a[3][0]; m[2][1]=a[3][1]; m[2][2]=a[3][2]; m[2][3]=a[3][3]; m[2][4]=a[3][4];
  m[3][0]=a[4][0]; m[3][1]=a[4][1]; m[3][2]=a[4][2]; m[3][3]=a[4][3]; m[3][4]=a[4][4];
  m[4][0]=a[5][0]; m[4][1]=a[5][1]; m[4][2]=a[5][2]; m[4][3]=a[5][3]; m[4][4]=a[5][4];
  return d-a[0][5]*det(m);
}
