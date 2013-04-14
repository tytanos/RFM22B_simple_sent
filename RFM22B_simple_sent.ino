
#include <RF22ReliableDatagram.h>
#include <RF22.h>
#include <SPI.h>
#include <JeeLib.h> // do usypiania procka
//#include <avr/sleep.h>
ISR(WDT_vect) { Sleepy::watchdogEvent(); }


#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2


RF22ReliableDatagram rf22(CLIENT_ADDRESS);


typedef struct {
int lp;
int liczba;

} Payload;
Payload dane;


void setup() 
{
  Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
     
  rf22.setFrequency(434.00, 0.01);
  rf22.setModemConfig(RF22::GFSK_Rb2Fd5); // duża czułość
  rf22.setTxPower(RF22_TXPOW_20DBM); // max mocy
  dane.lp = 0;
  dane.liczba=999;
  
  
}

// Dont put this on the stack:
//uint8_t buf[RF22_MAX_MESSAGE_LEN];
void loop()
{
  while (1)
  {
    Serial.println("Sending to rf22_datagram_server");
    
    // Send a message to rf22_server
    uint8_t data[] = "Hello World!";
    //rf22.sendto(data, sizeof(data), SERVER_ADDRESS);
   if (!rf22.sendtoWait((uint8_t*)&dane, sizeof(dane), SERVER_ADDRESS))
  Serial.println("sendtoWait failed"); 
  delay(50); // sprawdzić czy da się pominąć - bez tego mogą być błędy wysyłki na serial
   
     dane.lp += 1;
    rf22.setMode(RF22_ENLBD); 	// uśpienie modułu, 40h do rejestru h07
    Sleepy::loseSomeTime(2000); // czas uśpienia procka
    

  }
}

