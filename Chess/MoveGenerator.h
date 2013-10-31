#pragma once
#include <vector>
#include "Board.h"

using namespace std;

bool IsMovePossible(Board& board);
void GenerateAll(Board& board, vector<Move*>& result);
