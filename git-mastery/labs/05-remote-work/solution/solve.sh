#!/bin/bash
# Lab 05: Collaborate with a remote
set -e
cd local

# Create branch and add README content
git checkout -b add-readme
cat >> README.md << 'EOF'

This project is maintained by the learner.
EOF

git add README.md
git commit -m "docs: add README content"

# Push the branch
git push origin add-readme

# Fetch upstream changes (other dev pushed other.txt to main)
git fetch origin

# Rebase onto updated main
git rebase origin/main

# Push rebased branch
git push -f origin add-readme
