#if !defined(__FILEP_H__)
#define __FILEP_H__

#include <queue>
#include <utility>
#include <queue>
#include <tuple>

template<class T, class P> // T = type P = priorite  = int indice du lieu et double distance parcourru
class FilePrioritaire {
public:

    inline bool estVide() {
        return elements.empty();
    }

    inline void inserer(const T &element, const P &prioritee) {
        elements.emplace(prioritee, element);
    }

    inline T enleverMinimum() {
        T min = elements.top().second;
        elements.pop();
        return min;
    }

private:
    typedef std::pair <P, T> Element;
    std::priority_queue <Element, std::vector<Element>, std::greater<Element>> elements;

};

#endif
