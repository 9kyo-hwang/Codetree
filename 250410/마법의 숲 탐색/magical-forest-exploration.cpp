#include <iostream>
#include <vector>
#include <tuple>
#include <queue>

using namespace std;

const vector<pair<int, int>> Offset
{
    {-1, 0},    // 0: N
    {0, 1},     // 1: E
    {1, 0},     // 2: S
    {0, -1}     // 3: W
};

const vector<pair<int, int>> CheckOffsetToDown
{
    {1, -1},    // 0: SW
    {2, 0},     // 1: SS
    {1, 1},     // 2: SE
};

const vector<pair<int, int>> CheckOffsetToLeftDown
{
    {-1, -1},   // 0: NW
    {0, -2},    // 1: WW
    {1, -1},    // 2: SW
    {1, -2},    // 3: SWW
    {2, -1},    // 4: SSW
};

const vector<pair<int, int>> CheckOffsetToRightDown
{
    {1, 1},     // 0: SE
    {0, 2},     // 1: EE
    {-1, 1},    // 2: NE
    {2, 1},     // 3: SSE
    {1, 2},     // 4: SEE
};

using Golem = tuple<int, int, int>; // r, c, d

int R, C;
vector<vector<int>> Forest;
vector<Golem> Golems;

inline bool OutOfBound(const int r, const int c)
{
    return r < 0 || r >= R || c < 0 || c >= C;
}

bool IsGolemOutOfBound(const int r, const int c)
{
    for(const auto& [dr, dc] : Offset)
    {
        if(OutOfBound(r + dr, c + dc))
        {
            return true;
        }
    }

    return false;
}

bool CanMoveToDown(int& r, int& c)
{
    for(const auto& [dr, dc] : CheckOffsetToDown)
    {
        int CheckR = r + dr, CheckC = c + dc;
        if(OutOfBound(CheckR, CheckC))
        {
            if(CheckC < 0 || CheckC >= C || CheckR >= R)
            {
                return false;
            }
        }
        else if (Forest[CheckR][CheckC] > 0)
        {
            return false;
        }
    }

    r++;
    return true;
}

bool CanMoveToLeftDown(int& r, int& c)
{
    for(const auto& [dr, dc] : CheckOffsetToLeftDown)
    {
        int CheckR = r + dr, CheckC = c + dc;
        if(OutOfBound(CheckR, CheckC))
        {
            if(CheckC < 0 || CheckC >= C || CheckR >= R)
            {
                return false;
            }
        }
        else if (Forest[CheckR][CheckC] > 0)
        {
            return false;
        }
    }

    r++; c--;
    return true;
}

bool CanMoveToRightDown(int& r, int& c)
{
    for(const auto& [dr, dc] : CheckOffsetToRightDown)
    {
        int CheckR = r + dr, CheckC = c + dc;
        if(OutOfBound(CheckR, CheckC))
        {
            if(CheckC < 0 || CheckC >= C || CheckR >= R)
            {
                return false;
            }
        }
        else if (Forest[CheckR][CheckC] > 0)
        {
            return false;
        }
    }

    r++; c++;
    return true;
}

int GetLowestColFairyArrived(int Index)
{
    queue<pair<int, int>> Q;
    const auto& [r, c, d] = Golems[Index];

    vector<vector<bool>> Visited(R, vector<bool>(C, false));
    Visited[r][c] = true;

    int LowestCol = 0;

    Q.emplace(r, c);
    while(!Q.empty())
    {
        const auto [x, y] = Q.front();
        Q.pop();

        if(x > LowestCol)
        {
            LowestCol = x;
        }

        for(const auto& [dx, dy] : Offset)
        {
            int nx = x + dx, ny = y + dy;
            if(OutOfBound(nx, ny) || Visited[nx][ny] || Forest[nx][ny] == 0) continue;

            int SrcIndex = Forest[x][y];
            int DstIndex = Forest[nx][ny];
            if(SrcIndex == DstIndex)   // 같은 골렘 위
            {
                Visited[nx][ny] = true;
                Q.emplace(nx, ny);
                continue;
            }

            // 다른 골렘으로 옮겨갈 수 있는가?
            const auto& [SrcX, SrcY, ExitDir] = Golems[SrcIndex];
            int ExitX = SrcX + Offset[ExitDir].first;
            int ExitY = SrcY + Offset[ExitDir].second;

            // 현재 위치가 이 골렘의 탈출구 위치랑 동일한가?
            if(x == ExitX && y == ExitY)
            {
                Visited[nx][ny] = true;
                Q.emplace(nx, ny);
                continue;
            }
        }
    }

    return LowestCol + 1;   // 원래 1번부터 번호가 매겨졌으므로
}

void MarkGolemIndexAtForest(int r, int c, int Index)
{
    Forest[r][c] = Index;
    for(const auto& [dr, dc] : Offset)
    {
        Forest[r + dr][c + dc] = Index;
    }
}

int MoveGolem(int Index)
{
    auto& [r, c, d] = Golems[Index];
    int OriginR = r, OriginC = c, OriginDir = d;

    while(true)
    {
        bool bMove = false;
        if(CanMoveToDown(r, c))
        {
            bMove = true;
            continue;
        }

        if(CanMoveToLeftDown(r, c))
        {
            d = d == 0 ? 3 : d - 1;
            bMove = true;
            continue;
        }

        if(CanMoveToRightDown(r, c))
        {
            d = d == 3 ? 0 : d + 1;
            bMove = true;
            continue;
        }

        if(!bMove)
        {
            break;
        }
    }

    if(IsGolemOutOfBound(r, c))
    {
        r = OriginR;
        c = OriginC;
        d = OriginDir;
        return -1;
    }

    MarkGolemIndexAtForest(r, c, Index);
    return GetLowestColFairyArrived(Index);
}

int main() 
{
    cin.tie(nullptr)->sync_with_stdio(false);

    cin >> R >> C;
    Forest.assign(R, vector<int>(C, 0));    // 0 : Empty, 1~: Number of Golem

    int K; cin >> K;
    Golems.resize(K + 1);

    for(int Index = 1; Index <= K; ++Index)
    {
        auto& [r, c, d] = Golems[Index];
        cin >> c >> d;
        c -= 1; // 0번 열부터 시작하도록
        r = -2;
    }

    int Answer = 0;
    for(int Index = 1; Index <= K; ++Index)
    {
        int RetVal = MoveGolem(Index);
        if (RetVal == -1)
        {
            Forest.assign(R, vector<int>(C, 0));
        }
        else
        {
            Answer += RetVal;
        }
    }

    cout << Answer;
    return 0;
}