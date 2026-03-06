# Lab 7: Find the Bug with git bisect

Use binary search over git history to find the exact commit that introduced a regression.

## Tasks

- [ ] Start bisect mode: `git bisect start`
- [ ] Mark the current HEAD (commit 15) as bad: `git bisect bad`
- [ ] Find commit 1 (the oldest) with `git log --oneline` and mark it as good: `git bisect good <hash>`
- [ ] Run automated bisect using the provided test script: `git bisect run bash check-bug.sh`
- [ ] Write the hash of the first bad commit to `bad-commit.txt`
- [ ] Exit bisect mode: `git bisect reset`

## Hints

- `check-bug.sh` exits `0` if `greet()` returns `"hello"` (good), and `1` if it returns anything else (bad).
- `git bisect run <command>` runs the command at each step and uses the exit code to decide good/bad automatically.
- When bisect finishes it prints the first bad commit — copy that hash.
- The hash can be the full or short form; the test accepts either as long as `git cat-file -t` resolves it.
- After writing the hash to `bad-commit.txt`, you must run `git bisect reset` to return to HEAD.
- `git log --oneline | tail -1` prints the oldest commit (commit 1).

Use **Run Tests** in the testing panel to check your progress.
