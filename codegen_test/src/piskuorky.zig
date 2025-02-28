const ifj = @import("ifj24.zig");

pub fn print_board(board : []u8) void {
    var row = 0;
    var col = 0;

    ifj.write("\n  ");
    while(col < 8) {
        ifj.write(ifj.chr(col + 97));
        ifj.write(" ");
        col = col + 1;
    }
    ifj.write("\n");

    while(row < 8) {
        col = 0;
        ifj.write(row + 1);
        ifj.write(" ");
        while(col < 8) {
            ifj.write(ifj.substring(board, row * 8 + col, row * 8 + col + 1));
            ifj.write(" ");

            col = col + 1;
        }
        row = row + 1;
        ifj.write("\n");
    }
}

pub fn play_move(board : []u8, turn : i32, row : i32, col : []u8) []u8 {
    var move_char : []u8 = ifj.string("X");
    const move_index : i32 = (row - 1) * 8 + (ifj.ord(col, 0) - ifj.ord(ifj.string("a"), 0));

    if(ifj.ord(board, move_index) != ifj.ord(ifj.string("-"), 0)) {
        return board;
    } else {}

    if(turn == 2) {
        move_char = ifj.string("O");
    } else {
    }

    const first_half = ifj.substring(board, 0, move_index);
    const second_half = ifj.substring(board, move_index + 1, 64);

    var new_board : []u8 = ifj.string("");

    if(first_half) |FH| {
        if(second_half) |SH| {
            new_board = ifj.concat(FH, ifj.concat(move_char, SH));
        } else {
        }
    } else {
    }

    return new_board;
}

pub fn check_win(board : []u8, row : i32, col : []u8) i32 {
    const start_i = (row - 1) * 8 + ifj.ord(col, 0) - ifj.ord(ifj.string("a"), 0);
    const p_char = ifj.ord(board, start_i);

    var i = 0-4;
    var count = 0;
    while(i < 9) {
        if(ifj.ord(board, start_i + i) == p_char){
            count = count + 1;
        } else {
            count = 0;
        }

        if(count == 5) {
            return 1;
        } else {
            
        }

        i = i + 1;
    }

    i = 0-4;
    count = 0;
    while(i < 9) {
        if(ifj.ord(board, start_i + (i*8)) == p_char){
            count = count + 1;
        } else {
            count = 0;
        }

        if(count == 5) {
            return 1;
        } else {
            
        }

        i = i + 1;
    }

    i = 0-4;
    count = 0;
    while(i < 9) {
        if(ifj.ord(board, start_i + i + (i*8)) == p_char){
            count = count + 1;
        } else {
            count = 0;
        }

        if(count == 5) {
            return 1;
        } else {
            
        }

        i = i + 1;
    }

    i = 0-4;
    count = 0;
    while(i < 9) {
        if(ifj.ord(board, start_i - i + (i * 8)) == p_char){
            count = count + 1;
        } else {
            count = 0;
        }

        if(count == 5) {
            return 1;
        } else {
            
        }

        i = i + 1;
    }

    return 0;
}

pub fn check_input(row : i32, col : []u8) i32 {
    if(row > 0) {
        if(row <= 8) {
            if(ifj.ord(col, 0) >= ifj.ord(ifj.string("a"), 0)) {
                if(ifj.ord(col, 0) <= ifj.ord(ifj.string("h"), 0)) {
                    return 1;
                } else {
                    ifj.write("Col too big");
                }
            } else {
                ifj.write("Col too small");
            }
        } else {
            ifj.write("Row too big");
        }
    } else {
        ifj.write("Row too small");
    }

    return 0;
}

pub fn main() void {
    var board = ifj.string("----------------------------------------------------------------");

    var p_turn = 1;
    while(p_turn >= 0) {
        print_board(board);
        ifj.write("Zadej radek tahu  (1 - 8):");
        const row = ifj.readi32();
        ifj.write("Zadej sloupec tahu  (a - h):");
        const col = ifj.readstr();

        if(row) |ROW| {
            if(col) |COL| {
                if(check_input(ROW, COL) == 1) {
                    const new_board = play_move(board, p_turn, ROW, COL);

                    if(ifj.strcmp(board, new_board) != 0) {
                        board = new_board;
                        if(check_win(board, ROW, COL) == 1) {
                            p_turn = 0 - p_turn;
                        } else {

                        }

                        if(p_turn == 1) {
                            p_turn = 2;

                        } else {
                            if(p_turn == 2) {
                                p_turn = 1;
                            }else{}
                        }
                    } else {
                        ifj.write("Nevalidni tazik\n");
                    }
                } else {

                }
            } else {
                ifj.write("pismenko je spatne\n");
            }
        } else {
            ifj.write("cislo je spatne\n");
        }
    }

    ifj.write("Player ");
    ifj.write(0 - p_turn);
    ifj.write(" won!!");

    print_board(board);

    board = board;
}