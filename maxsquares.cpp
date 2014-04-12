#include <iostream>
#include <cstdint>
#include <bitset>
#include <vector>
#include <unordered_set>

using namespace std;

const int N = 18; // string length
const uint64_t MASK = ((uint64_t)1<<N)-1;
const uint64_t MAX_I = ((uint64_t)1<<(N-1))-1;

// return bitset of i
std::bitset<N> bits(uint64_t v) {
    std::bitset<N> x(v);
    return x;
}

// reverse lower N bits of v
uint64_t rev(uint64_t v) {
  uint64_t r = 0;
  for (int i=0; i<N; i++) {
    r <<= 1;
    r |= v & 1;
    v >>= 1;
  }
  return r;
}

// returns number of squares lower N bits of v
int count_squares(uint64_t v) {
	int ds = 0; // distinct squares
	std::unordered_set<uint64_t> squares; // distinct squares of length 2l

	for(int l=1; l<=N/2; l++) {
		uint64_t r = (((uint64_t)1 << l)-1); // root of square mask
		for(int i=0; i+2*l<N+1; i++) {
			uint64_t right = (v & r) >> i; // right half of substring
			if ((right == (v & (r << l)) >> (i+l)) && squares.insert(right).second) { // is substring of length 2l ending on bit i a new square?
					++ds; // new square of length 2l!
			}
			r <<= 1;
		}
		squares.clear(); // clear squares of length 2l
	}
	return ds;
}


int main() {
	cout << "N=" << N << endl;

	int max,s;
	std::vector<uint64_t> max_strings;
	for (uint64_t i = 0; i<=MAX_I; i++) {
		//debug
		//cout << bits(i) << endl;
		uint64_t r = rev(i);
		if (((i & 1) && i < ((~r) & MASK)) || (!(i & 1) && i > r)) {
			continue; // consider only one element from each equiv. class
		}

		s = count_squares(i);
		if (s > max) {
			max = s;
			max_strings.clear();
		}

		if (s == max) {
			max_strings.push_back(i);
		}
  }
	cout << "Max squares: " << max << endl;
	cout << "max_strings[" << N << "] = [";
	for (uint64_t i = 0; i<max_strings.size(); ++i) {
		cout << max_strings[i];
		if ( i<max_strings.size()-1 )
			cout << ",";
	}
	cout << "]" << endl;

  return 0;
}
