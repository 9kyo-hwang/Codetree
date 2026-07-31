#include <iostream>
#include <vector>
using namespace std;

enum class Color {
    None = 0,
    White = 1,
    Black = 2,
    Gray = 3,
};

struct Tile {
    int whiteCount = 0;
    int blackCount = 0;
    Color lastColor = Color::None;

    inline bool IsGrayColor() const { 
        return whiteCount >= 2 && blackCount >= 2; 
    }
    
    void SetColor(Color color) {
        lastColor = color;
    }
};

int main() {
    int N; cin >> N;
    
    vector<Tile> tiles(200001);
    int pos = 100000;

    while(N--) {
        int cnt{}; char dir{};
        cin >> cnt >> dir;

        bool isLeft = dir == 'L';
        if(isLeft) {  // white
            for(int end = pos - cnt; pos > end; --pos) {
                Tile& tile = tiles[pos];
                if(tile.IsGrayColor()) {
                    continue;
                }

                ++tile.whiteCount;
                if(tile.IsGrayColor()) {
                    tile.SetColor(Color::Gray);
                } else {
                    tile.SetColor(Color::White);
                }
            }
        } else {  // black
            for(int end = pos + cnt; pos < end; ++pos) {
                Tile& tile = tiles[pos];
                if(tile.IsGrayColor()) {
                    continue;
                }

                ++tile.blackCount;
                if(tile.IsGrayColor()) {
                    tile.SetColor(Color::Gray);
                } else {
                    tile.SetColor(Color::Black);
                }
            }
        }

        isLeft ? ++pos : --pos;
    }

    int w = 0, b = 0, g = 0;
    for(const Tile& tile : tiles) {
        switch(tile.lastColor) {
            case Color::White: ++w; break;
            case Color::Black: ++b; break;
            case Color::Gray: ++g; break;
            default: break;
        }
    }

    cout << w << " " << b << " " << g;

    return 0;
}