#pragma once

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

namespace sax_parser {
	class nfa2dfa {
	private:

		typedef pair< shared_ptr< set<int> >, shared_ptr< set<int> > > dfa_pair;
		typedef vector< pair< shared_ptr< set<int> >, shared_ptr < set<int> > > > dfa_table;

		typedef shared_ptr< vector<int> > DFAKEY;
		typedef shared_ptr< vector<int> > DFAROW;

		template<int ncols>
		static void eclosure(int state, int nfa_table[][ncols], int is_eclosure[], set<int>& ev /**/) {
			if (is_eclosure[state]) {
				int *p = nfa_table[state];
				for (; *p != -1; p++) {
					eclosure(*p, nfa_table, is_eclosure, ev);
				}
			}
			else {
				ev.insert(state);
			}
		}

		static int getindexfromset(vector< set<int> >& s, set<int>& key, vector< DFAROW >& dfa_table) {
			vector< set<int> >::const_iterator iter = s.begin();
			vector< set<int> >::const_iterator end = s.end();

			int index = 0;
			for (iter; iter != end; iter++, index++) {
				if (std::equal((*iter).begin(), (*iter).end(), key.begin(), key.end())) return index;
			}

			dfa_table.push_back(make_shared< vector<int> >());
			s.push_back(key);
			return dfa_table.size() - 1;
		}

	public:
		
		template<int ncols>
		static void print_dfa(int nfa_table[][ncols], int is_eclosure[]) {
			if (!is_eclosure) {
				cerr << "[ERROR] argument is_eclosure table is not given!, do nothing." << endl;
				return;
			}
			vector< DFAROW > dfa_table;
			vector< set<int> > dfa_indexes;
			set< pair<int, int> > dfa_inv_table; // {key, value} : {from_index | to_index | to_nfa, from_nfa} 

			int start_state = 0;
			set<int> dfa_state;
			eclosure(start_state, nfa_table, is_eclosure, dfa_state);
			assert(getindexfromset(dfa_indexes, dfa_state, dfa_table) == 0);

			for (int i = 0; i < dfa_table.size(); i++) {
				set<int> key = dfa_indexes[i];
				shared_ptr< vector<int> > dfa_row = dfa_table[i];
				for (int j = 0; j < ncols; j++) { // 모든 인풋기호에 대하여
					set<int>::const_iterator key_iter = key.begin();
					set<int>::const_iterator key_end = key.end();
					set<int> to_dfa_state;
					vector < pair<int, int> > tmp_dfa_inv_row;
					for (key_iter; key_iter != key_end; key_iter++) {
						int nfa_state = *key_iter;
						int moved_state = nfa_table[nfa_state][j];
						if (moved_state == -1) continue;
						set<int> eclosured_state;
						eclosure(moved_state, nfa_table, is_eclosure, eclosured_state);
						set<int>::const_iterator e_iter = eclosured_state.begin();
						set<int>::const_iterator e_end = eclosured_state.end();
						for (e_iter; e_iter != e_end; e_iter++) {
							tmp_dfa_inv_row.push_back(pair<int, int>(*e_iter, nfa_state)); // to, from
							to_dfa_state.insert(*e_iter);
						}
					}

					int from_dfa_idx = i;
					int to_dfa_idx = -1;

					if (to_dfa_state.size() != 0) {
						to_dfa_idx = getindexfromset(dfa_indexes, to_dfa_state, dfa_table);
					}

					// dfa 테이블 구성
					dfa_row->push_back(to_dfa_idx);
					// dfa inv 테이블 구성
					vector < pair<int, int> >::const_iterator tmp_row_iter = tmp_dfa_inv_row.begin();
					vector < pair<int, int> >::const_iterator tmp_row_end = tmp_dfa_inv_row.end();
					for (tmp_row_iter; tmp_row_iter != tmp_row_end; tmp_row_iter++) {
						int to_nfa = (*tmp_row_iter).first;
						int from_nfa = (*tmp_row_iter).second;
						int key = from_dfa_idx << 16 | to_dfa_idx << 8 | to_nfa;
						dfa_inv_table.insert(pair<int, int>(key, from_nfa));
					}
				}
			}

			cout << "int dfa_table[][" << ncols << "] = {\n";
			vector< DFAROW >::const_iterator dfa_row_iter = dfa_table.begin();
			vector< DFAROW >::const_iterator dfa_row_end = dfa_table.end();
			for (dfa_row_iter; dfa_row_iter != dfa_row_end; dfa_row_iter++) {
				cout << "{";
				vector<int>::const_iterator dfa_col_iter = (*dfa_row_iter)->begin();
				vector<int>::const_iterator dfa_col_end = (*dfa_row_iter)->end();
				for (dfa_col_iter; dfa_col_iter != dfa_col_end; dfa_col_iter++) {
					cout << *dfa_col_iter << ",";
				}
				cout << "},\n";
			}
			cout << "};\n";

			set< pair<int, int> >::const_iterator dfa_inv_row = dfa_inv_table.begin();
			set< pair<int, int> >::const_iterator dfa_inv_end = dfa_inv_table.end();

			cout << "int dfa_inv_table[][2] = {\n";
			for (dfa_inv_row; dfa_inv_row != dfa_inv_end; dfa_inv_row++) {
				int from_dfa_idx =	((*dfa_inv_row).first	& 0x00ff0000) >> 16;
				int to_dfa_idx =	((*dfa_inv_row).first	& 0x0000ff00) >> 8;
				int to_nfa_state =	((*dfa_inv_row).first	& 0x0000ff);
				cout << "{" << from_dfa_idx << " << 16 | " << to_dfa_idx << " << 8 | " << to_nfa_state << "," << (*dfa_inv_row).second << "},\n";
			}
			cout << "};" << endl;
		}

		template<int ncols>
		static void export_csv(int nfa_table[][ncols], int is_eclosure[], string& filename) {

			ofstream out(filename);

			if (!is_eclosure) {
				cerr << "[ERROR] argument is_eclosure table is not given!, do nothing." << endl;
				return;
			}
			vector< DFAROW > dfa_table;
			vector< set<int> > dfa_indexes;
			set< pair<int, int> > dfa_inv_table; // {key, value} : {from_index | to_index | to_nfa, from_nfa} 

			int start_state = 0;
			set<int> dfa_state;
			eclosure(start_state, nfa_table, is_eclosure, dfa_state);
			assert(getindexfromset(dfa_indexes, dfa_state, dfa_table) == 0);

			for (int i = 0; i < dfa_table.size(); i++) {
				set<int> key = dfa_indexes[i];
				shared_ptr< vector<int> > dfa_row = dfa_table[i];
				for (int j = 0; j < ncols; j++) { // 모든 인풋기호에 대하여
					set<int>::const_iterator key_iter = key.begin();
					set<int>::const_iterator key_end = key.end();
					set<int> to_dfa_state;
					vector < pair<int, int> > tmp_dfa_inv_row;
					for (key_iter; key_iter != key_end; key_iter++) {
						int nfa_state = *key_iter;
						int moved_state = nfa_table[nfa_state][j];
						if (moved_state == -1) continue;
						set<int> eclosured_state;
						eclosure(moved_state, nfa_table, is_eclosure, eclosured_state);
						set<int>::const_iterator e_iter = eclosured_state.begin();
						set<int>::const_iterator e_end = eclosured_state.end();
						for (e_iter; e_iter != e_end; e_iter++) {
							tmp_dfa_inv_row.push_back(pair<int, int>(*e_iter, nfa_state)); // to, from
							to_dfa_state.insert(*e_iter);
						}
					}

					int from_dfa_idx = i;
					int to_dfa_idx = -1;

					if (to_dfa_state.size() != 0) {
						to_dfa_idx = getindexfromset(dfa_indexes, to_dfa_state, dfa_table);
					}

					// dfa 테이블 구성
					dfa_row->push_back(to_dfa_idx);
					// dfa inv 테이블 구성
					vector < pair<int, int> >::const_iterator tmp_row_iter = tmp_dfa_inv_row.begin();
					vector < pair<int, int> >::const_iterator tmp_row_end = tmp_dfa_inv_row.end();
					for (tmp_row_iter; tmp_row_iter != tmp_row_end; tmp_row_iter++) {
						int to_nfa = (*tmp_row_iter).first;
						int from_nfa = (*tmp_row_iter).second;
						int key = from_dfa_idx << 16 | to_dfa_idx << 8 | to_nfa;
						dfa_inv_table.insert(pair<int, int>(key, from_nfa));
					}
				}
			}

			//std::sort(dfa_inv_table.begin(), dfa_inv_table.end(), [](auto &left, auto &right) {
			//	return left.first < right.first;
			//});

			//out << "int dfa_table[][" << ncols << "] = {\n";
			vector< DFAROW >::const_iterator dfa_row_iter = dfa_table.begin();
			vector< DFAROW >::const_iterator dfa_row_end = dfa_table.end();
			for (dfa_row_iter; dfa_row_iter != dfa_row_end; dfa_row_iter++) {
				//out << "";
				vector<int>::const_iterator dfa_col_iter = (*dfa_row_iter)->begin();
				vector<int>::const_iterator dfa_col_end = (*dfa_row_iter)->end();
				for (dfa_col_iter; dfa_col_iter != dfa_col_end; dfa_col_iter++) {
					out << *dfa_col_iter << ",";
				}
				out << "\n";
			}
			out << "\n";

			set< pair<int, int> >::const_iterator dfa_inv_row = dfa_inv_table.begin();
			set< pair<int, int> >::const_iterator dfa_inv_end = dfa_inv_table.end();

			out << "from_dfa_idx, to_dfa_idx, to_nfa_state, key, from_nfa_state\n";
			for (dfa_inv_row; dfa_inv_row != dfa_inv_end; dfa_inv_row++) {
				int from_dfa_idx = ((*dfa_inv_row).first & 0x00ff0000) >> 16;
				int to_dfa_idx = ((*dfa_inv_row).first & 0x0000ff00) >> 8;
				int to_nfa_state = ((*dfa_inv_row).first & 0x0000ff);
				out << from_dfa_idx << "," << to_dfa_idx << "," << to_nfa_state << "," << (*dfa_inv_row).first << "," << (*dfa_inv_row).second << "\n";
			}
			//out << "};" << endl;
			out << endl;
		}
	};
}