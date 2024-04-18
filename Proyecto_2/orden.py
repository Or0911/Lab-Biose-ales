import os
import pandas as pd

# Ruta de la carpeta 'RegistrosCon'
carpeta_registros = 'RegistrosSin'

# Recorrer cada carpeta de 'RegistrosCon'
for carpeta in os.listdir(carpeta_registros):
    ruta_carpeta = os.path.join(carpeta_registros, carpeta)
    if os.path.isdir(ruta_carpeta):
        # Leer el archivo de texto de cada registro
        archivo_txt = os.path.join(ruta_carpeta, 'datos.txt')
        df = pd.read_csv(archivo_txt, header=None, names=['EMG'])

        # Agregar la columna 'time' con valores de 1 a 1000
        df['time'] = range(1, 1001)

        # Guardar los datos en un nuevo archivo de texto
        nuevo_archivo = os.path.join(ruta_carpeta, 'datos_modificados.txt')
        df.to_csv(nuevo_archivo, index=False, float_format='%.18e', sep=' ')

        print(f"Se modificó el archivo {archivo_txt} y se guardó en {nuevo_archivo}")