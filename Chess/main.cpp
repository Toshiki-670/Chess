//
//  main.cpp
//  Chess
//
//  Created by Toshiki on 7/16/16.
//  Copyright © 2016 Toshiki. All rights reserved.
//
#include <iostream>
#include "chess.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    cout << "Game start!" << endl;
    bool end = false;
    game chess;
    
    while(end == false){
        chess.show();
        if(chess.turn_now()){
            cout << "Player 2" << endl;
        }else{
            cout << "Player 1" << endl;
        }
        end = chess.in_out();
    }
    cout << "The winner is ";
    if(chess.turn_now()){
        cout << "Player 2" << endl;
    }else{
        cout << "Player 1" << endl;
    }
    chess.show();
    
    return 0;
}