
#Escribir un script de Python que permita generar un archivo llamado <sgn.h>, que contenga 64
#valores enteros de una señal tipo rampa, entre 0 y 3300, distribuídos uniformemente. Los
#valores deben estar separados por coma para poder ser incluído como header de C/C++


#Fichero: es un String con el nombre del fichero. Si no indicamos ninguna ruta de archivo se entiende que el fichero de texto 
# #reside en la misma carpeta del código Python que estamos ejecutando, pero si intentamos leer un fichero que no existe obtenemos 
# #un error del tipo FileNotFoundError que nos indica que el fichero no se ha encontrado

# w+	Leer y escribir. Si el fichero no existe lo crea, en caso que exista elimina su contenido
Valores= list(range(0,3300,51))     # 64 valores de 0 a 3300
print(Valores)

handler = open('sgn.h','w+') 


for item in Valores:
    handler.write("%d, "%item)
    


handler.close()


