#include <iostream>
#include <vector>
#include <map>
using namespace std;

/*
4명의 개발자, 한 사람 당 2번
최초: 2번

5초: 2 - 3 O
6초: 2 - 4 O
7초: 1 - 2 x
*/

struct Infection {
    bool infected;
    int count;

    Infection(bool infected = false, int count = 0)
    : infected(infected), count(count)
    {

    }

    inline bool CanInfect() const { 
        return infected && count > 0; 
    }

    void InfectTo(Infection& target, const int K) {
        // 재감염 방지
        if(!target.infected) {
            target.infected = true;
            target.count = K;
        } else {
            --target.count;
        }

        --count;
    }
};

int main() {
    // N명 개발자, T번, 최초 감염자 P, 전염 횟수 K
    int N, K, P, T;
    cin >> N >> K >> P >> T;

    vector<Infection> infections(N + 1);
    infections[P] = {true, K};  // 최초 감염자

    map<int, pair<int, int>> timeline;

    while(T--) {
        // t초에 개발자 x, y가 악수
        int t, x, y;
        cin >> t >> x >> y;

        timeline[t] = {x, y};
    }

    for(const auto& [time, link] : timeline) {
        const auto& [x, y] = link;

        if(infections[x].CanInfect()) {
            infections[x].InfectTo(infections[y], K);
        } else if(infections[y].CanInfect()) {
            infections[y].InfectTo(infections[x], K);
        }
    }

    for(int i = 1; i <= N; ++i) {
        cout << infections[i].infected;
    }

    return 0;
}