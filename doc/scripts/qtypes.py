#
# Helper script to generate predefined types and literals.
#

from typing import NamedTuple


class SqType(NamedTuple):
    type: str
    short: str
    size: int
    fFrom: int
    fTo: int

sqTypeList = [
    SqType('int8_t', 'i8', 8, -4, 7),
    SqType('uint8_t', 'u8', 8, -4, 7),
    SqType('int16_t', 'i16', 16, -8, 15),
    SqType('uint16_t', 'u16', 16, -8, 15),
    SqType('int32_t', 'i32', 32, -16, 31),
    SqType('uint32_t', 'u32', 32, -16, 31),
]


class QType(NamedTuple):
    type: str
    short: str
    size: int
    fFrom: int
    fTo: int
    defOvf: str

qTypeList = [
    QType('int8_t', 'i8', 8, -4, 7, 'forbidden'),
    QType('uint8_t', 'u8', 8, -4, 7, 'forbidden'),
    QType('int16_t', 'i16', 16, -8, 15, 'forbidden'),
    QType('uint16_t', 'u16', 16, -8, 15, 'forbidden'),
    QType('int32_t', 'i32', 32, -16, 31, 'forbidden'),
    QType('uint32_t', 'u32', 32, -16, 31, 'forbidden'),
]


def get_f_from_range(fromF, toF):
    f_range = [*range(fromF, toF + 1)]
    return map(lambda f: (f, str(f) if f >= 0 else "m" + str(abs(f))), f_range)


def print_sq_types():
    for sq in sqTypeList:
        f_range = get_f_from_range(sq.fFrom, sq.fTo)
        for f, fStr in f_range:
            print("template< double lower = fpm::detail::lowestRealVMin<%8s, %3s>(), double upper = fpm::detail::highestRealVMax<%8s, %3s>() > using %8s = sq<%8s, %3s, lower, upper>;" % (sq.type, f, sq.type, f, sq.short + "sq" + fStr, sq.type, f))

def print_sq_literals():
    for sq in sqTypeList:
        f_range = get_f_from_range(sq.fFrom, sq.fTo)
        for f, fStr in f_range:
            templSqType = sq.short + "sq" + fStr
            print("FPM_SQ_BIND_LITERAL(%8s<>, %-8s)" % (templSqType, templSqType))

def print_q_types():
    for q in qTypeList:
        f_range = get_f_from_range(q.fFrom, q.fTo)
        for f, fStr in f_range:
            print("template< double lower = fpm::detail::lowestRealVMin<%8s, %3s>(), double upper = fpm::detail::highestRealVMax<%8s, %3s>(), Overflow ovfBx = Ovf::%-9s > using %7s = q<%8s, %3s, lower, upper, ovfBx>;" % (q.type, f, q.type, f, q.defOvf, q.short + "q" + fStr, q.type, f))

def print_q_literals():
    for q in qTypeList:
        f_range = get_f_from_range(q.fFrom, q.fTo)
        for f, fStr in f_range:
            templQType = q.short + "q" + fStr
            print("FPM_Q_BIND_LITERAL(%7s<>, %-7s)" % (templQType, templQType))


def main():
    print_sq_types()
    print_sq_literals()
    print_q_types()
    print_q_literals()

# execute main function
main()
