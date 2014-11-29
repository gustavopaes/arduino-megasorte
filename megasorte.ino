/**
 * Gera números para o sorteio da Mega da Virada!
 * @author Gustavo Paes <gustavo.paes@gmail.com>
 */

#include <LiquidCrystal.h>

// LCD de 16 pinos
// 16 colunas / 2 linhas
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Botão para gerar números
const int BotaoGeraNumeros = 8;

// Botão para reset dos números
const int BotaoResetar = 9;

// Guarda o estado atual do botão
int podeResetar = LOW;
int podeGerar = LOW;

// Qual o estado atual do sistema
// 0 -> resetado
// 1 -> gerando números
// 2 -> números gerados, exibindo no lcd
int estado = 0;

// total de números que devem ser sorteados
const int MAX_NUMBERS = 6;

// maior número que pode ser sorteado
const int MAJOR_NUMBER = 60;

const int CURSOR_RESULTADO[MAX_NUMBERS][2] = {
  {0, 0}, {3, 0}, {6, 0},
  {0, 1}, {3, 1}, {6, 1}
};

void setup() {
  lcd.begin(16, 2);
  
  pinMode(BotaoGeraNumeros, INPUT);
  pinMode(BotaoResetar, INPUT);

  Serial.begin(9600);
  
  resetaNumeros();
}

void gerarNumeros() {
  Serial.println("gerarNumeros()");
  lcd.clear();
  delay(300);

  estado = 1;

  // qual o número que está sendo sorteado
  int numberIndex = 0;

  // número sorteado
  int randomNumber;

  // números sorteados
  int randomNumbers[MAX_NUMBERS + 1] = { };

  // String para guardar o número sorteado
  // e tratar com zero a esquerda, para manter
  // sempre o mesmo número de dígitos
  char strRandomNumber[2];

  while(numberIndex < MAX_NUMBERS) {
    randomNumber = sortearNumero(randomNumbers, numberIndex + 1);
    sprintf(strRandomNumber, "%d", randomNumber);

    // adiciona um 0 a esquerda, para manter
    // sempre com dois digitos
    if(randomNumber < 10) {
      sprintf(strRandomNumber, "0%d", randomNumber);
    }

    Serial.print("Numero sorteado: ");
    Serial.println(randomNumber);

    // posição do cursor
    int cursorColumn = CURSOR_RESULTADO[numberIndex][0];
    int cursorRow    = CURSOR_RESULTADO[numberIndex][1];

    // faz suspense com jogatina
    rodaJogatina(cursorColumn, cursorRow);

    // escreve o número sorteado no LDC, com
    // formatação de dois dígitos
    lcd.setCursor(cursorColumn, cursorRow);
    lcd.print(strRandomNumber);

    numberIndex += 1;

    delay(100);
  }
  
  boaSorte();
  
  estado = 2;
}

void boaSorte() {
  lcd.setCursor(11, 0);
  lcd.print("Boa");
  
  lcd.setCursor(10, 10);
  lcd.print("Sorte!");
}

int sortearNumero(int sorteados[], int index) {
  int number;

  // indica se o número está repetido
  boolean tryAgain;

  do {
    // sorteia um número
    number = random(1, MAJOR_NUMBER + 1);

    tryAgain = false;

    int t = 0;
    while(t < index) {
      if(sorteados[t] == number) {
        // é um número repetido
        tryAgain = true;
        break;
      }
      
      t += 1;
    }
    
  } while(tryAgain == true);
  
  // armazena o número sorteado, para fazer futuras comparações.
  sorteados[index] = number;

  return number;
}

/**
 * FAz efeito de jogatina, para dar mais emoção
 * ao sorteio dos números.
 */
void rodaJogatina(int column, int row) {
  Serial.println("rodaJogatina()");

  int jogatinaTime = 9;

  // apenas faz uma brincadeira de jogatina
  // para exibir números aleatórios antes de exibir
  // o número já sorteado.
  while(jogatinaTime--) {
    lcd.setCursor(column, row);
    lcd.print(random(1, MAJOR_NUMBER + 1));
    delay(100);
  }
}

/**
 * Reseta o display LCD e adiciona mensagem
 * para usuário.
 */
void resetaNumeros() {
  Serial.println("podeResetar()");
  lcd.clear();
  delay(300);

  // define o estado da maquina  
  estado = 0;

  // mensagem primeira linha  
  lcd.setCursor(0, 0);
  lcd.print("Maquina da sorte");
  
  // mensagem segunda linha
  lcd.setCursor(1, 1);
  lcd.print("Clique e ganhe!");
}

void loop() {
  //Serial.println("loop()");

  // estado do botão, se clicado (HIGH), se não clicado (LOW)
  podeResetar = digitalRead(BotaoResetar);
  podeGerar = digitalRead(BotaoGeraNumeros);

  //Serial.print(podeResetar);
  //Serial.print(" - ");
  //Serial.println(podeGerar);
  
  // só aceita ação nos botões quando o estado for diferente de 1
  if(estado != 1) {
    
    if(podeResetar == HIGH) {
      // faz o reset do LCD
      resetaNumeros();
    }
    
    else if(podeGerar == HIGH) {
      // gera os números
      gerarNumeros();
    }
    
  }
  // Temos espaço para 16 dígitos por linha.
  // Considerando sempre dois dígitos para cada um dos 6 número da mega-sena,
  // vamos usar no total 12 espaços para os dígitos.
 
  delay(300);
}
