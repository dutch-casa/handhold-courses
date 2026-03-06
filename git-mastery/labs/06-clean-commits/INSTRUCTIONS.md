# Lab 6: Clean Commits with Interactive Rebase

Squash 6 messy work-in-progress commits into a single clean conventional commit.

## Tasks

- [ ] Use `git rebase -i HEAD~6` to start an interactive rebase covering all 6 commits
- [ ] Mark the first commit as `reword` and all subsequent commits as `fixup`
- [ ] Set the final commit message to exactly: `feat: add user authentication`

## Hints

- `git rebase -i HEAD~6` opens the rebase instruction list in your editor.
- The list shows oldest-first. Change `pick` to `fixup` for commits 2 through 6.
- Change `pick` to `reword` for the first commit — this lets you rename it.
- `fixup` is like `squash` but silently discards the commit message.
- **This environment suppresses the interactive editor.** Use `GIT_SEQUENCE_EDITOR` to pass
  the rebase instructions non-interactively:
  ```
  GIT_SEQUENCE_EDITOR="sed -i '1s/pick/reword/; 2,\$s/pick/fixup/'" git rebase -i HEAD~6
  ```
  Then set the commit message explicitly:
  ```
  git commit --amend -m "feat: add user authentication"
  ```
- Alternatively, use a plain reset + recommit which produces the same result:
  ```
  git reset HEAD~6
  git add .
  git commit -m "feat: add user authentication"
  ```
- The commit message must be **exactly**: `feat: add user authentication`
- After the rebase, `git log --oneline` should show exactly 1 commit.
- `auth.js` must still contain all 6 lines — squashing only combines the commits, not the content.

Use **Run Tests** in the testing panel to check your progress.
