import data_table

a = data_table.new_col(10)
# a.print_column()

a.set_name("my column")
a.print_column()
a.print_column()
a.print_column()
p_list = [1+i/9 for i in range(a.len())]
a.fill_from_list(p_list)
a.print_column()


b = data_table.new_table("D:\\Programming\\Python\\PycharmProjects\\Data-processing-and-analysis\\data\\1.csv")
b.print_table()
