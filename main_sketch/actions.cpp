#include "wireless.h"
#include "buttons.h"
#include "display.h"
#include "cards.h"
#include "actions.h"

extern Deck_struct* deck; // Declare the deck as an external variable

void hit(player* currPlay) 
{
    Card_struct *newCard = (Card_struct *)malloc(sizeof(Card_struct)); // Fixed type name
    if (!newCard) {
        Serial.println("Memory allocation failed");
        return;
    }
    
    *newCard = draw_card(deck);  // Fixed function name to match cards.h

    if (currPlay->head == NULL) {
        currPlay->head = newCard;
    } 
    else {
        Card_struct *traverse = currPlay->head;
        while (traverse->next != NULL) {
            traverse = traverse->next;
        }
        traverse->next = newCard;
    }

    currPlay->totalSum += newCard->value;

    if (currPlay->totalSum > 21) {
        currPlay->over21 = true;
    }
}

void stand(player *currPlay)
{
    //Serial.print("Player %d stands with a total: %d\n", currPlay.playerNumber, currPlay.totalSum);
    currPlay->stand = true;
}

void fold(player *currPlay)
{
    currPlay->totalBet *= 0.5;
    //Serial.print("Player %d folds! Half the bet is retracted, leaving %d. Player card total: %d\n", currPlay.playerNumber, currPlay.totalBet, currPlay.totalSum);
    currPlay->fold = true;
}

void initialBet(player *currPlay)
{
    //Serial.print("Place your initial bet. You have %d dollars.\n", currPlay.tokens);
    
    int potBet = 0; //CHANGE THIS CHANGE THIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    while(potBet > currPlay->totalMoney) {
        Serial.println("Insufficient funds. Please put in a value within your range");
        //ash again when know how
    }
    currPlay->totalBet = potBet;
}

void doubleDown(player *currPlay)
{
    if (currPlay->totalMoney >= currPlay->totalBet) {
        currPlay->totalMoney -= currPlay->totalBet;
        currPlay->totalBet *= 2;
        //Serial.print("Player %d doubles down to bet %d.\n", currPlay.playerNumber, currPlay.totalBet);
    } else {
        Serial.println("Not enough tokens to double down.");
    }
}
