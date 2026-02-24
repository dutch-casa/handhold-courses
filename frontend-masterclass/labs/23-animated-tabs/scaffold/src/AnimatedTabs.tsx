// TODO: Import from "react":
//   createContext, use, useCallback, useEffect, useRef, useState,
//   Children, isValidElement, type ReactNode

// TODO: Import from "framer-motion":
//   motion, AnimatePresence, useReducedMotion, MotionConfig

// ── Context ──────────────────────────────────────────────────
// TODO: Define TabsContextValue type
//   activeTab: string
//   direction: number (1 = forward, -1 = backward)
//   selectTab: (id: string) => void
//   registerTab: (id: string) => void

// TODO: Create TabsContext with createContext (default: null)
// TODO: Create useTabsContext hook — use(TabsContext), throw if null

// ── Root ─────────────────────────────────────────────────────
// TODO: Create TabsRoot component
// Props: children: ReactNode, defaultTab: string
//
// State:
//   - activeTab (string, init to defaultTab)
//   - direction (number, init to 0)
// Refs:
//   - tabIds: string[] — ordered list of registered tab IDs
//   - activeTabRef: string — mirrors activeTab for stable callbacks
//
// Hooks:
//   - useReducedMotion() — true when user prefers reduced motion
//
// Functions:
//   - registerTab(id): push id to tabIds ref if not already present
//   - selectTab(id): compare indexOf old vs new in tabIds,
//     set direction to 1 (forward) or -1 (backward), update state
//
// Render:
//   <TabsContext value={...}>
//     <MotionConfig transition={reducedMotion ? { duration: 0 } : { type: "spring", bounce: 0, duration: 0.6 }}>
//       <div className="tabs">{children}</div>
//     </MotionConfig>
//   </TabsContext>

// ── TabList ──────────────────────────────────────────────────
// TODO: Create TabList component
// Renders: <div className="tab-list" role="tablist">{children}</div>

// ── Tab ──────────────────────────────────────────────────────
// TODO: Create Tab component
// Props: id: string, children: ReactNode
//
// - Get activeTab, selectTab, registerTab from context
// - useEffect to call registerTab(id) on mount
// - Render a <button> with:
//     className: "tab-trigger" + " tab-trigger-active" when active
//     role="tab", aria-selected={isActive}
//     onClick → selectTab(id)
// - When active, render inside the button:
//     <motion.div className="tab-indicator" layoutId="tab-indicator" />

// ── TabContent ───────────────────────────────────────────────
// TODO: Create TabContent component
// Props: children: ReactNode
//
// - Get activeTab, direction from context
// - Find the active child panel:
//     Children.toArray(children).find(child => isValidElement(child) && child.props.id === activeTab)
// - Render:
//     <div className="tab-content">
//       <AnimatePresence mode="popLayout" custom={direction} initial={false}>
//         {activeChild}
//       </AnimatePresence>
//     </div>

// ── TabPanel ─────────────────────────────────────────────────
// TODO: Create TabPanel component
// Props: id: string, children: ReactNode
//
// - Get direction from context
// - Define variants:
//     enter: (d) => ({ opacity: 0, x: d * 80 })
//     center: { opacity: 1, x: 0 }
//     exit: (d) => ({ opacity: 0, x: d * -80 })
// - Render:
//     <motion.div
//       key={id} className="tab-panel" role="tabpanel"
//       custom={direction} variants={...}
//       initial="enter" animate="center" exit="exit"
//     >

// ── Compound export ──────────────────────────────────────────
// TODO: Combine with Object.assign:
//   const Tabs = Object.assign(TabsRoot, { List: TabList, Tab, Content: TabContent, Panel: TabPanel });
//   export { Tabs };
