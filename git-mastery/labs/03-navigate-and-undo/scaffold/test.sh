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

# 1. answer.txt exists
check "$(test -f answer.txt && echo true || echo false)" "answer.txt exists"

# 2. The hash in answer.txt is a valid commit object
HASH=$(cat answer.txt 2>/dev/null | tr -d '[:space:]')
check "$(git cat-file -t $HASH 2>/dev/null | grep -q '^commit$' && echo true || echo false)" "answer.txt contains a valid commit hash"

# 3. That commit is the one that first added feature.txt
check "$(git show $HASH --name-only --format='' 2>/dev/null | grep -q 'feature.txt' && echo true || echo false)" "commit in answer.txt first added feature.txt"

# 4. config.txt matches its last committed state (no diff against HEAD)
check "$(git diff HEAD -- config.txt 2>/dev/null | grep -q '.' && echo false || echo true)" "config.txt is restored to last committed state"

# 5. The most recent commit message starts with "Revert"
check "$(git log --format='%s' 2>/dev/null | head -1 | grep -qi '^Revert' && echo true || echo false)" "most recent commit is a revert commit"

echo "1..$count"
exit $fail
