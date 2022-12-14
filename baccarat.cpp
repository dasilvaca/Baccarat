#include<bits/stdc++.h>

using namespace std;

int player_points;
int banker_points;

vector<int> player_cards_idx;
vector<int> banker_cards_idx;

string card_names[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"}; // Card names in deck
int cards_shoe[13]; // each value corresponds to the card value in deck. ex: {Ace, 2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King}
int cards_left_in_shoe[13]; // number of cards left in shoe. Ex: {32, 32, 32, 32, ..., 32}

int total_ammount_of_cards = 416; // 416 cards in shoe. Because there are 8 decks of cards

enum bet_type {player = 1, banker, will_tie, none}; // bet types

enum bet_type chosen_bet;
enum bet_type winner = none;

void choose_bet(){
    int bet;
    bool valdid_bet = false;
    while (!valdid_bet){
        cout << "Enter bet (who you thinkis going to win?, choose an ): \n1. Player\n2. Banker\n3. Tie\n";
        cin >> bet;
        switch (bet){
        case player:
            cout << "You chose player\n";
            chosen_bet = player;
            valdid_bet = true;
            break;
        case banker:
            cout << "You chose banker\n";
            chosen_bet = banker;
            valdid_bet = true;
            break;
        case will_tie:
            cout << "You chose tie\n";
            chosen_bet = will_tie;
            valdid_bet = true;
            break;
        default:
            cout << "Invalid bet\n";
            break;
    }
    }
}

void initialize_cards_shoe() {
    for (int i = 0; i < 13; i++) {
        if (i < 9) {
            cards_shoe[i] = i + 1;
        } else {
            cards_shoe[i] = 0;
        }
        cards_left_in_shoe[i] = 32;
    }
    total_ammount_of_cards = 416;
    player_points = 0;
    banker_points = 0;
}

int actual_points(vector<int> &actual_cards){
    int sum_cards = 0;
    for (int i = 0; i < actual_cards.size(); i++){
        sum_cards += cards_shoe[actual_cards[i]] % 10;
    }
    sum_cards = sum_cards % 10;

    return sum_cards;
}

void give_card(vector<int> &actual_cards){
    int card_idx = rand() % 13;
    while (cards_left_in_shoe[card_idx] == 0){
        card_idx = rand() % 13;
    }
    actual_cards.push_back(card_idx);
    cards_left_in_shoe[card_idx] -= 1;
    total_ammount_of_cards -= 1;
}

void burn_card(){
    int card_idx = rand() % 13;
    while (cards_left_in_shoe[card_idx] == 0){
        card_idx = rand() % 13;
    }
    cards_left_in_shoe[card_idx] -= 1;
    total_ammount_of_cards -= 1;
}

bool player_rules(vector<int> &actual_cards, int &actual_points){ // does player need another card?
    if (actual_points <= 5){
        give_card(actual_cards);
        return true;
    }
    return false;
}

bool banker_rules(bool &player_got_card){
    if (player_got_card){
        if (banker_points <= 2){
            give_card(banker_cards_idx);
            return true;
        }
        else if (banker_points == 3){
            if (cards_shoe[player_cards_idx[2]] != 8){
                give_card(banker_cards_idx);
                return true;
            }
        }
        else if (banker_points == 4){
            if (cards_shoe[player_cards_idx[2]] >= 2 && cards_shoe[player_cards_idx[2]] <= 7){
                give_card(banker_cards_idx);
                return true;
            }
        }
        else if (banker_points == 5){
            if (cards_shoe[player_cards_idx[2]] >= 4 && cards_shoe[player_cards_idx[2]] <= 7){
                give_card(banker_cards_idx);
                return true;
            }
        }
        else if (banker_points == 6){
            if (cards_shoe[player_cards_idx[2]] >= 6 && cards_shoe[player_cards_idx[2]] <= 7){
                give_card(banker_cards_idx);
                return true;
            }
        }

    } else {
        return player_rules(banker_cards_idx, banker_points);
    }
    return false;
}

void get_cards_on_string(vector<int> &actual_cards){
    for (int i = 0; i < actual_cards.size(); i++){
        if (i == actual_cards.size() - 1){
            cout << card_names[actual_cards[i]] << " - ";
        } else {
            cout << card_names[actual_cards[i]] << ",";
        }
    }
}

void output(){
    cout<<"PHand - BHand - Outcome\n";
    get_cards_on_string(player_cards_idx);
    get_cards_on_string(banker_cards_idx);
    switch (winner){
    case player:
        cout << "Player wins\n";
        break;
    case banker:
        cout << "Banker wins\n";
        break;
    case will_tie:
        cout << "Tie\n";
        break;
    default:
        cout << "No winner\n";
        break;
    }
}

int main(){
    srand ( time(NULL) );
    initialize_cards_shoe();
    // choose_bet(); //! Just in case a bet must have to be chosen

    give_card(player_cards_idx);
    give_card(banker_cards_idx);
    give_card(player_cards_idx);
    give_card(banker_cards_idx);
    //? As the third card of each player is not shown. For the purpose of the simulation, the card is not going to be given yet.
    burn_card();

    bool player_got_card = false;
    bool banker_got_card = false;

    player_points = actual_points(player_cards_idx);
    banker_points = actual_points(banker_cards_idx);

    if (player_points >= 8 || banker_points >= 8){ // Verify if at this point, someone has won
        if (player_points > banker_points){
            winner = player;
        } else if (player_points < banker_points){
            winner = banker;
        } else {
            winner = will_tie;
        }
    }
    else{
        player_got_card = player_rules(player_cards_idx, player_points);
        banker_got_card = banker_rules(player_got_card);
        player_points = actual_points(player_cards_idx);
        banker_points = actual_points(banker_cards_idx);

        if (player_points > banker_points){
            winner = player;
        } else if (player_points < banker_points){
            winner = banker;
        } else {
            winner = will_tie;
        }
    }

    output(); 

    return 0;
}

