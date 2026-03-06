# Lab 3: Navigate History and Undo Changes

Find a specific commit in history, restore a corrupted file, and revert a bad commit.

## Tasks

- [ ] Find the commit where `feature.txt` was first added and write its full hash to `answer.txt`
- [ ] Restore `config.txt` to its last committed state (discard the working-tree change)
- [ ] Create a revert commit that undoes the most recent non-revert commit (`add final`)

## Hints

- `git log --oneline` gives a compact history. The first column is the short hash.
- `git log --diff-filter=A --name-only --oneline` shows only commits that added files.
- `git show <hash> --name-only` lists the files changed in a commit.
- `git restore config.txt` discards working-tree changes to a file (git 2.23+).
  Older syntax: `git checkout -- config.txt`
- `git revert <hash>` creates a new commit that is the inverse of the target commit.
  The commit to revert is the most recent one: `git revert HEAD`.
- The revert commit message is set automatically — no need to change it.

Use **Run Tests** in the testing panel to check your progress.
