import data_table

a = data_table.new_col(2)
b = data_table.new_table("..\\..\\data\\1.csv")
c = data_table.new_col(2)
a.fill_from_list([1.0, 7890000.700009])
c.fill_from_list([456, 4684])
a.set_name("abigus")
b.set_column([a, 0])
b.set_column([c, 1])

print("печать типов колонок таблицы")
b.print_column_types()
print("получение максимального и минимального значений в колонке")
print(c.max())
print(a.min())
