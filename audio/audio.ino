//https://www.youtube.com/BitaBit
//https://bitabittecnologia.com.br

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini playerMini;
SoftwareSerial sf(8, 9); //8-RX, 9-TX

char opcaoMenu;
bool pausado = true;
bool repetirTudo = false;
int equalizacaoAtual = 0;
int volumeAtual;

//Menu de controle
void exibirMenu()
{
  Serial.println();
  Serial.println("__________________________________________");
  Serial.print("Volume atual: ");
  Serial.println(volumeAtual);

  Serial.print("Equalização atual: ");
  Serial.println(obterTipoEqualizacao());
  
  Serial.print("Modo repetir tudo: ");
  Serial.println( repetirTudo ? "Ativado" : "Desativado");
  Serial.println();

  Serial.println("Utilize o teclado para dar os comandos:");
  Serial.println("[c] Continuar / Pausar");
  Serial.println("[p] Parar");
  Serial.println("[>] Música seguinte");
  Serial.println("[<] Música anterior");
  Serial.println("[+] Aumentar volume");
  Serial.println("[-] Diminuir volume");
  Serial.println("[e] Equalizar");
  Serial.println("[r] Repetir tudo / Parar repetir tudo");
}

//Retorna o tipo de equalização
String obterTipoEqualizacao()
{
  switch(equalizacaoAtual)
  {
    case 0:
    return "Normal";
    break;

    case 1:
    return "Pop";
    break;

    case 2:
    return "Rock";
    break;

    case 3:
    return "Jazz";
    break;

    case 4:
    return "Classic"; 
    break;

    default:
    return "Bass";
  }
}

void setup() 
{
  Serial.begin(9600); //Inicializa a Serial
  sf.begin(9600); //Inicializa o SoftwareSerial

  Serial.println("Inicializando, aguarde...");
  Serial.println();

//Inicializa o módulo
if (!playerMini.begin(sf)) 
  {
    Serial.println("Não foi possível inicializar.");
    Serial.println("Verifique as conexões e se o cartão está inserido!");
    while(true); 
  }

  Serial.println("Inicializado com sucesso!");
  Serial.println();

  Serial.print("Quantidade de arquivo(s): ");
  Serial.println(playerMini.readFileCounts());
  
  playerMini.setTimeOut(500);

  volumeAtual= 30;
  playerMini.volume(volumeAtual);

  exibirMenu();
}

void loop() 
{
   if(Serial.available())
   {
     opcaoMenu = Serial.read();

     //Opção continuar / pausar
     if(opcaoMenu == 'c')
     {
       if(pausado)
       {
         playerMini.start();
         pausado = false;
       }
       else
       {
         playerMini.pause();
         pausado = true;
       }
     }

     //Opção parar
     if(opcaoMenu == 'p')
     {
       playerMini.stop();
       pausado = true;
     }

     //Música seguinte
     if(opcaoMenu == '>')
     {
       playerMini.next();
     }

     //Música anterior
    if(opcaoMenu == '<')
     {
       playerMini.previous();
     }

     //Aumentar volume
     if(opcaoMenu == '+')
     {
       
       volumeAtual++;
       if(volumeAtual > 30)
       {
         volumeAtual = 30;
       }
       playerMini.volumeUp();
       
       exibirMenu();
     }

     //Diminuir volume
     if(opcaoMenu == '-')
     {
       
       volumeAtual--;
       if(volumeAtual < 1)
       {
         volumeAtual = 1;
       }
       playerMini.volumeDown();
      exibirMenu();
     }

     //Equalizar
     if(opcaoMenu == 'e')
     {
       equalizacaoAtual++;
       if(equalizacaoAtual == 6)
       {
         equalizacaoAtual = 0;
       }

       playerMini.EQ(equalizacaoAtual);
       exibirMenu();
     }

     //Repetir tudo / Parar repetir tudo
     if(opcaoMenu == 'r')
     {
       if(repetirTudo)
       {
         playerMini.disableLoop();
         repetirTudo = false;
       }
       else
       {
         playerMini.enableLoopAll();
         repetirTudo = true;
       }
       exibirMenu();
     }

     if(opcaoMenu == 'l')
     {
         pausado = false;
         playerMini.loop(3);
         exibirMenu();
     }
     if(opcaoMenu == '1')
     {
         pausado = false;
         playerMini.loop(1);
         exibirMenu();
     }
     if(opcaoMenu == '2')
     {
         pausado = false;
         playerMini.loop(2);
         exibirMenu();
     }
     if(opcaoMenu == '3')
     {
         pausado = false;
         playerMini.loop(3);
         exibirMenu();
     }
     if(opcaoMenu == '4')
     {
         pausado = false;
         playerMini.loop(4);
         exibirMenu();
     }
     if(opcaoMenu == '5')
     {
         pausado = false;
         playerMini.loop(5);
         exibirMenu();
     }
     if(opcaoMenu == '6')
     {
         pausado = false;
         playerMini.loop(6);
         exibirMenu();
     }
     if(opcaoMenu == '7')
     {
         pausado = false;
         playerMini.loop(7);
         exibirMenu();
     }
     if(opcaoMenu == '8')
     {
         pausado = false;
         playerMini.loop(8);
         exibirMenu();
     }
      
   }
}
