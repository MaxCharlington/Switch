#include <type_traits>
#include <concepts>
#include <iostream>
#include <cassert>


// Default_helper equal to anything and always true
struct Default_helper {
    constexpr Default_helper(auto...) {}
    constexpr bool operator==(const auto& val) {
        return true;
    }
    constexpr operator bool() {
        return true;
    }
};

constexpr bool operator==(auto, Default_helper) {
    return true;
}


// Cases
struct CaseBase{};

template <typename T, std::invocable Action>
struct Case : public CaseBase{
    using type = T;
    
    T val;
    Action act;

    constexpr Case(T val, Action act) : val{val}, act{act} {}
};

template <std::invocable Action>
struct Default : Case<Default_helper, Action> {
    constexpr Default(Action act) : Case<Default_helper, Action>{true, act} {}
};


template<typename T>
concept Case_c = std::derived_from<T, CaseBase>;

template<typename T>
concept notCase_c = !std::derived_from<T, CaseBase>;



struct Switch {
    // Compare based
    constexpr Switch(notCase_c auto val, Case_c auto cur_case, Case_c auto... cases) {
        static_assert(std::equality_comparable_with<typename decltype(cur_case)::type, decltype(val)>);
        if (val == cur_case.val) {
            cur_case.act();
            if constexpr(false) {}  // Implement Fallthrough
        }
        else {
            Switch{val, cases...};
        }
    }

    constexpr Switch(notCase_c auto) {}


    // Bool based
    constexpr Switch(Case_c auto cur_case, Case_c auto... cases) {
        static_assert(
            std::is_same_v<typename decltype(cur_case)::type, bool> ||
            std::is_same_v<typename decltype(cur_case)::type, Default_helper>
        );
        if (cur_case.val) {
            cur_case.act();
        }
        else {
            Switch{cases...};
        }
    }

    consteval Switch() {}
};


// static_assert(std::is_same_v<Default<decltype([]{})>::type, Default_helper>);
// static_assert(std::equality_comparable_with<Default_helper, int>);
// static_assert(Default_helper{} == 1);
//assert(Default{[]{}}.val == true);

