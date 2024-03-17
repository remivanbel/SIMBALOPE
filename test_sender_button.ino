/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-mesh-esp32-esp8266-painlessmesh/
  
  This is a simple example that uses the painlessMesh library: https://github.com/gmag11/painlessMesh/blob/master/examples/basic/basic.ino
*/
#include "painlessMesh.h"

#define   MESH_PREFIX     "simba"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5554

const int buttonPin = 3;  // the number of the pushbutton pin
const int ledPin = 0;    // the number of the LED pin
unsigned long last_send_time = 0;
int buttonState = 0;

painlessMesh  mesh;

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  digitalWrite(ledPin, HIGH);
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  if ((digitalRead(buttonPin) == LOW) && (micros() >= last_send_time + 500000)) {
    last_send_time = micros();
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    String msg = "1_";
    msg += mesh.getNodeId();
    msg += mesh.getNodeTime();
    mesh.sendBroadcast( msg );
    Serial.printf("broadcasting %s\n", msg);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
  
}