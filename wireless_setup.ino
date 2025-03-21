#include <SoftwareSerial.h>
CommandPacket packet;

SoftwareSerial HC12(11,12); // HC-12 TX Pin, HC-12 RX Pin

// Card struct definition
typedef struct {
  char suit;          // 'H' = Hearts, 'D' = Diamonds, 'C' = Clubs, 'S' = Spades
  int value;          // 1-13 (Ace to King)
  char friendlyName;  // 'A' for Ace, '2'-'9', 'T' for 10, 'J' for Jack, 'Q' for Queen, 'K' for King
} Card_struct;

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
    int ID;               // Player ID
    CommandType command;  // Command type
    Card_struct card;     // Card (optional)
    int bet;              // Bet amount (optional)
} CommandPacket;

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12

}

//TRANSMISSION STANDARD: \ID, COMMAND, CARD, BET/

void loop() {
if(HC12.available()) test_receive();
    CommandPacket packet;
    packet.ID = 0;
    packet.command = CMD_PING;
    packet.card = {'H', 1, 'A'};
    packet.bet = 0;
    test_send(packet);
    delay(3000);
}\

void test_send(CommandPacket packet){
    HC12.print("\\");
    HC12.print(packet.ID);
    HC12.print(",");
    HC12.print(packet.command);
    HC12.print(",");
    HC12.print(packet.card.suit);
    HC12.print(",");
    HC12.print(packet.card.value);
    HC12.print(",");
    HC12.print(packet.card.friendlyName);
    HC12.print(",");
    HC12.print(packet.bet);
    HC12.print("/");
    Serial.println("Sent Packet");

}

void test_receive(){
    while(HC12.peek() != '\\'){
        HC12.read();
    }

    HC12.read(); //remove the delimiter \
    packet.ID = HC12.parseInt();
    HC12.read(); //remove the delimiter ,
    packet.command = (CommandType)HC12.parseInt();
    HC12.read(); //remove the delimiter ,
    packet.card.suit = HC12.read();
    HC12.read(); //remove the delimiter ,
    packet.card.value = HC12.parseInt();
    HC12.read(); //remove the delimiter ,
    packet.card.friendlyName = HC12.read();
    HC12.read(); //remove the delimiter ,
    packet.bet = HC12.parseInt();
    HC12.read(); //remove the delimiter /
}