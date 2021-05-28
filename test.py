def pascal(row, col):
    if row == col or col == 0:
        return 1
    return pascal(row - 1, col) + pascal(row - 1, col - 1)

def print_Pascal_triangle(n):
    for i in range(0, n + 1):
        print(f'Row{i}: ', end = '')
        for j in range(0, i + 1):
            print(f'{pascal(i, j)} ', end = '')
        print('')
        
def args_test(*myargs):
    sum = 0
    for i in myargs:
        sum = sum + i
    return sum

my_lambda = lambda x, y: x if x > y else y

if __name__ == "__main__":
    # n = 10
    # print_Pascal_triangle(n)
    print(args_test(1, 2, 3, 4, 5))
    print(my_lambda(1, 2))