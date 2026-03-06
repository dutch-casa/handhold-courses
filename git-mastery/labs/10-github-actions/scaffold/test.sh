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

# 1. Workflow file exists
check "$(test -f .github/workflows/ci.yml && echo true || echo false)" "workflow file exists at .github/workflows/ci.yml"

# 2. Workflow triggers on push
check "$(grep -q 'push' .github/workflows/ci.yml 2>/dev/null && echo true || echo false)" "workflow triggers on push"

# 3. Workflow triggers on pull_request
check "$(grep -q 'pull_request' .github/workflows/ci.yml 2>/dev/null && echo true || echo false)" "workflow triggers on pull_request"

# 4. Workflow has a jobs section
check "$(grep -q '^jobs:' .github/workflows/ci.yml 2>/dev/null && echo true || echo false)" "workflow has a jobs section"

# 5. Workflow runs npm test
check "$(grep -q 'npm test' .github/workflows/ci.yml 2>/dev/null && echo true || echo false)" "workflow runs npm test"

# 6. Workflow actually executes successfully
check "$(node run-ci.js > /tmp/ci-out.txt 2>&1 && echo true || echo false)" "workflow runs clean (run-ci passes)"

echo "1..$count"
exit $fail
