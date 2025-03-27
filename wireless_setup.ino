#include <SoftwareSerial.h>

#define PROGRAM false; // Set to false to run the program, true to program the HC12

// Define available commands
typedef enum {
    CMD_NONE = 0,           // Default / No command
    CMD_PING,               // Ping command
    CMD_ACK,                // Acknowledge
    CMD_RECEIVE_THIS_CARD,  // Send card data
    CMD_HIT,                // Player requests a hit
    CMD_STAY,               // Player chooses to stay
    CMD_FOLD,               // Player folds
    CMD_WIN,                // Player wins this game
    CMD_LOSE                // Player lost this game
} CommandType;

typedef struct {
  char suit;          // 'H' = Hearts, 'D' = Diamonds, 'C' = Clubs, 'S' = Spades
  int value;          // 1-13 (Ace to King)
  char friendlyName;  // 'A' for Ace, '2'-'9', 'T' for 10, 'J' for Jack, 'Q' for Queen, 'K' for King
} Card_struct;


// Struct to hold a command and optional card data
typedef struct {
    int ID; // Player ID
    int betAmount; // bet amount
    CommandType command;  // Command type
    Card_struct card;     // Card (optional)
} CommandPacket;


SoftwareSerial HC12(12, 11);  // TX: 12, RX: 11

void setup(){
  Serial.begin(9600);
  HC12.begin(9600);
  delay(3000);
}

void loop(){
  if(PROGRAM){
    while(1){
        if(HC12.available()) debugRead();
        HC12.print(Serial.read());
    }
  } else {
    while(1){
        if(HC12.available()) hc12_receive();
        Card_struct card = {'H', 1, 'A'};
        CommandPacket packet = {1, 10, CMD_PING, card};  // Changed to CMD_PING for testing
        hc12_send(packet);
        delay(5000);
    }
  }

}


// Function to send a command packet
void hc12_send(CommandPacket packet) {
    // Message order: <ID, CMD, BET, (SUIT, VALUE, FRIENDLYNAME)>
    String message = '<' + String(packet.ID) + "," + String(packet.command) + "," + String(packet.betAmount) + "," + '(' + String(packet.card.suit) + ',' 
    + String(packet.card.value) + ',' + String(packet.card.friendlyName) + ')' + '>';    
    Serial.println("sent: " + message);
    // Send the string via HC12
    HC12.print(message);
}

// Function to receive a command packet
void hc12_receive() {
    String data = "";
  
    // If HC-12 has data, read it until '>'
    if (HC12.peek() == '<') {
        while (HC12.available()) {        
            char receivedChar = (char)HC12.read();
            data += receivedChar;
            if (receivedChar == '>'){
              // DEBUG
              Serial.println("rcv: " + data);
              break;  // Stop reading when '>' is found
            }
        }
    } else {
      while(HC12.peek() != '<') Serial.print(HC12.read()); // consume char
    }

    CommandPacket packet; // Create a new packet
    packet.ID = data.substring(1, data.indexOf(",")).toInt(); // Extract ID
    data = data.substring(data.indexOf(",") + 1); // Remove ID from string
    packet.command = (CommandType)data.substring(0, data.indexOf(",")).toInt(); // Extract command
    data = data.substring(data.indexOf(",") + 1); // Remove command from string
    packet.betAmount = data.substring(0, data.indexOf(",")).toInt(); // Extract bet amount
    data = data.substring(data.indexOf("(") + 1); // Remove bet amount from string
    packet.card.suit = data.substring(0, data.indexOf(",")).charAt(0); // Extract suit
    data = data.substring(data.indexOf(",") + 1); // Remove suit from string
    packet.card.value = data.substring(0, data.indexOf(",")).toInt(); // Extract value
    data = data.substring(data.indexOf(",") + 1); // Remove value from string
    packet.card.friendlyName = data.substring(0, data.indexOf(",")).charAt(0); // Extract friendly name
        
    // Process command or do something with the packet here
    return;
}

void debugRead(){
  if(Serial.available()){
    Serial.print(HC12.read());
  }
}
