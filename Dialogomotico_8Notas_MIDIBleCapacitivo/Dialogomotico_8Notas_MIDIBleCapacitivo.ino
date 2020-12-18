/*
Maestría en Artes Electrónicas UNTREF - Soportes y Tecnologías: Electrónica
Guillermo Miotti
1 AÑO - COHORTE 2020
Realizado en proyecto grupal con Juan Miceli
Activacion de comandos/notas MIDI por Bluetooth en ESP32
*/
//Mod de Guille a 8 notas
//Declaramos librerias e instancias

#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>
//#include <hardware/BLEMIDI_ESP32.h>


BLEMIDI_CREATE_INSTANCE("ESP32", MIDI)

bool estaConectado = false;
int canalMIDI = 1; //Canal MIDI donde se realizará la comunicación

// Variables para el uso de los sensores capacitivos---------------------------
int sensCap[] = {T0, T3, T4, T5, T6, T7, T9, T8}; //Entradas capacitivas
int valorSC[] = {0, 0, 0, 0, 0, 0, 0, 0};
int umbral[] = {60, 60, 60, 60, 60, 60, 60, 60};
int cantidadLecturas = 50;
int cantidadSC = 8;
int notas[] = {60, 62, 64, 65, 67, 69, 71, 72}; //notas MIDI
boolean estadoActual[] = {0, 0, 0, 0, 0, 0, 0, 0};
boolean estadoAnterior[] = {0, 0, 0, 0, 0, 0, 0, 0};

// Setup ----------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  MIDI.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  BLEMIDI.setHandleConnected(conectado);
  BLEMIDI.setHandleDisconnected(desconectado);

}


// Loop ------------------------------------------------------------------------
void loop()
{

  if (estaConectado)
  {
    // -------sensores capacitivos a notas -----------------------------------
    for (int e = 0; e < cantidadSC; e++) {
      for (int i = 0; i < cantidadLecturas; i++) { //corro un ciclo For la cantidad de veces equivalente
        // a la cantidad de lecturas que quiero guardar
        valorSC[e] = valorSC[e] + touchRead(sensCap[e]); // sumo la lectura actual en cada ejecución del ciclo
      }
      valorSC[e] = valorSC[e] / cantidadLecturas; // divido por la cantidad de lecturas realizadas
      if (valorSC[e] <= umbral[e]) { //Si el valor promediado es menor o igual al umbral
        estadoActual[e] = true; //cambio el valor a verdadero
      }
      else {
        estadoActual[e] = false; // dejo en falso
      }
      if (estadoActual[e] != estadoAnterior[e]) { // si el estado actual es distinto del anterior
        if (estadoActual[e] == true) { //Si además es verdadero
          MIDI.sendNoteOn(notas[e], 100, canalMIDI); //envío nota MIDI correspondiente con velocity de 100
        }
        estadoAnterior[e] = estadoActual[e]; // actualizo el estado
        if (estadoActual[e] == false) { //Si es falso
          MIDI.sendNoteOff(notas[e], 100, canalMIDI); //"apago" nota MIDI
        }
      }
    }
  }
  Serial.print ("T0: "),
               Serial.print (valorSC[0]);
  Serial.print ("\t");
    Serial.print ("T3: "),
               Serial.print (valorSC[1]);
  Serial.print ("\t");
  Serial.print ("T4: "),
               Serial.print (valorSC[2]);
  Serial.print ("\t");
  Serial.print ("T5: "),
               Serial.print (valorSC[3]);
  Serial.print ("\t");
  Serial.print ("T6: "),
               Serial.print (valorSC[4]);
  Serial.print ("\t");
  Serial.print ("T7: "),
               Serial.print (valorSC[5]);
  Serial.print ("\t");
  Serial.print ("T8: "),
               Serial.print (valorSC[6]);
  Serial.print ("\t");
  Serial.print ("T9: "),
               Serial.println (valorSC[7]);
}

// -----------------------------------------------------------------------------
// Dispositivo Conectado
// -----------------------------------------------------------------------------
void conectado() {
  estaConectado = true;
  digitalWrite(LED_BUILTIN, HIGH);
}

// -----------------------------------------------------------------------------
// Dispositivo Desonectado
// -----------------------------------------------------------------------------
void desconectado() {
  estaConectado = false;
  digitalWrite(LED_BUILTIN, LOW);
}

// -----------------------------------------------------------------------------
// Recibido Nota ON
// -----------------------------------------------------------------------------
void OnNoteOn(byte channel, byte note, byte velocity) {
}

// -----------------------------------------------------------------------------
// Recibido Nota OFF
// -----------------------------------------------------------------------------
void OnNoteOff(byte channel, byte note, byte velocity) {
}
