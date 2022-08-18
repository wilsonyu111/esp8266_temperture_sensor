void readAllCredential()
{
  String credential = "";
  int startLocation = 0;
  for (int i =0; i<5; i++)
  {
    infoArr[i] = readFromRom(startLocation);
    startLocation += credential.length()+1;// update start location to the following string
    Serial.println ("read from rom:" + credential);

    if (infoArr[i].compareTo("") == 0)
    {
      Serial.println ("fail to read value, starting AP to get new info");
      setupAP(); // start AP to get new info
    }
  }
}

int writeToRom(String credentials, int startLocation)
{
  int strLength = credentials.length(); // string length of a string
  
  // write the string to the given starting location
  for (int i=0; i < strLength; i++)
  {
    EEPROM.write(startLocation, credentials[i]);
    startLocation++;
  }
  // return the start location of the next character
  // +1 to account for the null terminator
  return startLocation+1;
}

// infoArr [5];
void readFromRomHelper ()
{
  String credential = "";
  int startLocation = 0;
  for (int i = 0; i<arrayLength; i++)
  {
    credential = readFromRom(startLocation);
    infoArr[i] = credential;
    startLocation += credential.length()+1;// update start location to the following string
    Serial.println ("read from rom:" + credential);
  }
}

String readFromRom(int startingLocation)
{
  String readStr = "";
  char temp = ' '; 
  
  do
  {
    // first read a byte from the rom
    temp =  char(EEPROM.read(startingLocation));

    // only concatnate the string when the character is not a null terminator
    if (temp != '\0')
    {
      readStr += temp;
    }
    startingLocation++;
  }while (temp != '\0'); // end the loop if it's null terminator
  return readStr;
}

// write a bunch of null terminator up till the given size
void clearRom (int romSize)
{
  for (int i =0; i<romSize; i++)
  {
    // clear eeprom by writing null terminator to it
    EEPROM.write(i, '\0');
  }
}
