import {
  Children,
  createContext,
  isValidElement,
  use,
  useCallback,
  useEffect,
  useRef,
  useState,
  type ReactNode,
} from "react";
import {
  AnimatePresence,
  motion,
  MotionConfig,
  useReducedMotion,
} from "framer-motion";

type TabsContextValue = {
  readonly activeTab: string;
  readonly direction: number;
  readonly selectTab: (id: string) => void;
  readonly registerTab: (id: string) => void;
};

const TabsContext = createContext<TabsContextValue | null>(null);

function useTabsContext() {
  const ctx = use(TabsContext);
  if (!ctx) throw new Error("Tabs components must be used within <Tabs>");
  return ctx;
}

const panelVariants = {
  enter: (d: number) => ({ opacity: 0, x: d * 40 }),
  center: { opacity: 1, x: 0 },
  exit: (d: number) => ({ opacity: 0, x: d * -40 }),
};

function TabsRoot({
  children,
  defaultTab,
}: {
  readonly children: ReactNode;
  readonly defaultTab: string;
}) {
  const [activeTab, setActiveTab] = useState(defaultTab);
  const [direction, setDirection] = useState(0);
  const tabIds = useRef<string[]>([]);
  const activeTabRef = useRef(defaultTab);
  const prefersReducedMotion = useReducedMotion();

  const registerTab = useCallback((id: string) => {
    if (!tabIds.current.includes(id)) {
      tabIds.current.push(id);
    }
  }, []);

  const selectTab = useCallback((id: string) => {
    const oldIndex = tabIds.current.indexOf(activeTabRef.current);
    const newIndex = tabIds.current.indexOf(id);
    setDirection(newIndex > oldIndex ? 1 : -1);
    activeTabRef.current = id;
    setActiveTab(id);
  }, []);

  return (
    <TabsContext value={{ activeTab, direction, selectTab, registerTab }}>
      <MotionConfig
        transition={
          prefersReducedMotion
            ? { duration: 0 }
            : { type: "spring", bounce: 0.15, duration: 0.35 }
        }
      >
        <div className="tabs">{children}</div>
      </MotionConfig>
    </TabsContext>
  );
}

function TabList({ children }: { readonly children: ReactNode }) {
  return (
    <div className="tab-list" role="tablist">
      {children}
    </div>
  );
}

function Tab({
  id,
  children,
}: {
  readonly id: string;
  readonly children: ReactNode;
}) {
  const { activeTab, selectTab, registerTab } = useTabsContext();
  const isActive = activeTab === id;

  useEffect(() => {
    registerTab(id);
  }, [id, registerTab]);

  return (
    <button
      className={`tab-trigger${isActive ? " tab-trigger-active" : ""}`}
      role="tab"
      aria-selected={isActive}
      onClick={() => selectTab(id)}
    >
      {children}
      {isActive && (
        <motion.div className="tab-indicator" layoutId="tab-indicator" />
      )}
    </button>
  );
}

function TabContent({ children }: { readonly children: ReactNode }) {
  const { activeTab, direction } = useTabsContext();

  const activeChild = Children.toArray(children).find(
    (child) =>
      isValidElement<{ readonly id: string }>(child) &&
      child.props.id === activeTab,
  );

  return (
    <div className="tab-content">
      <AnimatePresence mode="wait" custom={direction} initial={false}>
        {activeChild}
      </AnimatePresence>
    </div>
  );
}

function TabPanel({
  id,
  children,
}: {
  readonly id: string;
  readonly children: ReactNode;
}) {
  const { direction } = useTabsContext();

  return (
    <motion.div
      key={id}
      className="tab-panel"
      role="tabpanel"
      custom={direction}
      variants={panelVariants}
      initial="enter"
      animate="center"
      exit="exit"
    >
      {children}
    </motion.div>
  );
}

const Tabs = Object.assign(TabsRoot, {
  List: TabList,
  Tab,
  Content: TabContent,
  Panel: TabPanel,
});

export { Tabs };
