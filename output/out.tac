
===== Three-Address Code (TAC) =====

func int add:
    param a : int
    param b : int
    t0 = a + b
    return t0
end func add

func int main:
    x = 10
    y = 20
    z = 3.14
    param x
    param y
    t1 = call add, 2
    sum = t1
    t2 = x + y
    t3 = t2 * 2
    t4 = t3 - 5
    result = t4
    t5 = 2 * 3
    t6 = t5 + 4
    constant = t6
    t7 = x * 1
    t8 = t7 + 0
    identity = t8
    t9 = z * z
    area = t9
    param sum
    printf "sum = %d\n", 1 args
    param result
    printf "result = %d\n", 1 args
    param constant
    printf "constant = %d\n", 1 args
    param identity
    printf "identity = %d\n", 1 args
    param area
    printf "area = %f\n", 1 args
    return 0
end func main
====================================

