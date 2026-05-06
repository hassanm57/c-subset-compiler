
===== Three-Address Code (TAC) =====

func int factorial:
    param n : int
    t0 = n <= 1
    if_false t0 goto L0
    return 1
L0:
    t1 = n - 1
    sub = t1
    param t1
    t2 = call factorial, 1
    rec = t2
    t3 = n * t2
    return t3
end func factorial

func int sum_array:
    param arr : int
    param size : int
    total = 0
    i = 0
L2:
    t4 = i < size
    if_false t4 goto L3
    t5 = arr[t5]
    t6 = total + t5
    total = t6
    t7 = i + 1
    i = t7
    goto L2
L3:
    return total
end func sum_array

func int max:
    param a : int
    param b : int
    t8 = a > b
    if_false t8 goto L4
    return a
L4:
    return b
end func max

func int main:
    x = 30
    y = 30
    z = 60
    param 30
    printf "x = %d\n", 1 args
    param 30
    printf "y = %d\n", 1 args
    param 60
    printf "z = %d\n", 1 args
    score = 73
    if_false 0 goto L6
    printf "Grade: A\n", 0 args
    goto L7
L6:
    t15 = score >= 80
    if_false t15 goto L8
    printf "Grade: B\n", 0 args
    goto L9
L8:
    t16 = score >= 70
    if_false t16 goto L10
    printf "Grade: C\n", 0 args
    goto L11
L10:
    printf "Grade: F\n", 0 args
L11:
L9:
L7:
    running_total = 0
    i = 1
L12:
    t17 = i <= 5
    if_false t17 goto L13
    t18 = running_total + i
    running_total = t18
    param i
    param t18
    printf "Step %d: total = %d\n", 2 args
    t19 = i + 1
    i = t19
    goto L12
L13:
    nums[0] = 10
    nums[1] = 20
    nums[2] = 30
    nums[3] = 40
    nums[4] = 50
    param nums
    param 5
    t20 = call sum_array, 2
    arr_sum = t20
    param t20
    printf "Array sum = %d\n", 1 args
    param 6
    t21 = call factorial, 1
    f = t21
    param t21
    printf "6! = %d\n", 1 args
    param t21
    param t20
    t22 = call max, 2
    bigger = t22
    param t22
    printf "max(6!, arr_sum) = %d\n", 1 args
    n = 1
L14:
    t23 = n <= 100
    if_false t23 goto L15
    t24 = n * n
    t25 = t24 > 50
    if_false t25 goto L16
L16:
    t26 = n + 1
    n = t26
    goto L14
L15:
    param n
    printf "First n where n*n > 50: %d\n", 1 args
    return 0
end func main
====================================

