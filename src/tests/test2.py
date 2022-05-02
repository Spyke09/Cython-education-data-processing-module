import data_table

a = data_table.new_col(2)
b = data_table.new_table("..\\..\\data\\1.csv")
c = data_table.new_col(2)
a.fill_from_list([3, 2])
c.fill_from_list([7456, 4684])
a.set_name("abigus")
b.set_column(0, a)
b.set_column(1, c)

print("печать типов колонок таблицы")
b.print_column_types()
print("получение максимального и минимального значений в колонке")
print(c.max())
print(a.min())

print("Соортировка")
b.print(10)
b.sort_by(1).print(10)

print("вставка колонки в конец таблицы")
table = data_table.new_table("..\\..\\data\\1.csv")
table.head()
column = data_table.new_col(2)
column.set_name("4")
column.fill_from_list(["new", "values"])
column.print()
table.insert(column)
table.head()

