# Lab 5: Remote Work — Push, Fetch, Rebase

Work with a remote repository: create a branch, push it, incorporate upstream changes via rebase.

## Tasks

- [ ] Inside the `local/` directory, create a branch named `add-readme`
- [ ] Add at least one paragraph of content to `README.md`
- [ ] Commit the changes and push the branch to `origin`
- [ ] Fetch the latest changes from `origin` (another developer pushed `other.txt`)
- [ ] Rebase `add-readme` onto the updated `origin/main`
- [ ] Push the rebased branch to `origin` (use `--force-with-lease` since history was rewritten)

## Hints

- Your working directory is `local/` (a git clone inside your workspace).
- The remote is at `remote.git/` — a bare repo acting as `origin`.
- Another developer has already pushed a commit to `origin/main`. After you fetch, `origin/main` will be ahead of your local `main`.
- `git fetch origin` downloads new commits without changing your working branch.
- `git rebase origin/main` replays your commits on top of the fetched `origin/main`.
- After rebasing, the branch history has changed, so a normal push will be rejected. Use:
  `git push --force-with-lease origin add-readme`
- Once rebased, `other.txt` will exist in your working tree (it came from the other developer).

Use **Run Tests** in the testing panel to check your progress.
