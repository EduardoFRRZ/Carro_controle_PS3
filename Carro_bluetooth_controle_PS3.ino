//Inclui as Bibliotecas nescessárias.
#include <PS3BT.h>
#include <Servo.h>

// Declaramos as variáveis e seus valores que serão os pinos utilizados
byte PinoDirecao = 2;      // Pino do Servo Motor
byte PinoLuzRe = 3;        // Pino da Luz de Ré
byte PinoFarolFrente = 4;  // Pino do Farol
byte PinoFrente = 5;       // Pino Motor para Frente
byte PinoTras = 6;         // Pino Motor para Trás

byte a = 0;                // Variável para indicar estado do valor do Farol

// Funções da Biblioteca PS3BT
USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd); 

Servo ServoDirecao; //Cria a instancia para o Servo da direção

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);                                       
  if (Usb.Init() == -1) {                                            
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  
  // Declamos os Pinos abaixo como Saída
  pinMode(PinoDirecao, OUTPUT);
  pinMode(PinoLuzRe, OUTPUT);  
  pinMode(PinoFarolFrente, OUTPUT);
  pinMode(PinoFrente, OUTPUT);
  pinMode(PinoTras, OUTPUT);

  //Aqui associamos o pino que controlara o Servo a Variável "PinoDirecao"
  ServoDirecao.attach(PinoDirecao);

}

void loop() {
  // put your main code here, to run repeatedly:
  Usb.Task();

  //Verifica se o Controle do PS3 esta conectado se estiver continua com as instruções abaixo
  if(PS3.PS3Connected || PS3.PS3NavigationConnected) {
    
    //Faz a leitura do Controle Analógico Esquerdo que vai de 0 a 255 e com a função MAP converto esta escala em Graus de 179 a 0. Pois o Servo foi montado invertido.
    ServoDirecao.write(map(PS3.getAnalogHat(LeftHatX), 0, 255, 110, 70));

    // Faz a leitura do Botão R2 Analógico que vai de 0 a 255 e envia para a variável PinoFrente
    analogWrite(PinoFrente, PS3.getAnalogButton(R2_ANALOG));

    // Faz a leitura do Botão L2 Analógico que vai de 0 a 255 e envia para a variável PinoTras
    analogWrite(PinoTras, PS3.getAnalogButton(L2_ANALOG));
    //Ao ler qualquer valor no Botão L2 liga a variável LuzRe
    digitalWrite(PinoLuzRe, PS3.getAnalogButton(L2_ANALOG));
    
    // Verifica se o botão X (Cross) for apertado liga o Farol dianteiro
    if(PS3.getButtonClick(CROSS)==1){ // Se o botão X for pressionado ==1
    a++; // Se a condição acima for verdadeira acrescenta 1 na variável “a” 
    digitalWrite(PinoFarolFrente, a); // Passamos o Valor da variável "a" que agora vale 1 para o pino PinoFarolFrente, 1 = HIGH
    delay(10);// Este delay é so por segurança  
    if (a>=2){ // Se a variavel for sigual o maior que 2, "Segunda pressionada no botão X" 
      a =0; // A variável a volta a assumir o valor 0
    digitalWrite(PinoFarolFrente, a);// Passamos o Valor da variável "a" que agora vale 0 para o pino PinoFarolFrente, 0 = LOW
    delay(10);// Este delay é so por segurança  
    }
    }
    
  }
  
  // Caso o controle de PS3 não estiver conectado continua com as instruções abaixo
  else
   {
    ServoDirecao.write(89); // Caso o controle não estiver conectado inicia o Servo a 90 Graus, ou seja no meio.
   }
    
    if(PS3.getButtonClick(PS)) // Caso o Botão PS do controle for apertado, desliga a conexão do controle.
    {
      PS3.disconnect();
    }
}
