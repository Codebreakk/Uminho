import numpy as np
file = "/home/jc/Documents/LC/Trabalhos/tp0/output_2"

#TP 0 - exercicio 2.a
# gerar matriz com dimensao 1024*1024 e com valores aleatórios 0 e 1
my_array = np.random.randint(2, size = (1024,1024))
# guardar matriz compacta para o ficheiro na variável file
np.savez_compressed(file, my_array = my_array)
# carregar ficheiro para verificar se a matriz foi guardada correctamente
loadedFile = np.load(file + '.npz')

print ("Array before compressing: \n")
print (my_array)

print("Array after compressing: \n")
print (loadedFile['my_array'])
print ('\n')

arrays_equal = np.array_equal(my_array, loadedFile['my_array'])
print("Are the arrays equal?: " + str(arrays_equal))
