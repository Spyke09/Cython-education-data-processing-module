import data_table

print("создание и печать колонки")
a = data_table.new_col(10)
a.print()

# задание имени колонки
a.set_name("my column")

print("заполнение колонки элементами типа double")
p_list = [1+i/9 for i in range(a.len())]
a.fill_from_list(p_list)
a.print()

print("заполнение колонки элементами типа int")
p_list = [i for i in range(a.len())]
a.fill_from_list(p_list)
a.print()

print("заполнение колонки элементами типа str")
p_list = [f"number {i}" for i in range(a.len())]
a.fill_from_list(p_list)
a.print()

print("заполнение колонки элементами типа bool")
p_list = [i%2==0 for i in range(a.len())]
a.fill_from_list(p_list)
a.print()

print("создание таблицы")
b = data_table.new_table("..\\..\\data\\1.csv")
b.head()
print("выдача колонки по ее имени ('3')")
b.column_from_name("3").print()
print("выдача колонки по ее индексу (0)")
b.column_from_index(0).print()

print("замена колонки с индексом 2 на созданную колонку")
a = data_table.new_col(2)
c = data_table.new_col(2)
a.fill_from_list([1.0, 7890000.700009])
c.fill_from_list([456, 4684])
a.set_name("abigus")
b.set_column(0, a)
b.set_column(1, c)
b.head()

b = data_table.new_table("..\\..\\data\\orders.csv")
b.print(4)
b.head()
