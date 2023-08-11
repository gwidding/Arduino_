const int pins[]={2,3,4,5,6,7,8,9};
//set bits at each number
                                   //dp/gfedcba
const int numberCode[10]={B11000000, //0
                    B11111001, //1
                    B10100100, //2
                    B10110000, //3
                    B10011001, //4
                    B10010010, //5
                    B10000010, //6
                    B11011000, //7
                    B10000000, //8
                    B10010000}; //9
 
void setup()
{
  for(int i=0; i<7; i++)
  pinMode(pins[i], OUTPUT);
}
 
void loop()
{
for(int n=0; n<10; n++)
   {//show the number
     showNumber(n);
    //delay
     delay(200);
    }
delay(200);
}
 
void showNumber(int number)
{
  boolean value;
  
  for(int bitPosition=0; bitPosition<8; bitPosition++)
  {
    //if number isn't 0 to 9, all off.
    if(number<0 || number>9)
    value = 0;
    //if number is 0 to 9, 
    else
    //Read bit.
    value = bitRead(numberCode[number], bitPosition);
    
    //and Reading value 0, off. Reading value 1, on.
    digitalWrite(pins[bitPosition], value);
  }
 
  return ;
}