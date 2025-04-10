#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

using FVector2 = pair<int, int>;

const vector<FVector2> Offset
{
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
};

const vector<FVector2> CheckOffset
{
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 0},
    {0, 1},
    {1, 0}
};

int R, C, K;
vector<vector<int>> Forest;
vector<vector<bool>> ExitPosition;

bool IsLocatedInForest(int x, int y)
{
    return 3 <= x && x < R + 3 && 0 <= y && y < C;
}

void Clear()
{
    Forest.assign(R + 3, vector<int>(C, 0));
    ExitPosition.assign(R + 3, vector<bool>(C, false));
}

int GetLowestRowOf(const int x, const int y)
{
    int LowestRow = x;

    queue<FVector2> Queue;
    vector<vector<bool>> Visited(R + 3, vector<bool>(C, false));

    Queue.emplace(x, y);
    Visited[x][y] = true;

    while(!Queue.empty())
    {
        const auto [r, c] = Queue.front();
        Queue.pop();

        if(r > LowestRow)
        {
            LowestRow = r;
        }

        for(const auto& [dr, dc] : Offset)
        {
            int nr = r + dr, nc = c + dc;
            if(!IsLocatedInForest(nr, nc) || Visited[nr][nc] || Forest[nr][nc] == 0)
            {
                continue;
            }

            if(Forest[r][c] == Forest[nr][nc]
            || ExitPosition[r][c])
            {
                Visited[nr][nc] = true;
                Queue.emplace(nr, nc);
            }
        }
    }

    return LowestRow - 3 + 1;
}

bool CanMoveTo(int x, int y)
{
    if(x + 1 >= R + 3 || y - 1 < 0 || y + 1 >= C)
    {
        return false;
    }

    for(const auto& [dx, dy] : CheckOffset)
    {
        int nx = x + dx, ny = y + dy;
        if(Forest[nx][ny] > 0)
        {
            return false;
        }
    }

    return true;
}

void Arrive(int x, int y, int d, int Index)
{
    Forest[x][y] = Index;
    for(const auto& [dx, dy] : Offset)
    {
        Forest[x + dx][y + dy] = Index;
    }

    const auto& [dx, dy] = Offset[d];
    ExitPosition[x + dx][y + dy] = true;
}

int Move(int x, int y, int d, int Index)
{
    for(const auto& [dx, dy, nd] 
        : vector<tuple<int, int, int>>{{1, 0, d}, {1, -1, (d + 3) % 4}, {1, 1, (d + 1) % 4}})
    {
        int nx = x + dx, ny = y + dy;
        if(CanMoveTo(nx, ny))
        {
            return Move(nx, ny, nd, Index);
        }
    }

    // + 모양이므로 ㅁ 모양으로 생각, 좌상단/우하단 두 점만 체크
    if(!IsLocatedInForest(x - 1, y - 1) || !IsLocatedInForest(x + 1, y + 1))
    {
        return -1;
    }

    Arrive(x, y, d, Index);
    return GetLowestRowOf(x, y);
}

int main() 
{
    cin.tie(nullptr)->sync_with_stdio(false);

    cin >> R >> C >> K;
    Clear();

    int Answer = 0;
    for(int Index = 1; Index <= K; ++Index)
    {
        int y, d; cin >> y >> d; y--;

        int RetVal = Move(0, y, d, Index);
        if(RetVal == -1)
        {
            Clear();
        }
        else
        {
            Answer += RetVal;
        }
    }

    cout << Answer;
    return 0;
}