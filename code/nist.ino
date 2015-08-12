#define P10A 14
#define P10C 15
#define P9A 16
#define P9C 17
#define P8A 18
#define P8C 19
#define P7A 20
#define P7C 21


#define P5A 22
#define P5C 23
#define P4A 24
#define P4C 25
#define P3A 26
#define P3C 27

//set bi directional buffer to output
void set_parallel_write(void){
  digitalWrite(P4A,LOW); //set DIR high
  digitalWrite(P3A,LOW); //set OE low
  pinMode(P10A,OUTPUT);
  pinMode(P10C,OUTPUT);
  pinMode(P9A,OUTPUT);
  pinMode(P9C,OUTPUT);
  pinMode(P8A,OUTPUT);
  pinMode(P8C,OUTPUT);
  pinMode(P7A,OUTPUT);
  pinMode(P7C,OUTPUT);
}


//set bi directional buffer to input
void set_parallel_read(void){
  digitalWrite(P4A,HIGH); //set DIR low
  digitalWrite(P3A,LOW); //set OE low
  pinMode(P10A,INPUT);
  pinMode(P10C,INPUT);
  pinMode(P9A,INPUT);
  pinMode(P9C,INPUT);
  pinMode(P8A,INPUT);
  pinMode(P8C,INPUT);
  pinMode(P7A,INPUT);
  pinMode(P7C,INPUT);
}


//turn off bidir buffer output
void set_parallel_off(void){
  digitalWrite(P3A,HIGH);  //set OE high
}

void set_parallel(int c){
  set_parallel_write();
  digitalWrite(P10A,c % 2);
  digitalWrite(P10C,(c / 2) % 2);
  digitalWrite(P9A,(c / 4) % 2);
  digitalWrite(P9C,(c / 8) % 2);
  digitalWrite(P8A,(c / 16) % 2);
  digitalWrite(P8C,(c / 32) % 2);
  digitalWrite(P7A,(c / 64) % 2);
  digitalWrite(P7C,(c / 128) % 2);
}

// read bi dir buffer
int read_parallel(void){
  set_parallel_read();
  int ret = digitalRead(P10A) \
            + (digitalRead(P10C) << 1) \
            + (digitalRead(P9A) << 2) \
            + (digitalRead(P9C) << 3) \
            + (digitalRead(P8A) << 4) \
            + (digitalRead(P8C) << 5) \
            + (digitalRead(P7A) << 6) \
            + (digitalRead(P7C) << 7);
  return ret;
}

// set the binary counter to c
void set_counter(int c){
  set_parallel_write();
  set_parallel(c);
  
  digitalWrite(P4C,LOW);  // toggle LOAD
  digitalWrite(P4C,HIGH);
  clock();
  digitalWrite(P4C,LOW);
  
  set_parallel_off();
}

// read current value of binary counter
int read_counter(void){
  set_parallel_read();
  digitalWrite(P5C, LOW);
  digitalWrite(P4C,HIGH);
  int ret = read_parallel();
  digitalWrite(P4C,LOW);
  return ret;
}

// clock the binary counter
void clock(){
  set_parallel_write();
  digitalWrite(P5C,LOW);   // toggle clock
  digitalWrite(P5C,HIGH);
  set_parallel_off();
}
  
  
void setup(){
  Serial.begin(9600);
  
  pinMode(P5A,OUTPUT);
  pinMode(P5C,OUTPUT);
  pinMode(P4A,OUTPUT);
  pinMode(P4C,OUTPUT);
  pinMode(P3A,OUTPUT);
  pinMode(P3C,OUTPUT);

  digitalWrite(P5A,LOW);
  digitalWrite(P5C,LOW);
  digitalWrite(P4A,LOW);
  digitalWrite(P4C,LOW);
  digitalWrite(P3A,LOW);
  digitalWrite(P3C,HIGH);
  
  digitalWrite(P10A,LOW);
  digitalWrite(P10C,LOW);
  digitalWrite(P9A,LOW);
  digitalWrite(P9C,LOW);
  digitalWrite(P8A,LOW);
  digitalWrite(P8C,LOW);
  digitalWrite(P7A,LOW);
  digitalWrite(P7C,LOW);
  
  set_counter(120);

}


//turn on eeprom output
void enable_eeprom_out(){
  set_parallel_read();
  digitalWrite(P3A,LOW);
  digitalWrite(P4A,HIGH);
  digitalWrite(P4C,LOW);
  digitalWrite(P5C,LOW);
}
  


void loop(){
  while(Serial.available()){
    Serial.read();
    clock();
    enable_eeprom_out();

    Serial.print(read_parallel());
    Serial.print('\t');
    Serial.print(digitalRead(P10A));
    Serial.println();
    
  }

  //Serial.println(read_parallel());
  //delay(1000);
  //set_parallel(0b10101010);
  //delay(1000);
  
}

  
