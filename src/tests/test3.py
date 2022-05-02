import  data_table

table = data_table.new_table("..\\..\\data\\orders.csv")
table.head()
table.set_types(["int", "str", "str", "str", "float"])
table.head()
sorted_table = table.sort_by(4)
sorted_table.print(50)
