import  data_table

table = data_table.new_table("..\\..\\data\\orders.csv")
table.head()
table.set_types(["int", "str", "str", "str", "float"])
sorted_table = table.sort_by(0)
sorted_table.print(1000)
table.print_column_types()
print(table.shape())
