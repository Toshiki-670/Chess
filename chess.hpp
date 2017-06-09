//
//  piece.hpp
//  Chess
//
//  Created by Toshiki on 7/16/16.
//  Copyright © 2016 Toshiki. All rights reserved.
//
#ifndef piece_hpp
#define piece_hpp
using namespace std;

struct piece{
      bool on;
      bool player;
     short id;
       int cnt;
    string icon;
};
struct yx{int y; int x;};

class game{
private:
    piece field[8][8], back[8][8], null;
    bool turn;
    int i, j;
    
    void move(const int *); //倒す; 自駒, 敵駒
    bool movable(const int *, short);
    bool decition(const int *);
    int check(const int *);
    int check2(int, int, bool);
    
    void backup(void);
    void redo(void);
    void turn_change(void);
    
    bool king(const int *);
    bool queen(const int *);
    bool bishop(const int *);
    bool knight(const int *);
    bool rook(const int *);
    bool pawn(const int *, bool);
public:
    game();
    ~game();
    void show(void);
    bool in_out(void);
    bool turn_now(void);
};
#endif /* piece_hpp */