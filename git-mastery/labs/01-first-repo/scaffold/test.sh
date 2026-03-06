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

# 1. A git repo exists
check "$(git rev-parse --git-dir > /dev/null 2>&1 && echo true || echo false)" "git repo exists"

# 2. At least one commit exists
check "$(git log 2>/dev/null | grep -c 'commit' | grep -qv '^0$' && echo true || echo false)" "at least one commit exists"

# 3. A commit with message containing "first commit" exists (case-insensitive)
check "$(git log --format='%s' 2>/dev/null | grep -qi 'first commit' && echo true || echo false)" "commit message contains 'first commit'"

# 4. Working tree is clean (untracked scaffold files are ignored)
WT=$(git status --short 2>/dev/null | grep -v '^??' || true)
check "$([ -z "$WT" ] && echo true || echo false)" "working tree is clean"

# 5. hello.txt is tracked by git
check "$(git ls-files 2>/dev/null | grep -q 'hello.txt' && echo true || echo false)" "hello.txt is committed"

echo "1..$count"
exit $fail
