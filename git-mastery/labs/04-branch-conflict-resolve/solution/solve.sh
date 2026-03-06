#!/bin/bash
# Lab 04: Branch, conflict, resolve
set -e

# Attempt merge — will conflict on README.md
git merge --no-ff feature || true

# Write resolved README with both paragraphs
cat > README.md << 'EOF'
# Project

Welcome to the project.

This project does amazing things.

Install with: npm install
EOF

git add README.md
GIT_EDITOR=true git commit -m "Merge branch 'feature'"
