#include "stm32f10x.h"
#include "Delay.h"
#include "Motor.h"
#include "Sensor.h"

int main(void)
{
    Sensor_Init();
    Motor_Init();

    int base_speed = 160;
    int Kp = 35;

    int error = 0;
    int last_error = 0;

    while(1)
    {
        uint8_t s0 = !Sensor0_Get_State();
        uint8_t s1 = !Sensor1_Get_State();
        uint8_t s2 = !Sensor2_Get_State();
        uint8_t s3 = !Sensor3_Get_State();

        error = (-3)*s0 + (-1)*s1 + (1)*s2 + (3)*s3;

        if(s0==0 && s1==0 && s2==0 && s3==0)
        {
            if(last_error > 0)
                Motor_SetSpeed(100, 200);
            else
                Motor_SetSpeed(200, 100);
            continue;
        }

        last_error = error;

        int left  = base_speed - Kp * error;
        int right = base_speed + Kp * error;

        if(left > 255) left = 255;
        if(left < 0)   left = 0;

        if(right > 255) right = 255;
        if(right < 0)   right = 0;

        Motor_SetSpeed(left, right);
    }
}
