//
// Created by kiva on 2017/9/17.
//

/**
 * Check whether compiler supports label address
 * @return 1 when supports, 0 when not
 */
int main() {
    void *p = &&label;
    goto *p;

    return 0;

    label:
    return 1;
}
