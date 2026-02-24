import { Accordion } from "./Accordion";
import "./styles.css";

export default function App() {
  return (
    <main>
      <h1>FAQ</h1>
      <Accordion>
        <Accordion.Item>
          <Accordion.Trigger id="what">What is the compound component pattern?</Accordion.Trigger>
          <Accordion.Content id="what">
            Compound components share implicit state through context, letting the parent manage logic
            while children declare structure. It eliminates boolean prop sprawl and makes the API
            composable -- consumers assemble exactly what they need from discrete parts.
          </Accordion.Content>
        </Accordion.Item>

        <Accordion.Item>
          <Accordion.Trigger id="why">Why use context instead of props?</Accordion.Trigger>
          <Accordion.Content id="why">
            Context decouples the data flow from the component tree structure. Child components access
            shared state without prop drilling through intermediary wrappers, keeping each part focused
            on its own rendering responsibility.
          </Accordion.Content>
        </Accordion.Item>

        <Accordion.Item>
          <Accordion.Trigger id="react19">What changed in React 19?</Accordion.Trigger>
          <Accordion.Content id="react19">
            React 19 introduces the <code>use()</code> hook which replaces <code>useContext()</code>.
            It can be called conditionally and works with both context and promises, streamlining
            how components consume shared values.
          </Accordion.Content>
        </Accordion.Item>
      </Accordion>
    </main>
  );
}
