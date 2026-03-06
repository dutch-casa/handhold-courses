#!/bin/bash
set -e

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

# 1. Exactly 1 commit in the repo
COMMIT_COUNT=$(git log --oneline 2>/dev/null | wc -l | tr -d ' ')
check "$([ $COMMIT_COUNT = 1 ] && echo true || echo false)" "exactly 1 commit in the repo"

# 2. The commit message is exactly "feat: add user authentication"
SUBJECT=$(git log --format='%s' 2>/dev/null | head -1)
check "$([ "$SUBJECT" = 'feat: add user authentication' ] && echo true || echo false)" "commit message is 'feat: add user authentication'"

# 3. auth.js exists and has exactly 6 lines
check "$(test -f auth.js && echo true || echo false)" "auth.js exists"
LINE_COUNT=$(wc -l < auth.js 2>/dev/null | tr -d ' ')
check "$([ $LINE_COUNT = 6 ] && echo true || echo false)" "auth.js has 6 lines (content preserved)"

echo "1..$count"
exit $fail
