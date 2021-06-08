
if __name__ == '__main__': 
    print('zip example:')
    print('__________')
    print('EX 1: reverse dictionary')
    print('__________')
    d = {'a': 1, 'b': 2, 'c': 3, 'd': 4}
    reverse_d = dict(zip(d.values(), d.keys()))
    print(d)
    print(reverse_d)
    print('__________')
    print('EX 2')
    print('__________')
    list1 = [1, 2, 3, 4]
    list2 = ['a', 'b', 'c', 'd']
    z = zip(list1, list2)
    print(z)
    print(list(z))
    print('__________')
    print('EX 3')
    print('__________')
    list3 = [(1, 2), (3, 4), (5, 6), (7, 8)]
    print(list3)
    print(list(zip(*list3)))



