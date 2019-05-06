#!/bin/bash

docker build -f Dockerfile.tests . --tag so_builder

docker run -v ${PWD}:/sources/ so_builder
