---
title: Your dev environment
---

# JavaScript beyond the browser

{{show: node-diagram reveal 0.3s spring}} You've been writing JavaScript that runs in the browser. The browser has a JavaScript engine built in — Chrome uses V8, Firefox uses SpiderMonkey. But what if you want to run JavaScript outside the browser? On your machine. On a server. As a build tool.

{{focus: runtime}} {{zoom: 1.2x}} {{pulse: runtime}} {{annotate: runtime "Runtime = engine + APIs"}} That's Node.js. It takes V8 out of Chrome and wraps it with system APIs — file access, networking, processes. Same language, different environment. The browser gives you `document` and `window`. Node gives you `fs` and `path`.

{{zoom: 1x}} {{focus: npm-node}} {{zoom: 1.2x}} {{annotate: npm-node "npm ships with Node"}} Install Node.js and you get two things: the `node` command to run JavaScript files, and `npm` — the Node Package Manager. npm is how you install other people's code. React, TypeScript, Vite — they're all npm packages.

{{zoom: 1x}} {{focus: none}}

```diagram:node-diagram
Browser [label="Browser" shape=box]
V8 [label="V8 Engine"]
NodeJS [label="Node.js" shape=box]
NPM [label="npm"]
FileSystem [label="fs, path, http"]

Browser --> V8
NodeJS --> V8
NodeJS --> FileSystem
NodeJS --> NPM
---
runtime: NodeJS, V8, FileSystem
npm-node: NodeJS, NPM
```

# package.json

{{clear: slide}}

{{show: pkg-code typewriter 2s linear}} Every Node.js project has a `package.json`. It's the manifest — the single file that describes your project, its dependencies, and how to run it.

{{focus: meta}} {{zoom: 1.2x}} {{pulse: meta}} {{annotate: meta "Project identity"}} `name` and `private: true`. The name is for humans. `private` prevents accidental publishing to npm. Every project you build should have this.

{{zoom: 1x}} {{focus: scripts}} {{zoom: 1.2x}} {{pulse: scripts}} {{annotate: scripts "npm scripts"}} The `scripts` block defines commands you can run with `npm run <name>`. `npm run dev` starts a dev server. `npm run build` creates a production bundle. These are just shell commands with a name.

{{zoom: 1x}} {{focus: deps}} {{zoom: 1.2x}} {{annotate: deps "Runtime dependencies"}} `dependencies` — packages your app needs to run. React goes here. The version `^19.0.0` means "19.0.0 or any compatible update." The caret is a safety net — you get bug fixes but not breaking changes.

{{zoom: 1x}} {{focus: dev-deps}} {{zoom: 1.2x}} {{pan: dev-deps}} {{annotate: dev-deps "Build-time only"}} `devDependencies` — packages you need during development but not in production. TypeScript, Vite, linters. Your users never download these. The distinction matters for bundle size.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:pkg-code lang=json
{
  "name": "my-react-app",
  "private": true,
  "scripts": {
    "dev": "vite",
    "build": "tsc && vite build",
    "preview": "vite preview"
  },
  "dependencies": {
    "react": "^19.0.0",
    "react-dom": "^19.0.0"
  },
  "devDependencies": {
    "@types/react": "^19.0.0",
    "@types/react-dom": "^19.0.0",
    "@vitejs/plugin-react": "^4.3.0",
    "typescript": "^5.7.0",
    "vite": "^6.0.0"
  }
}
---
meta: 2-3
scripts: 4-8
deps: 9-12
dev-deps: 13-19
```

# npm install

{{clear: slide}}

{{show: install-code typewriter 1.5s linear}} {{show: tree-diagram reveal 0.3s spring}} You have a `package.json`. Now you need the actual code. `npm install` reads your dependencies and downloads them into a `node_modules` folder.

{{focus: install-cmd}} {{zoom: 1.2x}} {{pulse: install-cmd}} {{annotate: install-cmd "One command"}} Run `npm install` in your project directory. npm reads `package.json`, resolves every dependency (and their dependencies, and their dependencies), and downloads them all. The result is `node_modules` — a folder that can contain thousands of packages.

{{zoom: 1x}} {{focus: lockfile}} {{zoom: 1.2x}} {{annotate: lockfile "Exact versions"}} npm also creates `package-lock.json` — a lockfile that pins exact versions. Your `package.json` says `^19.0.0`. The lockfile says `19.0.1`. This guarantees everyone on the team gets identical dependencies. Commit the lockfile.

{{zoom: 1x}} {{focus: add-pkg}} {{zoom: 1.2x}} {{pan: add-pkg}} {{annotate: add-pkg "Adding packages"}} Want to add a package? `npm install react-query` adds it to `dependencies` and downloads it. `npm install -D vite` adds it to `devDependencies`. The `-D` flag means "development only."

{{pan: none}} {{zoom: 1x}} {{focus: none}} One rule: never edit `node_modules` by hand. It's generated. Delete it and run `npm install` again — same result.

```code:install-code lang=bash
# Install all dependencies from package.json
npm install

# The lockfile pins exact versions
# Commit this to version control
cat package-lock.json

# Add a new dependency
npm install @tanstack/react-query

# Add a dev dependency
npm install -D tailwindcss
---
install-cmd: 2
lockfile: 5-6
add-pkg: 8-12
```

```diagram:tree-diagram
PackageJSON [label="package.json" shape=box]
NPMInstall [label="npm install"]
NodeModules [label="node_modules/" shape=box]
Lockfile [label="package-lock.json" shape=box]

PackageJSON --> NPMInstall
NPMInstall --> NodeModules
NPMInstall --> Lockfile
```

# Vite

{{clear: slide}}

{{show: vite-code typewriter 2s linear}} {{show: vite-diagram reveal 0.3s spring}} You have React installed. You have TypeScript. But the browser doesn't understand `.tsx` files. Something needs to compile your code, serve it to the browser, and refresh when you save. That something is Vite.

{{focus: what-vite}} {{zoom: 1.2x}} {{pulse: what-vite}} {{annotate: what-vite "Dev server + bundler"}} Vite does two things. In development, it runs a lightning-fast dev server with hot module replacement — you save a file, the browser updates instantly without losing state. In production, it bundles your code into optimized files for deployment.

{{zoom: 1x}} {{focus: config}} {{zoom: 1.2x}} {{annotate: config "Minimal config"}} The config is tiny. Import the React plugin. Tell Vite to use it. Done. Vite handles TypeScript, JSX, CSS, images, and hot reloading out of the box. No webpack config hell. No babel. Just works.

{{zoom: 1x}} {{focus: entry}} {{zoom: 1.2x}} {{pan: entry}} {{annotate: entry "Entry point"}} Vite starts from `index.html`. The HTML file has a `<script type="module">` tag pointing to your TypeScript entry point. Vite intercepts, compiles on the fly, and serves the result. The browser thinks it's loading plain JavaScript.

{{pan: none}} {{zoom: 1x}} {{focus: none}}

```code:vite-code lang=typescript
// vite.config.ts — the entire config
import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

export default defineConfig({
  plugins: [react()],
});

// index.html — Vite's entry point
// <script type="module" src="/src/main.tsx"></script>

// src/main.tsx — your app's entry point
import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import App from "./App";

createRoot(document.getElementById("root")!).render(
  <StrictMode>
    <App />
  </StrictMode>,
);
---
config: 2-7
what-vite: 2-7
entry: 13-21
```

```diagram:vite-diagram
IndexHTML [label="index.html" shape=box]
Vite [label="Vite Dev Server"]
TSX [label=".tsx files" shape=box]
Browser [label="Browser" shape=box]
HMR [label="Hot Module Replacement"]

IndexHTML --> Vite
TSX --> Vite
Vite --> Browser
Vite --> HMR
HMR --> Browser
---
what-vite: Vite, HMR
entry: IndexHTML, TSX
```

# Your first React project

{{clear: slide}}

{{show: scaffold-code typewriter 2s linear}} Let's put it all together. One command scaffolds a complete React + TypeScript project with Vite. Then two more commands and you're running it in the browser.

{{focus: create}} {{zoom: 1.2x}} {{pulse: create}} {{annotate: create "Scaffold"}} `npm create vite@latest my-app -- --template react-ts`. This creates a new directory with `package.json`, `vite.config.ts`, `index.html`, `tsconfig.json`, and a `src/` folder with a starter component. Everything wired up. Ready to go.

{{zoom: 1x}} {{focus: run}} {{zoom: 1.2x}} {{pulse: run}} {{annotate: run "Start developing"}} `cd my-app`, `npm install`, `npm run dev`. Vite starts the dev server and prints a URL — usually `http://localhost:5173`. Open it. You see your app. Edit a component. Save. The browser updates instantly.

{{zoom: 1x}} {{focus: structure}} {{zoom: 1.2x}} {{pan: structure}} {{annotate: structure "Project structure"}} The project structure is simple. `src/App.tsx` is your root component. `src/main.tsx` mounts it into the DOM. `index.html` loads `main.tsx`. `vite.config.ts` configures the build. `package.json` tracks dependencies and scripts. That's the whole thing.

{{pan: none}} {{zoom: 1x}} {{focus: none}} From here on, every React lab in this course uses this exact setup. Run `npm install`, then `npm run dev`, and you'll see your work in the browser as you build it.

```code:scaffold-code lang=bash
# Scaffold a new React + TypeScript project
npm create vite@latest my-app -- --template react-ts

# Enter the project and install dependencies
cd my-app
npm install

# Start the dev server
npm run dev
# → Local: http://localhost:5173/

# Project structure:
# my-app/
#   index.html          ← Vite entry point
#   vite.config.ts      ← Build config
#   package.json        ← Dependencies and scripts
#   tsconfig.json       ← TypeScript config
#   src/
#     main.tsx          ← Mounts App into DOM
#     App.tsx           ← Your root component
---
create: 2
run: 5-9
structure: 12-20
```
