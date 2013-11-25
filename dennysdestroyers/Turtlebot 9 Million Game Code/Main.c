#include <RASLib/inc/common.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/adc.h>

// The 'main' function is the entry point of the program

tMotor *motors[2];
tI2C *bus;
tLineSensor *gls;
int main(void) {
    int error;
		float line[8];
		int x;
		int lastError = 0;
	  float speedR = -0.4; //these work at least up to 0.4
	  float speedL = -0.4;
		float kp = .30; //these might be too high? not sure
		float kd = .3
	;
		float PIDvalue;
		InitializeMCU();
    gls = InitializeGPIOLineSensor(PIN_B5, PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_E0, PIN_C6, PIN_C7);
		motors[0] = InitializeMotor(PIN_B7, PIN_B6, true, false);
		motors[1] = InitializeMotor(PIN_F3, PIN_F2, true, false);
    
    while (1) {
        
      SetMotor(motors[0],speedL);
			SetMotor(motors[1],speedR);
			
			LineSensorReadArray(gls, line);
			
			//here be filtering
			for(x = 0; x < 8; x++)
			{
				if(line[x] > 0.85)
					line[x] = 1;
				else
					line[x] = 0;
			}
			
<<<<<<< HEAD
			error = line[0] * -4 + line[1] * -2 + line[2] * -1 + line[5] + line[6] * 2 + line[7] * 4;
			if(error == 4) //far right sensor only - error = 8
				error += 4;
			if(error == -4)//far left sensor only - error = -8
				error -= 4;
			if(error == 0)
			{
				/*
				Not sure if these if statements will fix our turning problem. The idea is that,
				if the error suddenly drops to zero from a high amount, then it has lost track
				of the line and attempts a sharp turn to find it again. The values here are
				somewhat arbitrary, and may need adjustment. If this doesn't work at all, just
				cut it out and either slow the robot down or try to fix it another way.
				*/
				if(lastError >= 6)
				{
					speedR = -1.0;
					speedL = -0.5;
				}
				else if(lastError <= -6)
				{
					speedR = -0.5;
					speedL = -1.0;
				}
				else
				{
=======
			error = line[0] * -3 + line[1] * -2 + line[2] * -1 + line[5] + line[6] * 2 + line[7] * 3;
			if(error == 0)
			{
>>>>>>> 539d94918917788260088fdbc5a60de977b35f62
				speedR = -.5; //make sure you change the speed here too
				speedL = -.5;
			}
			else
			{
				PIDvalue = error * kp + (error - lastError)/2 * kd;
				speedR -= PIDvalue;
				speedL += PIDvalue;
				if(speedR > 1) //this part prevents motor values overflowing
					speedR = 1;
				if(speedR < -1)
					speedR = -1;
				if(speedL > 1)
					speedL = 1;
				if(speedL < -1)
					speedL = -1;
				lastError = error;
			}
			
			/*switch(error)
			{
				case -5:
					speedL = speedR - .1;
					break;
				case -3:
					speedL = speedR - .067;
					break;
				case -1:
					speedL = speedR - .033;
					break;
				case 1:
					speedL = speedR + .033;
					break;
				case 3:
					speedL = speedR + .067;
					break;
				case 5:
					speedL = speedR + .1;
					break;
				default:
					speedL = speedR;
					break;
			}*/
			/*if(error < 0)
			{
				SetMotor(motors[0],.35);
				SetMotor(motors[1],0);
			}
			else if(error > 0)
			{
				SetMotor(motors[1],.42);
				SetMotor(motors[0],0);
			}
			else
			{
				SetMotor(motors[0],.35);
				SetMotor(motors[1],.35);
			}*/
        
    }
}
