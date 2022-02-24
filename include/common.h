#pragma once

#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <unordered_map>
#include <random>

using namespace std;

#ifndef SEED
#define SEED 1337
#endif

struct Role {
    int skill, level;
};

struct Contributor {
    string name;
    int id;
    int skills_num;
    vector<Role> skills;
};

struct Project {
    string name;
    int id;
    int days_to_complete, score, best_before, roles_num;
    vector<Role> roles;
};

struct ProjectComp {
    float Potential(const Project& project) const {
        return float(project.days_to_complete) - 0.0 * project.score;
    }

    bool operator()(const Project& first, const Project& second) const {
        return Potential(first) < Potential(second);
    }
};

struct Assignment {
    int project_id;
    vector<int> chosen; 
};

struct Context {
    using TSolution = vector<Assignment>;
    TSolution Solution;

    std::mt19937_64 rng;

    int contributors_num, projects_num;
    map<string, int> skill_name_to_id;
    vector<Contributor> contributors;
    vector<Project> projects;

    Context() {
        rng.seed(SEED);
    }

    int get_skill_id(const string& name) {
        auto iter = skill_name_to_id.find(name);
        if (iter == skill_name_to_id.end()) {
            skill_name_to_id[name] = skill_name_to_id.size();
        }
        return skill_name_to_id[name];
    }

    void Input() {
        cin >> contributors_num >> projects_num; 
        contributors.resize(contributors_num);
        for (int i = 0; i < contributors_num; ++i) {
            Contributor& c = contributors[i];
            cin >> c.name >> c.skills_num;
            c.id = i;
            c.skills.resize(c.skills_num);
            for (int j = 0; j < c.skills_num; ++j) {
                string name;
                cin >> name >> c.skills[j].level;
                c.skills[j].skill = get_skill_id(name);
                // cerr << c.skills[j].skill << endl; 
            }
        }
        projects.resize(projects_num);
        for (int i = 0; i < projects_num; ++i) {
            Project& p = projects[i];
            cin >> p.name >> p.days_to_complete >> p.score >> p.best_before >> p.roles_num;
            p.id = i;
            // cerr << p.days_to_complete << " " << p.score << " " << p.best_before << " " << p.roles_num << endl;
            p.roles.resize(p.roles_num);
            for (int j = 0; j < p.roles_num; ++j) {
                string name;
                cin >> name >> p.roles[j].level;
                p.roles[j].skill = get_skill_id(name);
                // cerr << p.roles[j].skill << endl;
            }
        }
    }

    void Output() {
        cout << Solution.size() << endl;
        for (auto ass: Solution) {
            cout << projects[ass.project_id].name << endl;
            for (auto person: ass.chosen) {
                cout << contributors[person].name << " ";
            }
            cout << endl;
        }
    }

    const int END_OF_TIME = 2e5;

    uint64_t GetScore() {
        uint64_t score = 0;
        vector<int> free_since(contributors_num);
        // cerr << "Important: order of assignments should be correct!" << endl;
        for (int pointer = 0; pointer < (int)Solution.size(); ++pointer) {
            Assignment& ass = Solution[pointer];
            Project& p = projects[ass.project_id];

            int start_time = 0;
            for (int c: ass.chosen) {
                start_time = max(start_time, free_since[c]);
            }

            if (start_time + p.days_to_complete <= p.best_before + p.score) {
                int t = start_time + p.days_to_complete;
                // cerr << "project " << ass.project_id << " done on day " << t << endl;
                if (t <= p.best_before) {
                    score += p.score;
                } else if (t <= p.best_before + p.score) {
                    score += p.score - (t - p.best_before);
                }

                for (int person: ass.chosen) {
                    free_since[person] = t;
                }
            } else {
                break;
            }
        }
        return score;
    }
    
};
