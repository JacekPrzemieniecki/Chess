#include "Move.h"
#include <iostream>

Move::Move():
from(-1),
to(-1)
{
}


Move::Move(int _from, int _to) :
from(_from),
to(_to)
{}