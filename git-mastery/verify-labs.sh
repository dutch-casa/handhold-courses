#!/bin/bash
# Run all lab tests against their solutions in Docker.
# Usage: bash verify-labs.sh [lab-number]  (omit to run all)

COURSE_DIR="$(cd "$(dirname "$0")" && pwd)"
PASS=0
FAIL=0

run_lab() {
  local num=$1
  local dir
  dir=$(ls -d "$COURSE_DIR/labs/${num}-"* 2>/dev/null | head -1)
  if [ -z "$dir" ]; then
    echo "  SKIP: lab $num not found"
    return
  fi
  local name
  name=$(basename "$dir")
  echo ""
  echo "=== $name ==="

  local yaml="$dir/lab.yaml"
  local solve="$dir/solution/solve.sh"
  local scaffold="$dir/scaffold"

  if [ ! -f "$solve" ]; then
    echo "  SKIP: no solution/solve.sh"
    return
  fi

  local image
  image=$(grep '^image:' "$yaml" | awk '{print $2}')
  local test_cmd
  test_cmd=$(grep '^test:' "$yaml" | sed 's/^test: *//')

  # Build a single entrypoint script: scaffold copy + setup + solve + test
  local run_script
  run_script=$(mktemp /tmp/lab-run-XXXXXX.sh)

  cat > "$run_script" << 'SCRIPTEOF'
#!/bin/bash
set -e
SCRIPTEOF

  # 1. Copy non-test scaffold files to /workspace (simulates Handhold's workspace:fresh).
  #    test.sh stays in /handhold/scaffold — Handhold runs it from outside the learner workspace.
  echo "mkdir -p /workspace" >> "$run_script"
  echo "for f in /handhold/scaffold/*; do [ \"\$(basename \$f)\" != test.sh ] && cp \"\$f\" /workspace/ || true; done" >> "$run_script"
  echo "" >> "$run_script"

  # 2. Setup commands from yaml
  awk '/^setup:/{found=1; next} found && /^[^ ]/{found=0} found && /^  - /{sub(/^  - /, ""); print}' "$yaml" >> "$run_script"
  echo "" >> "$run_script"

  # 3. Solution
  echo "bash /handhold/solution/solve.sh" >> "$run_script"
  echo "" >> "$run_script"

  # 4. Tests — if test command uses a relative path (e.g. "bash test.sh"),
  #    resolve it against the scaffold dir so it doesn't pollute /workspace.
  if [[ "$test_cmd" == bash\ test.sh* ]]; then
    echo "cd /workspace && bash /handhold/scaffold/test.sh" >> "$run_script"
  else
    echo "cd /workspace && $test_cmd" >> "$run_script"
  fi

  local result="PASS"
  # Labs 05/08/09/10 use /workspace/local as the git root; their setup commands
  # reference /workspace/scaffold/. Mount scaffold there too for those labs.
  local extra_mount=""
  if [[ "$test_cmd" == *"/workspace/local/"* ]]; then
    extra_mount="-v $scaffold:/workspace/scaffold:ro"
  fi

  local cid
  # shellcheck disable=SC2086
  cid=$(docker run -d --rm \
    -v "$scaffold:/handhold/scaffold:ro" \
    $extra_mount \
    -v "$solve:/handhold/solution/solve.sh:ro" \
    -v "$run_script:/handhold/run.sh:ro" \
    "$image" \
    sleep 300)

  if docker exec -i "$cid" bash /handhold/run.sh > /tmp/lab-out.txt 2>&1; then
    echo "  PASS"
    grep -E "^(ok|not ok)" /tmp/lab-out.txt || true
  else
    echo "  FAIL"
    tail -30 /tmp/lab-out.txt
    result="FAIL"
  fi

  docker stop "$cid" > /dev/null 2>&1 || true
  rm -f "$run_script"

  if [ "$result" = "PASS" ]; then
    PASS=$((PASS + 1))
  else
    FAIL=$((FAIL + 1))
  fi
}

TARGET="$1"

if [ -n "$TARGET" ]; then
  run_lab "$TARGET"
else
  for n in 01 02 03 04 05 06 07 08 09 10 11; do
    run_lab "$n"
  done
fi

echo ""
echo "==========================="
echo "Results: $PASS passed, $FAIL failed"
echo "==========================="
[ "$FAIL" -eq 0 ]
