#!/bin/bash
# Lab 09: Complete PR workflow
set -e
cd local

# upstream is already configured by lab setup
git checkout -b feat/hello-world

cat > src/index.js << 'EOF'
function hello() {
  return 'hello, world';
}

module.exports = { hello };
EOF

git add src/index.js
git commit -m "feat: add hello function"
git push origin feat/hello-world
