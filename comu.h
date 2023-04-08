#include "Arduino.h"

#define livre 11
#define ocupado 22
#define enviado 33

class Comu{
  byte _palavra[3];
  int _pos;
  int _saida;
  int _entrada;
  int _flag;
  int _pos_info;
  int _pos_recebe;
  int _count_up;
  int _count_down;
  byte _info_recebido;
  int _estado_recebimento;
  public:
    Comu(int, int);
    byte recebe(void);
    int envia(byte);
    void processa_envio(void);
    void processa_recebimento(void);
};
