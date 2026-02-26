# Lab 18: Fetch and Display Posts

Use React Query to fetch a list of posts, display them, and navigate to post details. The cache should make revisits instant.

## Tasks

1. **Set up QueryClientProvider** in `App.tsx`. Create a `QueryClient` outside the component. Wrap your app in `<QueryClientProvider>`.

2. **Fetch posts** in `PostList.tsx`. Use `useQuery` with key `["posts"]` and fetch from `https://jsonplaceholder.typicode.com/posts` (limit to first 10 with `.slice(0, 10)`). Show a loading state while fetching. Show an error state on failure.

3. **Render the post list** — Map each post to a clickable `<li>`. Display the post title. When clicked, set the `selectedPostId` state.

4. **Fetch post detail** in `PostDetail.tsx`. Use `useQuery` with key `["post", postId]` and fetch from `https://jsonplaceholder.typicode.com/posts/{postId}`. Show loading and error states.

5. **Back button** — A button in `PostDetail` that calls `onBack` (passed as a prop) to clear the selected post and return to the list.

6. **Cache behavior** — Set `staleTime: 60_000` on the QueryClient. Visit a post, go back, visit the same post — the second visit should be instant (cached).

## Hints

- `App.tsx` manages `selectedPostId: number | null` state. When null, show `PostList`. When set, show `PostDetail`.
- The fetch function: `() => fetch(url).then(res => res.json())`.
- `isPending` replaces the old `isLoading` in React Query v5.
- Check `isError` before rendering data — TypeScript narrows the type.
- Run `npm run dev` to start a live preview. Open the URL in your browser to see your component as you build it.
