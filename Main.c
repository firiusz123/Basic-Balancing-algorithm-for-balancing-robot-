#include <hFramework.h>
#include <Hitechnic_Accel.h>
#include <math.h>

#define PI 3.14159265

int16_t x, y, z;
float power,actuall_degree,error_degree_Kp,error_degree,Ki_error,Kd_error ;
float offset_degree;
///////////////////////////////////////
//PID parameters
float Kp =12.5 ;
float Ki= 0.00004;
float Kd =5;
 ;
float Multi = 1;
//////////////////////////////////////

float degree(int x,int z)
{

    int v0[]={199,-8};
    int v1[]={x,z};                    //{156,120};

    float cosine, result , denominator ,lenght1 ,
    lenght2,dot_product,power1,power2;

    power1=pow(v0[0],2)+pow(v0[1],2);
    //printf("%f\n",power1);
    power2=pow(v1[0],2)+pow(v1[1],2);
    // printf("%f\n",power2);
    lenght1=sqrt(power1);
    lenght2=sqrt(power2);

    dot_product=v0[0]*v1[0]+v0[1]*v1[1];
    //printf("%f\n",dot_product);

    denominator=lenght1*lenght2;

    cosine = dot_product/denominator;
    result=acos(cosine)*180/PI;
    if(v1[1]<0)
    {result=result*(-1);}
    printf("%f what degree do we have here ? \n",result);
    printf("%d i %d to x i z  \n",x,z);
 return result;

}
using namespace hSensor
void acc(void)
{
    int16_t xOffset, yOffset, zOffset;
    xOffset = 200;
    yOffset = 6;
    zOffset = -12;

    //sys.setLogDev(&Serial);
    hLegoSensor_i2c ls(hSens1);
    Hitechnic_Accel sensor(ls);

    for (;;)
    {
        int16_t  lenght1, lenght2, dot_product;
        sensor.readRaw(x, y, z);
        //trying to calculate degree of this piece of crap
        //////////////////////////////////////////////
        lenght1 = sqrt(pow((xOffset), 2) + pow((zOffset), 2));
        //printf("%d\n", lenght1);
         actuall_degree = degree(x,z);
        //printf("%f\n", actuall_degree);
        /////////////////////////////////////////////
        //x=x-xOffset;
        //y=y-yOffset;
        //z=z-zOffset;
        LED1.toggle();
        //printf("x %5d y %5d z %5d\r\n", x, y, z);
        //sys.delay_ms(1000);
    }
}
void calculations()
{   float last_error = 0 ;
    Ki_error=0;
    Kd_error=0;
    offset_degree=0; // so the basic degree that robot tends to is 0
    for(;;)
    {
        error_degree_Kp = offset_degree - actuall_degree ;
        error_degree_Kp=error_degree_Kp*1.2;
        if(last_error >= 0 && error_degree_Kp <=0){Ki_error=0;}
        else if(last_error <= 0 && error_degree_Kp >=0){Ki_error=0;}

        Ki_error= Ki_error + error_degree_Kp ;
        Kd_error=error_degree-last_error;

        last_error=error_degree_Kp;

        power = ((error_degree_Kp*Kp)+(Ki_error*Ki)+(Kd_error*Kd))*Multi;
        if (power > 1000){power=1000;}
        else if (power < -1000){power=-1000;}

    }

}
void motors()
{
    for(;;)
    {
        //printf("distance: %d\n",dist);
        //sys.delay_ms(1000);
        hMot1.setPower(power);  // set hMot1 power to 50%
        hMot4.setPower(-power);
        //sys.delay(1000);

    }

void encoder()
{
    while (true)
    {
      Serial.printf("pos: %d\r\n", hMot1.getEncoderCnt()); // print the current position of Motor 1 (no. of encoder ticks)
      Serial.printf("pos: %d\r\n", hMot4.getEncoderCnt());
        hLED1.toggle();
        printf("power is equal to %f\n",power);
        sys.delay(1);

    }
}
void hMain(void)
{
    hMot1.setEncoderPolarity(Polarity::Reversed);  //changing encoder polarity (Polarity::Normal is default)
    hMot4.setEncoderPolarity(Polarity::Reversed);  //changing encoder polarity (Polarity::Normal is default)
    sys.taskCreate(&acc);
    sys.taskCreate(&calculations);
    sys.taskCreate(&motors);

    //sys.taskCreate(&encoder);
}


//hMot1.setEncoderPolarity(Polarity::Reversed);  //changing encoder polarity (Polarity::Normal is default)
//hMot1.setMotorPolarity(Polarity::Normal); //changing motor polarity
//hMot4.setEncoderPolarity(Polarity::Reversed);  //changing encoder polarity (Polarity::Normal is default)
//hMot4.setMotorPolarity(Polarity::Normal);
    //sys.taskCreate(encoder); // this creates a task that will execute `encoder` concurrently
    //hMot1.rotRel(720, 200, false, INFINITE);
    //hMot4.rotRel(-720, 200, false, INFINITE);


















  }
