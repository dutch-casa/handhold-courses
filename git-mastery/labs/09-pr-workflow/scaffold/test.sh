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

# 1. upstream remote exists
check "$(git remote 2>/dev/null | grep -q '^upstream$' && echo true || echo false)" "upstream remote is configured"

# 2. Branch feat/hello-world exists locally
check "$(git branch 2>/dev/null | grep -q 'feat/hello-world' && echo true || echo false)" "branch feat/hello-world exists"

# 3. src/index.js exports a hello function that returns "hello, world"
check "$(node -e "const {hello} = require('./src/index'); process.exit(hello() === 'hello, world' ? 0 : 1)" 2>/dev/null && echo true || echo false)" "hello() returns 'hello, world'"

# 4. Branch was pushed to origin (fork)
REMOTE_COMMITS=$(git log origin/feat/hello-world --oneline 2>/dev/null | wc -l | tr -d ' ')
check "$([ $REMOTE_COMMITS -gt 0 ] && echo true || echo false)" "feat/hello-world pushed to origin"

echo "1..$count"
exit $fail
