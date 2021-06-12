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

Thread t1, t2, t3, t4;
Timer t;


float send_angle = 0.0;
int cnt = 0;
int done = 0;
int type = 0;
int kind = 2;
int la = 0;
char str1[45];
char str2[20];
int modify = 0, flag = 0;

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

void PPPping(void)
{
   	float val;
//   	pc.set_baud(9600);
	xbee.set_baud(9600);
	char buff[25] = {0};
   	
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
//     	printf("Ping = %lf\r\n", val*17700.4f);
		sprintf(buff, "dist %.2lf angle %.2f\r\n", val*17700.4f, send_angle);
//		printf(buff);
		xbee.write(buff, sizeof(buff));
      	t.stop();
      	t.reset();

      	ThisThread::sleep_for(1s);
   	}

}


void april()
{
	float dist, angle, Tx, Tz, tmp_angle, tTx, tTz;
	int delay = 0, go = 0, d = 0, st = 0, der = 1;
	int sign = 1;

/*
	car.goStraight(0);	
	ThisThread::sleep_for(500ms);	

		Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
//		angle = 100 * (int(str1[24]) - 48) + 10 * (int(str1[25]) - 48) + (int(str1[26]) - 48) + 0.1 * (int(str1[28]) - 48) + 0.01 * (int(str1[29]) - 48);
//		dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);
		tTx = Tx;
		tTz = Tz;
		car.goStraight(50);
		ThisThread::sleep_for(1000ms);
		Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		if (((Tx - tTx) <= 1 || (Tx - tTx) >= -1) && ((Tz - tTz) <= 0.5 || (Tz - tTz) >= -0.5)) st = 1;
		else 
			go = 1;
//			st = 0;
		

		car.goStraight(-50);
		ThisThread::sleep_for(1000ms);
		car.stop();
		ThisThread::sleep_for(1000ms);
*/

	while (1) {
		
		if (str1[4] == '-') sign = -1;
		if (str1[4] == '0') sign = 1;
		Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		angle = 100 * (int(str1[24]) - 48) + 10 * (int(str1[25]) - 48) + (int(str1[26]) - 48) + 0.1 * (int(str1[28]) - 48) + 0.01 * (int(str1[29]) - 48);
		dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);
//		printf("%.2f %.2f %.2f %.2f\n", Tx, Tz, dist, angle);
		if ((kind == 3 || kind == 1) && !modify) {

			ThisThread::sleep_for(1000ms);
			if (str1[4] == '-') sign = -1;
			if (str1[4] == '0') sign = 1;
			Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
			printf("%d\n",kind);
			if (Tx >= 0) {
				
				while ((Tx <= 1 && Tx >= -1) == false) {

					car.turn_new(20, 20, 0.1, 1);
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
					car.turn_new(20, 20, 1, 0.1);
					ThisThread::sleep_for(50ms);
					if (str1[4] == '-') sign = -1;
					if (str1[4] == '0') sign = 1;
					Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
				}
				modify = 1;
				car.stop();
				ThisThread::sleep_for(50ms);

			}
			printf("modify done!\n");
					if (str1[4] == '-') sign = -1;
					if (str1[4] == '0') sign = 1;
					Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));

			printf("%.2f %.2f %.2f %.2f\n", Tx, Tz, dist, angle);
			ThisThread::sleep_for(500ms);
			dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);
			printf("dist : %.2f\n", dist);
			if (dist >= 10) {
				steps = 0;
				last = 0;
				car.turn_new(50,50,0.85,1);
			    while(steps*6.5*3.14/32 < (dist - 10 - 30)) {
        			// printf("encoder = %d\r\n", steps);
        			ThisThread::sleep_for(100ms);			
				}	
				car.stop();
				modify = 2;
				printf("dist done!\n");
/*				
		if (str1[4] == '-') sign = -1;
		if (str1[4] == '0') sign = 1;
		Tx = sign * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		angle = 100 * (int(str1[24]) - 48) + 10 * (int(str1[25]) - 48) + (int(str1[26]) - 48) + 0.1 * (int(str1[28]) - 48) + 0.01 * (int(str1[29]) - 48);
		dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);
		printf("%.2f %.2f %.2f %.2f\n", Tx, Tz, dist, angle);
*/
				ThisThread::sleep_for(50ms);
			}
		
		}

		if (kind == 4) {
			if (str1[43] == '0') {
				car.turn_new(100,100,1,0.1);
				ThisThread::sleep_for(500ms);
				car.stop();
				modify = 3;
				done = 1;
				ThisThread::sleep_for(50ms);
			}
			if (str1[43] == '4' || str1[43] == '5') {
				ThisThread::sleep_for(500ms);
				if (angle > 300) {
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
				} 		
				else {
					delay = int((90 - angle)/90*1.0*1000);
					car.turn_new(100, 100, 0.1, 1);

					while (d < delay - 300) {
						d += 100;
						ThisThread::sleep_for(100ms);
					}
					

					car.stop();
					ThisThread::sleep_for(1000ms);
					car.turn_new(100, 100, 1, 0.1);

					ThisThread::sleep_for(1200ms);

					car.stop();
					ThisThread::sleep_for(50ms);					
				} 		
				car.stop();
				if (str1[43] == '5')
					modify = 6;
				else 
					modify = 4;
				
				done = 1;
				ThisThread::sleep_for(50ms);
			}
		}
			if (kind == 5) {
				ThisThread::sleep_for(500ms);
				flag = 1;
				car.turn_new(-50,-50,1,0.87);
				while (dist <= 80) {
					ThisThread::sleep_for(100ms);
					dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);	
				}
				car.stop();
				ThisThread::sleep_for(100ms);
				
				car.turn_new(100,100,0.1,1);
				ThisThread::sleep_for(1400ms);
				car.stop();
				ThisThread::sleep_for(50ms);
				modify = 5;
			}

		

		ThisThread::sleep_for(100ms);

	}
/*
	while (1) {
	if (kind == 3 || kind == 1) {
	go = 1;
	printf("%d\n",kind);		
		der = 1;
		Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		angle = 100 * (int(str1[24]) - 48) + 10 * (int(str1[25]) - 48) + (int(str1[26]) - 48) + 0.1 * (int(str1[28]) - 48) + 0.01 * (int(str1[29]) - 48);
		dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);
		printf("%.2f %.2f %.2f %.2f\n", Tx, Tz, dist, angle);
//		printf("%c %d\n", str1[0], go);

	if (st && str1[0] == 'A') {
		if ((Tx <= 1 || Tx >= -1) && (angle <= 5 || angle >= 355)) {
			car.goStraight(100);
	    	while(steps*6.5*3.14/32 < (-1*Tz*6.2-15)) {
        	// printf("encoder = %d\r\n", steps);
        	ThisThread::sleep_for(100ms);
    		}	
			car.stop();
			ThisThread::sleep_for(100ms);
			st = 0;	
		}
		else
			go = 1;
	}

	if (go && str1[0] == 'A' && dist >= 20) {
		d = 0;
		
		if (angle > 300) {
			last = 1;
			delay = int((90 - (360 - angle))/90*1.0*1000);
			car.turn_new(100, 100, 1, 0.1);

			while (d < delay - 100) {
				d += 100;
				ThisThread::sleep_for(100ms);
			}
			if (delay <= 600) der = 2;

			car.stop();
			ThisThread::sleep_for(1000ms);
			car.turn_new(100, 100, 0.1, 1);

			ThisThread::sleep_for(1000ms / der + 50ms);

			car.stop();
			ThisThread::sleep_for(100ms);
		}
		else {
			last = 2;
			delay = int((90 - angle)/90*1.0*1000);
			car.turn_new(100, 100, 0.1, 1);
			
			while (d < delay ) {
				d += 100;
				ThisThread::sleep_for(100ms);
			}
			if (delay <= 550) der = 2;
			car.stop();
			ThisThread::sleep_for(1000ms);
			car.turn_new(100, 100, 1, 0.1);

			ThisThread::sleep_for(900ms / der - 50ms);

			car.stop();
			ThisThread::sleep_for(100ms);		
		}
		
	}
		ThisThread::sleep_for(1000ms);
		Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		angle = 100 * (int(str1[24]) - 48) + 10 * (int(str1[25]) - 48) + (int(str1[26]) - 48) + 0.1 * (int(str1[28]) - 48) + 0.01 * (int(str1[29]) - 48);
		dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);		
		ThisThread::sleep_for(1000ms);
		printf("%.2f %.2f %.2f %.2f\n", Tx, Tz, dist, angle);
		if ((Tx <= 1 || Tx >= -1) && (angle <= 5 || angle >= 355) && !done) {
			go = 0;
			modify = 1;
			done = 1;
			send_angle = angle;
			printf("done\n");
			if (dist >= 20) {
				car.goStraight(50);
			    while(steps*6.5*3.14/32 < 10) {
        		// printf("encoder = %d\r\n", steps);
        		ThisThread::sleep_for(100ms);
    			}			
			}
//			printf("%.2f %.2f %.2f %.2f\n", Tx, Tz, dist, angle);
			car.stop();
			ThisThread::sleep_for(1000ms);

		}

		if (modify && str1[0] == 'A' && !done) {
			printf("modify\n");
			Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
//			printf("%.2f\n", Tx);
			if ((last == 1 && Tx > 0) || (last == 2 && Tx > 0)) {
//				Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));		
				while (!((Tx <= 0.6 || Tx >= -0.6) /*&& str1[0] == 'A')) {
					Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
					car.turn_new(20, 20, 0.1, 1);
					ThisThread::sleep_for(100ms);
				}
				car.stop();
				ThisThread::sleep_for(200ms);
				modify = 0;
				go = 0;
			}
			if ((last == 1 && Tx < 0) || (last == 2 && Tx < 0)) {
//				Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));		
				while (!((Tx <= 0.6 || Tx >= -0.6) /*&& str1[0] == 'A')) {
					Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
					car.turn_new(20, 20, 1, 0.1);
					ThisThread::sleep_for(100ms);
				}
				car.stop();
				ThisThread::sleep_for(200ms);
				modify = 0;
				go = 0;
			}
		}
		ThisThread::sleep_for(1000ms);
		if ((!done && go && str1[0] == 'B' && dist >= 20) || (!modify  && str1[0] == 'B')){
			printf("lost\n");
			Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));	
			printf("%.2f\n",Tx);	
			if ((last == 1 && Tx >= 0) ||(last == 2 && Tx >= 0)) {
				car.goStraight(-100);
				ThisThread::sleep_for(500ms);
				while (!(str1[0] == 'A')) {
					car.turn_new(30, 30, 0.1, 1);
					ThisThread::sleep_for(200ms);
				}
				car.stop();
				ThisThread::sleep_for(1000ms);
			}
			if ((last == 1 && Tx < 0) || (last == 2 && Tx < 0)) {
				car.goStraight(-100);
				ThisThread::sleep_for(700ms);
				while (!(str1[0] == 'A')) {
					car.turn_new(30, 30, 1, 0.1);
					ThisThread::sleep_for(200ms);
				}
				car.stop();
				ThisThread::sleep_for(1000ms);
			}
		}

		
	}
		ThisThread::sleep_for(100ms);
	}
*/

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
	printf("%d\n",kind);
	if (str2[0] == 'A') {
		x1 = 100 * (int(str2[1]) - 48) + 10 * (int(str2[2]) - 48) + (int(str2[3]) - 48);
		y1 = 100 * (int(str2[5]) - 48) + 10 * (int(str2[6]) - 48) + (int(str2[7]) - 48);
		x2 = 100 * (int(str2[9]) - 48) + 10 * (int(str2[10]) - 48) + (int(str2[11]) - 48);
		y2 = 100 * (int(str2[13]) - 48) + 10 * (int(str2[14]) - 48) + (int(str2[15]) - 48);		
//		printf("%d %d %d %d\n", x1, y1, x2, y2);
//		ThisThread::sleep_for(50ms);
//		car.goStraight(40);
//   		while(steps*6.5*3.14/32 < 8) {
//			printf("going\n");
//      		ThisThread::sleep_for(100ms);
//   		}

		delta = x2 - x1;
		sign = 1;
		if (delta < 0) { 
			delta = -delta;
			sign = -1;
		}
		if (sign == -1) {
			if (delta <= 10) delta *= 1;			
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


void control_center()
{
	
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
		if (modify == 5) 
			done = 0;
		

			
		printf("last : %d kind : %d\n", la, kind);
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




