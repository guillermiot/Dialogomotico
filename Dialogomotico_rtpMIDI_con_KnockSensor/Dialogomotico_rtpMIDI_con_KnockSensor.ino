/*
Maestría en Artes Electrónicas UNTREF - Soportes y Tecnologías: Electrónica
Guillermo Miotti
1 AÑO - COHORTE 2020
Realizado en proyecto grupal con Juan Miceli
Activacion de Relays por Sensor de golpe y comandos MIDI por WIFI en ESP32
*/

//Se declaran e incluyen las librerias para la comunicacion wifi/midi
#include <AppleMIDI.h>
USING_NAMESPACE_APPLEMIDI

#include <Control_Surface.h>
#include <MIDI_Interfaces/Wrappers/FortySevenEffects.hpp>

#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "WiFi-Credentials.h" // See instructions above

//Declaraciones para el Knock Sensor ----------------------------- //

int MIC=36; // D0 de modulo a pin 36
int LED=2; // respuesta a pin 2
int VALOR; // variable para almacenar valor de D0
int ESTADO; // variable para almacenar estado del LED

// Interface MIDI ----------------------------- //

// creamos la instancia AppleMIDI con puerto, tipo de instancia, nombre de instancia y puerto local
APPLEMIDI_CREATE_INSTANCE(WiFiUDP, MIDI, "ESP32", 5004);

// se empaqueta todo en una interface de control MIDI
FortySevenEffectsMIDI_Interface<decltype(MIDI) &> AppleMIDI_interface = MIDI;

// Elementos MIDI ----------------------------- //

// sumamos notas MIDI

using namespace MIDI_Notes;
NoteButton buttonA = {
    4, note(C,3),  // Pulsador GPIO0
};
NoteValueLED ledA = {
    LED_BUILTIN, note(C,3),
};

///////////

NoteButton buttonB = {
    15, note(D,3),  // Pulsador GPIO2
};
NoteValueLED ledB = {
    LED_BUILTIN, note(D,3),
};

///////////

NoteButton buttonC = {
    13, note(E,3),  // Pulsador GPIO13
};
NoteValueLED ledC = {
    LED_BUILTIN, note(E,3),
};

///////////

NoteButton buttonD = {
    12, note(F,3),  // Pulsador GPIO12
};
NoteValueLED ledD = {
    LED_BUILTIN, note(F,3),
};

///////////

NoteButton buttonE = {
    14, note(G,3),  // Pulsador GPIO14
};
NoteValueLED ledE = {
    LED_BUILTIN, note(G,3),
};

///////////

NoteButton buttonF = {
    27, note(A,3),  // Pulsador GPIO27
};
NoteValueLED ledF = {
    LED_BUILTIN, note(A,3),
};

///////////

NoteButton buttonG = {
    33, note(B,3),  // Pulsador GPIO33
};
NoteValueLED ledG = {
    LED_BUILTIN, note(B,3),
};

///////////

NoteButton buttonH = {
    32, note(C,4),  // Pulsador GPIO32
};
NoteValueLED ledH = {
    LED_BUILTIN, note(C,4),
};

///////////

// Llamadas de libreria AppleMIDI -------------------------- //

void onAppleMidiConnected(const ssrc_t &ssrc, const char *name) {
  Serial << F("Connected to session ") << name << endl;
}
void onAppleMidiDisconnected(const ssrc_t &ssrc) {
  Serial << F("Disconnected") << endl;
}
void onAppleMidiError(const ssrc_t &ssrc, int32_t err) {
  Serial << F("Exception ") << err << F(" from ssrc 0x") << hex << ssrc << dec
         << endl;
}

//Setup --------------------------------- //

void setup() {
/////////////////// KNOCK/////////////////////
 pinMode(MIC,INPUT); // pin 36 como entrada
 pinMode(LED,OUTPUT); // pin 2 como salida
 ////////////////////////////////////////////
 
  Serial.begin(115200);

  // Conenxion WiFi
  Serial << endl << F("Connecting to ") << ssid << ' ';
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    Serial.print("."), delay(250);
  Serial << endl
         << F("Connected!") << endl
         << F("IP address: ") << WiFi.localIP() << endl;

  // Setup mDNS:
  if (!MDNS.begin(AppleMIDI.getName()))
    FATAL_ERROR(F("Error setting up MDNS responder!"), 0x0032);
  Serial << F("mDNS responder started (") << AppleMIDI.getName() << ".local)" 
         << endl;
  MDNS.addService("apple-midi", "udp", AppleMIDI.getPort());

  // Seteo de AppleMIDI callbacks
  AppleMIDI.setHandleConnected(onAppleMidiConnected);
  AppleMIDI.setHandleDisconnected(onAppleMidiDisconnected);
  AppleMIDI.setHandleError(onAppleMidiError);

  // Se inicialice el control MIDI
  Control_Surface.begin();
}

//Loop ---------------------------------- //

void loop() {
  // Update de elementos MIDI
  Control_Surface.loop();

  /////////////////////sector de KnockSensor/////////////////////////
  
    VALOR = digitalRead(MIC);   // obtiene valor de D0
  if (VALOR == HIGH){     // si D0 tiene un valor alto
    ESTADO = digitalRead(LED);    // almacena estado del led (encendido o apagado)
    digitalWrite(LED, !ESTADO);   // escribe en LED el valor opuesto al que tenia
    delay(500);       // demora de medio seg. para evitar pulsos parasitos
  }
  }
