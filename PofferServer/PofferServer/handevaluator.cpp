#include "handevaluator.h"

//////////////////////////////////////////////////////////////////////////////
bool HandEvaluator::isGoldenHand(const QVector<Card>& sortedCards){
    try{
        bool flag = true;
        for(int i=0; i<5 ; i++ ){
            if(sortedCards[0].getSuit() != sortedCards[i].getSuit()){  // checking type of 5 cards: true -> continue
                flag = false;
                break;
            }
        }

        if(!flag) {
            throw HandEvaluator_exception("Hand is not GoldenHand!");
        }
        // bit , king , queen , solder  , ten
        bool isGold = (sortedCards[4].getRank() == Card::BITCOIN) &&
                      (sortedCards[3].getRank() == Card::KING) &&
                      (sortedCards[2].getRank()==Card::QUEEN) &&
                      (sortedCards[1].getRank()==Card::SOLDIER) &&
                      (sortedCards[0].getRank()==Card::TEN);

        return isGold;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return false ;
    }
}
//////////////////////////////////////////////////////////////////////////////
bool HandEvaluator::isOrderHand(const QVector<Card>& sortedCards){
    try{
        bool flag = true;
        for(int i=0; i<4 ; i++ ){
            // check type and sequence : arr[i]+1 = arr[i+1]
            if((sortedCards[0].getSuit() != sortedCards[i].getSuit()) || sortedCards[i].getRank() + 1  != sortedCards[i+1].getRank() ){  // checking type of 5 cards: true -> continue
                flag = false;
                break;
            }
        }


        if(!flag) {
            throw HandEvaluator_exception("Hand is not orderHand!");
        }

        if(sortedCards[4].getRank()==Card::BITCOIN){
            throw HandEvaluator_exception("Hand is not orderHand :: warrning highest value is bitcoin so Hand most be golden hand! check out again!");
        }

        return true;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return false ;
    }
}
//////////////////////////////////////////////////////////////////////////////
bool HandEvaluator::isFourOfAKind(const QVector<Card>& sortedCards){
    try{
     // 2 way: 1: aaaa B
     // 2: B aaaa
     // means just compare index
        bool isFour = (sortedCards[0].getRank() == sortedCards[3].getRank()) || (sortedCards[1].getRank()==sortedCards[4].getRank());
        return isFour;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return false ;
    }
}
//////////////////////////////////////////////////////////////////////////////
bool HandEvaluator::isPenthouse(const QVector<Card>& sortedCards){
    try{
        // AA BBB
        // BBB AA
        bool isPent = (
                        ((sortedCards[0].getRank() == sortedCards[1].getRank()) && (sortedCards[2].getRank() == sortedCards[4].getRank())) ||
                        ((sortedCards[0].getRank() == sortedCards[2].getRank()) && (sortedCards[3].getRank() == sortedCards[4].getRank()))
                       );

        return isPent;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return false ;
    }
}
//////////////////////////////////////////////////////////////////////////////
bool HandEvaluator::isMscHand(const QVector<Card>& sortedCards){
    try{
        bool flag = true;
        for(int i=0; i<5 ; i++ ){
            if(sortedCards[0].getSuit() != sortedCards[i].getSuit()){  // checking type of 5 cards: true -> continue
                flag = false;
                break;
            }
        }
        if(flag)
            return true;

        return false;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return false ;
    }
}
//////////////////////////////////////////////////////////////////////////////
bool HandEvaluator::isSeries(const QVector<Card>& sortedCards){
    try{
        bool flag = true;
        for(int i=0 ; i<4 ; i++ ){
            if(sortedCards[i].getRank()+1 != sortedCards[i+1].getRank()){
                flag = false ;
                break;
            }
        }

        return flag;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return false ;
    }
}
//////////////////////////////////////////////////////////////////////////////
bool HandEvaluator::isThreeOfAKind(const QVector<Card>& sortedCards){
    try{
        // AAA b c
        // b AAA c
        // bc AAA
        bool isThree = (sortedCards[0].getRank() == sortedCards[2].getRank()) ||
                       (sortedCards[1].getRank() == sortedCards[3].getRank()) ||
                       (sortedCards[2].getRank() == sortedCards[4].getRank());
        return isThree;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return false ;
    }
}
//////////////////////////////////////////////////////////////////////////////
bool HandEvaluator::isDoublePair(const QVector<Card>& sortedCards){
    try{
        // AA b CC
        // AA CC b
        // b AA CC
        bool isDouble = ((sortedCards[0].getRank() == sortedCards[1].getRank()) && (sortedCards[3].getRank() == sortedCards[4].getRank()) ) ||
                        ((sortedCards[0].getRank() == sortedCards[1].getRank()) && (sortedCards[2].getRank() == sortedCards[3].getRank()) ) ||
                        ((sortedCards[1].getRank() == sortedCards[2].getRank()) && (sortedCards[3].getRank() == sortedCards[4].getRank()) ) ;

        return isDouble;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return false ;
    }
}
//////////////////////////////////////////////////////////////////////////////
bool HandEvaluator::isSinglePair(const QVector<Card>& sortedCards){
    try{
        // AA b c d ---> 2 card is neibhor
        for (int i=0 ; i < 4 ; i++){
            if(sortedCards[i].getRank() == sortedCards[i+1].getRank())
                return true;
        }

        return false ;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return false ;
    }
}
//////////////////////////////////////////////////////////////////////////////
/// \brief HandEvaluator::evaluateHand
/// \param hand
/// \return HandRank
///this function return a value to show value hand that possible to compare and determine wiinner
HandEvaluator::HandRank HandEvaluator::evaluateHand(const Hand& hand){
    try{
        if(hand.getCards().size() != 5){
            throw HandEvaluator_exception("The number of cards in hand is less than 5!");
        }

        QVector<Card> cards = hand.getCards();

        // sroot here because make simply is(fun) functions
        sort(cards.begin() , cards.end() , [](const Card & a , const Card & b){ return a.getRank() < b.getRank(); });


        if (isGoldenHand(cards)) return GOLDEN_HAND;
        if (isOrderHand(cards)) return ORDER_HAND;
        if (isFourOfAKind(cards)) return FOUR_OF_A_KIND;
        if (isPenthouse(cards)) return PENTHOUSE;
        if (isMscHand(hand.getCards())) return MSC_HAND;
        if (isSeries(cards)) return SERIES;
        if (isThreeOfAKind(cards)) return THREE_OF_A_KIND;
        if (isDoublePair(cards)) return DOUBLE_PAIR;
        if (isSinglePair(cards)) return SINGLE_PAIR;

        return MESSY_HAND;
    }
    catch(const HandEvaluator_exception& err){
        err.outputOnDebug();
        return HandRank::INVALID_HAND;
    }
}
