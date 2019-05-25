import pandas as pd
import threading
from numba import cuda

df = pd.read_csv('../dataset/kddcup.data_10_percent_names')

def change_value(first_index, last_index, column, unique_values):

    print('In column:', column, 'indexes', first_index, last_index)

    for index in range(first_index, last_index):
        for i in range(len(unique_values)):
            if(df.iloc[index][column] == unique_values[i]):
                df.set_value(index, column, i)
                break

@cuda.jit
def encode(column):

    tx = cuda.threadIdx.x
    ty = cuda.blockIdx.x

    block_size = cuda.blockDim.x
    grid_size = cuda.gridDim.x
    
    unique_values = df[column].value_counts().index.tolist()

    num_rows = 494021
    available_cores = 72 - 3 # 3: number of columns to encode

    threads_per_row = int(available_cores / 3) # 3: number of columns to encode
    rows_per_thread = int(num_rows / threads_per_row)
    missing_rows = int(num_rows % threads_per_row)

    first_index = 0
    last_index = rows_per_thread + missing_rows # Give missing rows to first thread

    for i in range(threads_per_row):

        

        first_index = last_index
        last_index += rows_per_thread

    # Change column type
    df[column] = df[column].astype('int')


# Main part of the script
if __name__ == '__main__':

    columns = ['protocol_type', 'service', 'flag']
    for col in columns:
        encode(col)

    df.to_csv('finalized.csv')
