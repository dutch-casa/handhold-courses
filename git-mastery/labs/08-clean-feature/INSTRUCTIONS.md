# Lab 8: Clean Feature Branch

Implement a function on a dedicated branch and push it with a single conventional commit.

## Tasks

- [ ] Create branch `feat/add-greeting` from `main`
- [ ] Implement `greet(name)` in `greeting.js` so it returns `"Hello, <name>!"`
- [ ] Export the function: `module.exports = { greet }`
- [ ] Commit with the EXACT message: `feat: add greet function`
- [ ] Push the branch to `origin`

## Hints

- Your working directory is `local/` (a git clone inside your workspace).
- `git checkout -b feat/add-greeting` creates and switches to the new branch.
- The function signature: `function greet(name) { return "Hello, " + name + "!"; }`
- Or using a template literal: `` return `Hello, ${name}!`; ``
- The commit message must be **exactly**: `feat: add greet function` — spacing and case matter.
- `git push -u origin feat/add-greeting` sets the upstream and pushes in one step.
- After pushing, `git log origin/feat/add-greeting --oneline` should show your commit.

Use **Run Tests** in the testing panel to check your progress.
