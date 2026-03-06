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

# 1. answers.txt exists
check "$(test -f answers.txt && echo true || echo false)" "answers.txt exists"

# 2. answers.txt contains the word "blob" on its own line
check "$(grep -qx 'blob' answers.txt 2>/dev/null && echo true || echo false)" "answers.txt contains 'blob'"

# 3. answers.txt contains the word "tree" on its own line
check "$(grep -qx 'tree' answers.txt 2>/dev/null && echo true || echo false)" "answers.txt contains 'tree'"

# 4. answers.txt contains the word "commit" on its own line
check "$(grep -qx 'commit' answers.txt 2>/dev/null && echo true || echo false)" "answers.txt contains 'commit'"

# 5. first-commit.txt exists
check "$(test -f first-commit.txt && echo true || echo false)" "first-commit.txt exists"

# 6. The content of first-commit.txt is a valid commit hash
HASH=$(cat first-commit.txt 2>/dev/null | tr -d '[:space:]')
check "$(git cat-file -t $HASH 2>/dev/null | grep -q '^commit$' && echo true || echo false)" "first-commit.txt contains a valid commit hash"

echo "1..$count"
exit $fail
