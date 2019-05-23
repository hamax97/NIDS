import pandas as pd
import threading

df = pd.read_csv('../dataset/kddcup.data_10_percent_names')

def change_value(first_index, last_index, column, unique_values):
    print('From:', first_index, 'to:', last_index)
#    for index in range(first_index, last_index):
    for index in range(10):
        for i in range(len(unique_values)):
            if(df[column][index] == unique_values[i]):
                print('Value:', df[column][index], 'equals to:', unique_values[i])
                #df.at[index, column] = i
                break

def encode(column):

    unique_values = df[column].value_counts().index.tolist()

    num_rows = 494021
    available_cores = 36 - 3 # 3: number of columns to encode

    threads_per_row = int(available_cores / 3) # 3: number of columns to encode
    threads_list = list() # List to start and join the created threads
    rows_per_thread = int(num_rows / threads_per_row)
    missing_rows = int(num_rows % threads_per_row)

    first_index = 0
    last_index = rows_per_thread + missing_rows # Give missing rows to first thread

    for i in range(threads_per_row):

        thr = threading.Thread(target=change_value,
                               args=(first_index, last_index, column, unique_values))
#        thr = threading.Thread(target=hello)
        first_index = last_index
        last_index += rows_per_thread

        threads_list.append(thr)
        thr.start()

    for thr in threads_list:
        thr.join()

    # Change column type
#    df[column] = df[column].astype('int')

def hello():
    print('Hello')

if __name__ == '__main__':

    columns = ['protocol_type', 'service', 'flag']
    column_threads = list()

    for col in columns:
        thr = threading.Thread(target=encode, args=(col,))
        column_threads.append(thr)
        thr.start()

    for thr in column_threads:
        thr.join()

#    df.to_csv('finalized.csv')
