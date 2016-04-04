// L1     L2        R1        R2        Ation
// --------------------------------------------
// 1    0        1        0        Move forward
// 0    1        0        1        Move backward
// 0    1        1        0        Move left
// 1    0        0        1        Move right
// 0    0        0        0        Stop

// Motor power H bridge pins
int motorLeft1Pin  = 3;  // pin that the H bridge 2 (left moter) is attached to
int motorLeft2Pin  = 2; // pin that the H bridge 7 (left moter) is attached to
int motorRight1Pin = 5;  // pin that the H bridge 2 (right moter) is attached to
int motorRight2Pin = 4;  // pin that the H bridge 7 (right moter) is attached to

int motorSpeedPin  = 6;  // pin that the H bridge 1, 9 (speed) is attached to
int motorSpeed = 105; // 0 - 255 

int trigPin = 10;
int echoPin = 11;
int distance =0;

int frontLDR = A0;
int backLDR = A1;
int frontThreshold, backThreshold;
boolean front, back;

String state = "f";

// Sets up the program by setting the initial values and pinmodes.
void setup() 
{
  Serial.begin(9600);
  Serial.println("Setting up ...");

  // set the LED pins as outputs and the switch pin as input:  

  pinMode(motorLeft1Pin, OUTPUT);
  pinMode(motorLeft2Pin, OUTPUT);
  pinMode(motorRight1Pin, OUTPUT);
  pinMode(motorRight2Pin, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);

  pinMode (trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
  
  delay (750);
  frontThreshold = analogRead (frontLDR) + 75;
  backThreshold = analogRead (backLDR) + 75;
  
  digitalWrite (motorSpeedPin, HIGH);
  setMotors (0, 0, 0, 0);

}

void setMotors(int left1, int left2, int right1, int right2) 
{
  digitalWrite(motorLeft1Pin, left1 == 1?HIGH:LOW);
  digitalWrite(motorLeft2Pin, left2 == 1?HIGH:LOW);
  digitalWrite(motorRight1Pin, right1 == 1?HIGH:LOW);
  digitalWrite(motorRight2Pin, right2 == 1?HIGH:LOW);
}

void readSensors() 
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  distance = pulseIn(echoPin, HIGH) / 58.2;

  front = (analogRead (frontLDR) > frontThreshold);
  back = (analogRead (backLDR) > backThreshold);
}

void changeState (String s)
{
  if (state != s)
  {
    delay (100);
    state = s;
  }
  Serial.print (s);
}

// Main loop.
void loop() 
{
  readSensors();
  Serial.print(distance);
  Serial.print(" cm ");
  if (front && back)
  {
    changeState ("e");
    analogWrite(motorSpeedPin, 255);          
    setMotors(0, 1, 0, 0);
    delay (1000);
  }
  else if (back || (distance != 0 && distance <= 40))
  {
    changeState ("f");
    analogWrite(motorSpeedPin, 255);          
    setMotors(1,0,1,0);
  }
  else if (front)
  {
    changeState ("b");
    analogWrite(motorSpeedPin, 255);          
    setMotors(0, 1, 0, 1);
  }
  else
  {
    changeState ("t");
    analogWrite(motorSpeedPin, 96);          
    setMotors (1, 0, 0, 1);
  }
  Serial.println ();
  delay(50);
}


