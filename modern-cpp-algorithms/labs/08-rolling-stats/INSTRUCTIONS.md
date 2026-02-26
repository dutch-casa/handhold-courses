# Rolling Statistics

Compute rolling mean, median, and standard deviation over a `vector<double>` using `std::span` for windowing.

## Tasks

- [ ] Implement `rolling_mean(span<const double> data, int window_size)` — mean of the last `window_size` elements
- [ ] Implement `rolling_median(span<const double> data, int window_size)` — median of the last `window_size` elements
- [ ] Implement `rolling_stddev(span<const double> data, int window_size)` — population standard deviation of the last `window_size` elements

## Hints

Use `data.last(window_size)` to extract the trailing window as a subspan.

For median, copy the window into a local vector and use `std::sort` or `std::nth_element`. Median of an even-length window is the average of the two middle values.

Standard deviation: compute mean first, then the average of squared deviations, then `std::sqrt`.

If `window_size` exceeds `data.size()`, use the entire span.

Use `Run Tests` in the testing panel to check your progress.
