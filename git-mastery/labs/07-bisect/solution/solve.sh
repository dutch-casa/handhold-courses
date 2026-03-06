#!/bin/bash
# Lab 07: Bisect a bug
set -e

GOOD=$(git log --oneline | tail -1 | awk '{print $1}')

git bisect start
git bisect bad HEAD
git bisect good "$GOOD"
git bisect run bash check-bug.sh || true

# After bisect run, HEAD points at the first bad commit (before reset)
git rev-parse HEAD > bad-commit.txt

git bisect reset
