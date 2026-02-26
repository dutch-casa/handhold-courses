#!/bin/bash
# TAP wrapper for ctest — converts GoogleTest/ctest output to TAP format
# Usage: bash tap_runner.sh [build_dir]

BUILD_DIR="${1:-build}"

# Build first
cmake --build "$BUILD_DIR" 2>&1
if [ $? -ne 0 ]; then
  echo "1..1"
  echo "not ok 1 - compilation failed"
  exit 1
fi

# Run ctest and capture output
cd "$BUILD_DIR"
CTEST_OUTPUT=$(ctest --output-on-failure 2>&1)
CTEST_EXIT=$?
cd ..

# Parse ctest output into TAP
count=0
fail=0
results=""

while IFS= read -r line; do
  if echo "$line" | grep -qE '^\s*\d+/\d+\s+Test\s+#\d+:'; then
    test_name=$(echo "$line" | sed -E 's/.*Test\s+#[0-9]+:\s+//' | sed -E 's/\s*\.+.*//')
    if echo "$line" | grep -q "Passed"; then
      count=$((count + 1))
      results="${results}ok ${count} - ${test_name}\n"
    elif echo "$line" | grep -q "Failed"; then
      count=$((count + 1))
      fail=$((fail + 1))
      results="${results}not ok ${count} - ${test_name}\n"
    fi
  fi
done <<< "$CTEST_OUTPUT"

# If no tests were parsed, try fallback parsing
if [ $count -eq 0 ]; then
  while IFS= read -r line; do
    if echo "$line" | grep -qE '\[\s*OK\s*\]'; then
      test_name=$(echo "$line" | sed -E 's/.*\]\s*//')
      count=$((count + 1))
      results="${results}ok ${count} - ${test_name}\n"
    elif echo "$line" | grep -qE '\[\s*FAILED\s*\]'; then
      test_name=$(echo "$line" | sed -E 's/.*\]\s*//')
      count=$((count + 1))
      fail=$((fail + 1))
      results="${results}not ok ${count} - ${test_name}\n"
    fi
  done <<< "$CTEST_OUTPUT"
fi

# If still no tests found, report the raw output
if [ $count -eq 0 ]; then
  echo "1..1"
  if [ $CTEST_EXIT -eq 0 ]; then
    echo "ok 1 - all tests passed"
  else
    echo "not ok 1 - tests failed (see output below)"
    echo "# $CTEST_OUTPUT"
  fi
  exit $CTEST_EXIT
fi

echo "1..${count}"
printf "$results"
exit $fail
