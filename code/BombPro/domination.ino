void domination(){

  //INICIALIZA EL TIEMPO
  int minutos = GAMEMINUTES-1;
  boolean showGameTime=true;
  unsigned long a;
  unsigned long iTime=millis(); 
  unsigned long aTime;
 
  team=0;
  iZoneTime=0;
  aTime=0;
  redTime=0;
  greenTime=0;

  int largoTono = 50;
  // 0 = neutral, 1 = equipo verde, 2 = equipo rojo
  a=millis();
  
  
  //COMIENZA EL CÓDIGO
  while(1)  // Este código es importante, no esta muy ordenado, pero funciona correctamente.
  {
    if(endGame){
      gameOver();
    }
    
    keypad.getKey();
    aTime = millis() - iTime;
    
    //Código para que el led parpade.
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      if(team==1)digitalWrite(GREENLED, HIGH);  
      if(team==2)digitalWrite(REDLED, HIGH);  
    }
    if(timeCalcVar >= 50 && timeCalcVar <= 100)
    {    
      if(team==1)digitalWrite(GREENLED, LOW);  
      if(team==2)digitalWrite(REDLED, LOW);
    }
    // Sonido para el juego
    if(timeCalcVar >= 0 && timeCalcVar <= 40 && soundEnable)tone(tonepin,tonoActivada,largoTono);

    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    
    //Cuenta 3 segundos
    if(a+2000<millis()){
      a=millis();   
      showGameTime=!showGameTime;
      cls();
    }
    //LOS PRÓXIMOS MÉTODOS MUESTRA "TIEMPO DE JUEGO" Y "TIEMPO DE ZONA CONTROLADA"

    if(showGameTime){
      lcd.setCursor(0,0);
      lcd.print("TIEMPO DE JUEGO");
      lcd.setCursor(3,1);
      printTime(minutos, aTime);
    }
    else if (!showGameTime){

      lcd.setCursor(2,0);
      if(team == 0)lcd.print("ZONA NEUTRAL");
      if(team == 1)lcd.print(" ZONA VERDE");
      if(team == 2)lcd.print(" ZONA ROJA");

      if(team>0){
        lcd.setCursor(3,1);
        printTimeDom(millis()-iZoneTime,true);
      }
    }

    //################CHECKINGS##################

    //COMPRUEBA SI EL JUEGO HA TERMINADO
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    {
      gameOver();
    }

    //COMPRUEBA SI SE ESTA NEUTRALIZANDO
    while((defuseando || cancelando) && team > 0)
    {
      cls();
      if(team>0)lcd.print("NEUTRALIZANDO...");
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defuseando || cancelando)
      {
        //Chequea si se esta neutralizando mientras se juega
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        
        keypad.getKey();
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(REDLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        if(percent >= 100)
        {
          delay(1000);

          if(team==1){ 
            greenTime+=millis()-iZoneTime;
            iZoneTime=0; 

          }
          if(team==2){ 
            redTime+=millis()-iZoneTime;
            iZoneTime=0; 
          }
          team=0;
          break;
        }
      }
      cls();
    }

    //Capturando el equipo Rojo

    while(defuseando && team == 0 )
    {
      cls();
      if(team==0)lcd.print("CAPTURANDO ZONA");
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); 
      while(defuseando)
      {
        keypad.getKey();
        //Chequea si se esta capturando mientras se juega
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(REDLED, HIGH);  
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(REDLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        if(percent >= 100)
        {
          digitalWrite(GREENLED, LOW);
          team=2;
          iZoneTime=millis();
          delay(1000);
          break;
        }
      }
      cls();
      digitalWrite(REDLED, LOW);
    }

    //getting to green zone
    while(cancelando && team == 0 )
    {
      cls();
      if(team==0)lcd.print("CAPTURANDO ZONA");
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis();     
      while(cancelando)
      {
        keypad.getKey();
        //comprueba si el juego sigue corriendo mientras se captura la zona
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(GREENLED, HIGH);  
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(GREENLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        if(percent >= 100)
        {
          digitalWrite(GREENLED, LOW);
          team=1;
          iZoneTime=millis();
          delay(1000);
          break;
        }
      }
      cls();
      digitalWrite(GREENLED, LOW);  
    }
  }
}

void gameOver(){

  if(team==1)greenTime+=millis()-iZoneTime;
  if(team==2)redTime+=millis()-iZoneTime;
  digitalWrite(GREENLED, LOW);
  digitalWrite(REDLED, LOW);
  while(!defuseando){
    keypad.getKey();
    if(defuseando){
      keypad.getKey();
      break;
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FIN DE PARTIDA!");
    lcd.setCursor(1,1);


    if(greenTime>redTime){
      //El equipo verde gana
      lcd.print(" E.VERDE GANA!");
      digitalWrite(GREENLED, HIGH);
    }
    else{
      //El equipo rojo gana
      lcd.print("E.ROJO GANA!");
      digitalWrite(REDLED, HIGH);
    }
    delay(3000);
    keypad.getKey();
    if(defuseando){
      keypad.getKey();
      break;
    }
    cls();
    lcd.print(" TIEMPO E.ROJO:");
    lcd.setCursor(5,1);
    printTimeDom(redTime,false);
    delay(3000);
    keypad.getKey();
    if(defuseando){
      
      break;
    }
    cls();
    lcd.print("TIEMPO E.VERDE:");
    lcd.setCursor(5,1);
    printTimeDom(greenTime,false);
    delay(3000);
    keypad.getKey();
    if(defuseando){
      keypad.getKey();
      break;
    }
  }
  cls();
  delay(100);
  lcd.print("Jugar de nuevo?");
  lcd.setCursor(0,1);
  lcd.print("A : Si B : No");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      cls();
      domination();
      break;
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();
      break;
    }  
  } 
}

