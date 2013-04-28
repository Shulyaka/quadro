#define CMDUNKNOWN 0
#define CMDPING    1 // echo test
#define CMDGYRO    2
#define CMDACCEL   3
#define CMDLAMP    4
#define CMDZERO    5
#define CMDTAKEOFF 6
#define CMDLAND    7
#define CMDEMERG   8
#define CMDDEBUG   9 // toggle debug flag
#define CMDSETA    10
#define CMDSETB    11
#define CMDSETC    12
#define CMDUSR1    13
#define CMDUSR2    14
#define CMDUSR3    15

#define cmdBufLen 127
char cmdBuf[cmdBufLen]={0};
unsigned char cmdPos=0;
char cmdBuf2[cmdBufLen]={0};
unsigned char cmdPos2=0;

void serialEvent(void)
{
  unsigned char cmd;
  int param=0;
  while(Serial.available())
  {
    cmdBuf[cmdPos++]=(char)Serial.read();
    if ((cmdBuf[cmdPos-1]=='\n') || (cmdBuf[cmdPos-1]=='\r'))
      break;
    if (cmdPos==cmdBufLen)
    {
      error("The command is too long");
      Serial.flush();
      clear_cmdBuf();
      break;
    }
  }
  if (cmdPos==0 || ((cmdBuf[cmdPos-1]!='\n') && (cmdBuf[cmdPos-1]!='\r')))
    return;
  if(debug) print_cmdBuf();
  cmd=parse_cmd(&param);
  clear_cmdBuf();
  
  if(debug)
  {
    Serial.print("Command: ");
    Serial.print((int)cmd);
    Serial.print(", Params: ");
    Serial.print(param);
    Serial.print(".\n");
  }
  
  switch(cmd)
  {
    case CMDPING:
      cmd_ping();
      break;
    case CMDGYRO:
      cmd_gyro();
      break;
    case CMDACCEL:
      cmd_accel();
      break;
    case CMDLAMP:
      cmd_lamp(param);
      break;
    case CMDZERO:
      cmd_zero();
      break;
    case CMDTAKEOFF:
      cmd_takeoff();
      break;
    case CMDLAND:
      cmd_land();
      break;
    case CMDEMERG:
      cmd_emerg();
      break;
    case CMDDEBUG:
      cmd_debug(param);
      break;
    case CMDSETA:
      cmd_seta(param);
      break;
    case CMDSETB:
      cmd_setb(param);
      break;
    case CMDSETC:
      cmd_setc(param);
      break;
    case CMDUSR1:
      cmd_usr1(param);
      break;
    case CMDUSR2:
      cmd_usr2();
      break;
    case CMDUSR3:
      cmd_usr3();
      break;
    
    case CMDUNKNOWN:
      error("Unknown command");
      break;
    default:
      error("The command has been disabled");
  }
  if(Serial.available())
    serialEvent();
}

void serialEvent2(void)
{
  unsigned char cmd;
  int param=0;
  while(Serial2.available())
  {
    cmdBuf2[cmdPos2++]=(char)Serial2.read();
    if ((cmdBuf2[cmdPos2-1]=='\n') || (cmdBuf2[cmdPos2-1]=='\r'))
      break;
    if (cmdPos2==cmdBufLen)
    {
      error("The command is too long");
      Serial2.flush();
      clear_cmdBuf2();
      break;
    }
  }
  if (cmdPos2==0 || ((cmdBuf2[cmdPos2-1]!='\n') && (cmdBuf2[cmdPos2-1]!='\r')))
    return;
  if(debug) print_cmdBuf2();
  cmd=parse_cmd(&param);
  clear_cmdBuf2();
  
  if(debug)
  {
    Serial.print("Command: ");
    Serial.print((int)cmd);
    Serial.print(", Params: ");
    Serial.print(param);
    Serial.print(".\n");
  }
  
  switch(cmd)
  {
    case CMDPING:
      cmd_ping();
      break;
    case CMDGYRO:
      cmd_gyro();
      break;
    case CMDACCEL:
      cmd_accel();
      break;
    case CMDLAMP:
      cmd_lamp(param);
      break;
    case CMDZERO:
      cmd_zero();
      break;
    case CMDTAKEOFF:
      cmd_takeoff();
      break;
    case CMDLAND:
      cmd_land();
      break;
    case CMDEMERG:
      cmd_emerg();
      break;
    case CMDDEBUG:
      cmd_debug(param);
      break;
    case CMDSETA:
      cmd_seta(param);
      break;
    case CMDSETB:
      cmd_setb(param);
      break;
    case CMDSETC:
      cmd_setc(param);
      break;
    case CMDUSR1:
      cmd_usr1(param);
      break;
    case CMDUSR2:
      cmd_usr2();
      break;
    case CMDUSR3:
      cmd_usr3();
      break;

    case CMDUNKNOWN:
      error("Unknown command");
      break;
    default:
      error("The command has been disabled");
  }
  if(Serial2.available())
    serialEvent2();
}


unsigned char parse_cmd(int *param)
{
  if (!memcmp(cmdBuf,"ping",4))
    return CMDPING;
  if (!memcmp(cmdBuf,"gyro",4))
    return CMDGYRO;
  if (!memcmp(cmdBuf,"accel",5))
    return CMDACCEL;
  if (!memcmp(cmdBuf,"lamp",4))
  {
    if (!memcmp(cmdBuf+5,"on",2))
      *param=HIGH;
    else if (!memcmp(cmdBuf+5,"off",3))
      *param=LOW;
    else
      return CMDUNKNOWN;
    return CMDLAMP;
  }
  if (!memcmp(cmdBuf,"zero",4)||!memcmp(cmdBuf,"0",1))
    return CMDZERO;
  if (!memcmp(cmdBuf,"takeoff",7))
    return CMDTAKEOFF;
  if (!memcmp(cmdBuf,"land",4))
    return CMDLAND;
  if (!memcmp(cmdBuf,"emerg",5))
    return CMDEMERG;
  if (!memcmp(cmdBuf,"debug",5))
  {
    if (!memcmp(cmdBuf+6,"on",2))
      *param=true;
    else if (!memcmp(cmdBuf+6,"off",3))
      *param=false;
    else
      return CMDUNKNOWN;
    return CMDDEBUG;
  }
  if (!memcmp(cmdBuf,"a ",2))
  {
    sscanf(cmdBuf+2,"%d",param);
    return CMDSETA;
  }
  if (!memcmp(cmdBuf,"b ",2))
  {
    sscanf(cmdBuf+2,"%d",param);
    return CMDSETB;
  }
  if (!memcmp(cmdBuf,"c ",2))
  {
    sscanf(cmdBuf+2,"%d",param);
    return CMDSETC;
  }
  if (!memcmp(cmdBuf,"m ",2))
  {
    sscanf(cmdBuf+2,"%d",param);
    return CMDUSR1;
  }
//  if (!memcmp(cmdBuf,"Z",1))
//    return CMDUSR2;
  if (!memcmp(cmdBuf,"f",1))
    return CMDUSR3;
  if (!memcmp(cmdBuf,"e",1))
    return CMDEMERG;
  
  return CMDUNKNOWN;
}

unsigned char parse_cmd2(int *param)
{
  if (!memcmp(cmdBuf2,"ping",4))
    return CMDPING;
  if (!memcmp(cmdBuf2,"gyro",4))
    return CMDGYRO;
  if (!memcmp(cmdBuf2,"accel",5))
    return CMDACCEL;
  if (!memcmp(cmdBuf2,"lamp",4))
  {
    if (!memcmp(cmdBuf2+5,"on",2))
      *param=HIGH;
    else if (!memcmp(cmdBuf2+5,"off",3))
      *param=LOW;
    else
      return CMDUNKNOWN;
    return CMDLAMP;
  }
  if (!memcmp(cmdBuf2,"zero",4)||!memcmp(cmdBuf2,"0",1))
    return CMDZERO;
  if (!memcmp(cmdBuf2,"takeoff",7))
    return CMDTAKEOFF;
  if (!memcmp(cmdBuf2,"land",4))
    return CMDLAND;
  if (!memcmp(cmdBuf2,"emerg",5))
    return CMDEMERG;
  if (!memcmp(cmdBuf2,"debug",5))
  {
    if (!memcmp(cmdBuf2+6,"on",2))
      *param=true;
    else if (!memcmp(cmdBuf2+6,"off",3))
      *param=false;
    else
      return CMDUNKNOWN;
    return CMDDEBUG;
  }
  if (!memcmp(cmdBuf2,"a ",2))
  {
    sscanf(cmdBuf2+2,"%d",param);
    return CMDSETA;
  }
  if (!memcmp(cmdBuf2,"b ",2))
  {
    sscanf(cmdBuf2+2,"%d",param);
    return CMDSETB;
  }
  if (!memcmp(cmdBuf2,"c ",2))
  {
    sscanf(cmdBuf2+2,"%d",param);
    return CMDSETC;
  }
  if (!memcmp(cmdBuf2,"z",1))
    return CMDUSR1;
  if (!memcmp(cmdBuf2,"Z",1))
    return CMDUSR2;
  if (!memcmp(cmdBuf2,"f",1))
    return CMDUSR3;
  if (!memcmp(cmdBuf2,"e",1))
    return CMDEMERG;
  
  return CMDUNKNOWN;
}

void print_cmdBuf(void)
{
  Serial.print("Incomming command (");
  Serial.print(strlen(cmdBuf));
  Serial.print("):\n"); // a smiley
  Serial.println(cmdBuf);
}

void clear_cmdBuf (void)
{
  memset(cmdBuf,'\0',cmdBufLen);
  //for (int i=0; i<cmdBufLen; i++)
  //  cmdBuf[i]=0;
  cmdPos=0;
}

void print_cmdBuf2(void)
{
  Serial.print("Incomming command (");
  Serial.print(strlen(cmdBuf2));
  Serial.print("):\n"); // a smiley
  Serial.println(cmdBuf2);
}

void clear_cmdBuf2(void)
{
  memset(cmdBuf2,'\0',cmdBufLen);
  cmdPos2=0;
}

