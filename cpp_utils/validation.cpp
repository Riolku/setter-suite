struct IntegerValidation {
    ll lo, hi;

    bool check(ll x) { return lo <= x && x <= hi; }
    template <range R> bool check(const R &r) {
        for (auto x : r)
            if (!check(x))
                return false;
        return true;
    }

    template <typename T, typename... Ts> bool check(const T &arg, Ts &&... args) {
        return check(arg) && check(args...);
    }
};
