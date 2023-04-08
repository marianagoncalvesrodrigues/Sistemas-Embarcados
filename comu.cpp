#include "Comu.h"
#include "Arduino.h"

Comu::Comu(int saida, int entrada){
  _pos = 0;
  _entrada = entrada;
  _saida = saida;
  _flag=livre;
  _pos_info = 0;
  _pos_recebe;
  _count_up;
  _count_down;
  _info_recebido;
  _estado_recebimento = 1;
  pinMode(_entrada, INPUT);
  pinMode(_saida, OUTPUT);
  
}

byte Comu::recebe(void){
  if(_estado_recebimento == 3){
    return _info_recebido;
    _estado_recebimento = 1;
  }else{
    return ocupado;
  }
}

int Comu::envia(byte informacao){
  if(_flag == livre){
    _palavra[0] = 0xAA;
    _palavra[1] = informacao;
    _palavra[2]= 0xCC;
    _flag=ocupado;
    return enviado;
  }else{
    return ocupado;
  }
  
}


void Comu::processa_envio(void){
  if(_pos_info < 3 && _flag == ocupado){ 
    if(_pos<8){
      if(_palavra[_pos_info] & 0x80){ //0b10000000
        digitalWrite(_saida, HIGH);
        //Serial.print("1");
        //Serial.println(_saida);
      }else{
        digitalWrite(_saida, LOW);
        //Serial.print("0");
      }
      _pos++;
      _palavra[_pos_info]=_palavra[_pos_info]<<1;
      
    }
    if(_pos>=8){
    _pos=0;
    _pos_info++;
    //Serial.print(".");
    }
  }

  if(_pos_info >= 3){
    _pos_info=0;
    _flag = livre;
    //Serial.println(" ");
  }
}

void Comu::processa_recebimento(void){
  switch(_estado_recebimento){
    case 1:
        if(digitalRead(_entrada)){
          _count_up++;
          _count_down = 0;
        }else{
          _count_down++;
          _count_up = 0;
        }
        Serial.print(_count_up);
        if(_count_up >= 9){
           _count_up = 0;
          _info_recebido = _info_recebido | 0b00000001;
          _info_recebido=_info_recebido << 1;
        }else if(_count_down >=9){
          _count_down = 0;
          _info_recebido = _info_recebido & 0b11111110;
          _info_recebido=_info_recebido << 1;
        }
        if(_info_recebido == 0xAA){
          _estado_recebimento = 2;
        }
        break;
    case 2:
       if(digitalRead(_entrada)){
          _count_up++;
          _count_down = 0;
        }else{
          _count_down++;
          _count_up = 0;
        }
        if(_count_up >= 9){
          _count_up = 0;
          _info_recebido = _info_recebido | 0b00000001;
          _info_recebido=_info_recebido << 1;
          _pos_recebe++;
        }else if(_count_down >=9){
           _count_down = 0;
          _info_recebido = _info_recebido & 0b11111110;
          _info_recebido=_info_recebido << 1;
          _pos_recebe++;
        }
        if(_pos_recebe == 8){
          _pos_recebe = 0;
          _estado_recebimento = 3;
        }
        break;
    case 3:
      break;
      
  }

  
}
