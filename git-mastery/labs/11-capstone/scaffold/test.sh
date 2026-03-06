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

# 1. add(2, 3) returns 5
check "$(node -e "const c = require('./calculator'); process.exit(c.add(2,3) === 5 ? 0 : 1)" 2>/dev/null && echo true || echo false)" "add(2, 3) returns 5"

# 2. subtract(10, 4) returns 6
check "$(node -e "const c = require('./calculator'); process.exit(c.subtract(10,4) === 6 ? 0 : 1)" 2>/dev/null && echo true || echo false)" "subtract(10, 4) returns 6"

# 3. multiply(3, 4) returns 12
check "$(node -e "const c = require('./calculator'); process.exit(c.multiply(3,4) === 12 ? 0 : 1)" 2>/dev/null && echo true || echo false)" "multiply(3, 4) returns 12"

# 4. main has more than the single initial commit
CURRENT_COUNT=$(git log --oneline main 2>/dev/null | wc -l | tr -d ' ')
check "$([ $CURRENT_COUNT -gt 1 ] && echo true || echo false)" "at least one commit added to main beyond initial"

# 5. The squashed commit message is correct — appears somewhere in main's history
check "$(git log --format='%s' main 2>/dev/null | grep -q '^feat: add calculator module$' && echo true || echo false)" "squashed commit message is 'feat: add calculator module'"

# 6. calculator.js is on main (committed)
check "$(git ls-files main -- calculator.js 2>/dev/null | grep -q 'calculator.js' && echo true || echo false)" "calculator.js is committed on main"

echo "1..$count"
exit $fail
