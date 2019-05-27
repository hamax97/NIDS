#!/bin/bash

ssh -N -f -L localhost:8888:localhost:8889 htobonm@192.168.10.115

# Run this after the jupyter notebook server has been launched in the remote side.
# Now open the browser in localhost:8888.
