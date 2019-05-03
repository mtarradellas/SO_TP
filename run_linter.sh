#!/bin/bash

docker build . --tag so_linter

docker run -v ${PWD}:/sources/ so_linter