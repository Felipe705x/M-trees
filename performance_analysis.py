import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("build/results.csv", delimiter=";", index_col= "Query")

z_score = 1.96

SS = df[df['Method'] == "SS"]
CP = df[df["Method"] == "CP"]
def get_stats(df, n):
    df_n = df[df['n'] == n]
    disk_avg = df_n['Disk Accesses'].mean()
    disk_std = df_n['Disk Accesses'].std()
    disk_int_l = disk_avg - 1.96*disk_std/10
    disk_int_r = disk_avg + 1.96*disk_std/10
    return disk_avg, disk_std, disk_int_l, disk_int_r


for i in range(10,18):
    disk_avg, disk_std, disk_int_l, disk_int_r = get_stats(SS, i)
    print(f"Método SS {i} AVG = {disk_avg},  STD = {disk_std}")
    print(f"Intervalo de confianza:[{disk_int_l},{disk_int_r}]")
    print()


for i in range(10,26):
    disk_avg, disk_std, disk_int_l, disk_int_r = get_stats(CP, i)

    print(f"Método CP {i} AVG = {disk_avg},  STD = {disk_std}")
    print(f"Intervalo de confianza:[{disk_int_l},{disk_int_r}]")
    print()


ss_grouped = SS.groupby('n')['Disk Accesses'].mean()
cp_grouped = CP.groupby('n')['Disk Accesses'].mean()

plt.figure(figsize=(10, 6))
plt.plot(ss_grouped.index, ss_grouped.values, marker='o', label='Sexton-Swinbank', color = 'blue')
plt.plot(cp_grouped.index, cp_grouped.values, marker='x', label='Ciaccia-Patella', color='orange')

plt.title('Accesos promedio a disco por método y tamaño')
plt.xlabel('Tamaño (potencia de $2$)')
plt.ylabel('Accesos promedio a disco')
plt.xticks(rotation=45)
plt.legend()
plt.grid(True)
plt.savefig("images/disk.png")
#plt.show()

ss_filtered = ss_grouped[(ss_grouped.index >= 10) & (ss_grouped.index <= 17)]
cp_filtered = cp_grouped[(cp_grouped.index >= 10) & (cp_grouped.index <= 17)]

plt.figure(figsize=(10, 6))
plt.plot(ss_filtered.index, ss_filtered.values, marker='o', label='Sexton-Swinbank', color = 'blue')
plt.plot(cp_filtered.index, cp_filtered.values, marker='x', label='Ciaccia-Patella', color='orange')

plt.title('Accesos promedio a disco por método y tamaño (hasta $2^{17}$)')
plt.xlabel('Tamaño (potencia de $2$)')
plt.ylabel('Accesos promedio a disco')
plt.xticks(rotation=45)
plt.legend()
plt.grid(True)
plt.savefig("images/disk17.png")
#plt.show()

ss_query_time_grouped = SS.groupby('n')['Query Time'].mean()
cp_query_time_grouped = CP.groupby('n')['Query Time'].mean()

plt.figure(figsize=(10, 6))
plt.plot(cp_query_time_grouped.index, cp_query_time_grouped.values, marker='x', label='Ciaccia-Patella', color='orange')
plt.plot(ss_query_time_grouped.index, ss_query_time_grouped.values, marker='o', label='Sexton-Swinbank', color = 'blue')
plt.title('Tiempo promedio de consulta por método y tamaño')
plt.xlabel('Tamaño (potencia de $2$)')
plt.ylabel('Tiempo promdio de consulta')
plt.xticks(rotation=45)
plt.legend()
plt.grid(True)
plt.savefig("images/query.png")
#plt.show()

plt.figure(figsize=(10, 6))
plt.plot(ss_query_time_grouped.index, ss_query_time_grouped.values, marker='o', label='Sexton-Swinbank', color = 'blue')
plt.plot(cp_query_time_grouped[cp_query_time_grouped.index <= 17].index, 
         cp_query_time_grouped[cp_query_time_grouped.index <= 17].values, marker='x', label='Ciaccia-Patella', color='orange')
plt.title('Tiempo promedio de consulta por método y tamaño (hasta $2^{17}$)')
plt.xlabel('Tamaño (potencia de $2$)')
plt.ylabel('Tiempo promdio de consulta')
plt.xticks(rotation=45)
plt.legend()
plt.grid(True)
plt.savefig("images/query17.png")
#plt.show()

SS = df[df['Method'] == "SS"]
CP = df[df['Method'] == "CP"]

ss_construction_time_grouped = SS.groupby('n')['Construction Time'].mean()
cp_construction_time_grouped = CP.groupby('n')['Construction Time'].mean()

plt.figure(figsize=(12, 7))
plt.plot(ss_construction_time_grouped.index, ss_construction_time_grouped.values, marker='o', label='Sexton-Swinbank', color = 'blue')
plt.plot(cp_construction_time_grouped.index, cp_construction_time_grouped.values, marker='x', label='Ciaccia-Patella', color='orange')

plt.title('Tiempo de construcción promedio por método y tamaño')
plt.xlabel('Tamaño (potencia de $2$)')
plt.ylabel('Tiempo de construcción promedio (ns)')
plt.xticks(rotation=45)
plt.legend()
plt.grid(True)
plt.savefig("images/construction.png")
#plt.show()
