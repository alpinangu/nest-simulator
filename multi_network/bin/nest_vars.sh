#!/bin/sh

# Make PyNEST available by prepending its path to PYTHONPATH in a safe way.
export PYTHONPATH="/Users/alp/nest-env/lib/python3.13/site-packages${PYTHONPATH:+:$PYTHONPATH}"

# Make NEST executables available by prepending their path to PATH.
export PATH="/Users/alp/nest-env/bin:${PATH}"
