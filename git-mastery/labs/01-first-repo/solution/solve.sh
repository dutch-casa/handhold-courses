#!/bin/bash
# Lab 01: Your first repo
set -e

echo "Hello, world!" > hello.txt
git init
git add hello.txt
git commit -m "first commit"
