#include "cards.h"
#include "player.h"

int tokens;
int numPlayers;
**Card_Struct p1,p2,p3,p4;
**Card_Struct[] players = {p1,p2,p3,p4} // setting empty array to hold poiters to each players linked list
//*int indAdd

typedef struct playerHand{
    
}

void startUp()
{
    //GET NUMBER OF PLAYERS//
    for (int i = 1; i <=numPlayers)
    {
        
    }
}



void hit(player *currPlay) 
{
    Card_Struct *newCard = (Card_Struct *)malloc(sizeof(Card_Struct));
    *newCard = draw_Card();  // Assign the drawn card
    newCard->next = NULL;    // Ensure new card's next is NULL

    Card_Struct *traverse = currPlay->head;
    
    if (traverse == NULL)  // Case when the list is empty
    {
        currPlay->head = newCard;
    }
    else
    {
        while (traverse->next != NULL)
        {
            traverse = traverse->next;
        }
        traverse->next = newCard;  // add new card at end
    }

    currPlay->totalSum += newCard->value;  // Update total sum
    
    if (currPlay->totalSum > 21)
    {
        //Make them lose- Talk to Erin / Yasmine
        printf("Player %d loses! Total: %d\n", currPlay.playerumber, currPlay->totalSum);
    }
}

void stand(player currPlay)
{
    //how to next turn?
    printf("Player %d stands with a total: %d\n", currPlay.playerumber, currPlay->totalSum);
}

void fold(player currPlay)
{
    printf("Player %d folds! hal the bet in retracted, leaving %d. Player card Total: %d\n", currPlay.playerumber, currPlay.totalBet *(1/2), currPlay->totalSum);
}

int initialBet(player currPlay)
{
    Serial.println("Place your initial bet. You have %n dollers.", tokens);
    

}

int doubleDown(player currPlay)
{
    if()
    currPlay.totalBet *= 2;
    return (currPlay.totalBet);
}