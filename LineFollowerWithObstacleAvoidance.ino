#define enA 10  // Enable1 L298 Pin enA
#define in1 9   // Motor1 L298 Pin in1
#define in2 8   // Motor1 L298 Pin in1
#define in3 7   // Motor2 L298 Pin in1
#define in4 6   // Motor2 L298 Pin in1
#define enB 5   // Enable2 L298 Pin enB

#define L_S A0   // IR sensor Left
#define R_S A1   // IR sensor Right

#define echo A2    // Echo pin
#define trigger A3 // Trigger pin

#define servo A5

int Set = 20;
int speed = 150;
int distance_L, distance_F, distance_R;

void setup()
{
  Serial.begin(9600); // Start serial communication at 9600bps

  pinMode(R_S, INPUT); // Declare right sensor as input
  pinMode(L_S, INPUT); // Declare left sensor as input

  pinMode(echo, INPUT);   // Declare ultrasonic sensor Echo pin as input
  pinMode(trigger, OUTPUT); // Declare ultrasonic sensor Trigger pin as Output

  pinMode(enA, OUTPUT); // Declare as output for L298 Pin enA
  pinMode(in1, OUTPUT); // Declare as output for L298 Pin in1
  pinMode(in2, OUTPUT); // Declare as output for L298 Pin in2
  pinMode(in3, OUTPUT); // Declare as output for L298 Pin in3
  pinMode(in4, OUTPUT); // Declare as output for L298 Pin in4
  pinMode(enB, OUTPUT); // Declare as output for L298 Pin enB

  analogWrite(enA, speed); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed
  analogWrite(enB, speed); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed

  pinMode(servo, OUTPUT);
  
  for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
 for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }
 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }

  distance_F = Ultrasonic_read();
  Serial.println("distance_F: "+distance_F);
  delay(500);
}

void loop()
{
distance_F = Ultrasonic_read();
Serial.print("Ultrasonics read=");Serial.println(distance_F);

  if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 1))
  {
    if (distance_F > Set)
    {
      forward();
    }
    else{
      Check_side();
    }
  }
  else if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 0))
  {
    turnRight();
  }
  else if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 1))
  {
    turnLeft();
  }

  delay(10);
}

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(50); // Refresh cycle of servo
}


//**********************Ultrasonic_read****************************
long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn (echo, HIGH);
  return time / 29 / 2;
}

void compareDistance(){
    if(distance_L > distance_R){
      turnLeft();
      delay(700);
      forward();
      delay(600);
      turnRight();
      delay(500);
      forward();
      delay(600);
      turnRight();
      delay(400);
    }else{
      turnRight();
      delay(700);
      forward();
      delay(600);
      turnLeft();
      delay(500);
      forward();
      delay(600);  
      turnLeft();
      delay(400);
  }
}

void Check_side(){
    Stop();
    Serial.println("Check_side");
    delay(100);
 for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_R = Ultrasonic_read();
    Serial.print("D R=");Serial.println(distance_R);
    delay(100);
  for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_L = Ultrasonic_read();
    Serial.print("D L=");Serial.println(distance_L);
    delay(100);
 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    compareDistance();
}

void forward()
{
  digitalWrite(in1, LOW);  // Left Motor backward Pin 
  digitalWrite(in2, HIGH); // Left Motor forward Pin 
  digitalWrite(in3, LOW);  // Right Motor forward Pin 
  digitalWrite(in4, HIGH); // Right Motor backward Pin 
  Serial.println("forward");
}

void backward()
{
  // Move backward
  digitalWrite(in1, HIGH); // Left Motor backward Pin 
  digitalWrite(in2, LOW);  // Left Motor forward Pin 
  digitalWrite(in3, HIGH); // Right Motor forward Pin 
  digitalWrite(in4, LOW);  // Right Motor backward Pin  
  Serial.println("backward");
}

void turnRight()
{
  digitalWrite(in1, HIGH); //Left Motor backward Pin 
  digitalWrite(in2, LOW); //Left Motor forward Pin 
  digitalWrite(in3, LOW); //Right Motor forward Pin 
  digitalWrite(in4, HIGH); //Right Motor backward Pin
  Serial.println("Turn Right");
}

void turnLeft()
{
  digitalWrite(in1, LOW);  // Left Motor backward Pin
  digitalWrite(in2, HIGH); // Left Motor forward Pin
  digitalWrite(in3, HIGH); // Right Motor forward Pin
  digitalWrite(in4, LOW);  // Right Motor backward Pin
  Serial.println("Turn Left");
}

void Stop()
{
  digitalWrite(in1, LOW); // Left Motor backward Pin
  digitalWrite(in2, LOW); // Left Motor forward Pin
  digitalWrite(in3, LOW); // Right Motor forward Pin
  digitalWrite(in4, LOW); // Right Motor backward Pin
  Serial.println("Stop");
}
