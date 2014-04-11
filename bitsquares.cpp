#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <set>

using namespace std;

struct searchres
{
    int maxsquares;
    bool canflip;
};


// Count the number of squares in a string.
// Outputs the squares and derivation if verbose.
int countsquares(const string& bs, bool verb)
{
    if (verb)
        cout << "Analyzing: " << bs << endl;
    set<string> squares;
    vector<int> deriv(bs.length(), 0);
    int cs = 0;
    int n = bs.length();
    for (int i = n-2; i >= 0; --i) {
        for (int j = 2; j+i <= n; j+=2) {
            string sq1 = bs.substr(i,j/2);
            string sq2 = bs.substr(i+j/2, j/2);
            if (sq1 == sq2 && squares.insert(sq1).second == true) {
                ++cs;
                deriv[i]++;
            }
        }
    }
    if (verb) {
        cout << "Squares:" << endl;
        for (set<string>::iterator it = squares.begin(); it != squares.end(); ++it)
            cout << *it << endl;
        cout << "Derivation:" << endl;
        for (vector<int>::iterator it = deriv.begin(); it != deriv.end(); ++it)
            cout << *it;
        cout << endl;
    }
    return cs;
}

bool checksets_helper(const vector<int>& ones, string& s, int flipped, int pos, const int sq, bool verb)
{
    // Does a flip-set of size two always exist? (TODO: Should check all
    // flipsets probably).
    if (flipped > 2)
        return false;
    bool ret = false;
    if (pos == ones.size()) {
        if (flipped == 0)
            return false;
        int sq2 = countsquares(s, false);
        ret |= (sq2 + flipped * 2 >= sq);
        if (ret && verb)
            cout << "Flipped-set string with " << flipped << " flipped ones and " << sq2 << " squares: " << s << endl;
    }
    else {
        ret = ret || checksets_helper(ones, s, flipped, pos+1, sq, verb);
        if (!ret) {
            s[ones[pos]] = '0';
            ret = ret || checksets_helper(ones, s, flipped+1, pos+1, sq, verb);
            s[ones[pos]] = '1';
        }
    }
    return ret;
}

// Check if a set of bits exists such that flipping them is "good"
// Only checks sets of ones -- is this okay? (TODO)
bool checksets(const string& bs, bool verb)
{
    int n = bs.length();
    int sq = countsquares(bs, false);
    vector<int> ones;
    for (int i = 0; i < bs.length(); ++i)
        if (bs[i] == '1')
            ones.push_back(i);
    if (ones.size() == 0)
        return true;
    string teststr(bs);
    return checksets_helper(ones, teststr, 0, 0, sq, verb);
}

void analyze(const string& bs, bool verb)
{
    int n = bs.length();
    cout << countsquares(bs, verb) << endl;
    cout << "flipset exists: " << checksets(bs, verb) << endl;
}

searchres search(int pos, string& s)
{
    searchres ret = {0,0};
    if (pos == s.length()) {
        ret.maxsquares = countsquares(s, false);
        ret.canflip = checksets(s, false);
    }
    else {
        ret = search(pos+1, s);
        s[pos] = '1';
        searchres res2 = search(pos+1, s);
        ret.maxsquares = max(ret.maxsquares, res2.maxsquares);
        ret.canflip = (ret.canflip && res2.canflip);
        s[pos] = '0';
    }
    return ret;
}

int main(int argc, char *argv[])
{
    if (argc > 2 && strcmp(argv[1], "-a") == 0) {
        // -a <bitstring>
        analyze(argv[2], true);
    }
    else if (argc > 2 && strcmp(argv[1], "-s") == 0) {
        int maxn = atoi(argv[2]);
        for (int i = 2; i < maxn; ++i) {
            string s(i, '0');
            searchres result = search(0, s);
            cout << "len: " << i << ", maxsquares: " << result.maxsquares << ", all flippable? " << result.canflip << endl;
        }
    }
    else {
        cout << "Invalid input. Must use -a to analyze a bitstring." << endl;
        return 1;
    }
    return 0;
}
