#
# Helper script to generate the predefined sq user types in the sq class.
#

from typing import NamedTuple


class SqType(NamedTuple):
    type: str
    short: str
    size: int
    qFrom: int
    qTo: int

sqTypeList = [
    SqType('int8_t', 'i8', 8, -4, 7),
    SqType('uint8_t', 'u8', 8, -4, 7),
    SqType('int16_t', 'i16', 16, -8, 15),
    SqType('uint16_t', 'u16', 16, -8, 15),
    SqType('int32_t', 'i32', 32, -16, 31),
    SqType('uint32_t', 'u32', 32, -16, 31),
]

def get_q_from_range(fromQ, toQ):
    q_range = [*range(fromQ, toQ + 1)]
    return map(lambda q: (q, str(q) if q >= 0 else "m" + str(abs(q))), q_range)

def print_sq_types():
    for sq in sqTypeList:
        q_range = get_q_from_range(sq.qFrom, sq.qTo)
        for q, qStr in q_range:
            print("template< double lower, double upper > using %8s = sq<%8s, %3s, lower, upper>;" % (sq.short + "sq" + qStr, sq.type, q))

def print_sq_literals():
    for sq in sqTypeList:
        q_range = get_q_from_range(sq.qFrom, sq.qTo)
        for q, qStr in q_range:
            print("template< char ...chars > consteval auto operator\"\"_%-10s { return sqFromLiteral<%8s, %3s, chars...>(); }" % (sq.short + "sq" + qStr + "()", sq.type, q))


def main():
    print_sq_types()
    print_sq_literals()

# execute main function
main()
