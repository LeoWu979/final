#include"mbed.h"
#include "bbcar.h"


BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BufferedSerial xbee(D10, D9);


Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);
DigitalInOut ping(D12);
volatile int steps;
volatile int last;
BBCar car(pin5, pin6, servo_ticker);

Thread t1, t2, t3, t4, t5;
Timer t;

int cnt = 0;
int done = 0;
int type = 0;
int kind = 2;
int la = 0;
char str1[47];
char str2[20];
float ppp = 0.0;
int modify = 0, flag = 0, back = 0, flag1;

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

void PPPping(void)
{
   	float val;
	   	
	while(1) {
		
    	ping.output();
      	ping = 0; wait_us(200);
      	ping = 1; wait_us(5);
      	ping = 0; wait_us(5);

      	ping.input();
      	while(ping.read() == 0);
      	t.start();
      	while(ping.read() == 1);
      	val = t.read();
		ppp = val*17700.4f;

      	t.stop();
      	t.reset();

      	ThisThread::sleep_for(100ms);
   	}

}


void april()
{
	float dist, angle, Tx, Tz, tmp_angle, tTx, tTz, c_angle;
	int delay = 0, go = 0, d = 0, st = 0, der = 10;
	int sign = 1;


	while (1) {
		
		if (str1[4] == '-') sign = -1;
		if (str1[4] == '0') sign = 1;
		Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		angle = 100 * (int(str1[24]) - 48) + 10 * (int(str1[25]) - 48) + (int(str1[26]) - 48) + 0.1 * (int(str1[28]) - 48) + 0.01 * (int(str1[29]) - 48);
		dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);

		if ((kind == 3 || kind == 1) && !modify && str1[43] != '0') {

			ThisThread::sleep_for(1000ms);
			if (str1[4] == '-') sign = -1;
			if (str1[4] == '0') sign = 1;
			Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));

			if (Tx >= 0) {
				
				while ((Tx <= 1 && Tx >= -1) == false) {

					car.turn_new(15, 15, 0.1, 1);
					ThisThread::sleep_for(50ms);
					if (str1[4] == '-') sign = -1;
					if (str1[4] == '0') sign = 1;
					Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
				}
				modify = 1;
				car.stop();
				ThisThread::sleep_for(50ms);
			}			
			else {
				while ((Tx <= 1 && Tx >= -1) == false) {
					car.turn_new(15, 15, 1, 0.1);
					ThisThread::sleep_for(50ms);
					if (str1[4] == '-') sign = -1;
					if (str1[4] == '0') sign = 1;
					Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
				}
				modify = 1;
				car.stop();
				ThisThread::sleep_for(50ms);

			}

				if (str1[4] == '-') sign = -1;
				if (str1[4] == '0') sign = 1;
				Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));


				ThisThread::sleep_for(500ms);
				dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);
		

			if (dist >= 10) {
				steps = 0;
				last = 0;
				car.turn_new(50,50,0.87,1);
			    while(steps*6.5*3.14/32 < (dist - 10 - 30 - der)) {
        			// printf("encoder = %d\r\n", steps);
        			ThisThread::sleep_for(100ms);			
				}	

				car.stop();
				modify = 2;

				der = 10;

				ThisThread::sleep_for(50ms);
			}
		
		}
		if ((kind == 3 || kind == 1) && !modify && str1[43] == '0') {
			Tz = 1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
			dist = Tz * 6.2;
			while (dist >= 40) {
				car.turn_new(50,50,0.93,1);	
				ThisThread::sleep_for(50ms);
				dist = 6.2 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));	
			}
			car.stop();
			modify = 2;
			ThisThread::sleep_for(50ms);			
		}

		if (kind == 4) {
			if (str1[43] == '0' && !flag1) {
				car.stop();
				ThisThread::sleep_for(100ms);
				car.turn_new(100,100,1,0.1);
				ThisThread::sleep_for(750ms);
				
				car.stop();
//				ThisThread::sleep_for(100ms);
				done = 1;
				modify = 3;
				flag1 = 1;
			}
			if (str1[43] == '4') {
				d = 0;
				ThisThread::sleep_for(50ms);
				c_angle = angle;
				if (angle > 300) c_angle = 360 - angle;
				if (str1[4] == '-') sign = -1;
				if (str1[4] == '0') sign = 1;
				Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));

					delay = int((90 - (360 - angle))/90*1.0*1000);
					car.turn_new(100, 100, 1, 0.1);

					while (d < delay - 300) {
						d += 100;
						ThisThread::sleep_for(100ms);
					}
					

					car.stop();
					ThisThread::sleep_for(1500ms);
					car.turn_new(100, 100, 0.1, 1);

					ThisThread::sleep_for(1150ms);

					car.stop();
					ThisThread::sleep_for(50ms);					
						

				modify = 4;
				done = 1;
			}		
				
			if (str1[43] == '5' && modify == 2)
				modify = 6;
			
		}
			if (kind == 5) {
				steps = 0;
				last = 0;
				ThisThread::sleep_for(500ms);
				flag = 1;
				car.turn_new(-50,-50,1,0.87);

				while(steps*6.5*3.14/32 <= 90) {
        			// printf("encoder = %d\r\n", steps);
        			ThisThread::sleep_for(50ms);			
				}	
				car.stop();
				ThisThread::sleep_for(100ms);
				
				car.turn_new(100,100,0.1,1);
				ThisThread::sleep_for(1400ms);
				car.stop();
				modify = 5;
				back = 1;
				ThisThread::sleep_for(50ms);
				
			}
			if (kind == 8 && modify != 9) {
				steps = 0;
				last = 0;
				car.turn_new(50,50,0.87,1);
				ThisThread::sleep_for(500ms);
				car.stop();
				ThisThread::sleep_for(100ms);
				car.turn_new(100,100,1,0.1);
				ThisThread::sleep_for(1150ms);
				car.stop();
				ThisThread::sleep_for(50ms);
				
				car.turn_new(50,50,0.87,1);
				while(steps*6.5*3.14/32 <= 27) {
        			// printf("encoder = %d\r\n", steps);
        			ThisThread::sleep_for(50ms);			
				}
				car.stop();
				modify = 9;
				ThisThread::sleep_for(50ms);

			}

		

		ThisThread::sleep_for(100ms);

	}

}


void follow_line()
{
	int x1, y1 ,x2, y2, delta = 0, sign = 1, modify = 0;
	char temp[3];
	encoder_ticker.attach(&encoder_control, 10ms);
	steps = 0;
	last = 0;
	car.stop();

while (1) {
if (kind == 2) {
	ThisThread::sleep_for(500ms);

	if (str2[0] == 'A') {
		x1 = 100 * (int(str2[1]) - 48) + 10 * (int(str2[2]) - 48) + (int(str2[3]) - 48);
		y1 = 100 * (int(str2[5]) - 48) + 10 * (int(str2[6]) - 48) + (int(str2[7]) - 48);
		x2 = 100 * (int(str2[9]) - 48) + 10 * (int(str2[10]) - 48) + (int(str2[11]) - 48);
		y2 = 100 * (int(str2[13]) - 48) + 10 * (int(str2[14]) - 48) + (int(str2[15]) - 48);		


		delta = x2 - x1;
		sign = 1;
		if (delta < 0) { 
			delta = -delta;
			sign = -1;
		}
		if (sign == -1) {
			if (delta <= 10) delta *= 1.5;		
			if (delta > 10) delta /= 1.5;	
			car.turn_new(30,30+delta,1, 1);
			ThisThread::sleep_for(50ms);
		}	
		else {		
			car.turn_new(30+delta/4,30, 1 ,1);
			ThisThread::sleep_for(50ms);
		}
	
	}
	if (str2[0] == 'B') {
		car.stop();
		ThisThread::sleep_for(50ms);
	}
}
	ThisThread::sleep_for(50ms);
}

}

void parking()
{
	int x1 = 0, y1 = 0, dist = 0, sign = 1;
	float Tx = 0.0, Tz = 0.0;
	char buff[15] = {0};

while (1) {
	if (kind == 6) {
		ThisThread::sleep_for(1000ms);
		car.turn_new(100,100,0.1,1);
		ThisThread::sleep_for(1300ms);
		car.stop();
		ThisThread::sleep_for(1000ms);
		y1 = ppp;
		sprintf(buff, "Distance %.1f\r\n", y1);
		xbee.write(buff,sizeof(buff));
		ThisThread::sleep_for(100ms);
		car.turn_new(-100,-100,0.1,1);
		ThisThread::sleep_for(1000ms);
		car.stop();			

		Tz = 1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		x1 = 30 - Tz;

		ThisThread::sleep_for(1000ms);
		modify = 7;
	}

if (kind == 7) {

	ThisThread::sleep_for(500ms);
			if (str1[4] == '-') sign = -1;
			if (str1[4] == '0') sign = 1;
			Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));

			if (Tx >= 0) {
				
				while ((Tx <= 1 && Tx >= -1) == false) {

					car.turn_new(15, 15, 0.1, 1);
					ThisThread::sleep_for(50ms);
					if (str1[4] == '-') sign = -1;
					if (str1[4] == '0') sign = 1;
					Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
				}
				
				car.stop();
				ThisThread::sleep_for(50ms);
			}			
			else {
				while ((Tx <= 1 && Tx >= -1) == false) {
					car.turn_new(15, 15, 1, 0.1);
					ThisThread::sleep_for(50ms);
					if (str1[4] == '-') sign = -1;
					if (str1[4] == '0') sign = 1;
					Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
				}
				
				car.stop();
				ThisThread::sleep_for(50ms);

			}

	ThisThread::sleep_for(500ms);

    steps = 0;
    last = 0;
    car.turn_new(-30, -30, 1, 0.84);
    x1 = 15;
	y1 = 12;
	while(steps*6.5*3.14/32 < x1 - 3.5) {
        
        ThisThread::sleep_for(100ms);
    }
    car.stop();

    car.turn(-100,0.1);
    ThisThread::sleep_for(900ms);

    car.stop();

    steps = 0;
    last = 0;
    car.turn_new(-30, -30, 1, 0.84);

    while(steps*6.5*3.14/32 < y1 + 18) {
        
        ThisThread::sleep_for(100ms);
    }
    car.stop();
	modify = 8;
	ThisThread::sleep_for(50ms);
}
	ThisThread::sleep_for(100ms);
}


}


void control_center()
{
	char buff[15] = {0};
	int ctt = 0;
	xbee.set_baud(9600);
	while(1) {
		if (str1[0] == 'A' && str2[0] == 'B') {
			if ((la == 2 || la == 0 || la == 5) && !done) {
				kind = 1;
				modify = 0;
			}
		}
		if (str1[0] == 'B' && str2[0] == 'A') {
			if ((la == 1 || la == 0 || la == 3 || la == 4) && done) {
				kind = 2;
				done = 0;
			}
		}
		if (str1[0] == 'A' && str2[0]== 'A') {
			if (la == 2 || la == 0) {
				kind = 3;
				modify = 0;
			}
		}
		if (str1[0] == 'B' && str2[0] == 'B' && done)
			kind = 0;
		if (modify == 2)
			kind = 4;
		if (modify == 4 && !flag) 
			kind = 5;
		if (modify == 5 && back) {
			back = 0;
			done = 0;
			modify = 0;
		}
		if (modify == 6)
			kind = 6;
		if (modify == 7)
			kind = 7;
		if (modify == 8)
			kind = 8;
		
		if (kind == 3 && modify == 0 && la == 2) {
			sprintf(buff, "Line Complete\r\n");
			xbee.write(buff,sizeof(buff));
		} 
		if ((kind == 4 && modify == 2 && la == 1) || (kind == 5 && modify == 4 && la == 1) || (kind == 4 && modify == 3)) {
			sprintf(buff, "Apri Complete\r\n");
			xbee.write(buff,sizeof(buff));
		}
		if (kind == 6 && modify == 5 && la == 5) {
			sprintf(buff, "back Complete\r\n");
			xbee.write(buff,sizeof(buff));
		} 		 		
		if (kind == 7 && modify == 7 && la == 6) {
			sprintf(buff, "dete Complete\r\n");
			xbee.write(buff,sizeof(buff));
		} 
		if (kind == 8 && modify == 8 && la == 7) {
			sprintf(buff, "Park Complete\r\n");
			xbee.write(buff,sizeof(buff));
		} 
		if (modify == 9 && kind != 9) {
			sprintf(buff, "labelnumber %c\r\n", str1[1]);
			xbee.write(buff,sizeof(buff));

			ctt++;
			if (ctt >= 5) {
				kind = 9;
				sprintf(buff, "pred Complete\r\n");
				xbee.write(buff,sizeof(buff));			
			}
		}

			
//		printf("last : %d kind : %d modify : %d flag : %d\n", la, kind, modify, flag);
		la = kind;
		ThisThread::sleep_for(100ms);
	}



}



int main(void) {
	uart.set_baud(9600);

	t1.start(follow_line);
	t2.start(april);
	t3.start(PPPping);
	t4.start(control_center);
	t5.start(parking);
	while(1){
		if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            if (type == 0)
				str1[cnt] = recv[0];
			if (type == 1)
				str2[cnt] = recv[0];
            cnt++;
            if (recv[0] == 'C' && type == 0) {
            	cnt = 0;
				type = 1;
//				printf("str1 : %s\n", str1);
			}
			if (recv[0] == 'E' && type == 1) {
				cnt = 0;
				type = 0;
//				printf("str2 : %s\n", str2);
			}


			
//            pc.write(recv, sizeof(recv));
		}

   	}
}




