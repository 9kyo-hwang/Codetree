#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N, M; cin >> N >> M;
    vector<vector<int>> v(N, vector<int>(M, 0));

    const vector<pair<int, int>> offset{
        {-1, 0},
        {0, 1},
        {1, 0},
        {0, -1}
    };

    int x = 0, y = 0;
    v[x][y] = 1;

    // 북, 동, 남, 서
    int dir = 1;

    for(int i = 2; i <= N * M; ++i) {
        int nx = x + offset[dir].first;
        int ny = y + offset[dir].second;

        // 해당 방향으로 못가면 rotate
        if(nx < 0 || nx >= N || ny < 0 || ny >= M
            || v[nx][ny] != 0) {
                dir = (dir + 1) % 4;
        }

        x += offset[dir].first;
        y += offset[dir].second;

        v[x][y] = i;
    }

    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < M; ++j) {
            cout << v[i][j] << ' ';
        }
        cout << "\n";
    }

    return 0;
}