#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library
#include <string.h>
#include <EEPROM.h>
 
// ST7789 TFT module connections
#define TFT_CS     10
#define TFT_RST    8  // define reset pin, or set to -1 and connect to Arduino RESET pin
#define TFT_DC     9  // define data/command pin

#define BLUE 0x001F
#define RED 0xF800
#define CYAN 0x07FF
#define GREEN 0x07E0
#define GRAY 0x8410

// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//escolher as letras para poder digitar o nome
char alfabeto [] = "AABCDEFGHIJKLMNOPQRSTUVWXYZ";

//numero de jogadores existentes
byte jogadores = 0;

float p = 3.1415926;
//struct dos nomes e pontos
typedef struct {
  char nome[4];
  int pontos;
} Ranking;
//inicializacao da struct
Ranking Inicial[4] = {
  {"###", 0},
  {"###", 0},
  {"###", 0}
};


void menu(void);

int botao(void);

void movercursor(int quadrado);

int BotaoQuadrado(void);

void MoverQuadrado(byte quadrado, byte b2);

void jogar(void);

int Jogando(void);

void boneco (byte x);

void ranking(void);

void organiza(void);

void creditos(void);

void sair(void);

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST7789 TFT Test"));

  //config dos botoes
  pinMode(2, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  // if the tft has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE2);    // Init ST7789 tft 240x240 pixel
  // if the screen is flipped, remove this command
  tft.setRotation(2);
  Serial.println(F("Initialized"));
  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;
  delay(500);

  Serial.println("done");
  delay(1000);

  //endereco para EEPROM
  int endereco = 0;

  //colocar da EEPROM nas variaveis
  EEPROM.get(endereco, Inicial);

  //verifica se tem jogadores existentes se tiver adiciona o numero de jogadores
  for (byte j = 0; j < 3; j++)
  {
    if (strcmp(Inicial[j].nome, "###") != 0)
    {
      jogadores++;
    }
  }

  //Titulo do jogo e delay de 5 segundos
  tft.drawRect(0, 0, 239, 239, ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(45, 100);
  tft.print(F("TRASH DESTINY"));
  delay(5000);
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
}

void loop()
{
  int opc;
  menu();
  opc = botao();

  switch (opc)
  {
    case 1:
      jogar();
      break;

    case 2:
      ranking();
      break;

    case 3:
      creditos();
      break;

    case 4:
      sair();
      break;
  }
}

//imprime as opcoes do menu do jogo
void menu (void)
{
  tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(30, 10);
  tft.print(F("TRASH DESTINY"));
  tft.setCursor(25, 35);
  tft.fillRect(24, 34, 100, 18, ST77XX_WHITE);
  tft.setTextColor(ST77XX_BLACK);
  tft.print(F("JOGAR"));
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(25, 55);
  tft.print(F("RANKING"));
  tft.setCursor(25, 75);
  tft.print(F("CREDITOS"));
  tft.setCursor(25, 95);
  tft.print(F("SAIR"));
}

//mover cursor com botao no menu
int botao(void)
{
  byte b1 = 1;
  byte b2 = 1;
  byte b2b = 1;
  byte quadrado = 1;

  while (b2 == 1)
  {
    b1 = digitalRead(2);  //botao esquerdo
    b2 = digitalRead(7);  //botao direito
    delay(1);
    b2b = digitalRead(7);
    //Serial.println(b2); //verifica se o botao funciona

    if ((b2b - b1) == 1) // move o cursor
    {
      quadrado++;
      if (quadrado > 4)
      {
        quadrado = 1;
      }
      //Serial.println(cursor); //verif. a função movercursor
      movercursor(quadrado, b2);
      delay(200);
    }
  }
  return quadrado;
}
//mover o cursor do botao no menu
void movercursor(byte quadrado, byte b2)
{
  switch (quadrado)
  {

    case 1:
      tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
      tft.fillRect(24, 34, 100, 18, ST77XX_WHITE);
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(25, 35);
      tft.println(F("JOGAR"));
      tft.fillRect(24, 94, 100, 18, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(25, 95);
      tft.println(F("SAIR"));
      break;

    case 2:
      tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
      tft.fillRect(24, 54, 100, 18, ST77XX_WHITE);
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(25, 55);
      tft.println(F("RANKING"));
      tft.fillRect(24, 34, 100, 18, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(25, 35);
      tft.println(F("JOGAR"));
      break;

    case 3:
      tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
      tft.fillRect(24, 74, 100, 18, ST77XX_WHITE);
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(25, 75);
      tft.println(F("CREDITOS"));
      tft.fillRect(24, 54, 100, 18, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(25, 55);
      tft.println(F("RANKING"));
      break;

    case 4:
      tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
      tft.fillRect(24, 94, 100, 18, ST77XX_WHITE);
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(25, 95);
      tft.println(F("SAIR"));
      tft.fillRect(24, 74, 100, 18, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(25, 75);
      tft.println(F("CREDITOS"));
      break;
  }
}
//mover cursor na selecao de jogadores
int BotaoQuadrado(void)
{
  byte b1 = 1;
  byte b2 = 1;
  byte b2b = 1;
  byte quadrado = 1;

  while (b2 == 1)
  {
    b1 = digitalRead(2);  //botao esquerdo
    b2 = digitalRead(7);  //botao direito
    delay(1);
    b2b = digitalRead(7);
    //Serial.println(b2); //verifica se o botao funciona

    if ((b2b - b1) == 1) // move o cursor
    {
      quadrado++;
      if (quadrado > 4)
      {
        quadrado = 1;
      }
      //Serial.println(cursor); //verif. a função movercursor
      MoverQuadrado(quadrado, b2);
      delay(200);
    }

  }
  return quadrado;
}
//mover cursor na selecao de jogadores
void MoverQuadrado(byte quadrado, byte b2)
{
  switch (quadrado)
  {

    case 1:
      tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
      tft.fillRect(100, 34, 42, 18, ST77XX_WHITE);
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(104, 35);
      tft.println(Inicial[0].nome);
      tft.fillRect(99, 94, 51, 18, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(101, 95);
      tft.println(F("NOVO"));
      break;

    case 2:
      tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
      tft.fillRect(100, 54, 42, 18, ST77XX_WHITE);
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(104, 55);
      tft.println(Inicial[1].nome);
      tft.fillRect(100, 34, 42, 18, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(104, 35);
      tft.println(Inicial[0].nome);
      break;

    case 3:
      tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
      tft.fillRect(100, 74, 42, 18, ST77XX_WHITE);
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(104, 75);
      tft.println(Inicial[2].nome);
      tft.fillRect(100, 54, 42, 18, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(104, 55);
      tft.println(Inicial[1].nome);
      break;

    case 4:
      tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
      tft.fillRect(99, 94, 51, 18, ST77XX_WHITE);
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(101, 95);
      tft.println(F("NOVO"));
      tft.fillRect(100, 74, 42, 18, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(104, 75);
      tft.println(Inicial[2].nome);
      break;
  }
}
//logar com um jogador ou criar um novo jogador
void jogar(void)
{
  byte opc = 0;
  int pontosNovo = 0;
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);

  tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
  tft.setTextSize(2);
  tft.fillRect(100, 34, 42, 18, ST77XX_WHITE);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(104, 35);
  tft.println(Inicial[0].nome);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(104, 55);
  tft.print(Inicial[1].nome);
  tft.setCursor(104, 75);
  tft.print(Inicial[2].nome);
  tft.setCursor(101, 95);
  tft.print(F("NOVO"));


  opc = BotaoQuadrado();

  switch (opc)
  {
    case 1:
      tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
      //verifica se o nome existe se nao existir volta ao menu
      if (strcmp (Inicial[0].nome, "###") == 0)
      {
        tft.drawRect(0, 0, 239, 239, ST77XX_WHITE);
        tft.setCursor(45, 100);
        tft.print(F("Nao registrado"));
        delay(3000);
        break;
      }
      tft.setCursor(25, 35);
      tft.print(Inicial[0].nome);
      delay(500);
      pontosNovo = Jogando();
      
      //verifica se a pontuacao e maior se sim, substitui ela
      if (pontosNovo >= Inicial[0].pontos)
      {
        Inicial[0].pontos = pontosNovo;
      }
      break;
    case 2:
      tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
      if (strcmp (Inicial[1].nome, "###") == 0)
      {
        tft.drawRect(0, 0, 239, 239, ST77XX_WHITE);
        tft.setCursor(45, 100);
        tft.print(F("Nao registrado"));
        delay(3000);
        break;
      }
      tft.setCursor(25, 55);
      tft.print(Inicial[1].nome);
      delay(500);
      pontosNovo = Jogando();
      if (pontosNovo >= Inicial[1].pontos)
      {
        Inicial[1].pontos = pontosNovo;
      }
      break;
    case 3:
      tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
      if (strcmp (Inicial[2].nome, "###") == 0)
      {
        tft.drawRect(0, 0, 239, 239, ST77XX_WHITE);
        tft.setCursor(45, 100);
        tft.print(F("Nao registrado"));
        delay(3000);
        break;
      }
      tft.setCursor(25, 75);
      tft.print(Inicial[2].nome);
      delay(500);
      pontosNovo = Jogando();
      if (pontosNovo >= Inicial[2].pontos)
      {
        Inicial[2].pontos = pontosNovo;
      }
      break;
    case 4:
      tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
      tft.setCursor(0, 30);
      tft.print(F("Novo Jogador: "));
      byte a = 1, b = 1, c = 1;
      byte i;
      byte x = 155;
      int encontrado = -1;
      char nome1[3];
      //Serial.println(digitalRead(7));

      //para escrever o nome
      for (byte j = 0; j < 3; j++)
      {
        while (c == 1)
        {
          c = digitalRead(2);


          a = digitalRead(7);
          delay(5);
          b = digitalRead(7);

          if (( a - b) == 1) {
            i++;
            tft.fillRect(x, 30, 15, 15, ST77XX_BLACK);
            tft.setCursor(x, 30);
            tft.print(alfabeto[i]);
          }

          if (i >= 26) {
            i = 0;
          }
        }
        nome1[j] = alfabeto[i];
        delay(300);
        c = digitalRead(2);
        i = 0;
        x += 15;
      }
      nome1[3] = '\0';

      //verifica se o jogador já existe e volta ao menu
      for (byte j = 0; j < jogadores; j++)
      {
        if (strcmp(nome1, Inicial[j].nome) == 0)
        {
          encontrado = 1;
          tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
          tft.drawRect(0, 0, 239, 239, ST77XX_WHITE);
          tft.setCursor(23, 120);
          tft.print(F("Jogador Existente"));
          delay(1000);
          break;
        }
      }
      //se nao existe jogador ele adiciona e comeca o jogo
      if (encontrado == -1)
      {
        encontrado = jogadores;
        strcpy(Inicial[encontrado].nome, nome1);
        jogadores++;

        tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
        tft.drawRect(0, 0, 239, 239, ST77XX_WHITE);
        tft.setCursor(16, 120);
        tft.print(F("Jogador Adicionado"));
        delay(3000);
        pontosNovo = Jogando();
        Inicial[encontrado].pontos = pontosNovo;
      }
      break;
  }

  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
  delay(1000);
}
//creditos subindo
void creditos (void)
{
  int i;

  for (i = 240; i > -100; i -= 10)
  {
    tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(30, i);
    tft.print(F("TRASH DESTINY"));
    tft.setCursor(10, i + 15);
    tft.print(F("Desenvolvido por:"));
    tft.setCursor(15, i + 35);
    tft.print(F("\tRoberto Passos"));
    tft.setCursor(15, i + 55);
    tft.print(F("\tDouglas Oliveira"));
    tft.setCursor(15, i + 75);
    tft.print(F("\tLuiz Henz"));
    delay(500);
  }
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
}

//ranking que organiza por pontuacao
void ranking(void)
{
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
  tft.drawRect(1, 1, 239, 239, ST77XX_WHITE);
  organiza();
  tft.setCursor(30, 60);
  tft.print(F("Organizado"));
  delay(500);
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(75, 35);
  tft.print(Inicial[0].nome);
  tft.setCursor(115, 35);
  tft.print(F("-"));
  tft.setCursor(130, 35);
  tft.print(Inicial[0].pontos);
  tft.setCursor(75, 55);
  tft.print(Inicial[1].nome);
  tft.setCursor(115, 55);
  tft.print(F("-"));
  tft.setCursor(130, 55);
  tft.print(Inicial[1].pontos);
  tft.setCursor(75, 75);
  tft.print(Inicial[2].nome);
  tft.setCursor(115, 75);
  tft.print(F("-"));
  tft.setCursor(130, 75);
  tft.print(Inicial[2].pontos);
  delay(3000);
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
}

//bubble sort que organiza por pontuacao
void organiza(void)
{
  Ranking aux;
  for (byte i = 0; i < 3; i++)
  {
    for (byte j = 0; j < 3; j++)
    {
      if (Inicial[j].pontos < Inicial[j + 1].pontos)
      {
        aux = Inicial[j];
        Inicial[j] = Inicial[j + 1];
        Inicial[j + 1] = aux;
      }
    }
  }
}
//limpa a tela, salva na EEPROM e da adeus
void sair(void)
{
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);

  int endereco = 0;
  //colocar na EEPROM a struct com nome e pontos
  EEPROM.put(endereco, Inicial);
  
  tft.setCursor(50, 100);
  tft.setTextColor(0xF81F);
  tft.print(F("Volte sempre!"));


  delay(5000);
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
}
//jogo funcionando
int Jogando (void)
{
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);

  //tela de fundo
  tft.drawRect(0, 170, 240, 2, ST77XX_WHITE);
  tft.drawRect(20, 210, 30, 2, ST77XX_WHITE);
  tft.drawRect(60, 210, 30, 2, ST77XX_WHITE);
  tft.drawRect(100, 210, 30, 2, ST77XX_WHITE);
  tft.drawRect(140, 210, 30, 2, ST77XX_WHITE);
  tft.drawRect(180, 210, 30, 2, ST77XX_WHITE);
  tft.drawRect(0, 239, 240, 2, ST77XX_WHITE);
  //boneco
  tft.fillRect(5, 175, 11, 12, ST77XX_WHITE);
  tft.fillRect(11, 179, 5, 4, ST77XX_BLACK);
  tft.fillRect(7, 187, 6, 13, RED);
  tft.fillRect(13, 191, 7, 5, RED);
  tft.fillRect(7, 200, 6, 9, BLUE);
  //lata de lixo
  tft.fillRect(19, 179, 11, 18, GRAY);
  tft.drawRect(19, 179, 11, 7, ST77XX_WHITE);
  tft.drawRect(19, 187, 1, 8, ST77XX_WHITE);
  tft.drawRect(21, 188, 1, 6, ST77XX_WHITE);
  tft.drawRect(23, 189, 1, 4, ST77XX_WHITE);
  tft.drawRect(25, 189, 1, 4, ST77XX_WHITE);
  tft.drawRect(27, 188, 1, 6, ST77XX_WHITE);
  tft.drawRect(29, 187, 1, 8, ST77XX_WHITE);

  byte vida = 3;
  byte x = 0;
  byte b = 1;
  byte a = 1;
  byte y = 20;
  byte z;
  int pontosNovo = 0;
  //while em qual ocorre o jogo
  while (vida > 0)
  {
    y = 20;
    z = random (20, 221);
    //z e uma variavel que define a posicao que o lixo vai cair
    
    while (y < 215)
    {
      //de acordo com a pontuacao dele a dificuldade aumenta
      if(pontosNovo >= 21)
      {
        y += 15; 
      }
      else if (pontosNovo >= 11 && pontosNovo <= 20)//velocidade 1
      {
        y += 10;
      }
      else//velocidade 2
      {
        y += 5;
      }
      //printa a vida na tela na direita
      tft.fillRect(220, 10, 10, 10, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(220, 10);
      tft.print(vida);
      //printa os pontos na tela na esquerda
      tft.fillRect(5, 10, 25, 20, ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(5, 10);
      tft.print(pontosNovo);

      delay(100);
      //le os botoes para mover o boneco
      b = digitalRead(2);
      a = digitalRead(7);

      //mover o boneco
      if (b == 0)
      {
        x -= 20;
      }
      else if (a == 0)
      {
        x += 20;
      }
      //verifica se algum botao foi apertado para printa o boneco
      if (b == 0 || a == 0)
      {
        boneco(x);
      }
      //estrada
      tft.drawRect(0, 170, 240, 2, ST77XX_WHITE);
      tft.drawRect(20, 210, 30, 2, ST77XX_WHITE);
      tft.drawRect(60, 210, 30, 2, ST77XX_WHITE);
      tft.drawRect(100, 210, 30, 2, ST77XX_WHITE);
      tft.drawRect(140, 210, 30, 2, ST77XX_WHITE);
      tft.drawRect(180, 210, 30, 2, ST77XX_WHITE);
      tft.drawRect(0, 239, 240, 2, ST77XX_WHITE);


      //velocidade do lixo para apagar de acordo com a dificuldade
      if (pontosNovo >= 21)
      {
        tft.fillRect(z, y - 15, 5, 2, ST77XX_BLACK);
        tft.drawRect(z + 1, (y + 2) - 15, 3, 1, ST77XX_BLACK);
        tft.drawRect(z, (y + 3) - 15, 5, 1, ST77XX_BLACK);
        tft.drawRect(z - 1, (y + 4) - 15, 7, 1, ST77XX_BLACK);
        tft.drawRect(z - 2, (y + 6) - 15, 1, 6, ST77XX_BLACK);
        tft.drawRect(z + 2, (y + 11) - 15, 4, 1, ST77XX_BLACK);
        tft.drawRect(z + 7, (y + 9) - 15, 1, 3, ST77XX_BLACK);
        tft.fillRect(z - 1, (y + 5) - 15, 8, 9, ST77XX_BLACK);      
      }
      else if (pontosNovo >= 11 && pontosNovo <= 20)//apagando 2
      {
        tft.fillRect(z, y - 10, 5, 2, ST77XX_BLACK);
        tft.drawRect(z + 1, (y + 2) - 10, 3, 1, ST77XX_BLACK);
        tft.drawRect(z, (y + 3) - 10, 5, 1, ST77XX_BLACK);
        tft.drawRect(z - 1, (y + 4) - 10, 7, 1, ST77XX_BLACK);
        tft.drawRect(z - 2, (y + 6) - 10, 1, 6, ST77XX_BLACK);
        tft.drawRect(z + 2, (y + 11) - 10, 4, 1, ST77XX_BLACK);
        tft.drawRect(z + 7, (y + 9) - 10, 1, 3, ST77XX_BLACK);
        tft.fillRect(z - 1, (y + 5) - 10, 8, 9, ST77XX_BLACK);
      }
      else //apagando 1
      {
        tft.fillRect(z, y - 5, 5, 2, ST77XX_BLACK);
        tft.drawRect(z + 1, (y + 2) - 5, 3, 1, ST77XX_BLACK);
        tft.drawRect(z, (y + 3) - 5, 5, 1, ST77XX_BLACK);
        tft.drawRect(z - 1, (y + 4) - 5, 7, 1, ST77XX_BLACK);
        tft.drawRect(z - 2, (y + 6) - 5, 1, 6, ST77XX_BLACK);
        tft.drawRect(z + 2, (y + 11) - 5, 4, 1, ST77XX_BLACK);
        tft.drawRect(z + 7, (y + 9) - 5, 1, 3, ST77XX_BLACK);
        tft.fillRect(z - 1, (y + 5) - 5, 8, 9, ST77XX_BLACK);
      }
      //saco de lixo
      tft.fillRect(z, y, 5, 2, ST77XX_WHITE);
      tft.drawRect(z + 1, y + 2, 3, 1, ST77XX_WHITE);
      tft.drawRect(z, y + 3, 5, 1, ST77XX_WHITE);
      tft.drawRect(z - 1, y + 4, 7, 1, ST77XX_WHITE);
      tft.drawRect(z - 2, y + 6, 1, 6, ST77XX_WHITE);
      tft.drawRect(z + 2, y + 11, 4, 1, ST77XX_WHITE);
      tft.drawRect(z + 7, y + 9, 1, 3, ST77XX_WHITE);
      tft.fillRect(z - 1, y + 5, 8, 9, ST77XX_WHITE);

      //apaga lixo 
      if (y + 11 >= 210)
      {
        tft.fillRect(z, y, 5, 2, ST77XX_BLACK);
        tft.drawRect(z + 1, (y + 2), 3, 1, ST77XX_BLACK);
        tft.drawRect(z, (y + 3), 5, 1, ST77XX_BLACK);
        tft.drawRect(z - 1, (y + 4), 7, 1, ST77XX_BLACK);
        tft.drawRect(z - 2, (y + 6), 1, 6, ST77XX_BLACK);
        tft.drawRect(z + 2, (y + 11), 4, 1, ST77XX_BLACK);
        tft.drawRect(z + 7, (y + 9), 1, 3, ST77XX_BLACK);
        tft.fillRect(z - 1, (y + 5), 8, 9, ST77XX_BLACK);
      }

      //do boneco ao lixo
      if ((y + 11) >= 171 && (y + 11) <= 200)
      {
        if ((z - 2) >= (x - 9) && (z + 7) <= (+(x + 24) + 9))

        {
          tft.fillRect(z, y, 5, 2, ST77XX_BLACK);
          tft.drawRect(z + 1, (y + 2), 3, 1, ST77XX_BLACK);
          tft.drawRect(z, (y + 3), 5, 1, ST77XX_BLACK);
          tft.drawRect(z - 1, (y + 4), 7, 1, ST77XX_BLACK);
          tft.drawRect(z - 2, (y + 6), 1, 6, ST77XX_BLACK);
          tft.drawRect(z + 2, (y + 11), 4, 1, ST77XX_BLACK);
          tft.drawRect(z + 7, (y + 9), 1, 3, ST77XX_BLACK);
          tft.fillRect(z - 1, (y + 5), 8, 9, ST77XX_BLACK);

          pontosNovo++;
          break;
        }
      }

      //colisao com o chao
      if ((y + 11) >= 210)
      {
        vida --;
        break;
      }

    }
  }
  tft.fillRect(0, 0 , 240, 240, ST77XX_BLACK);
  //o famoso game over
  tft.drawRect(0, 0, 239, 239, ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(65, 100);
  tft.print(F("GAME OVER"));
  delay(3000);

  return pontosNovo;
}

//boneco
void boneco (byte x)
{

  tft.fillRect(0, 175, 240, 40,ST77XX_BLACK);
  

  
  
  
  //boneco
  tft.fillRect(x, 175, 11, 12, ST77XX_WHITE);
  tft.fillRect(x + 6, 179, 5, 4, ST77XX_BLACK);
  tft.fillRect(x + 2, 188, 6, 13, RED);
  tft.fillRect(x + 8, 190, 7, 6, RED);
  tft.fillRect(x + 2, 200, 6, 9, BLUE);
  //lata de lixo
  tft.fillRect(x + 14, 179, 11, 18, GRAY);
  tft.drawRect(x + 14, 179, 11, 7, ST77XX_WHITE);
  tft.drawRect(x + 14, 187, 1, 8, ST77XX_WHITE);
  tft.drawRect(x + 16, 188, 1, 6, ST77XX_WHITE);
  tft.drawRect(x + 18, 189, 1, 4, ST77XX_WHITE);
  tft.drawRect(x + 20, 189, 1, 4, ST77XX_WHITE);
  tft.drawRect(x + 22, 188, 1, 6, ST77XX_WHITE);
  tft.drawRect(x + 24, 187, 1, 8, ST77XX_WHITE);
}
