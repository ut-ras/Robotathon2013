#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/i2c.h>
#include <RASLib/inc/common.h>


// The 'main' function is the entry point of the program
void delay(int);
void SetPin(tPin, tBoolean);
tLineSensor *ls;
//tLineSensor *InitializeGPIOLineSensor(PIN_B5, PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_E0, PIN_C6, PIN_C7);

int main(void) {
	double read;
//	tI2C *bus;
	tMotor *motor[2];
	float line[8];
	double error;
	double last_error;
	int i;
	InitializeMCU();
	ls = InitializeGPIOLineSensor(PIN_B5, PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_E0, PIN_C6, PIN_C7);
	motor[0] = InitializeMotor(PIN_C5, PIN_C4, true, false); //right wheel
	motor[1] = InitializeMotor(PIN_B7, PIN_B6, true, false); //left wheel
	SetMotor(motor[0], 0.20); //Good at -0.20
	SetMotor(motor[1], 0.2); //Good at -0.20
	last_error = 1; 
	while(1){ 
		LineSensorReadArray(ls, line);
		for (i = 0; i < 8; i++){
			if (line[i] > 0.3){
				line[i] = 1;
			}
			else{
				line[i] = 0;
			}
		}
		read = line[0] + line[1] + line[2] + line[3] + line[4] + line[5] + line[6] + line[7];
		if (read == 1 & line[0] == 1){
			error = -100;
		}
		else if (read == 1 & line[7] == 1){
			error = 100;
		}
		else{
			error = (-4 * line[0]) + (-3 * line[1]) + (-2 * line[2]) + (1 * line[3]) + (1 * line[4]) + 
			(2 *  line[5]) + (3 * line[6]) + (4 * line[7]);
		}
		if (error > 1 && error < 5){
			SetMotor(motor[1], 0.30); //turn right
			SetMotor(motor[0], 0.00);
		}
		else if (error >= 5 && error < 100){
			SetMotor(motor[1], 0.30); //turn right
			SetMotor(motor[0], 0.00);
		}
		else if (error == 100){
			SetMotor(motor[1], 0.35); //turn right
			SetMotor(motor[0], -0.3);
		}
		else if (error < 0 && error >-3){
			SetMotor(motor[0], 0.30); //turn left
			SetMotor(motor[1], 0.00);
		}
		else if (error <= -3 && error >-100){
			SetMotor(motor[0], 0.40); //turn left
			SetMotor(motor[1], 0.00);
		}
		else if (error == -100){
			SetMotor(motor[0], 0.35); //turn left
			SetMotor(motor[1], -0.3);
		}
//**************ERROR IS 0******************************************
		else if (error == 0){
			while (error == 0) {
				LineSensorReadArray(ls, line);
				for (i = 0; i < 8; i++){
					if (line[i] > 0.3){
						line[i] = 1;
					}
					else{
						line[i] = 0;
					}
				}
				read = line[0] + line[1] + line[2] + line[3] + line[4] + line[5] + line[6] + line[7];
				if (read == 1 & line[0] == 1){
					error = -100;
				}
				else if (read == 1 & line[7] == 1){
					error = 100;
				}
				else{
					error = (-4 * line[0]) + (-3 * line[1]) + (-2 * line[2]) + (1 * line[3]) + (1 * line[4]) + 
									(2 *  line[5]) + (3 * line[6]) + (4 * line[7]);
				}
				if (last_error > 1 && last_error < 5){
					SetMotor(motor[1], 0.30); //turn right
					SetMotor(motor[0], 0.00);
				}
				else if (last_error >= 5 && last_error < 100){
					SetMotor(motor[1], 0.30); //turn right
					SetMotor(motor[0], 0.00);
				}
				else if (last_error == 100){
					SetMotor(motor[1], 0.35); //turn right
					SetMotor(motor[0], -0.3);
				}
				else if (last_error < 0 && last_error >-3){
					SetMotor(motor[0], 0.30); //turn left
					SetMotor(motor[1], 0.00);
				}
				else if (last_error <= -3 && last_error >-100){
					SetMotor(motor[0], 0.40); //turn left
					SetMotor(motor[1], 0.00);
				}
				else if (last_error == -100){
					SetMotor(motor[0], 0.35); //turn left
					SetMotor(motor[1], -0.3);
				}
				else{
					SetMotor(motor[0], 0.30);
					SetMotor(motor[1], 0.30);
				}
			}
		}
//*************************************************************
		else{
			SetMotor(motor[0], 0.30);
			SetMotor(motor[1], 0.30);
		}
		last_error = error;
	}
}
