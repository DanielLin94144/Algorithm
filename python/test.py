# def pascal(row, col):
#     if row == col or col == 0:
#         return 1
#     return pascal(row - 1, col) + pascal(row - 1, col - 1)

# def print_Pascal_triangle(n):
#     for i in range(0, n + 1):
#         print(f'Row{i}: ', end = '')
#         for j in range(0, i + 1):
#             print(f'{pascal(i, j)} ', end = '')
#         print('')
        
# def args_test(*myargs):
#     sum = 0
#     for i in myargs:
#         sum = sum + i
#     return sum

# my_lambda = lambda x, y: x if x > y else y

# if __name__ == "__main__":
#     # n = 10
#     # print_Pascal_triangle(n)
#     print(args_test(1, 2, 3, 4, 5))
#     print(my_lambda(1, 2))

class Record:
    def __init__(self, category, name, amount):
        self._category = category
        self._name = name
        self._amount = amount
# 1. Define the formal parameters so that a Record can be instantiated
# by calling Record('meal', 'breakfast', -50).
# 2. Initialize the attributes from the parameters. The attribute
# names should start with an underscore (e.g. self._amount)
# Define getter methods for each attribute with @property decorator.
# Example usage:
# >>> record = Record('meal', 'breakfast', -50)
# >>> record.amount
# -50
    @property
    def category(self):
        return self._category
    @property
    def name(self):
        return self._name
    @property
    def amount(self):
        return self._amount
    
    

# record = Record('meal', 'breakfast', -50)
# print(record.amount)

class Records:
    """Maintain a list of all the 'Record's and the initial amount of money."""
    def __init__(self):
        try:
            with open('records.txt', 'r') as fh: 
                money = fh.readline() #read records.txt
                money = money.split('\n')[0] #remove \n
                price = int(money)  #chech the value of money
                print('Welcome back!')
        except:
            try:
                money = input('How much money do you have?') 
            except:
                print('Invalid value for money. Set to 0 by default.')
                money = 0 #set 0 if the input value is wrong
        #import List
        try:
            with open('records.txt', 'r') as fh:
                L = fh.readlines()
                L.pop(0)  #remove money
                L = [i.split('\n')[0] for i in L] #remove \n
        except:
            L = []
        self._records = L
        self._initial_money = int(money)



    def add(self, record, categories):
        record = record.split(' ')
        category = record[0]
        name = record[1]
        money = int(record[2])

        r = Record(category, name, money)

        if_valid = categories.is_category_valid(category)     

        try:
            order = int(money) #check whether the input expense has any error
        except:
            print('The format of a record should be like this: food breakfast -50.\nFail to add a record.')

        else:
            if if_valid == True:
                    self._records.append(r)
            else:
                 print('The specified category is not in the category list.\nYou can check the category list by command "view categories".\nFail to add a record.\n')

    def view(self):
        total = 0 #initial cond.
        print('Here\'s your expense and income records:\n')
        print('Category        Description          Amount')
        print('============== ==================== ======')
        for i in self._records:
            cate = i.category
            name = i.name #the record name
            amount = i.amount #the record amount
            print(f'{cate:<15}{name:<20}{amount:>6}')
            total = total + amount
        print('==========================================')
        totalmoney = total + self._initial_money #totalmoney
        print('Now you have',totalmoney,'dollars.')
        return('\n')


    def delete(self):
        L = self.records
        try:
            specify = input('Which record do you want to delete? ')
            specifyrecord = specify.split()
            deleteprice = int(specifyrecord[2]) #check whether the input specify has error 
        except (IndexError, ValueError): #chech for both error
            print('Invalid format. Fail to delete a record.') 
        else:
            if specify not in L: #check the delete is in former input
                print(f"There's no record with {specify}. Fail to delete a record.")
            else:
                delete = L.remove(specify) #remove the specify record
            total = 0  # set the initail cond. again
            totalmoney = 0
            fi_total = 0
        self.records = L
        return(L)

    def find(self, categories):
        if category == []:
            print('target category doesn\'t exist\ntry again' )
        else:    
            filted_list = list(filter(lambda n: n.split()[0] in which_category, L))
            print(filted_list)
            fi_total = 0 #initial cond.
            print(f'Here\'s your {fi_category} records:\n')
            print('Category        Description          Amount')
            print('============== ==================== ======')
            for i in filted_list:
                fi_cate = i.split()[0]
                fi_name = i.split()[1] #the record name
                fi_amount = int(i.split()[2]) #the record amount
                print(f'{fi_cate:<15}{fi_name:<20}{fi_amount:>6}')
                fi_total = fi_total + fi_amount
            print('==========================================')
            print('Now you have',fi_total,'dollars.')
            return('\n')

    @property
    def save(self):
        L = self.records
        money = self.initial_money
        fh = open('records.txt', 'w')
        fh.write(str(money)+'\n') #save the money value into records.txt
        records = []
        for i in L:
            records.append(i + '\n') 
        fh.writelines(records) #save the record into records.txt
        fh.close()
        return('\n')  

class Categories:
    """Maintain the category list and provide some methods."""
    def __init__(self):
        self._categories = ['expense', ['food', ['meal', 'snack', 'drink'], 'transportation', ['bus', 'railway']], 'income', ['salary', 'bonus']]

    def view(self):
        categories = self._categories
        if categories == None:
            return
        if type(categories) in {list,tuple}:
            for subcategories in categories:
                view_categorises(subcategories, level+1)
        else:
            print(f'{" "*2*level}- {categories}')

    def is_category_valid(self, category):
        categories = self.flatten(self._categories)
        if category in categories:
                return True
        else:
                return False 

    def flatten(self, categories):
        if type(categories) == list:
            result = []
            for child in categories:
                result.extend(self.flatten(child))
            return result
        else:
            return [categories]

    def find_subcategories(self, category):
        categories = self._categories
        def rec_find_subcategories(category, categories):
            if type(categories) == list:
                for v in categories:
                    p = find_subcategories(fi_category, v)
                    if p == True:
                        index = categories.index(v)
                        if index + 1 < len(categories) and                                 type(categories[index + 1]) == list:
                            return flatten(categories[index:index + 2])
                        else:
                            # return only itself if no subcategories
                            return [v]
                    if p != []:
                        return p
            return True if categories == fi_category else []
        return rec_find_subcategories(category, categories)

records = Records()
categories = Categories()
records.add('meal breakfast -50', categories)
records.view()
