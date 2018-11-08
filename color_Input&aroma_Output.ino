#include <TimerOne.h> 

#define S0    6   
#define S1    5                  
#define S2    4   
#define S3    3
#define OUT   2  

//i take fruit smell/minty smell/medicine smell as basic
//aroma constituents represent Red color/Green colr/Blue color
//respectively
#define FRUIT_SMELL 7
#define MINTY_SMELL 8
#define MEDICINE_SMELL 9


float g_SF[3];     
int   g_count = 0;  
int   g_array[3];  
int   g_flag = 0;

int   r_detected;
int   g_detected;
int   b_detected;

//using Chebyshev distance to judge
//which basic color is closer for the color 
//we have detected.
int   r_distance;
int   g_distance;
int   b_distance;

void TSC_Init()
{
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(FRUIT_SMELL, OUTPUT);
  pinMode(MINTY_SMELL, OUTPUT);
  pinMode(MEDICINE_SMELL, OUTPUT);
  
  digitalWrite(S0, LOW); 
  digitalWrite(S1, HIGH);
  digitalWrite(FRUIT_SMELL, LOW);
  digitalWrite(MINTY_SMELL, LOW);
  digitalWrite(MEDICINE_SMELL, LOW);
}
 
void TSC_FilterColor(int Level01, int Level02)
{
  if(Level01 != 0)
    Level01 = HIGH;
  if(Level02 != 0)
    Level02 = HIGH;
  digitalWrite(S2, Level01);
  digitalWrite(S3, Level02);
}
 

void TSC_Count()
{
  g_count ++ ;
}
 

void TSC_Callback()
{
  switch(g_flag)
  {
    case 0:
         Serial.println("->WB Start");
         TSC_WB(LOW, LOW); 
         break;
    case 1:
         Serial.print("->Frequency R=");
         Serial.println(g_count);   
         g_array[0] = g_count;    
         TSC_WB(HIGH, HIGH);  
         break;
    case 2:
         Serial.print("->Frequency G=");
         Serial.println(g_count);   
         g_array[1] = g_count;    
         TSC_WB(LOW, HIGH);  
         break;
 
    case 3:
         Serial.print("->Frequency B=");
         Serial.println(g_count);   
         Serial.println("->WB End");
         g_array[2] = g_count;     
         TSC_WB(HIGH, LOW);    
         break;
   default:
         g_count = 0;     
         break;
  }
}


void TSC_WB(int Level0, int Level1)    
{
  g_count = 0;   
  g_flag ++;     
  TSC_FilterColor(Level0, Level1); 
  Timer1.setPeriod(1000000);     
}

void setup()
{
  TSC_Init();
  Serial.begin(9600); 
  Timer1.initialize();   
  Timer1.attachInterrupt(TSC_Callback); 
  attachInterrupt(0, TSC_Count, RISING);
  delay(4000); 

  g_SF[0] = 255.0/ g_array[0];     
  g_SF[1] = 255.0/ g_array[1] ;    
  g_SF[2] = 255.0/ g_array[2] ;    

  Serial.println(g_SF[0],5);
  Serial.println(g_SF[1],5);
  Serial.println(g_SF[2],5);
  
  for(int i=0; i<3; i++)
    Serial.println(int(g_array[i] * g_SF[i]));   
}

void loop()
{
   g_flag = 0;
   delay(4000);

     r_detected=int(g_array[0] * g_SF[0]);
     g_detected=int(g_array[1] * g_SF[1]);
     b_detected=int(g_array[2] * g_SF[2]);

     r_distance=abs(255-r_detected)+abs(0-g_detected)+abs(0-b_detected);
     g_distance=abs(0-r_detected)+abs(255-g_detected)+abs(0-b_detected);
     b_distance=abs(0-r_detected)+abs(0-g_detected)+abs(255-b_detected);

  if(r_distance<g_distance){
    if(r_distance<b_distance){
      digitalWrite(FRUIT_SMELL, HIGH);
    }else{
      digitalWrite(MEDICINE_SMELL, HIGH);
    }
  }else{
    if(g_distance<b_distance){
      digitalWrite(MINTY_SMELL, HIGH);
    }else{
      digitalWrite(MEDICINE_SMELL, HIGH);
    }
  }


}

