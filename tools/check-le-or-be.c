//
// Created by kiva on 2017/9/8.
//

/**
 * Check BigEndian or LittleEndian
 * @return 0 when BigEndian while 1 when LittleEndian.
 */
int main() {
    union {
        int a;
        char b;
    } c;
    c.a = 1;
    return (c.b == 1);
}
