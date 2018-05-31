/*  
 *  Use the I2C bus with small EEPROMs
 *  24C01, 20C02, 24C04, 24C08, 24C16
 *  Sketch:    I2C_EEPROM_Small.pde
 *  
 *  Derived from sketch for 24C64 devices posted on
 *     http://www.arduino.cc/playground/Code/I2CEEPROM
 *  From  hkhijhe   Date: 01/10/2010
 *
 *  This one by davekw7x
 *  March, 2011
 *
 * For a single device, connect as follows:
 * EEPROM 4 (GND) to GND
 * EEPROM 8 (Vcc) to Vcc (5 Volts)
 * EEPROM 5 (SDA) to Arduino Analog Pin 4
 * EEPROM 6 (SCL) to Arduino Analog Pin 5
 * EEPROM 7 (WP)  to GND
 * EEPROM 1 (A0)  to GND
 * EEPROM 2 (A1)  to GND
 * EEPROM 3 (A2)  to GND
 */

#include <Wire.h>


// The seven-bit device address for EEPROMs
// I'll define it here rather than hard-code it inside all of the
// functions.
//
const byte DEVADDR = 0x50;

void setup()
{
   byte msg1[] = "Nasimaaaaaaaaa";   // data to write
   byte msg2[] = "1185555aaaaaaa";
   byte msg3[] = "That's all, folks!";
   byte msgf[16] = {
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
   };

   pinMode(PC0, INPUT_PULLUP);
   pinMode(PE5, OUTPUT);
   digitalWrite(PE5,HIGH);
   while(digitalRead(PC0));
   digitalWrite(PE5,LOW);
   
   Serial.begin(9600);
   Serial.println("Start");

   Wire.begin();
   Serial.println("After Wire.begin()");

   //
   // Change #if 0 to #if 1 and it will erase the
   // EEPROM pages that we are going to write to:
   //
   #if 0
     eeprom_write_page(DEVADDR, 0x000, msgf, 16);
     eeprom_write_page(DEVADDR, 0x010, msgf, 16);
     eeprom_write_page(DEVADDR, 0x020, msgf, 16);
         eeprom_write_page(DEVADDR, 0x100, msgf, 16);
     eeprom_write_page(DEVADDR, 0x1f0, msgf, 16);
     Serial.println("After erasing pages starting at 0x000, 0x100, and 0x1f0:");
     eeprom_dump(DEVADDR, 0, 512);
   #endif

   //
   // Change #if 1 to #if 0 so that it won't write over the stuff next time
   //
   #if 0
   // Write some stuff to EEPROM
   eeprom_write_page(DEVADDR, 0x40, msg1, sizeof(msg1));
   eeprom_write_page(DEVADDR, 0xA0, msg2, sizeof(msg2));
   eeprom_write_page(DEVADDR, 0x1f0, msg3, 16);
   #endif

   Serial.println("Memory written");
}

void loop()
{
   //
   // Read the first page in EEPROM memory, a byte at a time
   //
   Serial.println("eeprom_read_byte, starting at 0");
   for (int i = 0; i < 16; i++) {
       byte b = eeprom_read_byte(DEVADDR, i);
       Serial.print(b, HEX);
       Serial.print(' ');
   }
   Serial.println();
   
   //
   // Read the first page using the read_buffer function
   //
   Serial.println("eeprom_read_buffer, starting at 0");
   byte buffer[16];
   eeprom_read_buffer(DEVADDR, 0, buffer, sizeof(buffer));
   
   //
   //First print the hex bytes on this row
   //
   for (int i = 0; i < sizeof(buffer); i++) {
       char outbuf[6];
       sprintf(outbuf, "%02X ",buffer[i]);
       Serial.print(outbuf);
   }
   Serial.println();

   //
   // Now print the char if printable ASCII
   // otherwise print '.'
   //
   for (int i = 0; i < sizeof(buffer); i++) {
       if (isprint(buffer[i])) {
           Serial.print((char)buffer[i]);
       }
       else {
           Serial.print('.');
       }
   }
   Serial.println();
   
   // Now dump 512 bytes
   Serial.println("eeprom_dump(DEVADDR, 0, 512)");
   eeprom_dump(DEVADDR, 0, 512);
   Serial.println();

   delay(5000);

}

void eeprom_write_byte(byte deviceaddress, int eeaddress, byte data)
{
   // Three lsb of Device address byte are bits 8-10 of eeaddress
   byte devaddr = deviceaddress | ((eeaddress >> 8) & 0x07);
   byte addr    = eeaddress;
   Wire.beginTransmission(devaddr);
   Wire.write(int(addr));
   Wire.write(int(data));
   Wire.endTransmission();
   delay(10);
}

 // Pages are blocks of 16 bytes, starting at 0x000.
 // That is, pages start at 0x000, 0x010, 0x020, ...
 // For a device "page write", the last byte must be
 // on the same page as the first byte.
 //
 // No checking is done in this routine.
 //
 // TODO: Do some checking, or, better yet (maybe)
 // make length an int and do repeated device
 // page writes if necessary. (Then maybe rename to
 // eeprom_write_pages or some such thing.)
 //
void eeprom_write_page(byte deviceaddress, unsigned eeaddr,
                      const byte * data, byte length)
{
   // Three lsb of Device address byte are bits 8-10 of eeaddress
   byte devaddr = deviceaddress | ((eeaddr >> 8) & 0x07);
   byte addr    = eeaddr;
   Wire.beginTransmission(devaddr);
   Wire.write(int(addr));
   for (int i = 0; i < length; i++) {
       Wire.write(data[i]);
   }
   Wire.endTransmission();
   delay(10);
}

// TODO: Change to integer data type and return -1 if can't
// read.
//
int eeprom_read_byte(byte deviceaddress, unsigned eeaddr)
{
   byte rdata = -1;

   // Three lsb of Device address byte are bits 8-10 of eeaddress
   byte devaddr = deviceaddress | ((eeaddr >> 8) & 0x07);
   byte addr    = eeaddr;

   Wire.beginTransmission(devaddr);
   Wire.write(int(addr));
   Wire.endTransmission();
   Wire.requestFrom(int(devaddr), 1);
   if (Wire.available()) {
       rdata = Wire.read();
   }
   return rdata;
}

//
// Returns number of bytes read from device
//
// Due to buffer size in the Wire library, don't read more than 30 bytes
// at a time!  No checking is done in this function.
//
// TODO: Change length to int and make it so that it does repeated
// EEPROM reads for length greater than 30.

int eeprom_read_buffer(byte deviceaddr, unsigned eeaddr,
                       byte * buffer, byte length)
{
   // Three lsb of Device address byte are bits 8-10 of eeaddress
   byte devaddr = deviceaddr | ((eeaddr >> 8) & 0x07);
   byte addr    = eeaddr;
   
   Wire.beginTransmission(devaddr);
   Wire.write(int(addr));
   Wire.endTransmission();

   Wire.requestFrom(devaddr, length);
   int i;
   for (i = 0; i < length && Wire.available(); i++) {
       buffer[i] = Wire.read();
   }
   return i;
}

//
// The display is like hexdump -C.  It will always
// begin and end on a 16-byte boundary.
//

void eeprom_dump(byte devaddr, unsigned addr, unsigned length)
{
   // Start with the beginning of 16-bit page that contains the first byte
   unsigned startaddr = addr & (~0x0f);

   // stopaddr is address of next page after the last byte
   unsigned stopaddr  = (addr + length + 0x0f) & (~0x0f);

   for (unsigned i = startaddr; i < stopaddr; i += 16) {
       byte buffer[16]; // Hold a page of EEPROM
       char outbuf[6];  //Room for three hex digits and ':' and ' ' and '\0'
       sprintf(outbuf, "%03x: ", i);
       Serial.print(outbuf);
       eeprom_read_buffer(devaddr, i, buffer, 16);
       for (int j = 0; j < 16; j++) {
           if (j == 8) {
               Serial.print(" ");
           }
           sprintf(outbuf, "%02x ", buffer[j]);
           Serial.print(outbuf);            
       }
       Serial.print(" |");
       for (int j = 0; j < 16; j++) {
           if (isprint(buffer[j])) {
               Serial.print((char)buffer[j]);
           }
           else {
               Serial.print('.');
           }
       }
       Serial.println("|");
   }
}

