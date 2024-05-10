import pandas as pd
import numpy as np

df = pd.read_csv("build/results.csv", delimiter=";", index_col= "Query")

z_score = 1.96

SS = df[df['Method'] == "SS"]
CP = df[df["Method"] == "CP"]

def get_stats(df, n):
    df_n = df[df['n'] == n]
    disk_avg = df_n['Disk Accesses'].mean()
    disk_std = df_n['Disk Accesses'].std()
    return disk_avg, disk_std

disk_avg, disk_std = get_stats("SS", 10)

print(f"AVG = {disk_avg},  STD = {disk_std}")