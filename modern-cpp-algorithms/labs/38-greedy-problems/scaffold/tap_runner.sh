#!/bin/bash
# TAP wrapper for ctest
BUILD_DIR="${1:-build}"
cmake --build "$BUILD_DIR" 2>&1
if [ $? -ne 0 ]; then
  echo "1..1"
  echo "not ok 1 - compilation failed"
  exit 1
fi
cd "$BUILD_DIR"
CTEST_OUTPUT=$(ctest --output-on-failure 2>&1)
CTEST_EXIT=$?
cd ..
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
if [ $count -eq 0 ]; then
  echo "1..1"
  if echo "$CTEST_OUTPUT" | grep -q "No tests were found"; then
    echo "not ok 1 - no tests discovered (check CMakeLists.txt)"
    exit 1
  elif [ $CTEST_EXIT -eq 0 ]; then
    echo "ok 1 - all tests passed"
  else
    echo "not ok 1 - tests failed"
    echo "# $CTEST_OUTPUT"
  fi
  exit $CTEST_EXIT
fi
echo "1..${count}"
printf "$results"
exit $fail
