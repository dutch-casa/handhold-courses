#!/bin/bash
# Lab 08: Build a clean feature
set -e
cd local

git checkout -b feat/add-greeting

cat > greeting.js << 'EOF'
function greet(name) {
  return `Hello, ${name}!`;
}

module.exports = { greet };
EOF

git add greeting.js
git commit -m "feat: add greet function"
git push origin feat/add-greeting
