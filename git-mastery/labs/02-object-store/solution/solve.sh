#!/bin/bash
# Lab 02: Explore the object store
set -e

printf "blob\ntree\ncommit\n" > answers.txt

# Full hash of the oldest (first/root) commit
git rev-list --max-parents=0 HEAD > first-commit.txt
