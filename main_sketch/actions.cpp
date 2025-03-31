#include "wireless.h"
#include "buttons.h"
#include "display.h"
#include "cards.h"
#include "actions.h"

void hit(player *currPlay) 
{
    Card_Struct *newCard = (Card_Struct *)malloc(sizeof(Card_Struct)); // Sanity check- make sure the arduino is okay (sufficient memory cuz they're small)
    if (!newCard) {
        Serial.println("Memory allocation failed- womp womp");
        return;
    }
    
    *newCard = draw_Card();  // Assign the drawn card
    newCard->next = NULL;

    if (currPlay->head == NULL) {
        currPlay->head = newCard;
    } 
    
    else {
        Card_Struct *traverse = currPlay->head;
        while (traverse->next != NULL) {
            traverse = traverse->next;
        }
        traverse->next = newCard;
    }

    currPlay.totalSum += newCard->value;

    if (currPlay.totalSum > 21) {
        Serial.printf("Player %d loses! Total: %d\n", currPlay.playerNumber, currPlay.totalSum);
        currPlay.outOfGame = true;
    }
}

void stand(player *currPlay)
{
    Serial.printf("Player %d stands with a total: %d\n", currPlay.playerNumber, currPlay.totalSum);
    currPlay.stand = true;
}

void fold(player *currPlay)
{
    currPlay.totalBet *= 0.5;
    Serial.printf("Player %d folds! Half the bet is retracted, leaving %d. Player card total: %d\n", currPlay.playerNumber, currPlay.totalBet, currPlay.totalSum);
    currplay.fold = true;
}

void initialBet(player *currPlay)
{
    Serial.printf("Place your initial bet. You have %d dollars.\n", currPlay.tokens);
    
    int potBet = 0; //CHANGE THIS CHANGE THIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    while(potBet > currPlay.tokens) {
        Serial.println("Insufficient funds. Please put in a value within your range");
        //ash again when know how
    }
    currPlay.totalBet = potBet;
}

void doubleDown(player *currPlay)
{
    if (currPlay.tokens >= currPlay.totalBet) {
        currPlay.tokens -= currPlay.totalBet;
        currPlay.totalBet *= 2;
        Serial.printf("Player %d doubles down to bet %d.\n", currPlay.playerNumber, currPlay.totalBet);
    } else {
        Serial.println("Not enough tokens to double down.");
    }
}
