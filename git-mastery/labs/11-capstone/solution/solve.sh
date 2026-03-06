#!/bin/bash
# Lab 10: Capstone — Full Git Workflow
set -e
cd local

# Create feature branch
git checkout -b feat/calculator

# Implement calculator with two commits (as required by instructions)
cat > calculator.js << 'EOF'
function add(a, b) {
  return a + b;
}

function subtract(a, b) {
  return a - b;
}

module.exports = { add, subtract };
EOF

git add calculator.js
git commit -m "wip: add add and subtract"

# Second commit: add multiply
cat > calculator.js << 'EOF'
function add(a, b) {
  return a + b;
}

function subtract(a, b) {
  return a - b;
}

function multiply(a, b) {
  return a * b;
}

module.exports = { add, subtract, multiply };
EOF

git add calculator.js
git commit -m "wip: add multiply"

# Squash both commits into one with the correct message
git reset HEAD~2
git add calculator.js
git commit -m "feat: add calculator module"

# Push the branch
git push origin feat/calculator

# Merge into main and push
git checkout main
git merge feat/calculator
git push origin main
