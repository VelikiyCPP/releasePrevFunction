#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>

template <typename Iterator>
using iterator_traits_t = std::iterator_traits<Iterator>;

template <typename Iterator>
using difference_type_t = typename iterator_traits_t<Iterator>::difference_type;

template <typename Iterator>
struct is_bidirectional_iterator : std::is_base_of<std::bidirectional_iterator_tag, typename iterator_traits_t<Iterator>::iterator_category> {};

template <typename Iterator>
constexpr bool is_bidirectional_iterator_v = is_bidirectional_iterator<Iterator>::value;

template <typename Iterator>
Iterator my_prev(Iterator it, difference_type_t<Iterator> offset = 1) {
    if (offset < 0) {
        throw std::invalid_argument("Offset must be a non-negative number");
    }

    if constexpr (std::is_copy_constructible_v<Iterator> && std::is_assignable_v<Iterator, Iterator>) {
        Iterator iterator_copy = it;

        if constexpr (is_bidirectional_iterator_v<Iterator>) {
            return iterator_copy - offset;
        }

        for (difference_type_t<Iterator> i{}; i!= offset; ++i) {
            --iterator_copy;
        }

        return iterator_copy;
    } 

    for (difference_type_t<Iterator> i{}; i!= offset; ++i) {
        --it;
    }
    return it;
}

int main() {
    std::vector<int> vec {1,2,3,4,5,6,7,8,9,10};
    auto it = vec.begin() + 3; 

    try {
        std::cout << "Iterator: " << *my_prev(it, 2) << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
