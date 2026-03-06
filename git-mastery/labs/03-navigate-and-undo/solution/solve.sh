#!/bin/bash
# Lab 03: Navigate and undo
set -e

# Full hash of the first commit that touched feature.txt (oldest = tail -1)
git log --format='%H' -- feature.txt | tail -1 > answer.txt

# Restore config.txt to last committed state
git restore config.txt

# Revert the most recent commit (no-edit uses default message)
git revert HEAD --no-edit
