// Se usa para obtener claves, aquí se puede configurar cómo funciona la entrada sin modificar el otro código
boolean isPressed(char key) 
{

//  Serial.print("checkeando= ");
//  Serial.print(key);

//  Serial.print(" estado = ");
//  Serial.print(keypad.getState());

//  Serial.print(" estado = ");
//  Serial.print(keypad.getKey());

  if(keypad.getKey() == key)
  {
//    Serial.println(" TRUE");
    return true;
  }
  else if(keypad.getKey() == key && keypad.getState() == 2)
  {
//    Serial.print(" Hold!!");
//    Serial.println(key);
    return true;
  }
//  Serial.println(" Falso");
  return false;
}

// Esta función compara las variables  enteredText[8] and password[8] variables
boolean comparePassword(){

  for(int i=0;i<8;i++){                                        //LE PIDO METER EL CODIGO 8 DIGITOS
    if(enteredText[i]!=password[i])return false;
  }
  return true;

}

//Cambio la contraseña
void setCode(){

  lcd.setCursor(0, 1);
  for(int i=0;i<8;i++){                                        //LE PIDO METER EL CODIGO 8 DIGITOS
    while(1){
      var= getNumber();
      if(var !='x'){
        enteredText[i] = var;

        if (i != 0){
          lcd.setCursor(i-1,1);
          lcd.print("*");
          lcd.print(var);
        }
        else
        {
          lcd.print(var);
        }
        tone(tonepin,2400,30);
        break;
      }
    }
  }
}
void setCodeTime(){

  timeCalcVar=millis();

  for(int i=0;i<8;i++){
    while(1){
      if(10000+timeCalcVar-millis()<=100){
        enteredText[i]='x';
        break;
      }

      lcd.setCursor(11,0);
      printTimeDom(10000+timeCalcVar-millis(),false);

      var= getNumber();
      if(var !='x'){
        enteredText[i] = var;

        if (i != 0){
          lcd.setCursor(i-1,1);
          lcd.print("*");
          lcd.print(var);
        }
        else
        {
          lcd.print(var);
        }
        tone(tonepin,2400,30);
        break;
      }
    }
  }
}
void setPass(){
  lcd.setCursor(0, 1);

  for(int i=0;i<8;i++){ 
    while(1){
      var= getNumber();
      if(var !='x'){
        password[i] =  var;
        if (i != 0){
          lcd.setCursor(i-1,1);
          lcd.print("*");
          lcd.print(var);
        }
        else
        {
          lcd.print(var);
        }
        tone(tonepin,2400,30);
        break;
      }
    }  
  }
}

void setNewPass(){

  while(1){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Introduce Pass");
    setPass();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Repite Pass");

    setCode();

    if(comparePassword()){

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Codigo OK!!");
      delay(2000); 
      break; 
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ERROR!");
      if(soundEnable)tone(tonepin,errorTone,200);
      delay(2000); 

    }
  }
}


//Solo hasta que se presiona un botón...
char getNumber(){

  while(1){
    var = keypad.getKey();

    if (var){//
      switch (var) {
      case 'a': 
        return 'x';
        break;
      case 'b': 
        return 'x';
        break;

      case 'c': 
        return 'x';
        break;
      case 'd': 
        return 'x';
        break;
      case '*': 
        return 'x';
        break;
      case '#': 
        return 'x';
        break;
      default:
        return var;
        break;
      }
    }
    return 'x';
  }
}

byte getRealNumber(){

  while(1){
    var = keypad.waitForKey();

    if (var){//
      switch (var) {
      case '1': 
        return 1;
        break;
      case '2': 
        return 2;
        break;

      case '3': 
        return 3;
        break;
      case '4': 
        return 4;
        break;
      case '5': 
        return 5;
        break;
      case '6': 
        return 6;
      case '7': 
        return 7;
        break;
      case '8': 
        return 8;
        break;
      case '9': 
        return 9;
        break;
        case '0': 
        return 0;
        break;
        
      default:
        return 11;
        break;
      }

    }
    return 11;
  }
}
