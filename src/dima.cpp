#include <common.h>

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <chrono>

using namespace std;

#define mp make_pair
#define pb push_back
#define fi first
#define se second
#define li long long
#define pii pair<int, int>
#define vi vector<int>

#define forn(i, n) for (int i = 0; i < (int)n; i++)
#define fore(i, b, e) for (int i = (int)b; i <= (int)e; i++)
#define all(x) (x).begin(), (x).end()

struct MySolver : public Context {
    void Solve() {
        // Solution goes here
        // vi first {1, 0};
        // Assignment ass0 = {1, first};
        // Solution.pb(ass0);
        // vi second = {0}; 
        // Assignment ass1 = {0, second};
        // Solution.pb(ass1);

        // vi third = {2, 1};
        // Assignment ass2 = {2, third};
        // Solution.pb(ass2);

        vector<vi> contrib_to_skill(contributors_num, vi(skill_name_to_id.size()));
        forn(c, contributors_num) {
            for (auto s: contributors[c].skills) {
                contrib_to_skill[c][s.skill] = s.level;
            }
        }
        cerr << "built contrib to skill" << endl;


        vector<Project> order = projects;
        sort(all(order));
        vector<int> free_since(contributors_num);
        for (auto& p: order) {
            vi chosen(p.roles_num, -1);
            int start_date = 0;
            for (int c = 0; c < contributors_num; ++c) {
                forn(r, p.roles_num) {
                    if (chosen[r] == -1 && contrib_to_skill[c][p.roles[r].skill] >= p.roles[r].level) {
                        chosen[r] = c;
                        start_date = max(start_date, free_since[c]);
                        break;
                    }
                }
            }
            bool all_covered = true;
            forn(j, p.roles_num) {
                if (chosen[j] == -1) {
                    all_covered = false;
                    break;
                }
            }
            if (!all_covered) {
                continue;
            }
            // assert((int)chosen.size() == p.roles_num);
            if (start_date + p.days_to_complete <= p.best_before + p.score) {
                for (int person: chosen) {
                    free_since[person] = start_date + p.days_to_complete;
                }
                Solution.pb({p.id, chosen});
            }
        } 
    }
};

int main() {
    MySolver solver;

    solver.Input();

    auto start = std::chrono::system_clock::now();
    cerr << "Started solving..." << endl;
    solver.Solve();
    cerr << "Done!" << endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cerr << "Test time: " << elapsed_seconds.count() << endl;

    cerr << "Outputting" << endl;
    solver.Output();

    cerr << solver.GetScore() << endl;
    return 0;
}
