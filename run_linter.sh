#!/bin/bash

docker build -f Dockerfile.lint . --tag so_linter

docker run -v ${PWD}:/sources/ so_linter