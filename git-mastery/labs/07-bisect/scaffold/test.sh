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

# 1. bad-commit.txt exists and is not empty
check "$(test -f bad-commit.txt && [ -s bad-commit.txt ] && echo true || echo false)" "bad-commit.txt exists and is not empty"

# 2. The hash in bad-commit.txt resolves to a valid commit object
HASH=$(cat bad-commit.txt 2>/dev/null | tr -d '[:space:]')
check "$(git cat-file -t $HASH 2>/dev/null | grep -q '^commit$' && echo true || echo false)" "bad-commit.txt contains a valid commit hash"

# 3. That commit's message contains "commit 9" — it is the actual first bad commit
check "$(git show $HASH --format='%s' -s 2>/dev/null | grep -q 'commit 9' && echo true || echo false)" "identified commit is 'commit 9' (the first bad commit)"

echo "1..$count"
exit $fail
