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
const int rPin = 0;    // the number of the LED pin
const int gPin = 1;    // the number of the LED pin
const int bPin = 2;    // the number of the LED pin

const uint nodeId_1 = 1002424181;
const uint nodeId_2 = 1002419385;
const uint nodeId_3 = 211912477;
const uint nodeId_4 = 1002426133;

int ledPin = 0;
unsigned long last_send_time = 0;
int buttonState = 0;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  Serial.println('sending message');
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  Serial.println(msg);
  if (from ==  nodeId_1){
    digitalWrite(rPin, LOW);
  }
  if (from ==  nodeId_2){
    digitalWrite(gPin, LOW);
  }
  if (from ==  nodeId_3){
    digitalWrite(bPin, LOW);
  }
  if (from ==  nodeId_4){
    digitalWrite(bPin, LOW);
    digitalWrite(rPin, LOW);
  }


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
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  Serial.println(mesh.getNodeId());

  if (mesh.getNodeId() ==  nodeId_1){
    ledPin = rPin;
    Serial.println('node1');
  }
  if (mesh.getNodeId() ==  nodeId_2){
    ledPin=gPin;
    Serial.println('node2');
  }
  if (mesh.getNodeId() ==  nodeId_3){
    ledPin=bPin;
    Serial.println('node3');
  }
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  if ( (digitalRead(buttonPin) == LOW)&& (micros() >= last_send_time + 500000) ){
    last_send_time = micros();
    // turn LED on:
    digitalWrite(ledPin, LOW);
    if (mesh.getNodeId() ==  nodeId_4){
      digitalWrite(ledPin, HIGH);
      digitalWrite(rPin, LOW);
      digitalWrite(bPin, LOW);
    }
    String msg = "1_";
    msg += mesh.getNodeId();
    msg += mesh.getNodeTime();
    mesh.sendBroadcast( msg );
    Serial.printf("broadcasting %s\n", msg);
  } else {
    digitalWrite(rPin, HIGH);
    digitalWrite(gPin, HIGH);
    digitalWrite(bPin, HIGH);
  }
}