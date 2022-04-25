import data_table

a = data_table.new_col()

a.set_name("aboba")
a.print_column()
a.fill_from_list([i/10 for i in range(10)])
a.print_column()
