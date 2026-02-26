# Lab 15: Profile Card

Build a `ProfileCard` component with typed props. This is your first React component from scratch.

## Tasks

1. **Define a `ProfileCardProps` interface** in `ProfileCard.tsx`. Fields: `readonly name: string`, `readonly avatarUrl: string`, `readonly bio: string`, `readonly skills: readonly string[]`.

2. **Create the `ProfileCard` function component** that accepts `ProfileCardProps`. Destructure props in the parameter list.

3. **Render the avatar** as an `<img>` with `src={avatarUrl}` and `alt={name}`. Wrap it in a `div` with `className="avatar"`.

4. **Render name and bio** — `name` in an `<h2>`, `bio` in a `<p>`.

5. **Render skills as a list** — map over `skills` and render each as an `<li>` inside a `<ul>`. Each `<li>` needs a `key`.

6. **Use the component in `App.tsx`** — render at least one `<ProfileCard>` with all four props filled in.

## Hints

- Export both the interface and the component as named exports.
- Use `skills.map((skill) => <li key={skill}>{skill}</li>)` for the list.
- The `key` should be unique per item — the skill string itself works here since skills won't repeat.
- `readonly` on props prevents accidental mutation. This matches the course doctrine.
- Run `npm run dev` to start a live preview. Open the URL in your browser to see your component as you build it.
