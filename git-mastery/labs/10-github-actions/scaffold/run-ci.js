#!/usr/bin/env node
'use strict';
/**
 * Minimal GitHub Actions runner for local execution.
 * Parses .github/workflows/ci.yml without external dependencies.
 * Stubs common `uses:` actions. Executes `run:` steps for real.
 */
const { execSync } = require('child_process');
const fs = require('fs');

const STUBBED = new Set([
  'actions/checkout',
  'actions/setup-node',
  'actions/setup-python',
  'actions/setup-go',
  'actions/cache',
  'actions/upload-artifact',
  'actions/download-artifact',
]);

let text;
try {
  text = fs.readFileSync('.github/workflows/ci.yml', 'utf8');
} catch {
  console.error('Error: .github/workflows/ci.yml not found');
  process.exit(1);
}

const jobs = extractJobs(text);
if (Object.keys(jobs).length === 0) {
  console.error('Error: no jobs found in workflow file');
  process.exit(1);
}

for (const [name, steps] of Object.entries(jobs)) {
  console.log(`\n==> job: ${name}`);
  for (const step of steps) {
    if (step.uses) {
      const action = step.uses.split('@')[0];
      console.log(`  [${STUBBED.has(action) ? 'stub' : 'skip'}] ${step.uses}`);
      continue;
    }
    if (step.run) {
      const label = (step.name || step.run.split('\n')[0]).slice(0, 60);
      console.log(`  >> ${label}`);
      try {
        execSync(step.run, { stdio: 'inherit', shell: '/bin/bash' });
      } catch (e) {
        console.error(`\nFailed: ${label}`);
        process.exit(typeof e.status === 'number' ? e.status : 1);
      }
    }
  }
}

console.log('\nAll jobs passed.');

/**
 * Minimal YAML parser for GitHub Actions workflow files.
 * Handles the subset used in basic CI workflows:
 *   jobs → steps → uses / run (scalar or block literal) / name
 */
function extractJobs(yaml) {
  const lines = yaml.split('\n');
  const jobs = {};
  let inJobs = false;
  let jobIndent = -1;
  let currentJob = null;
  let inSteps = false;
  let currentStep = null;
  let collectRun = false;
  let runLines = [];
  let runBaseIndent = 0;

  for (const raw of lines) {
    // Detect indent before stripping
    const indent = raw.search(/\S/);
    if (indent < 0) {
      if (collectRun) { finalizeRun(); }
      continue;
    }
    const content = raw.slice(indent).trimEnd();

    // End of multi-line run block
    if (collectRun) {
      if (indent > runBaseIndent) {
        runLines.push(content);
        continue;
      }
      finalizeRun();
    }

    if (content === 'jobs:') {
      inJobs = true;
      jobIndent = -1;
      continue;
    }
    if (!inJobs) continue;

    // Detect job-level indent on first job entry
    if (jobIndent < 0 && /^[a-zA-Z_][a-zA-Z0-9_-]*:\s*$/.test(content)) {
      jobIndent = indent;
    }

    // Job header
    if (indent === jobIndent && /^[a-zA-Z_][a-zA-Z0-9_-]*:\s*$/.test(content)) {
      const name = content.replace(/:\s*$/, '');
      jobs[name] = [];
      currentJob = jobs[name];
      inSteps = false;
      currentStep = null;
      continue;
    }

    if (!currentJob) continue;

    if (content === 'steps:') {
      inSteps = true;
      currentStep = null;
      continue;
    }
    if (!inSteps) continue;

    // New step
    if (content.startsWith('- ')) {
      currentStep = {};
      currentJob.push(currentStep);
      applyKV(content.slice(2).trim(), currentStep, indent);
      continue;
    }

    // Property of current step
    if (currentStep) {
      const m = content.match(/^(uses|run|name|with):\s*(.*)/);
      if (!m || m[1] === 'with') continue;
      const [, key, val] = m;
      const trimVal = val.trim();
      if (key === 'run' && (trimVal === '|' || trimVal === '>' || trimVal === '')) {
        collectRun = true;
        runLines = [];
        runBaseIndent = indent;
      } else if (trimVal) {
        currentStep[key] = trimVal;
      }
    }
  }

  if (collectRun) finalizeRun();
  return jobs;

  function finalizeRun() {
    if (currentStep) currentStep.run = runLines.join('\n');
    collectRun = false;
    runLines = [];
  }
}

function applyKV(content, step) {
  const m = content.match(/^(uses|run|name):\s*(.*)/);
  if (m && m[2].trim()) step[m[1]] = m[2].trim();
}
