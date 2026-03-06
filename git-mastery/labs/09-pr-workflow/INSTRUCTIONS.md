# Lab 9: Fork and Pull Request Workflow

Simulate the fork+PR model: clone a fork, make changes on a branch, push to the fork.

## Tasks

- [ ] Confirm the `upstream` remote is configured: `git remote -v`
- [ ] Create branch `feat/hello-world` from `main`
- [ ] Update `src/index.js` to export a `hello()` function that returns `"hello, world"`
- [ ] Commit and push the branch to `origin` (your fork)

## Hints

- Your working directory is `local/` — your clone of the fork.
- `origin` points to `fork.git/` (your fork).
- `upstream` points to `upstream.git/` (the original project).
- In a real workflow you'd open a PR from `origin/feat/hello-world` → `upstream/main`.
- The `hello()` function must be exported for the test to find it:
  ```javascript
  function hello() {
    return "hello, world";
  }
  module.exports = { hello };
  ```
- Push with: `git push -u origin feat/hello-world`
- Note: the return value is lowercase `"hello, world"` — no capital H, comma after hello.

Use **Run Tests** in the testing panel to check your progress.
