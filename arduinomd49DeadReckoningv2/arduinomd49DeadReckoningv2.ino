
/*********************)
**********************
        Arduino example code for MD49
  Using LCD03 in serial mode to display data
*                                            *
          By James Henderson, 2012.
*********************************************/

#include <SoftwareSerial.h>

// Values of 0 being sent over serial write have to be cast as a byte so they arent misinterpreted as NULL
// This is a bug with arduino 1.0
#define CMD        (byte)0x00            // MD49 command address of 0 
#define GET_SPEED1       0x21                                
#define GET_ENC1         0x23  
#define GET_ENC2         0x24          
#define SET_SPEED1       0x31
#define SET_SPEED2       0x32
#define ENC_RESET        0x35
uint32_t encoderRightCountNew = 0;
uint32_t encoderLeftCountNew = 0;
byte enc1a, enc1b, enc1c, enc1d = 0;
byte enc2a, enc2b, enc2c, enc2d = 0;
byte speed1a = 0;
byte speed1b = 0;
String stringOne = "Hello String";
// reads the value of encoder 1 into an unsigned 32 bit int
int a, b, c, d;
unsigned int integerValue = 0; // Max value is 65535
char incomingByte;
int sensors[8] = {0};
static char outstr[15];
char inChar;
int countsPerRevolution = 980;
float diameterWheel = 12.5;//centimeters
float distancePerCount = (PI*diameterWheel)/countsPerRevolution;
float deltaDistance = 0.0;
//float leftCounts  = 0.0;
//float rightCounts = 0.0;
float countsPerRotation = 0.0;
float trackWidth = 36.0;
float radiansPerCount = 0;
float deltaHeading = 0;
float deltaX = 0.0;
float deltaY = 0.0;
float mPreviousLeftCounts  = 0;
float mPreviousRightCounts = 0;
float LeftCountsOld  = 0;
float RightCountsOld = 0;
void setRightCounts(float rCounts);
float getRightCounts(void);
void setLeftCounts(float lCounts);
float getLeftCounts(void);
//float encoderRightCount = 0;
//float encoderLeftCount = 0;
float leftCounts  = 0;
float rightCounts = 0;
//float heading = 0;
float deltaRight = 0;
float deltaLeft = 0;
float mX = 0;
float mY = 0;
float mHeading = 0;
//float deltaHeading = 0;
float getmX(void);
float getmY(void);
float getmHeading(void);
void setmX(float mXold);
void setmY(float mYold);
void setmHeading(float mHeadingold);




void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);

  Serial1.write(CMD);
  Serial1.write(ENC_RESET);

  setRightCounts(0);
  setLeftCounts(0);
}

void loop()
{


  Serial1.write(CMD);
  Serial1.write(SET_SPEED1);
  Serial1.write(140);                        // Set motor 1 speed left

  Serial1.write(CMD);
  Serial1.write(SET_SPEED2);
  Serial1.write(140);                        // Set motor 2 speed right

  mPreviousRightCounts =  getRightCounts();
  mPreviousLeftCounts =   getLeftCounts();

  Serial1.write(CMD);
  Serial1.write(GET_ENC1);                   // Recieve encoder 1 value
  delay(50);                                 // Right wheel 
  if (Serial1.available() > 3)
  {
    enc1a = Serial1.read();
    enc1b = Serial1.read();
    enc1c = Serial1.read();
    enc1d = Serial1.read();

    

  }

    encoderRightCountNew = (((uint32_t)enc1a << 24) +
   ((uint32_t)enc1b << 16) +
   ((uint32_t)enc1c << 8) +
   ((uint32_t)enc1d << 0));

  Serial.print("pulses right encoder:");
  Serial.println(encoderRightCountNew,DEC);
  delay(100);

  rightCounts = (float)encoderRightCountNew;

  //encoderRightCount = (float)encoderRightCountNew - mPreviousRightCounts;
  deltaRight = rightCounts - mPreviousRightCounts;

  Serial.print("encoder pulses right encoder:");
  //Serial.println(encoderRightCount,DEC);
  Serial.println(rightCounts,DEC);
  delay(100);


  Serial1.write(CMD);
  Serial1.write(GET_ENC2);                   // Recieve encoder 1 value
  delay(50);                                 // Right wheel 
  if (Serial1.available() > 3)
  {
    enc2a = Serial1.read();
    enc2b = Serial1.read();
    enc2c = Serial1.read();
    enc2d = Serial1.read();

  }

    encoderLeftCountNew = (((uint32_t)enc2a << 24) +
   ((uint32_t)enc2b << 16) +
   ((uint32_t)enc2c << 8) +
   ((uint32_t)enc2d << 0));

   Serial.print("pulses left encoder:");
   Serial.println(encoderLeftCountNew,DEC);
   delay(100);

  leftCounts = (float)encoderLeftCountNew;
  //encoderLeftCount = (float)encoderLeftCountNew - mPreviousLeftCounts;
  //leftCounts = (float)encoderLeftCountNew - mPreviousLeftCounts;
  deltaLeft = leftCounts - mPreviousLeftCounts;

  Serial.print("encoder left encoder:");
  //Serial.println(encoderLeftCount,DEC);
  Serial.println(leftCounts,DEC);
  delay(100);



  Serial1.write(CMD);
  Serial1.write(GET_SPEED1);                   // software version
  delay(50);


   if (Serial1.available() > 0)
  {
    speed1a = Serial1.read();

  }

  Serial.print("speed1a:");
  Serial.println(speed1a);
  delay(100);

  Serial.print("distancePerCount:");
  Serial.println(distancePerCount);
  delay(100);

  Serial.print("diameterWheel:");
  Serial.println(diameterWheel);
  delay(100);

  Serial.print(" countsPerRevolution:");
  Serial.println( countsPerRevolution);
  delay(100);

  Serial.print(" PI:");
  Serial.println(PI);
  delay(100);

  //deltaDistance = ((encoderLeftCount + encoderRightCount)/2.0)*distancePerCount;
  deltaDistance = ((deltaLeft + deltaRight)/2.0)*distancePerCount;

  Serial.print(" deltaDistance:");
  Serial.println( deltaDistance);
  delay(100);

  countsPerRotation = (trackWidth/diameterWheel)*countsPerRevolution;

  Serial.print(" countsPerRotation:");
  Serial.println( countsPerRotation);
  delay(100);

  radiansPerCount = PI*(diameterWheel/trackWidth)/countsPerRevolution;

  Serial.print(" radiansPerCount:");
  Serial.println( radiansPerCount);
  delay(100);

  //deltaHeading = (encoderRightCount - encoderLeftCount)*radiansPerCount;
  deltaHeading = (rightCounts - leftCounts)*radiansPerCount;

  Serial.print(" deltaHeading:");
  Serial.println( deltaHeading);
  delay(100);

  //deltaX = deltaDistance*cos(deltaHeading);

  mX = getmX();

  mY = getmY();

  mHeading = getmHeading();


  deltaHeading = (float)(deltaRight - deltaLeft)*radiansPerCount;

  mX = mX + deltaX;

  mY = mY + deltaY;

  mHeading = mHeading + deltaHeading;

  deltaX = deltaDistance*cos(mHeading);


  deltaY = deltaDistance*sin(mHeading);

   Serial.print(" mX:");
   Serial.println( mX);
   delay(100);

   Serial.print(" mY:");
   Serial.println( mY);
   delay(100);

  Serial.print(" mHeading:");
   Serial.println( mHeading);
   delay(100);

   setmX(mX);

   setmY(mY);

   setmHeading(mHeading);

  //setRightCounts( encoderRightCount);
  setRightCounts( rightCounts);

  //setLeftCounts(encoderLeftCount);
  setLeftCounts(leftCounts);

  //Serial1.write(CMD);
  //Serial1.write(ENC_RESET);                   // software version
  //delay(50);

}

void setRightCounts(float rCounts){
     RightCountsOld = rCounts;  
}

float getRightCounts(void){
     return RightCountsOld;
}

void setLeftCounts(float lCounts){
     LeftCountsOld = lCounts;  
}

float getLeftCounts(void){
     return LeftCountsOld;
}


void setmX(float mXold){
     mX = mXold;  
}

float getmX(void){
     return mX;
}


void setmY(float mYold){
     mY = mYold;  
}

float getmY(void){
     return mY;
}

void setmHeading(float mHeadingold){
     mHeading = mHeadingold;  
}

float getmHeading(void){
     return mHeading;
}
