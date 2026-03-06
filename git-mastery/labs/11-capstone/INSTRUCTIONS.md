# Lab 11: Capstone — Full Git Workflow

Complete the full professional workflow: branch, implement, squash, push, merge to main.

## Tasks

- [ ] Create branch `feat/calculator` from `main`
- [ ] Implement `add(a, b)`, `subtract(a, b)`, and `multiply(a, b)` in `calculator.js`
- [ ] Export them: `module.exports = { add, subtract, multiply }`
- [ ] Make at least 2 commits as you work (commit messages can be anything)
- [ ] Use `git rebase -i` to squash ALL your commits into ONE with message: `feat: add calculator module`
- [ ] Push the branch to `origin`: `git push origin feat/calculator`
- [ ] Merge your branch into `main` and push main

## Hints

- Your working directory is `local/` (a git clone inside your workspace).
- `git rebase -i HEAD~<N>` where `<N>` is the number of commits you made on the branch.
  Use `fixup` for all commits after the first, and `reword` (or edit) the first to set the message.
- The squashed commit message must be **exactly**: `feat: add calculator module`
- To merge: `git checkout main && git merge --no-ff feat/calculator`
  This preserves the branch topology and creates a merge commit.
  Alternatively: `git merge feat/calculator` (fast-forward, also accepted — the test only checks content and commit count).
- After merging, push main: `git push origin main`
- The test checks that `main` has exactly 2 commits total: the initial setup commit plus your squashed feature commit.

Use **Run Tests** in the testing panel to check your progress.
