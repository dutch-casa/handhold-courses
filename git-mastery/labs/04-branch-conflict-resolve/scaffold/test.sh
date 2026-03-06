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

# 1. No unresolved conflict markers in the working tree
check "$(! grep -q '^<<<<<<< \|^=======$\|^>>>>>>> ' README.md 2>/dev/null && echo true || echo false)" "no conflict markers in README.md"

# 2. Working tree is clean
WT=$(git status --short 2>/dev/null | grep -v '^??' || true)
check "$([ -z "$WT" ] && echo true || echo false)" "working tree is clean"

# 3. A merge commit exists in the log
check "$(git log --oneline 2>/dev/null | grep -qi 'merge' && echo true || echo false)" "merge commit exists in history"

# 4. README.md contains main's content
check "$(grep -q 'amazing things' README.md 2>/dev/null && echo true || echo false)" "README.md contains 'amazing things' (main branch content)"

# 5. README.md contains feature's content
check "$(grep -q 'npm install' README.md 2>/dev/null && echo true || echo false)" "README.md contains 'npm install' (feature branch content)"

# 6. README.md has at least 5 lines
LINE_COUNT=$(wc -l < README.md 2>/dev/null | tr -d ' ')
check "$([ "$LINE_COUNT" -ge 5 ] && echo true || echo false)" "README.md has at least 5 lines (both paragraphs present)"

echo "1..$count"
exit $fail
