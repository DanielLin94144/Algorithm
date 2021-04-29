print('customers: What are you serving today?')
menu = input('server: We have ')
order = menu.split(', ')
dict = {}
for item in order:
    name = item.split(' ')[:-1]
    name = ' '.join(name)
    money = item.split(' ')[-1]
    money = money.split('($')[-1]
    money = int(money.split(')')[0])
    print(name, money)
    dict[name] = money
print(order)
print(dict)

c1 = input('customer1: ')
c2 = input('customer2: ')
c3 = input('customer3: ')
c4 = input('customer4: ')


c1 = c1.split(', ')
c2 = c2.split(', ')
c3 = c3.split(', ')
c4 = c4.split(', ')

bill_dict = {}

for i in dict.keys():
    bill_dict[i] = 0

for item in c1:
    bill_dict[item] = bill_dict[item] + 1
for item in c2:
    bill_dict[item] = bill_dict[item] + 1
for item in c3:
    bill_dict[item] = bill_dict[item] + 1
for item in c4:
    bill_dict[item] = bill_dict[item] + 1

Sum = 0
for i in bill_dict.keys():
    Sum = Sum + dict[i]*bill_dict[i]
print(Sum)
# print(f'server: Okay, you ordered: ', {})