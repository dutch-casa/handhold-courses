#!/bin/bash
set -e
cd "$(dirname "$0")"

count=0
fail=0

check() {
  count=$((count + 1))
  if [ "$1" = "true" ]; then
    echo "ok $count - $2"
  else
    echo "not ok $count - $2"
    fail=$((fail + 1))
  fi
}

# 1. Branch feat/add-greeting exists locally
check "$(git branch 2>/dev/null | grep -q 'feat/add-greeting' && echo true || echo false)" "branch feat/add-greeting exists"

# 2. greeting.js exists
check "$(test -f greeting.js && echo true || echo false)" "greeting.js exists"

# 3. greet is a function
check "$(node -e "const {greet} = require('./greeting'); process.exit(typeof greet === 'function' ? 0 : 1)" 2>/dev/null && echo true || echo false)" "greet is a function"

# 4. greet('World') returns 'Hello, World!'
check "$(node -e "const {greet} = require('./greeting'); process.exit(greet('World') === 'Hello, World!' ? 0 : 1)" 2>/dev/null && echo true || echo false)" "greet('World') returns 'Hello, World!'"

# 5. Commit message on feat/add-greeting follows conventional commits
check "$(git log --format='%s' feat/add-greeting 2>/dev/null | grep -q '^feat: add greet function$' && echo true || echo false)" "commit message is 'feat: add greet function'"

# 6. Branch pushed to origin
REMOTE_COMMITS=$(git log origin/feat/add-greeting --oneline 2>/dev/null | wc -l | tr -d ' ')
check "$([ $REMOTE_COMMITS -gt 0 ] && echo true || echo false)" "branch pushed to origin"

echo "1..$count"
exit $fail
