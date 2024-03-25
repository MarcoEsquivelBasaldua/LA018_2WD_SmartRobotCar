#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"

// DDR
PID_ks const left_pid_K = {0.1, 0.55, 0.0};
PID_ks const right_pid_K = {0.09, 0.3, 0.0};
PID_vars left_pid_vars = {left_pid_K, 0.0, 0, 0, 0};
PID_vars right_pid_vars = {right_pid_K, 0.0, 0, 0, 0};
uint8 const ins[] = {11, 10, 9, 6};
uint8 const interrupts[] = {2, 3};

Wheel LEFTWHEEL = {left_pid_vars,
                   ins[0], ins[1],
                   interrupts[1],
                   STOP_RPM};
Wheel RIGHTWHEEL = {right_pid_vars,
                    ins[2], ins[3],
                    interrupts[0],
                    STOP_RPM};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);

void setup() {
  // put your setup code here, to run once:
  
  //ddrInit(&ddr);
  Serial.begin(9600);

}

void loop() {
  ddr.forward(65);
  uint8 rightRPM = ddr.rightWheel.u_velRPM;
  uint8 leftRPM = ddr.leftWheel.u_velRPM;
  Serial.print(leftRPM);
  Serial.print(",");
  Serial.println(rightRPM);

  delay(100);
}

