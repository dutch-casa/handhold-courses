# Lab 10: Write a CI Workflow

The project already has a `src/add.js` module and `test/add.test.js`. Your job: write the GitHub Actions workflow that runs the tests automatically on every push and pull request.

## Tasks

- [ ] Create `.github/workflows/ci.yml`
- [ ] Trigger on `push` and `pull_request`
- [ ] Define a job with at least one step that runs `npm test`
- [ ] Use `actions/checkout@v4` and `actions/setup-node@v4` as you would in a real workflow

## Hints

The workflow file lives at `.github/workflows/ci.yml` relative to your repo root.

Minimum structure:

```yaml
name: CI

on:
  push:
  pull_request:

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with:
          node-version: '22'
      - run: npm test
```

A local runner (`run-ci.js`) is provided in your workspace. It stubs `actions/checkout` and `actions/setup-node` (already done in the lab environment) and executes your `run:` steps for real. `npm test` runs `node test/add.test.js` — no install needed, no external dependencies.

To test manually: `node run-ci.js`
