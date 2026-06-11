/*---
descrition: Chinese related efuns
author: Annihilator <taedlar@gmail.com>
---*/

string chinese_number(int i) {
    return CHINESE_D->chinese_number(i);
}

string chinese_period(int i) {
    return CHINESE_D->chinese_period(i);
}

string to_chinese(string str) {
    return CHINESE_D->chinese(str);
}

string currency_string(int v) {
    if( v >= 100 ) {
        return chinese_number(v/100) + "兩銀子"
            + ((v%100) ? "又" + chinese_number(v%100) + "文錢" : "");
    }
    return chinese_number(v) + "文錢";
}
