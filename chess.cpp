//
//  piece.cpp
//  Chess
//
//  Created by Toshiki on 7/16/16.
//  Copyright © 2016 Toshiki. All rights reserved.
//
#include <iostream>
#include <ctype.h>
#include <math.h>
#include "chess.hpp"
#define TURN (1 + (-turn) * 2) //turn == 0の時1を返し、turn == 1の時-1を返す
#define PX in[0]
#define PY in[1]
#define TX in[2]
#define TY in[3]
using namespace std;

game::game(){
    //On, Player and id setting
    int n = 0x0;
    bool pl = false;
    for(i = 0; i < 8; i++){
        if(i < 2 || 5 < i){
            switch (i){
                case 6: n = 0x8; pl = true; break;
                case 7: n = 0x0; break;
            }
            for(j = 0; j < 8; j++, n++){
                field[i][j].on = true;
                field[i][j].player = pl;
                field[i][j].id = n;
            }
        }else{
            for(j = 0; j < 8; j++, n++){
                field[i][j].on = false;
                field[i][j].player = false;
                field[i][j].id = 0;
                field[i][j].icon = "-";
            }
        }
        field[i][j].cnt = 0;
    }

    //Icon setting
    for(j = 0; j < 8; j++){ //pawn
        field[1][j].icon = "♙";
        field[6][j].icon = "♟";
    }
    field[0][0].icon = "♖"; field[7][0].icon = "♜"; // Rook
    field[0][1].icon = "♘"; field[7][1].icon = "♞"; // Knight
    field[0][2].icon = "♗"; field[7][2].icon = "♝"; // Bishop
    field[0][3].icon = "♕"; field[7][3].icon = "♛"; // Queen
    field[0][4].icon = "♔"; field[7][4].icon = "♚"; // King
    field[0][5].icon = "♗"; field[7][5].icon = "♝"; // Bishop
    field[0][6].icon = "♘"; field[7][6].icon = "♞"; // Knight
    field[0][7].icon = "♖"; field[7][7].icon = "♜"; // Rook
    
    //Null
    null.on     = false;
    null.player = false;
    null.id     = 0x0;
    null.icon   = "-";
    
    //Turn
    turn = false;
    
    //Back
    backup();
}
game::~game(){
    cout << "End" << endl;
}
void game::show(){
    char c = 'a';
    cout << "  ";
    for(i = 0; i < 8; i++){
        cout << c++ << ' ';
    }
    cout << endl;
    
    for(i = 7; i >= 0; i--){
        cout << i + 1 << ' ';
        for(j = 0; j < 8; j++){
            cout << field[i][j].icon << ' ';
        }
        cout << i + 1 << endl;
    }
    
    c = 'a';
    cout << "  ";
    for(i = 0; i < 8; i++){
        cout << c++ << ' ';
    }
    cout << endl;
}
bool game::turn_now(void){
    return turn;
}
void game::backup(void){
    for(i = 0; i < 8; i++)
        for(j = 0; j < 8; j++)
            back[i][j] = field[i][j];
}
void game::redo(void){
    for(i = 0; i < 8; i++)
        for(j = 0; j < 8; j++)
            field[i][j] = back[i][j];
}
void game::turn_change(void){
    if(turn == false) turn = true;
    else              turn = false;
}
void game::move(const int in[]){
    field[TY][TX] = field[PY][PX];
    field[PY][PX] = null;
    field[TY][TX].cnt++;
}
bool game::in_out(void){
    string typed;
    int place[4];
    bool flag;
    do{
        cin >> typed;
        if(typed == "show") show(); //コマンド一覧
        
        //入力変換
        for(i = 0; i < 2; i++){
            place[i * 2] =  toupper(typed[i * 2]) - 'A';
            place[i * 2 + 1] = typed[i * 2 + 1] - '1';
        }
        
        flag = true;
        if(place[0] == place[2] && place[1] == place[3]) // 移動元 = 移動先
            flag = false;
        for(i = 0; i < 4 && flag == true; i++)//マップ内外判定
            if(0 > place[i] || place[i] > 7)
                flag = false;
        
    }while(flag == false);
    return decition(place);
}
bool game::decition(const int in[]){
    bool flag = true;
    if(
       ((true != field[PY][PX].on     ) ||
        (turn != field[PY][PX].player)) ||
       ((true == field[TY][TX].on     ) &&
        (turn == field[TY][TX].player))
       ){
        flag = false; //相手の駒を動かさない, 自分の駒を取らない
    }
    
    if(flag == true){
        switch(field[PY][PX].id){
            case 4: flag = king(in); break;
            case 3: flag = queen(in); break;
            case 2:
            case 5: flag = bishop(in); break;
            case 1:
            case 6: flag = knight(in); break;
            case 0:
            case 7: flag = rook(in); break;
            default: flag = pawn(in, false); break;
        }
    }
    bool checkmate = false;
    if(flag == true){
        move(in);
        int ch;
        ch = check(in);
        if(ch == 1){
            checkmate = true;
        }else if(ch == 0){
            turn_change();
            backup();
        }else{
            redo();
        }
    }
    return checkmate;
}
int  game::check(const int in[]){
    yx kings[2];
    int sub[2][8] = {//走査していくための添字の増える値 sub[0] is x, and sub[1] is y.
        {0, 1,  1,  1,  0, -1, -1, -1},
        {1, 1,  0, -1, -1, -1,  0,  1},
    };
    int r;
    
    //kingを記録
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++)
            if(field[i][j].id == 4){
                if(field[i][j].player == turn){
                    kings[0].y = i;
                    kings[0].x = j;
                }else{
                    kings[1].y = i;
                    kings[1].x = j;
                }
                break;
            }
    }
    r = check2(kings[0].x, kings[0].y, false);
    if(r < 0)return -1;
    
    int ch[4] = {TX, TY, kings[1].x, kings[1].y};//移動した後、敵のキングを取れるか判断
    bool checkmate = false;
    switch(field[ch[1]][ch[0]].id){
        case 4:  checkmate = king(ch); break;
        case 3:  checkmate = queen(ch); break;
        case 2:
        case 5:  checkmate = bishop(ch); break;
        case 1:
        case 6:  checkmate = knight(ch); break;
        case 0:
        case 7:  checkmate = rook(ch); break;
        default: checkmate = pawn(ch, true); break;
    }
    
    if(checkmate == true){//相手の王が避けられる範囲を探す
        int n, x, y;
        int able[2][8] = {0};
        for(i = n = 0; i < 8; i++){
            x = kings[1].x + sub[0][i];
            y = kings[1].y + sub[1][i];
            if(y < 0 || 7 < y || x < 0 || 7 < x)continue;
            if(field[y][x].on == false){
                able[0][n]   = x;
                able[1][n++] = y;
            }
        }
        turn_change();
        for(i = 0; i < n; i++){
            if(check2(able[0][i], able[1][i], false) == 0){
                checkmate = false;
                break;
            }
        }
        turn_change();
    }
    if(checkmate == true){//自分の駒の移動範囲が遮られているかどうか
        int n, x, y;
        int sx, sy;
        int able[2][8] = {0};
        sx = (kings[1].x == TX ? 0 : kings[1].x > TX ? +1 : -1);
        sy = (kings[1].y == TY ? 0 : kings[1].y > TY ? +1 : -1);
        x = TX + sx, y = TY + sy;
        for(n = 0; (x != kings[1].x || y != kings[1].y) && n < 8; n++){
            able[0][n] = x, x += sx;
            able[1][n] = y; y += sy;
        }
        for(i = 0; i < n; i++){
            if(check2(able[0][i], able[1][i], true) < 0){
                checkmate = false;
                break;
            }
        }
    }
    if(checkmate == true){//自分の駒が取られるかどうか
        int dt;
        
        if((dt = check2(TX, TY, false)) < 0){
            if(dt == -2){
                turn_change();
                dt = check2(TX, TY, false) == 0 ? -1 : 0;
                turn_change();
            }
            if(dt < 0)
                checkmate = false;
        }
    }
    return checkmate == true ? 1 : 0;
}
int  game::check2(int x, int y,bool sw){//swは王とポーンの動きを変化
    bool flag = true;
    int sub[4][8] = {//走査していくための添字の増える値 sub[0] is x, and sub[1] is y.
        {0, 1,  1,  1,  0, -1, -1, -1},
        {1, 1,  0, -1, -1, -1,  0,  1},
        {1, 2,  2,  1, -1, -2, -2, -1},//For knight
        {2, 1, -1, -2, -2, -1,  1,  2}
    };
    int m, n;
    
    //自分のキングがチェックされていないか？
    for(i = 0; i < 8; i++){//pieces[0]の縦横斜めの相対的な方向に駒があるところまで走査
        flag = true;
        m = y + sub[3][i];
        n = x + sub[2][i];
        if((0 <= m && m <= 7 && 0 <= n && n <= 7) && field[m][n].on == true)
            if(field[m][n].player != turn)
                if(field[m][n].id == 1 || field[m][n].id == 6)
                    return -1;//when knight
        
        m = y + sub[1][i];
        n = x + sub[0][i];
        if(m < 0 || 7 < m || n < 0 || 7 < n)continue;//範囲外の場合以下を評価しない
        while(field[m][n].on == false){
            m += sub[1][i];
            n += sub[0][i];
            if(m < 0 || 7 < m || n < 0 || 7 < n){
                flag = false;
                break;
            }
        }
        if(flag == true && field[m][n].player != turn){//駒があった時
            int id = field[m][n].id;
            int pm = m - y, pn = n - x;
            if(abs(pm) == 1 || abs(pn) == 1){
                if(sw == false && id == 4){
                    return -2;//When king
                }else if(id >= 8){
                    if(pm == TURN && (sw == false ? (pn == -1 || pn == 1) : (pn == 0)))
                        return -1;//when pawn
                }
            }
            if(i % 2){
                if(id == 3 || id == 2 || id == 5)
                    return -1;//When queen or bishop
            }else{
                if(id == 3 || id == 0 || id == 7)
                    return -1;//When queen or rook;
            }
        }
    }
    return 0;
}
bool game::movable(const int in[], short type){
    bool able = false;
    int sx = (PX == TX ? 0 : PX < TX ? +1 : -1);
    int sy = (PY == TY ? 0 : PY < TY ? +1 : -1);
    
    //Rook or Queen
    if(type & 1)
        if(PX == TX || PY == TY)
                able = true;
    
    //Bishop or Queen
    if(type & 2)
        if(abs(PX - TX) == abs(PY - TY))
                able = true;
    
    //駒を飛び越えていないか
    if(able == true){
        able = false;
        for(i = PY + sy, j = PX + sx;
            (i != TY || j != TX) && field[i][j].on == false;
            i += sy, j += sx);
        if(i == TY && j == TX)
            able = true;
    }
    return able;
}
bool game::king(const int in[]){
    bool king = false;
    if((PX - 1 <= TX && TX <= PX + 1) &&(PY - 1 <= TY && TY <= PY + 1))
        king = true;
    return king;
}
bool game::queen(const int in[]){
    return movable(in, 3);
}
bool game::bishop(const int in[]){
    return movable(in, 2);
}
bool game::knight(const int in[]){
    bool knight = false;
    int s2 = abs(PX - TX), s1 = abs(PY - TY);

    if(s2 < s1){
        int w = s2;
        s2 = s1;
        s1 = w;
    }
    if(s2 == 2 && s1 == 1)
        knight = true;
    return knight;
}
bool game::rook(const int in[]){
    return movable(in, 1);
}
bool game::pawn(const int in[], bool nopro){
    bool pawn = true;
    if(((field[PY][PX].cnt != 0) ||
        ( PY + 2 * TURN != TY) ||
        ( PX            != TX)) &&
       (( PY + 1 * TURN != TY) ||
        ((PX - 1        != TX) &&
         (PX            != TX) &&
         (PX + 1        != TX)))){
        pawn = false;
    }
    if(pawn != false){
        if(PX == TX){
            if(field[TY][TX].on == true)
                pawn = false;
        }else{
            if(field[TY][TX].on == false)
                pawn = false;
        }
    }
    if(nopro == false && TY == 7){
        int pr;
        int id = 0;
        string icon;
        cout << "プロモーションしてください" << endl;
        cout << "Queen  = 1" << endl;
        cout << "Bishop = 2" << endl;
        cout << "Knight = 3" << endl;
        cout << "Rook   = 4 :";
        while(1){
            cin >> pr;
            if(0 < pr && pr < 5)break;
        }
        switch(pr){
            case 1: id = 3; icon = turn ? "♛" : "♕"; break;
            case 2: id = 2; icon = turn ? "♝" : "♗"; break;
            case 3: id = 1; icon = turn ? "♞" : "♘"; break;
            case 4: id = 0; icon = turn ? "♜" : "♖"; break;
        }
        field[PY][PX].id = id; field[PY][PX].icon = icon;
    }
    return pawn;
}
