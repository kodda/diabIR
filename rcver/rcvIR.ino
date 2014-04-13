#include <IRLib.h> 
const int irReceiverPin = 2; 
IRrecv irrecv(irReceiverPin);
IRdecode My_Decoder;
unsigned int rawCodes[RAWBUF];
int rawCount;

void setup()
{
    Serial.begin(9600);
    irrecv.enableIRIn();
}

unsigned long last = millis();
void loop()
{
    delay(10);
    
    if (irrecv.GetResults(&My_Decoder)) //this is true if a message has been received
    {
        if (millis() - last > 250) {
            My_Decoder.decode();
            rawCount = My_Decoder.rawlen-1;
            for (int i = 1; i <=rawCount; i++) {
               rawCodes[i - 1] = My_Decoder.rawbuf[i];
            };
            Serial.println("lala"); 
            My_Decoder.DumpResults();
                    }
        last = millis();
        irrecv.resume();
    }
    
}    