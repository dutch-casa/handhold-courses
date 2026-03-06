#!/bin/bash
# Lab 06: Clean commits with interactive rebase
# Uses reset+recommit since core.editor=true blocks interactive editor
set -e

# Soft-reset to the root commit, keeping all changes staged, then amend it.
git reset --soft HEAD~5
git commit --amend -m "feat: add user authentication"
