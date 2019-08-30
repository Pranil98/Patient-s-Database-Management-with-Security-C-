#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>


int getFingerprintIDez();
uint8_t getFingerprintEnroll(uint8_t id);


SoftwareSerial mySerial(2, 3);


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  Serial.begin(9600);
 
  //pinMode(button, INPUT); 

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  delay(50);
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
    
  }
  
}

void loop()                     // run over and over again
{
  Serial.println("For Enrolling Press 1, For Searching Press 2");
  delay(50);
   int key=0;
  while (true) {
    while (! Serial.available());
    char k = Serial.read();
    if (! isdigit(k)) break;
     
    key = k - '0';
  }
  
  if(key == 1)
  {
Serial.println("Type in the ID # you want to save this finger as...");
  uint8_t id = 0;
  while (true) {
    while (! Serial.available());
    char c = Serial.read();
    if (! isdigit(c)) break;
    id *= 10;
    id += c - '0';}
    Serial.println("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll(id) );
  
  }
    if(key ==2)
{
  Serial.println("Waiting for valid finger...");
  int ret;
  do {
    ret = getFingerprintIDez();
    delay(50);
  } while (ret < 0);

}

  
//don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t n = finger.getImage();
  switch (n) {
    case FINGERPRINT_OK:
    
      Serial.println("Image taken");
      
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return n;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return n;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return n;
    default:
      Serial.println("Unknown error");
      return n;
  }

  // OK success!

  n = finger.image2Tz();
  switch (n) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return n;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return n;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return n;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return n;
    default:
      Serial.println("Unknown error");
      return n;
  }
  
  // OK converted!
  n = finger.fingerFastSearch();
  if (n == FINGERPRINT_OK) {
   
    Serial.println("Found a print match!");
    
  } else if (n == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return n;
  } else if (n == FINGERPRINT_NOTFOUND) {
    

    Serial.println("Did not find a match");
    return n;
  } else {
    Serial.println("Unknown error");
    return n;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.print(finger.confidence); 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {

  uint8_t n = finger.getImage();
  if (n != FINGERPRINT_OK)  return -1;

  n = finger.image2Tz();
  if (n != FINGERPRINT_OK)  return -1;

  n = finger.fingerFastSearch();
  if (n != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
  
  
}

uint8_t getFingerprintEnroll(uint8_t id) {
uint8_t p = -1;

  Serial.println("Waiting for valid finger to enroll");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  
  Serial.println("Place same finger again");
  delay(500);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
    
      Serial.println("Image converted");
      delay(1000);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  
  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    
    Serial.println("Prints matched!");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    
    Serial.println("Stored!");
    delay(500);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}
