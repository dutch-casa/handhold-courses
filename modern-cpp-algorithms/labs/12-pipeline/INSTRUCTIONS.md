# Pipeline Processor

Filter and transform data using lambdas and `std::function`.

## Tasks

- [ ] Implement `filter_positive(const vector<double>& data)` — keep only positive values
- [ ] Implement `transform_abs(const vector<double>& data)` — return absolute values of all elements
- [ ] Implement `sum_if(const vector<double>& data, function<bool(double)> predicate)` — sum elements where predicate returns true
- [ ] Implement `pipeline(const vector<double>& data, function<bool(double)> filter, function<double(double)> transform)` — filter elements, then transform the survivors

## Hints

Use `std::copy_if` or a range-based for loop with a push_back for filtering.

`std::transform` or a loop with `std::abs` works for `transform_abs`.

For `pipeline`, apply the filter first to get a subset, then apply the transform to each surviving element.

`std::function<bool(double)>` can wrap any callable: lambda, function pointer, or functor.

Use `Run Tests` in the testing panel to check your progress.
