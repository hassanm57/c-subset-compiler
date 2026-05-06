
===== Three-Address Code (TAC) =====

func void bubble_sort:
    param arr : int
    param n : int
    i = 0
L0:
    t0 = n - 1
    t1 = i < t0
    if_false t1 goto L1
    j = 0
L2:
    t2 = n - i
    t3 = t2 - 1
    t4 = j < t3
    if_false t4 goto L3
    t5 = arr[t5]
    t6 = j + 1
    t7 = arr[t7]
    t8 = t5 > t7
    if_false t8 goto L4
    t9 = arr[t9]
    temp = t9
    t10 = j + 1
    t11 = arr[t11]
    arr[j] = t11
    t12 = j + 1
    arr[t12] = temp
L4:
    t13 = j
    j = j + 1
    goto L2
L3:
    t14 = i
    i = i + 1
    goto L0
L1:
end func bubble_sort

func int array_sum:
    param arr : int
    param n : int
    sum = 0
    i = 0
L6:
    t15 = i < n
    if_false t15 goto L7
    t16 = arr[t16]
    t17 = sum + t16
    sum = t17
    t18 = i
    i = i + 1
    goto L6
L7:
    return sum
end func array_sum

func int linear_search:
    param arr : int
    param n : int
    param target : int
    i = 0
L8:
    t19 = i < n
    if_false t19 goto L9
    t20 = arr[t20]
    t21 = t20 == target
    if_false t21 goto L10
    return i
L10:
    t22 = i
    i = i + 1
    goto L8
L9:
    t23 = - t23
    return t23
end func linear_search

func int main:
    data[0] = 64
    data[1] = 34
    data[2] = 25
    data[3] = 12
    data[4] = 22
    n = 5
    printf "Before sort: ", 0 args
    i = 0
L12:
    t24 = i < n
    if_false t24 goto L13
    t25 = data[t25]
    param t25
    printf "%d ", 1 args
    t26 = i
    i = i + 1
    goto L12
L13:
    printf "\n", 0 args
    param data
    param n
    t27 = call bubble_sort, 2
    printf "After sort:  ", 0 args
    i = 0
L14:
    t28 = i < n
    if_false t28 goto L15
    t29 = data[t29]
    param t29
    printf "%d ", 1 args
    t30 = i
    i = i + 1
    goto L14
L15:
    printf "\n", 0 args
    param data
    param n
    t31 = call array_sum, 2
    param t31
    printf "Sum = %d\n", 1 args
    param data
    param n
    param 25
    t32 = call linear_search, 3
    idx = t32
    param idx
    printf "Search 25: index = %d\n", 1 args
    scores[0] = 9.5
    scores[1] = 8
    scores[2] = 7.5
    scores[3] = 6
    total = 0
    i = 0
L16:
    t33 = i < 4
    if_false t33 goto L17
    t34 = scores[t34]
    t35 = total + t34
    total = t35
    t36 = i
    i = i + 1
    goto L16
L17:
    t37 = total / 4
    param t37
    printf "Average score: %f\n", 1 args
    i = 0
L18:
    t38 = i < 9
    if_false t38 goto L19
    t39 = i * i
    matrix[i] = t39
    t40 = i
    i = i + 1
    goto L18
L19:
    printf "Squares: ", 0 args
    i = 0
L20:
    t41 = i < 9
    if_false t41 goto L21
    t42 = matrix[t42]
    param t42
    printf "%d ", 1 args
    t43 = i
    i = i + 1
    goto L20
L21:
    printf "\n", 0 args
    return 0
end func main
====================================

