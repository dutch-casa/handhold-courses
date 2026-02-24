import { Tabs } from "./AnimatedTabs";
import "./styles.css";

export default function App() {
  return (
    <main>
      <h1>Animated Tabs</h1>
      <Tabs defaultTab="design">
        <Tabs.List>
          <Tabs.Tab id="design">Design</Tabs.Tab>
          <Tabs.Tab id="engineering">Engineering</Tabs.Tab>
          <Tabs.Tab id="product">Product</Tabs.Tab>
        </Tabs.List>
        <Tabs.Content>
          <Tabs.Panel id="design">
            <h2>Design</h2>
            <p>
              Great design is invisible. It removes friction, communicates
              intent, and makes the complex feel simple. Every pixel serves a
              purpose.
            </p>
          </Tabs.Panel>
          <Tabs.Panel id="engineering">
            <h2>Engineering</h2>
            <p>
              The best code reads like prose. It handles edge cases the user
              never sees, performs under pressure, and makes the next
              developer&rsquo;s life easier.
            </p>
          </Tabs.Panel>
          <Tabs.Panel id="product">
            <h2>Product</h2>
            <p>
              Ship what matters. Say no to everything else. A product that does
              one thing well beats one that does ten things poorly.
            </p>
          </Tabs.Panel>
        </Tabs.Content>
      </Tabs>
    </main>
  );
}
