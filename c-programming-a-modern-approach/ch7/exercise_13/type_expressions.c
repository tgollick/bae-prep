#include <stdio.h>

int main()
{
    char c = '\1';
    short s = 2;
    int i = -3;
    long m = 5;
    float f = 6.5f;
    double d = 7.5;

    // a) c * i
    // Prediction: ASCII value of char multiplied by -3 to produce a signed integer
    // Result: Correct — char promotes to int via integer promotion, 1 * -3 = -3, type is int
    printf("c * i: %d\n", c * i);

    // b) s + m
    // Prediction: No implicit conversion needed, unsure on output type
    // Result: Close — s promotes to long to match m, 2 + 5 = 7, type is long not int
    printf("s + m: %ld\n", s + m);

    // c) f / c
    // Prediction: char's ASCII value converted to integer for division
    // Result: Correct direction but incomplete — char promotes to int first, then int promotes
    // to float to match f, division is float, 6.5 / 1 = 6.5, type is float
    printf("f / c: %f\n", f / c);

    // d) d / s
    // Prediction: short implicitly converts to double for division, output is double
    // Result: Correct — s promotes to double to match d, 7.5 / 2 = 3.75, type is double
    printf("d / s: %lf\n", d / s);

    // e) f - d
    // Prediction: double is second operand so float converts, guessed output is float
    // Result: Wrong on output type — float always promotes UP to double, never the other way.
    // Operand order doesn't matter, result is always double, 6.5 - 7.5 = -1.0, type is double
    printf("f - d: %lf\n", f - d);

    // f) (int) f
    // Prediction: cast from float to int, result is 7 rounded up
    // Result: Wrong on rounding — casting truncates toward zero, never rounds.
    // 6.5 cast to int drops the decimal entirely, result is 6, type is int
    printf("(int) f: %d\n", (int)f);

    return 0;
}