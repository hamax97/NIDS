#!/bin/bash

DATASET_DIR=../../dataset

if [ ! -d "$DATASET_DIR" ]; then
    # Dataset dir
    mkdir $DATASET_DIR
fi

# Prepare column names file
if [ ! -f "$DATASET_DIR/data.columns" ]; then

    # Labels
    wget http://kdd.ics.uci.edu/databases/kddcup99/kddcup.names
    labels=$(grep : kddcup.names | cut -d ':' -f 1)

    for label in $labels; do
	echo -n "$label," >> $DATASET_DIR/data.columns
    done

    echo "attack_type" >> $DATASET_DIR/data.columns
    mv kddcup.names $DATASET_DIR

fi

if [ "$1" == "10" ]; then

    wget http://kdd.ics.uci.edu/databases/kddcup99/kddcup.data_10_percent.gz
    gzip -d kddcup.data_10_percent.gz
    mv kddcup.data_10_percent $DATASET_DIR
    cat $DATASET_DIR/data.columns $DATASET_DIR/kddcup.data_10_percent > $DATASET_DIR/kddcup.data_10_percent_names

elif [ "$1" == "100" ]; then

    wget http://kdd.ics.uci.edu/databases/kddcup99/kddcup.data.gz
    gzip -d kddcup.data.gz
    mv kddcup.data $DATASET_DIR
    cat $DATASET_DIR/data.columns $DATASET_DIR/kddcup.data > $DATASET_DIR/kddcup.data_names

else

    echo "Nothing to download"

fi
