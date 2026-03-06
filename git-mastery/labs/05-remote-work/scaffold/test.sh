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

# 1. Branch add-readme exists locally
check "$(git branch 2>/dev/null | grep -q 'add-readme' && echo true || echo false)" "branch add-readme exists locally"

# 2. README.md has content beyond the initial "# Project" line
LINE_COUNT=$(wc -l < README.md 2>/dev/null | tr -d ' ')
check "$([ $LINE_COUNT -gt 1 ] && echo true || echo false)" "README.md has more than the initial line"

# 3. Branch was pushed to origin
check "$(git log origin/add-readme --oneline 2>/dev/null | wc -l | grep -qv '^0$' && echo true || echo false)" "add-readme branch pushed to origin"

# 4. No merge commits in add-readme history (rebase keeps history linear)
check "$(! git log --oneline add-readme 2>/dev/null | grep -qi 'merge' && echo true || echo false)" "add-readme history has no merge commits (rebased)"

# 5. other.txt exists — proof that the remote update was incorporated via rebase
check "$(test -f other.txt && echo true || echo false)" "other.txt exists (upstream changes incorporated)"

echo "1..$count"
exit $fail
