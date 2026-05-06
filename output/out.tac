
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
    param 10
    param 20
    t1 = call add, 2
    sum = t1
    result = 55
    constant = 10
    identity = 10
    area = 9.8596
    param t1
    printf "sum = %d\n", 1 args
    param 55
    printf "result = %d\n", 1 args
    param 10
    printf "constant = %d\n", 1 args
    param 10
    printf "identity = %d\n", 1 args
    param 9.8596
    printf "area = %f\n", 1 args
    return 0
end func main
====================================

