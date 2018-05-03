/*
  Function: find proper code and pass it to be sent
  Note:
  0b(dp)GFEDCBA
*/
void sevenSegDisplayChar(char displayChar, boolean group)
{
  switch (displayChar) //find proper code
  {
    case '^': //space
      if (!group)
      {
        //0
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);

      }
      else
      {
        sevenSegSendRaw(0b00000000, group);
      }
      break;
    case '0':
    case 'O':
      if (!group)
      {
        //0
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);

      }
      else
      {
        sevenSegSendRaw(0b00111111, group);
      }
      break;
    case '1':
      if (!group)
      {
        //1
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);

      }
      else
      {
        sevenSegSendRaw(0b00000110, group);
      }
      break;
    case '2':
      if (!group)
      {
        //2
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);
      }
      else
      {
        sevenSegSendRaw(0b01011011, group);
      }
      break;
    case '3':
      if (!group)
      {
        //3
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);

      }
      else
      {
        sevenSegSendRaw(0b01001111, group);
      }
      break;
    case '4':
      if (!group)
      {
        //4
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);
      }
      else
      {
        sevenSegSendRaw(0b01100110, group);
      }
      break;
    case '5':
    case 'S':
      if (!group)
      {
        //5
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);

      }
      else
      {
        sevenSegSendRaw(0b01101101, group);
      }
      break;
    case '6':
      if (!group)
      {
        //6
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);

      }
      else
      {
        sevenSegSendRaw(0b01111101, group);
      }
      break;
    case '7':
      if (!group)
      {
        //7

        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);

      }
      else
      {
        sevenSegSendRaw(0b00000111, group);
      }
      break;
    case '8':
      if (!group)
      {
        //8
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);

      }
      else
      {
        sevenSegSendRaw(0b01111111, group);
      }
      break;
    case '9':
    case 'g':
      if (!group)
      {
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , LOW); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);
        digitalWrite(sevenSegDataPinFalse , HIGH); //
        digitalWrite(sevenSegClockPinFalse, LOW);
        digitalWrite(sevenSegClockPinFalse, HIGH);

        digitalWrite(sevenSegLatchPinFalse, LOW);
        digitalWrite(sevenSegLatchPinFalse, HIGH);
      }
      else
      {
        sevenSegSendRaw(0b01101111, group);
      }
      break;
    case 'A':
      sevenSegSendRaw(0b01110111, group);
      break;
    case 'b':
      sevenSegSendRaw(0b01111100, group);
      break;
    case 'C':
      sevenSegSendRaw(0b00111001, group);
      break;
    case 'd':
      sevenSegSendRaw(0b01011110, group);
      break;
    case 'E':
      sevenSegSendRaw(0b01111001, group);
      break;
    case 'F':
      sevenSegSendRaw(0b01110001, group);
      break;
    case 'J':
      sevenSegSendRaw(0b00011110, group);
      break;
    case 'o':
      sevenSegSendRaw(0b01011100, group);
      break;
    case 'u':
      sevenSegSendRaw(0b00011100, group);
      break;
    case 'y':
      sevenSegSendRaw(0b01101110, group);
      break;
    case '-':
      sevenSegSendRaw(0b01000000, group);
      break;
    default:
      sevenSegSendRaw(0b00000000, group);
  }
  return;
}

/*
  Function: send code to display
*/
void sevenSegSendRaw(int code, boolean group)
{
  if (group)
  {
    shiftOut(sevenSegDataPinTrue, sevenSegClockPinTrue, MSBFIRST, code); //msb shift
  }
  else
  {
    shiftOut(sevenSegDataPinFalse, sevenSegClockPinFalse, MSBFIRST, code); //msb shift
  }
  return;
}
